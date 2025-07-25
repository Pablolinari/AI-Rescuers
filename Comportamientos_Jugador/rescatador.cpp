#include "../Comportamientos_Jugador/rescatador.hpp"
#include "comportamientos/comportamiento.hpp"
#include "motorlib/util.h"
#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <list>
#include <queue>
#include <set>
#include <system_error>
#include <utility>
#include <vector>
#include <random>
double get_random_0_1R() {
  static std::mt19937 gen(42); 
  static std::uniform_real_distribution<double> dis(
      0.0, 1.0); 

  return dis(gen); 
}
// usado para generar nodos en dijkstra
const vector<Action> genera_acciones = {WALK, RUN, TURN_L, TURN_SR};
// para calcular la vision dependiendo de la posicion
const vector<vector<pair<int, int>>> posiciones_vision = {
    // Norte (rumbo = 0)
    {
        {-1, -1},
        {-1, 0},
        {-1, 1}, // Primera fila
        {-2, -2},
        {-2, -1},
        {-2, 0},
        {-2, 1},
        {-2, 2}, // Segunda fila
        {-3, -3},
        {-3, -2},
        {-3, -1},
        {-3, 0},
        {-3, 1},
        {-3, 2},
        {-3, 3} // Tercera fila
    },
    // Noreste (rumbo = 1)
    {{-1, 0},
     {-1, 1},
     {0, 1},
     {-2, 0},
     {-2, 1},
     {-2, 2},
     {-1, 2},
     {0, 2},
     {-3, 0},
     {-3, 1},
     {-3, 2},
     {-3, 3},
     {-2, 3},
     {-1, 3},
     {0, 3}},
    // Este (rumbo = 2)
    {{-1, 1},
     {0, 1},
     {1, 1},
     {-2, 2},
     {-1, 2},
     {0, 2},
     {1, 2},
     {2, 2},
     {-3, 3},
     {-2, 3},
     {-1, 3},
     {0, 3},
     {1, 3},
     {2, 3},
     {3, 3}},
    // Sureste (rumbo = 3)
    {{0, 1},
     {1, 1},
     {1, 0},
     {0, 2},
     {1, 2},
     {2, 2},
     {2, 1},
     {2, 0},
     {0, 3},
     {1, 3},
     {2, 3},
     {3, 3},
     {3, 2},
     {3, 1},
     {3, 0}},
    // Sur (rumbo = 4)
    {{1, 1},
     {1, 0},
     {1, -1},
     {2, 2},
     {2, 1},
     {2, 0},
     {2, -1},
     {2, -2},
     {3, 3},
     {3, 2},
     {3, 1},
     {3, 0},
     {3, -1},
     {3, -2},
     {3, -3}},
    // Suroeste (rumbo = 5)
    {{1, 0},
     {1, -1},
     {0, -1},
     {2, 0},
     {2, -1},
     {2, -2},
     {1, -2},
     {0, -2},
     {3, 0},
     {3, -1},
     {3, -2},
     {3, -3},
     {2, -3},
     {1, -3},
     {0, -3}},
    // Oeste (rumbo = 6)
    {{1, -1},
     {0, -1},
     {-1, -1},
     {2, -2},
     {1, -2},
     {0, -2},
     {-1, -2},
     {-2, -2},
     {3, -3},
     {2, -3},
     {1, -3},
     {0, -3},
     {-1, -3},
     {-2, -3},
     {-3, -3}},
    // Noroeste (rumbo = 7)
    {{0, -1},
     {-1, -1},
     {-1, 0},
     {0, -2},
     {-1, -2},
     {-2, -2},
     {-2, -1},
     {-2, 0},
     {0, -3},
     {-1, -3},
     {-2, -3},
     {-3, -3},
     {-3, -2},
     {-3, -1},
     {-3, 0}}};
int ComportamientoRescatador::interact(Action accion, int valor) { return 0; }
Action ComportamientoRescatador::think(Sensores sensores) {
  Action accion = IDLE;

  switch (sensores.nivel) {
  case 0:
    accion = ComportamientoRescatadorNivel_0(sensores);
    break;
  case 1:
    accion = ComportamientoRescatadorNivel_1(sensores);
    break;
  case 2:
    accion = ComportamientoRescatadorNivel_2(sensores);
    break;
  case 3:
    // accion = ComportamientoRescatadorNivel_3 (sensores);
    break;
  case 4:
    accion = ComportamientoRescatadorNivel_4(sensores);
    break;
  }

  return accion;
}
/*
 * completa la matriz memoria del agente , sirve
 * para recordar todas las superficies y cotas que va teniendo
 * en su rango de vision .
 */

void SituarSensorEnMapaR(vector<vector<unsigned char>> &m,
                         vector<vector<unsigned char>> &a, Sensores sensores) {
  // Asignar la posición actual
  a[sensores.posF][sensores.posC] = sensores.cota[0];
  m[sensores.posF][sensores.posC] = sensores.superficie[0];

  // Iterar sobre los sensores (índices 1 a 15)
  for (int i = 0; i < 15; ++i) {
    int fila = sensores.posF + posiciones_vision[sensores.rumbo][i].first;
    int col = sensores.posC + posiciones_vision[sensores.rumbo][i].second;

    m[fila][col] = sensores.superficie[i + 1];
    a[fila][col] = sensores.cota[i + 1];
  }
}
/*
 * Funcion que comprueba si la casilla tiene una altura accesible
 *
 */
bool ViablePorAlturaR(int dif, bool zap) {
  return (abs(dif) <= 1 or (zap and abs(dif) <= 2));
}
/*
 *Devuelve true si una casilla es accesible para el nivel 1
 */
bool CasillaValida0R(char a) { return a == 'C' || a == 'D'; }
/*
 * Busca la casilla menos pisada entre las
 * tres primeras opciones del rango de vision
 */

int ComportamientoRescatador::MenosPisadaR(Sensores sensores) {
  int min_visitas = INFINITY;
  int pos = -1;
  int indice_rumbo = sensores.rumbo;
  vector<int> orden = {1, 0, 2, 3, 4, 5, 6, 7};
  camino_opcional = 0;
  for (int i : orden) {
    int posf = sensores.posF + posiciones_vision[indice_rumbo][i].first;
    int posc = sensores.posC + posiciones_vision[indice_rumbo][i].second;
    if (i <= 2) {
      if (memoria[posf][posc] < min_visitas &&
          CasillaValida0R(sensores.superficie[i + 1]) &&
          ViablePorAlturaR(sensores.cota[i + 1] - sensores.cota[0],
                           zapatillas) &&
          sensores.agentes[i + 1] == '_') {
        pos = i + 1;
        min_visitas = memoria[posf][posc];
      }
    } else {
      vector<int> caminos;
      switch (i) {
      case 3:
        caminos = {1};
        break;
      case 4:
        caminos = {2, 1};
        break;
      case 5:
        if (sensores.rumbo % 2 == 0)
          caminos = {2, 1, 3};
        else
          caminos = {2};
        break;
      case 6:
        caminos = {2, 3};
        break;
      case 7:
        caminos = {3};
        break;
      }
      for (int cam : caminos) {

        if (memoria[posf][posc] < min_visitas &&
            CasillaValida0R(sensores.superficie[i + 1]) &&
            CasillaValida0R(sensores.superficie[cam]) &&
            ViablePorAlturaR(sensores.cota[i + 1] - sensores.cota[cam],
                             zapatillas) &&
            ViablePorAlturaR(sensores.cota[cam] - sensores.cota[0],
                             zapatillas) &&
            sensores.agentes[cam] == '_' && sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;

          camino_opcional = cam;
        }
      }
    }
  }
  return pos;
}
/*
 * funcion que determina que casilla debe tomar como interesante
 * el agente , posteriormente se movera n esa direccion
 */

int ComportamientoRescatador::CasillainteresanteR(Sensores sensores) {
  // Calculo si son viables por altura las posibles casillas
  bool i = ViablePorAlturaR(sensores.cota[1] - sensores.cota[0], zapatillas);

  bool c = ViablePorAlturaR(sensores.cota[2] - sensores.cota[0], zapatillas);

  bool d = ViablePorAlturaR(sensores.cota[3] - sensores.cota[0], zapatillas);

  // compruebo los distintos casos segun superficie y altura , si no puedo
  // actuar devuelvo 0

  if (sensores.superficie[2] == 'X' && c && sensores.agentes[2] == '_')
    return 2;
  else if (sensores.superficie[1] == 'X' && i && sensores.agentes[1] == '_')
    return 1;
  else if (sensores.superficie[3] == 'X' && d && sensores.agentes[3] == '_')
    return 3;
  else if (!zapatillas) {
    if (sensores.superficie[2] == 'D' && c && sensores.agentes[2] == '_')
      return 2;
    else if (sensores.superficie[1] == 'D' && i && sensores.agentes[1] == '_')
      return 1;
    else if (sensores.superficie[3] == 'D' && d && sensores.agentes[3] == '_')
      return 3;
  }
  // Calculo la casilla menos pisada de las 3 que veo delante
  int menospisada = MenosPisadaR(sensores);
  if (menospisada == -1) { // si no encuentro una casilla camino menos pisada
    return 0;
  } else {
    return menospisada;
  }
}
Action
ComportamientoRescatador::ComportamientoRescatadorNivel_0(Sensores sensores) {
  Action action;
  SituarSensorEnMapaR(mapaResultado, mapaCotas, sensores);
  if (sensores.superficie[0] == 'D')
    zapatillas = true;

  if (sensores.superficie[0] == 'X') {
    action = IDLE;
  } else if (giroizq != 0) {
    action = TURN_SR;
    giroizq--;

  } else if (extrawalk) {
    memoria[sensores.posF][sensores.posC]++;
    action = WALK;
    extrawalk = false;
  } else if (extraturnd) {
    memoria[sensores.posF][sensores.posC]++;
    action = TURN_SR;
    extraturnd = false;
  } else if (extraturni) {
    memoria[sensores.posF][sensores.posC]++;
    action = TURN_L;
    giroizq = 1;
    extraturni = false;
  } else {
    int pos = CasillainteresanteR(sensores);

    switch (pos) {
    case 2:
      action = WALK;
      break;
    case 1:
      giroizq = 1;
      action = TURN_L;
      break;
    case 3:
      action = TURN_SR;
      break;
    case 4:
      extrawalk = true;
      action = TURN_L;
      giroizq = 1;
      break;
    case 5:
      if (camino_opcional == 2) {
        action = WALK;
      } else if (camino_opcional == 1) {
        extrawalk = true;
        action = TURN_L;
        extraturnd = true;
        giroizq = 1;
      }

    case 6:
      if (camino_opcional == 2) {
        action = WALK;
      } else if (camino_opcional == 1) {
        extrawalk = true;
        extraturnd = true;
        action = TURN_L;
        giroizq = 1;
      } else if (camino_opcional == 3) {
        action = TURN_SR;
        extrawalk = true;
        extraturni = true;
      }
      break;
    case 7:
      if (camino_opcional == 2) {
        action = WALK;
      } else if (camino_opcional == 3) {
        extrawalk = true;
        extraturni = true;
        action = TURN_SR;
      }
      break;
    case 8:
      action = TURN_SR;
      extrawalk = true;
      break;

    case 0:
      action = TURN_SR;
      break;
    }

    //  if (pos != 0)
    memoria[sensores.posF][sensores.posC]++;
  }
  next_action = action;

  return action;
}

////////////////////////////////////////////////////////////////////////////////
/// codigo para el nivel 1 ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

bool CasillaValida1R(char a) {
  return a == 'C' || a == 'D' || a == 'S' || a == 'X';
}
/*
 * funcion casilla interesante para el nivel 1
 */

int ComportamientoRescatador::MenosPisadaR1(Sensores sensores) {
  int min_visitas = INFINITY;
  int pos = -1;
  int indice_rumbo = sensores.rumbo;
  vector<int> orden = {1, 0, 2, 3, 4, 5, 6, 7};
  camino_opcional = 0;
  for (int i : orden) {
    int posf = sensores.posF + posiciones_vision[indice_rumbo][i].first;
    int posc = sensores.posC + posiciones_vision[indice_rumbo][i].second;
    if (i <= 2) {
      if (memoria[posf][posc] < min_visitas &&
          CasillaValida1R(sensores.superficie[i + 1]) &&
          ViablePorAlturaR(sensores.cota[i + 1] - sensores.cota[0],
                           zapatillas) &&
          sensores.agentes[i + 1] == '_') {
        pos = i + 1;
        min_visitas = memoria[posf][posc];
      }
    } else {
      vector<int> caminos;
      switch (i) {
      case 3:
        caminos = {1};
        break;
      case 4:
        caminos = {2, 1};
        break;
      case 5:
        if (sensores.rumbo % 2 == 0)
          caminos = {2, 1, 3};
        else
          caminos = {2};
        break;
      case 6:
        caminos = {2, 3};
        break;
      case 7:
        caminos = {3};
        break;
      }
      for (int cam : caminos) {

        if (memoria[posf][posc] < min_visitas &&
            CasillaValida1R(sensores.superficie[i + 1]) &&
            CasillaValida1R(sensores.superficie[cam]) &&
            ViablePorAlturaR(sensores.cota[i + 1] - sensores.cota[cam],
                             zapatillas) &&
            ViablePorAlturaR(sensores.cota[cam] - sensores.cota[0],
                             zapatillas) &&
            sensores.agentes[cam] == '_' && sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;

          camino_opcional = cam;
        }
      }
    }
  }
  return pos;
}
int ComportamientoRescatador::CasillainteresanteR1(Sensores sensores) {
  // Calculo si son viables por altura las posibles casillas
  bool i = ViablePorAlturaR(sensores.cota[1] - sensores.cota[0], zapatillas);

  bool c = ViablePorAlturaR(sensores.cota[2] - sensores.cota[0], zapatillas);

  bool d = ViablePorAlturaR(sensores.cota[3] - sensores.cota[0], zapatillas);

  // compruebo los distintos casos segun superficie y altura , si no puedo
  // actuar devuelvo 0

  if (!zapatillas) {
    if (sensores.superficie[2] == 'D' && c && sensores.agentes[2] == '_')
      return 2;
    else if (sensores.superficie[1] == 'D' && i && sensores.agentes[1] == '_')
      return 1;
    else if (sensores.superficie[3] == 'D' && d && sensores.agentes[3] == '_')
      return 3;
  }
  // Calculo la casilla menos pisada de las 3 que veo delante
  int menospisada = MenosPisadaR1(sensores);
  if (menospisada == -1) { // si no encuentro una casilla camino menos pisada
    return 0;
  } else {
    return menospisada;
  }
}
Action
ComportamientoRescatador::ComportamientoRescatadorNivel_1(Sensores sensores) {
  Action action;
  SituarSensorEnMapaR(mapaResultado, mapaCotas, sensores);
  if (sensores.superficie[0] == 'D')
    zapatillas = true;

  if (giroizq != 0) {
    action = TURN_SR;
    giroizq--;

  } else if (extrawalk) {
    action = WALK;
    extrawalk = false;
  } else if (extraturnd) {
    action = TURN_SR;
    extraturnd = false;
  } else if (extraturni) {
    action = TURN_L;
    giroizq = 1;
    extraturni = false;
  } else {
    int pos = CasillainteresanteR1(sensores);

    switch (pos) {
    case 2:
      action = WALK;
      break;
    case 1:
      giroizq = 1;
      action = TURN_L;
      break;
    case 3:
      action = TURN_SR;
      break;
    case 4:
      extrawalk = true;
      action = TURN_L;
      giroizq = 1;
      break;
    case 5:
      if (camino_opcional == 2) {
        action = WALK;
      } else if (camino_opcional == 1) {
        extrawalk = true;
        action = TURN_L;
        extraturnd = true;
        giroizq = 1;
      }

    case 6:
      if (camino_opcional == 2) {
        action = WALK;
      } else if (camino_opcional == 1) {
        extrawalk = true;
        extraturnd = true;
        action = TURN_L;
        giroizq = 1;
      } else if (camino_opcional == 3) {
        action = TURN_SR;
        extrawalk = true;
        extraturni = true;
      }
      break;
    case 7:
      if (camino_opcional == 2) {
        action = WALK;
      } else if (camino_opcional == 3) {
        extrawalk = true;
        extraturni = true;
        action = TURN_SR;
      }
      break;
    case 8:
      action = TURN_SR;
      extrawalk = true;
      break;

    case 0:
			action=TURN_SR;
      break;
    }

    //  if (pos != 0)
    memoria[sensores.posF][sensores.posC]++;
  }
  next_action = action;

  return action;
}

////////////////////////////////////////////////////////////////////////////////
/// codigo para el nivel 2
/// ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

void AnularMatrizR(vector<vector<unsigned char>> &m) {
  for (int i = 0; i < m[0].size(); i++) {
    for (int j = 0; j < m.size(); j++) {
      m[i][j] = 0;
    }
  }
}

void ComportamientoRescatador::VisualizaPlan(const EstadoR &st,
                                             const vector<Action> &plan) {
  AnularMatrizR(mapaConPlan);
  EstadoR cst = st;
  auto it = plan.begin();
  while (it != plan.end()) {
    switch (*it) {
    case RUN:
      switch (cst.brujula) {
      case 0:
        cst.f--;
        break;
      case 1:
        cst.f--;
        cst.c++;
        break;
      case 2:
        cst.c++;
        break;
      case 3:
        cst.f++;
        cst.c++;
        break;
      case 4:
        cst.f++;
        break;
      case 5:
        cst.f++;
        cst.c--;
        break;
      case 6:
        cst.c--;
        break;
      case 7:
        cst.f--;
        cst.c--;
        break;
      }
      mapaConPlan[cst.f][cst.c] = 3;
    case WALK:
      switch (cst.brujula) {
      case 0:
        cst.f--;
        break;
      case 1:
        cst.f--;
        cst.c++;
        break;
      case 2:
        cst.c++;
        break;
      case 3:
        cst.f++;
        cst.c++;
        break;
      case 4:
        cst.f++;
        break;
      case 5:
        cst.f++;
        cst.c--;
        break;
      case 6:
        cst.c--;
        break;
      case 7:
        cst.f--;
        cst.c--;
        break;
      }
      mapaConPlan[cst.f][cst.c] = 1;
      break;
    case TURN_SR:
      cst.brujula = (cst.brujula + 1) % 8;
      break;
    case TURN_L:
      cst.brujula = (cst.brujula + 6) % 8;
      break;
    }
    it++;
  }
}

EstadoR NextCasillaR(const EstadoR &st) {
  EstadoR siguiente = st;
  switch (st.brujula) {
  case norte:
    siguiente.f = st.f - 1;
    break;
  case noreste:
    siguiente.f = st.f - 1;
    siguiente.c = st.c + 1;
    break;
  case este:
    siguiente.c = st.c + 1;
    break;
  case sureste:
    siguiente.f = st.f + 1;
    siguiente.c = st.c + 1;
    break;
  case sur:
    siguiente.f = st.f + 1;
    break;
  case suroeste:
    siguiente.f = st.f + 1;
    siguiente.c = st.c - 1;
    break;
  case oeste:
    siguiente.c = st.c - 1;
    break;
  case noroeste:
    siguiente.f = st.f - 1;
    siguiente.c = st.c - 1;
  }
  return siguiente;
}

bool CasillaAccesibleR(const EstadoR &st,
                       const vector<vector<unsigned char>> &terreno,
                       const vector<vector<unsigned char>> &altura) {
  EstadoR next = NextCasillaR(st);
  bool check1 = false, check2 = false;
  check1 = terreno[next.f][next.c] != 'P' and terreno[next.f][next.c] != 'M' &&
           terreno[next.f][next.c] != 'B';

  check2 = ViablePorAlturaR(altura[next.f][next.c] - altura[st.f][st.c],
                            st.zapatillas);
  return check1 and check2;
}

bool CasillaAccesibleRunR(const EstadoR &st,
                          const vector<vector<unsigned char>> &terreno,
                          const vector<vector<unsigned char>> &altura) {

  EstadoR intermedio = NextCasillaR(st);
  EstadoR final = NextCasillaR(intermedio);
  bool check1 = false, check2 = false, check3 = false;
  check1 = terreno[intermedio.f][intermedio.c] != 'P' and
           terreno[intermedio.f][intermedio.c] != 'M' and
           terreno[intermedio.f][intermedio.c] != 'B';
  check2 = terreno[final.f][final.c] != 'P' and
           terreno[final.f][final.c] != 'M' and
           terreno[final.f][final.c] != 'B';
  check3 = ViablePorAlturaR(altura[final.f][final.c] - altura[st.f][st.c],
                            st.zapatillas);
  return check1 and check2 and check3;
}
EstadoR applyR(Action accion, const EstadoR &st,
               const vector<vector<unsigned char>> &terreno,
               const vector<vector<unsigned char>> &altura) {
  EstadoR next = st;
  EstadoR intermedio;
  switch (accion) {
  case WALK:
    if (CasillaAccesibleR(st, terreno, altura)) {
      next = NextCasillaR(st);
    }
    break;
  case RUN:
    if (CasillaAccesibleRunR(st, terreno, altura)) {
      intermedio = NextCasillaR(st);
      next = NextCasillaR(intermedio);
    }
    break;
  case TURN_SR:
    next.brujula = (next.brujula + 1) % 8;
    break;
  case TURN_L:
    next.brujula = (next.brujula + 6) % 8;
    break;
  }
  return next;
}

void ComportamientoRescatador::PintaPlan(const vector<Action> &plan, bool zap) {
  auto it = plan.begin();
  while (it != plan.end()) {
    if (*it == WALK) {
      cout << "W ";
    } else if (*it == RUN) {
      cout << "R ";
    } else if (*it == TURN_SR) {
      cout << "r ";
    } else if (*it == TURN_L) {
      cout << "L ";
    } else if (*it == CALL_ON) {
      cout << "C ";
    } else if (*it == CALL_OFF) {
      cout << "c ";
    } else if (*it == IDLE) {
      cout << "I ";
    } else {
      cout << "-_ ";
    }
    it++;
  }
  cout << "( longitud " << plan.size();
  if (zap)
    cout << "[Z]";
  cout << ")\n";
}
/*
 * se puede transitar por T , B (si aux tiene zapatillas como camino)  ,A para
 * auxiliar, C ,S
 * */
int CalcularCoste(Action accion, const EstadoR &actual, const EstadoR &destino,
                  const vector<vector<unsigned char>> &terreno,
                  const vector<vector<unsigned char>> &altura) {
  int coste = 0;
  int diferencia = altura[destino.f][destino.c] - altura[actual.f][actual.c];
  int signo;
  if (diferencia == 0) {
    signo = 0;
  } else if (diferencia < 0) {
    signo = -1;
  } else if (diferencia > 0) {
    signo = 1;
  }
  if (accion == WALK) {

    if (terreno[actual.f][actual.c] == 'S')
      coste = 2 + (1 * signo);
    else if (terreno[actual.f][actual.c] == 'T')
      coste = 20 + (5 * signo);
    else if (terreno[actual.f][actual.c] == 'A')
      coste = 100 + (10 * signo);
    else
      coste = 1;
  } else if (accion == TURN_SR) {
    if (terreno[actual.f][actual.c] == 'S')
      coste = 1;
    else if (terreno[actual.f][actual.c] == 'T')
      coste = 3;
    else if (terreno[actual.f][actual.c] == 'A')
      coste = 16;
    else
      coste = 1;
  } else if (accion == TURN_L) {
    if (terreno[actual.f][actual.c] == 'S')
      coste = 1;
    else if (terreno[actual.f][actual.c] == 'T')
      coste = 5;
    else if (terreno[actual.f][actual.c] == 'A')
      coste = 30;
    else
      coste = 1;
  } else if (accion == RUN) {
    if (terreno[actual.f][actual.c] == 'S')
      coste = 3 + (2 * signo);
    else if (terreno[actual.f][actual.c] == 'T')
      coste = 35 + (5 * signo);
    else if (terreno[actual.f][actual.c] == 'A')
      coste = 150 + (15 * signo);
    else
      coste = 1;
  }
  return coste;
}

vector<Action> ComportamientoRescatador::DijkstraRescatador(
    const EstadoR &inicio, const EstadoR &final,
    const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura) {

  // Comparador para la cola de prioridad (menor coste tiene mayor prioridad)
  struct ComparadorCoste {
    bool operator()(const NodoR &a, const NodoR &b) const {
      return a.coste > b.coste;
    }
  };

  priority_queue<NodoR, vector<NodoR>, ComparadorCoste> frontier;

  map<EstadoR, int> mejor_coste;

  NodoR actual;
  actual.estado = inicio;
  actual.coste = 0;
  mejor_coste[actual.estado] = 0;
  if (terreno[actual.estado.f][actual.estado.c] == 'D') {
    actual.estado.zapatillas = true;
  }
  frontier.push(actual);

  while (!frontier.empty()) {
    actual = frontier.top();
    frontier.pop();

    if (actual.estado.f == final.f && actual.estado.c == final.c) {
      return actual.secuencia;
    }


    for (const auto &accion : genera_acciones) {
      NodoR hijo = actual;
      hijo.estado = applyR(accion, actual.estado, terreno, altura);
      if (terreno[hijo.estado.f][hijo.estado.c] == 'D') {
        hijo.estado.zapatillas = true;
      }
      int coste_accion =
          CalcularCoste(accion, actual.estado, hijo.estado, terreno, altura);
      hijo.coste = actual.coste + coste_accion;

      auto it = mejor_coste.find(hijo.estado);
      if (it == mejor_coste.end() || it->second > hijo.coste) {
        mejor_coste[hijo.estado] = hijo.coste;
        hijo.secuencia.push_back(accion);
        frontier.push(hijo);
      }
    }
  }

  return {};
}

Action
ComportamientoRescatador::ComportamientoRescatadorNivel_2(Sensores sensores) {
  Action accion = IDLE;
  if (!hayPlan) {
    // Invocar al método de búsqueda
    EstadoR inicio, fin;
    inicio.f = sensores.posF;
    inicio.c = sensores.posC;
    inicio.brujula = sensores.rumbo;

    if (mapaResultado[inicio.f][inicio.c] == 'D') {
      zapatillas = true;
    }
    inicio.zapatillas = zapatillas;
    fin.f = sensores.destinoF;
    fin.c = sensores.destinoC;
    plan = DijkstraRescatador(inicio, fin, mapaResultado, mapaCotas);
    VisualizaPlan(inicio, plan);
    hayPlan = plan.size() != 0;
  }
  if (hayPlan and plan.size() > 0) {
    accion = plan.front();
    plan.erase(plan.begin());
  }
  if (plan.size() == 0) {
    hayPlan = false;
  }
  return accion;
}
////////////////////////////////////////////////////////////////////////////////
/// codigo para el nivel 4
/// ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

bool CasillaAccesibleR4(const EstadoR &st,
                        const vector<vector<unsigned char>> &terreno,
                        const vector<vector<unsigned char>> &altura,
                        const vector<vector<unsigned char>> &agentes) {
  EstadoR next = NextCasillaR(st);
  bool check1 = false, check2 = false, check3 = false;
  check1 = terreno[next.f][next.c] != 'P' and terreno[next.f][next.c] != 'M' &&
           terreno[next.f][next.c] != 'B';

  check2 = ViablePorAlturaR(altura[next.f][next.c] - altura[st.f][st.c],
                            st.zapatillas);
  check3 = agentes[next.f][next.c] == '?' || agentes[next.f][next.c] == '_';
  if (terreno[next.f][next.c] == '?') {
    return check1;
  } else {
    return check1 && check2 && check3;
  }
}

bool CasillaAccesibleRunR4(const EstadoR &st,
                           const vector<vector<unsigned char>> &terreno,
                           const vector<vector<unsigned char>> &altura,
                           const vector<vector<unsigned char>> &agentes) {

  EstadoR intermedio = NextCasillaR(st);
  EstadoR final = NextCasillaR(intermedio);
  bool check1 = false, check2 = false, check3 = false, check4 = false;
  check1 = terreno[intermedio.f][intermedio.c] != 'P' and
           terreno[intermedio.f][intermedio.c] != 'M' and
           terreno[intermedio.f][intermedio.c] != 'B';
  check2 = terreno[final.f][final.c] != 'P' and
           terreno[final.f][final.c] != 'M' and
           terreno[final.f][final.c] != 'B';
  check3 = ViablePorAlturaR(altura[final.f][final.c] - altura[st.f][st.c],
                            st.zapatillas);

  check4 =
      (agentes[intermedio.f][intermedio.c] == '?' ||
       agentes[intermedio.f][intermedio.c] == '_') &&
      (agentes[final.f][final.c] == '?' || agentes[final.f][final.c] == '_');
  if (terreno[final.f][final.c] == '?') {
    return check1 and check2 and check4;

  } else {
    return check1 and check2 and check3 and check4;
  }
}
EstadoR applyR4(Action accion, const EstadoR &st,
                const vector<vector<unsigned char>> &terreno,
                const vector<vector<unsigned char>> &altura,
                const vector<vector<unsigned char>> &agentes) {
  EstadoR next = st;
  EstadoR intermedio;
  switch (accion) {
  case WALK:
    if (CasillaAccesibleR4(st, terreno, altura, agentes)) {
      next = NextCasillaR(st);
    }
    break;
  case RUN:
    if (CasillaAccesibleRunR4(st, terreno, altura, agentes)) {
      intermedio = NextCasillaR(st);
      next = NextCasillaR(intermedio);
    }
    break;
  case TURN_SR:
    next.brujula = (next.brujula + 1) % 8;
    break;
  case TURN_L:
    next.brujula = (next.brujula + 6) % 8;
    break;
  }
  return next;
}

int CalcularCosteR4(Action accion, const EstadoR &actual,
                    const EstadoR &destino,
                    const vector<vector<unsigned char>> &terreno,
                    const vector<vector<unsigned char>> &altura) {
  int coste = 0;
  int diferencia = altura[destino.f][destino.c] - altura[actual.f][actual.c];
  int signo;
  if (diferencia == 0) {
    signo = 0;
  } else if (diferencia < 0) {
    signo = -1;
  } else if (diferencia > 0) {
    signo = 1;
  }
  if (accion == WALK) {

    if (terreno[actual.f][actual.c] == 'S')
      coste = 2 + (1 * signo);
    else if (terreno[actual.f][actual.c] == 'T')
      coste = 20 + (5 * signo);
    else if (terreno[actual.f][actual.c] == 'A')
      coste = 100 + (10 * signo);
    else if (terreno[actual.f][actual.c] == '?')
      coste = 5;

    else
      coste = 1;
  } else if (accion == TURN_SR) {
    if (terreno[actual.f][actual.c] == 'S')
      coste = 1;
    else if (terreno[actual.f][actual.c] == 'T')
      coste = 3;
    else if (terreno[actual.f][actual.c] == 'A')
      coste = 16;
    else if (terreno[actual.f][actual.c] == '?')
      coste = 2;

    else
      coste = 1;
  } else if (accion == TURN_L) {
    if (terreno[actual.f][actual.c] == 'S')
      coste = 1;
    else if (terreno[actual.f][actual.c] == 'T')
      coste = 5;
    else if (terreno[actual.f][actual.c] == 'A')
      coste = 30;
    else if (terreno[actual.f][actual.c] == '?')
      coste = 2;

    else
      coste = 1;
  } else if (accion == RUN) {
    if (terreno[actual.f][actual.c] == 'S')
      coste = 3 + (2 * signo);
    else if (terreno[actual.f][actual.c] == 'T')
      coste = 35 + (5 * signo);
    else if (terreno[actual.f][actual.c] == 'A')
      coste = 150 + (15 * signo);
    else if (terreno[actual.f][actual.c] == '?')
      coste = 3;

    else
      coste = 1;
  }
  return coste;
}

void SituarSensorEnMapaR4(vector<vector<unsigned char>> &m,
                          vector<vector<unsigned char>> &a,
                          vector<vector<unsigned char>> &e, Sensores sensores) {
  // Asignar la posición actual
  a[sensores.posF][sensores.posC] = sensores.cota[0];
  m[sensores.posF][sensores.posC] = sensores.superficie[0];
  e[sensores.posF][sensores.posC] = sensores.agentes[0];

  // Iterar sobre los sensores (índices 1 a 15)
  for (int i = 0; i < 15; ++i) {
    int fila = sensores.posF + posiciones_vision[sensores.rumbo][i].first;
    int col = sensores.posC + posiciones_vision[sensores.rumbo][i].second;

    m[fila][col] = sensores.superficie[i + 1];
    a[fila][col] = sensores.cota[i + 1];
    e[fila][col] = sensores.agentes[i + 1];
  }
}

int HeuristicaAestrella(const EstadoR &a, const EstadoR &b) {
  return std::max(abs(a.f - b.f), abs(a.c - b.c));
}

vector<Action> ComportamientoRescatador::AestrellaR4(
    const EstadoR &inicio, const EstadoR &final,
    const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura,
    const vector<vector<unsigned char>> &agentes) {

  struct ComparadorCoste {
    EstadoR final;

    ComparadorCoste(EstadoR final) : final(final) {}

    bool operator()(const NodoR &a, const NodoR &b) const {
      return a.coste + HeuristicaAestrella(a.estado, final) >
             b.coste + HeuristicaAestrella(b.estado, final);
    }
  };
  priority_queue<NodoR, std::vector<NodoR>, ComparadorCoste> frontier(
      (ComparadorCoste(final)));

  map<EstadoR, int> mejor_coste;

  NodoR actual;
  actual.estado = inicio;
  actual.coste = 0;
  actual.secuencia = {};
  mejor_coste[actual.estado] = 0;

  if (terreno[actual.estado.f][actual.estado.c] == 'D') {
    actual.estado.zapatillas = true;
  }
  if (actual.estado.f == final.f && actual.estado.c == final.c) {
    cout << "sale" << endl;
    return actual.secuencia;
  }
  frontier.push(actual);
  while (!frontier.empty()) {
    actual = frontier.top();
    frontier.pop();
    if (actual.estado.f == final.f && actual.estado.c == final.c) {
      return actual.secuencia;
    }

    for (const auto &accion : genera_acciones) {
      NodoR hijo = actual;
      hijo.estado = applyR4(accion, actual.estado, terreno, altura, agentes);
      if (terreno[hijo.estado.f][hijo.estado.c] == 'D') {
        hijo.estado.zapatillas = true;
      }

      int coste_accion =
          CalcularCosteR4(accion, actual.estado, hijo.estado, terreno, altura);
      hijo.coste = actual.coste + coste_accion;


      auto it = mejor_coste.find(hijo.estado);
      if (it == mejor_coste.end() || it->second > hijo.coste) {
        mejor_coste[hijo.estado] = hijo.coste;
        hijo.secuencia.push_back(accion);
        frontier.push(hijo);
      }
    }
  }

  return {};
}
bool Valida4R(int dif, char c, bool zap) {
  bool check1 = false, check2 = false;
  check1 = c != 'P' and c != 'M' and c != 'B';
  check2 = ViablePorAlturaR(dif, zap);
  return check1 && check2;
}
Action ComportamientoRescatador::ComportamientoRescatadorNivel_1mod4(
    Sensores sensores) {
  Action action;
  SituarSensorEnMapaR(mapaResultado, mapaCotas, sensores);

  if (sensores.superficie[0] == 'D')
    zapatillas = true;

  if (giroizq != 0) {
    action = TURN_SR;
    giroizq--;
  } else {
    int pos = 0;
    if (Valida4R(sensores.cota[0] - sensores.cota[2], sensores.superficie[2],
                 zapatillas) &&
        sensores.agentes[2] == '_') {
      pos = 2;
    } else if (Valida4R(sensores.cota[0] - sensores.cota[1],
                        sensores.superficie[1], zapatillas) &&
               sensores.agentes[1] == '_') {
      pos = 1;
    } else if (Valida4R(sensores.cota[0] - sensores.cota[1],
                        sensores.superficie[1], zapatillas) &&
               sensores.agentes[3] == '_') {
      pos = 3;
    }

    switch (pos) {
    case 2:
      action = WALK;
      break;
    case 1:
      giroizq = 1;
      action = TURN_SR;
      break;
    case 3:
      action = TURN_SR;
    case 0:
      action = TURN_SR;
      break;
    }
  }
  return action;
}
void ComportamientoRescatador::ResetEntidades() {
  for (int i = 0; i < mapaEntidades.size(); i++) {
    for (int j = 0; j < mapaEntidades.size(); j++) {
      mapaEntidades[i][j] = '?';
    }
  }
}
Action
ComportamientoRescatador::ComportamientoRescatadorNivel_4(Sensores sensores) {
  SituarSensorEnMapaR4(mapaResultado, mapaCotas, mapaEntidades, sensores);
  Action accion = IDLE;
  if (sensores.superficie[0] == 'X' && sensores.energia < 2000) {
    return IDLE;
  }

  if (sensores.posF == sensores.destinoF &&
      sensores.posC == sensores.destinoC) {
    if (extraturnd) {
      return IDLE;
    }
    if (sensores.gravedad) {
      accion = CALL_ON;
      sensores.venpaca = true;
      extraturnd = true;
    }
  } else {
    extraturnd = false;
    if (!hayPlan) {
      EstadoR inicio, fin;
      inicio.f = sensores.posF;
      inicio.c = sensores.posC;
      inicio.brujula = sensores.rumbo;
      if (mapaResultado[inicio.f][inicio.c] == 'D') {
        zapatillas = true;
      }
      inicio.zapatillas = zapatillas;
      fin.f = sensores.destinoF;
      fin.c = sensores.destinoC;
      plan = AestrellaR4(inicio, fin, mapaResultado, mapaCotas, mapaEntidades);
      VisualizaPlan(inicio, plan);
      hayPlan = plan.size() != 0;
    }
    if (hayPlan and plan.size() > 0) {

      accion = plan.front();
      if (accion == WALK &&
          !ViablePorAlturaR(sensores.cota[0] - sensores.cota[2], zapatillas)) {
        accion = ComportamientoRescatadorNivel_1mod4(sensores);
        hayPlan = false;
      }
      plan.erase(plan.begin());
    }
    if (plan.size() == 0 || sensores.choque) {
      ResetEntidades();
      hayPlan = false;
      if (sensores.superficie[0] == 'D')
        zapatillas = true;
      accion = ComportamientoRescatadorNivel_1mod4(sensores);
    }
  }
  return accion;
}
