#include "../Comportamientos_Jugador/auxiliar.hpp"
#include "comportamientos/comportamiento.hpp"
#include "motorlib/util.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <vector>

const vector<Action> genera_acciones = {WALK, TURN_SR};
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

int ComportamientoAuxiliar::interact(Action accion, int valor) { return 0; }
Action ComportamientoAuxiliar::think(Sensores sensores) {
  Action accion = IDLE;

  switch (sensores.nivel) {
  case 0:
    accion = ComportamientoAuxiliarNivel_0(sensores);
    break;
  case 1:
    accion = ComportamientoAuxiliarNivel_1(sensores);
    break;
  case 2:
    accion = ComportamientoAuxiliarNivel_2(sensores);
    break;
  case 3:
    accion = ComportamientoAuxiliarNivel_3(sensores);
    break;
  case 4:
    // accion = ComportamientoAuxiliarNivel_4 (sensores);
    break;
  }

  return accion;
}

/*
 * completa la matriz memoria del agente , sirve
 * para recordar todas las superficies y cotas que va teniendo
 * en su rango de vision .
 */
void SituarSensorEnMapaA(vector<vector<unsigned char>> &m,
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
bool ViablePorAlturaA(int dif) { return abs(dif) <= 1; }

/*
 * Busca la casilla menos pisada entre las
 * tres primeras opciones del rango de vision
 */
/*
int ComportamientoAuxiliar::MenosPisadaA(Sensores sensores) {
  int min_visitas = INFINITY;
  int pos = -1;
  int indice_rumbo = sensores.rumbo;
  vector<int> orden = {1, 0, 2, 5, 4, 6, 3, 7,1};
  for (int i : orden) {
    int posf = sensores.posF + posiciones_vision[indice_rumbo][i].first;
    int posc = sensores.posC + posiciones_vision[indice_rumbo][i].second;
    if (i <= 2) {
      if (memoria[posf][posc] < min_visitas &&
          (sensores.superficie[i + 1] == 'C' ||
           sensores.superficie[i + 1] == 'D') &&
          sensores.agentes[i + 1] == '_' &&
          ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0])) {
        pos = i + 1;
        min_visitas = memoria[posf][posc];
      }
    } else {
      if (memoria[posf][posc] < min_visitas &&(sensores.superficie[i + 1] == 'C'
|| sensores.superficie[i + 1] == 'D')) { pos = i + 1; min_visitas =
memoria[posf][posc];
      }
    }
  }
  return pos;
}
*/

bool CasillaValida0A(char a) { return a == 'C' || a == 'D'; }

int ComportamientoAuxiliar::MenosPisadaA(Sensores sensores) {
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
          CasillaValida0A(sensores.superficie[i + 1]) &&
          ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
          sensores.agentes[i + 1] == '_') {
        pos = i + 1;
        min_visitas = memoria[posf][posc];
      }
    } else {
      switch (i) {
      case 3:
        if (memoria[posf][posc] < min_visitas &&
            CasillaValida0A(sensores.superficie[i + 1]) &&
            CasillaValida0A(sensores.superficie[1]) &&
            ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
            ViablePorAlturaA(sensores.cota[1] - sensores.cota[0]) &&

            sensores.agentes[1] == '_' && sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;
          camino_opcional = 0;
        }
        break;
      case 4:

        if (memoria[posf][posc] < min_visitas &&
            CasillaValida0A(sensores.superficie[i + 1]) &&
            CasillaValida0A(sensores.superficie[2]) &&
            ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
            ViablePorAlturaA(sensores.cota[2] - sensores.cota[0]) &&
            sensores.agentes[2] == '_' && sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;
          camino_opcional = 2;
        } else if (memoria[posf][posc] < min_visitas &&
                   CasillaValida0A(sensores.superficie[i + 1]) &&
                   CasillaValida0A(sensores.superficie[1]) &&
                   ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
                   ViablePorAlturaA(sensores.cota[1] - sensores.cota[0]) &&
                   sensores.agentes[1] == '_' &&
                   sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;
          camino_opcional = 1;
        }

        break;
      case 5:
        if (memoria[posf][posc] < min_visitas &&
            CasillaValida0A(sensores.superficie[i + 1]) &&
            CasillaValida0A(sensores.superficie[2]) &&
            ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
            ViablePorAlturaA(sensores.cota[2] - sensores.cota[0]) &&
            sensores.agentes[2] == '_' && sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;
          camino_opcional = 2;
        } else if (memoria[posf][posc] < min_visitas &&
                   CasillaValida0A(sensores.superficie[i + 1]) &&
                   CasillaValida0A(sensores.superficie[1]) &&
                   ViablePorAlturaA(sensores.cota[1] - sensores.cota[0]) &&
                   ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
                   sensores.agentes[1] == '_' &&
                   sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;
          camino_opcional = 1;
        } else if (memoria[posf][posc] < min_visitas &&
                   CasillaValida0A(sensores.superficie[i + 1]) &&
                   CasillaValida0A(sensores.superficie[3]) &&
                   ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
                   ViablePorAlturaA(sensores.cota[3] - sensores.cota[0]) &&
                   sensores.agentes[3] == '_' &&
                   sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;
          camino_opcional = 3;
        }
        break;
      case 6:

        if (memoria[posf][posc] < min_visitas &&
            CasillaValida0A(sensores.superficie[i + 1]) &&
            CasillaValida0A(sensores.superficie[2]) &&
            ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
            ViablePorAlturaA(sensores.cota[2] - sensores.cota[0]) &&
            sensores.agentes[2] == '_' && sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;
          camino_opcional = 2;
        } else if (memoria[posf][posc] < min_visitas &&
                   CasillaValida0A(sensores.superficie[i + 1]) &&
                   CasillaValida0A(sensores.superficie[3]) &&
                   ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
                   ViablePorAlturaA(sensores.cota[3] - sensores.cota[0]) &&
                   sensores.agentes[3] == '_' &&
                   sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;
          camino_opcional = 3;
        }
        break;
      case 7:
        if (memoria[posf][posc] < min_visitas &&
            CasillaValida0A(sensores.superficie[i + 1]) &&
            CasillaValida0A(sensores.superficie[3]) &&
            ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
            ViablePorAlturaA(sensores.cota[3] - sensores.cota[0]) &&
            sensores.agentes[3] == '_' && sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;
          camino_opcional = 0;
        }

        break;
      case 8:
        break;
      }
    }
  }
  return pos;
}

/*
 * funcion que determina que casilla debe tomar como interesante
 * el agente , posteriormente se movera n esa direccion
 */

int ComportamientoAuxiliar::CasillainteresanteA(Sensores sensores) {
  // Calculo si son viables por altura las posibles casillas
  bool i = ViablePorAlturaA(sensores.cota[1] - sensores.cota[0]);

  bool c = ViablePorAlturaA(sensores.cota[2] - sensores.cota[0]);

  bool d = ViablePorAlturaA(sensores.cota[3] - sensores.cota[0]);

  // compruebo los distintos casos segun superficie y altura , si no puedo
  // actuar devuelvo 0

  if (sensores.superficie[2] == 'X' && c && sensores.agentes[2] == '_')
    return 2;
  else if (sensores.superficie[1] == 'X' && i && sensores.agentes[1] == '_')
    return 1;
  else if (sensores.superficie[3] == 'X' && d && sensores.agentes[3] == '_')
    return 3;
  else if (!zapatillas) {
    if (sensores.superficie[2] == 'D' && c)
      return 2;
    else if (sensores.superficie[1] == 'D' && i)
      return 1;
    else if (sensores.superficie[3] == 'D' && d)
      return 3;
  }
  // Calculo la casilla menos pisada de las 3 que veo delante
  int menospisada = MenosPisadaA(sensores);
  if (menospisada == -1) { // si no encuentro una casilla camino menos pisada
    return 0;
  } else {
    return menospisada;
  }
}

Action
ComportamientoAuxiliar::ComportamientoAuxiliarNivel_0(Sensores sensores) {
  // El comportamiento de seguir un camino hasta encontrar un puesto base.

  Action action;
  SituarSensorEnMapaA(mapaResultado, mapaCotas, sensores);

  if (sensores.superficie[0] == 'D')
    zapatillas = true;

  if (sensores.superficie[0] == 'X') {
    action = IDLE;
  } else if (giroizq != 0) {
    action = TURN_SR;
    giroizq--;
  } else if (extrawalk) {
    action = WALK;
    extrawalk = false;
  } else if (extraturnd) {
    action = TURN_SR;
    extraturnd = false;
  } else if (extraturni) {
    action = TURN_SR;
    giroizq = 6;
    extraturni = false;
  } else {

    int pos = CasillainteresanteA(sensores);
    switch (pos) {
    case 2:
      action = WALK;
      break;
    case 1:
      giroizq = 6;
      action = TURN_SR;
      break;
    case 3:
      action = TURN_SR;
      break;
    case 4:
      extrawalk = true;
      action = TURN_SR;
      giroizq = 6;
      break;
    case 5:
      if (camino_opcional == 2) {
        action = WALK;
      } else if (camino_opcional == 1) {
        extrawalk = true;
        action = TURN_SR;
        giroizq = 6;
      }

    case 6:
      if (camino_opcional == 2) {
        action = WALK;
      } else if (camino_opcional == 1) {
        extrawalk = true;
        extraturnd = true;
        action = TURN_SR;
        giroizq = 6;
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
    // if (pos != 0)
    memoria[sensores.posF][sensores.posC]++;
  }

  last_action = action;

  return action;
}

///////////////////////////////////////////////////////////////////////////////
/// codigo para el nivel 1 ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/*
 * Busca la casilla menos pisada entre las
 * tres primeras opciones del rango de vision
 */
bool CasillaValida1A(char a) {
  return a == 'C' || a == 'D' || a == 'S' || a == 'X';
}

int ComportamientoAuxiliar::MenosPisadaA1(Sensores sensores) {
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
          CasillaValida1A(sensores.superficie[i + 1]) &&
          ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
          sensores.agentes[i + 1] == '_') {
        pos = i + 1;
        min_visitas = memoria[posf][posc];
      }
    } else {
      switch (i) {
      case 3:
        if (memoria[posf][posc] < min_visitas &&
            CasillaValida1A(sensores.superficie[i + 1]) &&
            CasillaValida1A(sensores.superficie[1]) &&
            ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
            ViablePorAlturaA(sensores.cota[1] - sensores.cota[0]) &&

            sensores.agentes[1] == '_' && sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;
          camino_opcional = 0;
        }
        break;
      case 4:

        if (memoria[posf][posc] < min_visitas &&
            CasillaValida1A(sensores.superficie[i + 1]) &&
            CasillaValida1A(sensores.superficie[2]) &&
            ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
            ViablePorAlturaA(sensores.cota[2] - sensores.cota[0]) &&
            sensores.agentes[2] == '_' && sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;
          camino_opcional = 2;
        } else if (memoria[posf][posc] < min_visitas &&
                   CasillaValida1A(sensores.superficie[i + 1]) &&
                   CasillaValida1A(sensores.superficie[1]) &&
                   ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
                   ViablePorAlturaA(sensores.cota[1] - sensores.cota[0]) &&
                   sensores.agentes[1] == '_' &&
                   sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;
          camino_opcional = 1;
        }

        break;
      case 5:
        if (memoria[posf][posc] < min_visitas &&
            CasillaValida1A(sensores.superficie[i + 1]) &&
            CasillaValida1A(sensores.superficie[2]) &&
            ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
            ViablePorAlturaA(sensores.cota[2] - sensores.cota[0]) &&
            sensores.agentes[2] == '_' && sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;
          camino_opcional = 2;
        } else if (memoria[posf][posc] < min_visitas &&
                   CasillaValida1A(sensores.superficie[i + 1]) &&
                   CasillaValida1A(sensores.superficie[1]) &&
                   ViablePorAlturaA(sensores.cota[1] - sensores.cota[0]) &&
                   ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
                   sensores.agentes[1] == '_' &&
                   sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;
          camino_opcional = 1;
        } else if (memoria[posf][posc] < min_visitas &&
                   CasillaValida1A(sensores.superficie[i + 1]) &&
                   CasillaValida1A(sensores.superficie[3]) &&
                   ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
                   ViablePorAlturaA(sensores.cota[3] - sensores.cota[0]) &&
                   sensores.agentes[3] == '_' &&
                   sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;
          camino_opcional = 3;
        }
        break;
      case 6:

        if (memoria[posf][posc] < min_visitas &&
            CasillaValida1A(sensores.superficie[i + 1]) &&
            CasillaValida1A(sensores.superficie[2]) &&
            ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
            ViablePorAlturaA(sensores.cota[2] - sensores.cota[0]) &&
            sensores.agentes[2] == '_' && sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;
          camino_opcional = 2;
        } else if (memoria[posf][posc] < min_visitas &&
                   CasillaValida1A(sensores.superficie[i + 1]) &&
                   CasillaValida1A(sensores.superficie[3]) &&
                   ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
                   ViablePorAlturaA(sensores.cota[3] - sensores.cota[0]) &&
                   sensores.agentes[3] == '_' &&
                   sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;
          camino_opcional = 3;
        }
        break;
      case 7:
        if (memoria[posf][posc] < min_visitas &&
            CasillaValida1A(sensores.superficie[i + 1]) &&
            CasillaValida1A(sensores.superficie[3]) &&
            ViablePorAlturaA(sensores.cota[i + 1] - sensores.cota[0]) &&
            ViablePorAlturaA(sensores.cota[3] - sensores.cota[0]) &&
            sensores.agentes[3] == '_' && sensores.agentes[i + 1] == '_') {
          min_visitas = memoria[posf][posc];
          pos = i + 1;
          camino_opcional = 0;
        }

        break;
      case 8:
        break;
      }
    }
  }
  return pos;
}
/*
 * funcion casilla interesante para el nivel 1
 */

int ComportamientoAuxiliar::CasillainteresanteA1(Sensores sensores) {
  // Calculo si son viables por altura las posibles casillas
  bool i = ViablePorAlturaA(sensores.cota[1] - sensores.cota[0]);

  bool c = ViablePorAlturaA(sensores.cota[2] - sensores.cota[0]);

  bool d = ViablePorAlturaA(sensores.cota[3] - sensores.cota[0]);

  // compruebo los distintos casos segun superficie y altura , si no puedo
  // actuar devuelvo 0

  if (!zapatillas) {
    if (sensores.superficie[2] == 'D' && c)
      return 2;
    else if (sensores.superficie[1] == 'D' && i)
      return 1;
    else if (sensores.superficie[3] == 'D' && d)
      return 3;
  }
  // Calculo la casilla menos pisada de las 3 que veo delante
  int menospisada = MenosPisadaA1(sensores);
  if (menospisada == -1) { // si no encuentro una casilla camino menos pisada
    return 0;
  } else {
    return menospisada;
  }
}
Action
ComportamientoAuxiliar::ComportamientoAuxiliarNivel_1(Sensores sensores) {
  Action action;
  SituarSensorEnMapaA(mapaResultado, mapaCotas, sensores);

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
    action = TURN_SR;
    giroizq = 6;
    extraturni = false;
  } else {

    int pos = CasillainteresanteA1(sensores);
    switch (pos) {
    case 2:
      action = WALK;
      break;
    case 1:
      giroizq = 6;
      action = TURN_SR;
      break;
    case 3:
      action = TURN_SR;
      break;
    case 4:
      extrawalk = true;
      action = TURN_SR;
      giroizq = 6;
      break;
    case 5:
      if (camino_opcional == 2) {
        action = WALK;
      } else if (camino_opcional == 1) {
        extrawalk = true;
        action = TURN_SR;
        giroizq = 6;
      }

    case 6:
      if (camino_opcional == 2) {
        action = WALK;
      } else if (camino_opcional == 1) {
        extrawalk = true;
        extraturnd = true;
        action = TURN_SR;
        giroizq = 6;
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
    // if (pos != 0)
    memoria[sensores.posF][sensores.posC]++;
  }

  last_action = action;

  return action;
}

////////////////////////////////////////////////////////////////////////////////
/// codigo para el nivel 2 ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

list<Action> CrearPlan(Sensores sensores) {}

Action
ComportamientoAuxiliar::ComportamientoAuxiliarNivel_2(Sensores sensores) {
  Action action = IDLE;
  if (!hayPlan) {
    hayPlan = true;
  }
  if (hayPlan && plan.size() > 0) {
    action = plan.front();
    plan.pop_front();
  }
  if (plan.size() == 0) {
    hayPlan = false;
  }
  return action;
}

////////////////////////////////////////////////////////////////////////////////
/// codigo para el nivel 3 ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

void AnularMatrizA(vector<vector<unsigned char>> &m) {
  for (int i = 0; i < m[0].size(); i++) {
    for (int j = 0; j < m.size(); j++) {
      m[i][j] = 0;
    }
  }
}

void ComportamientoAuxiliar::VisualizaPlan(const EstadoA &st,
                                           const list<Action> &plan) {
  AnularMatrizA(mapaConPlan);
  EstadoA cst = st;
  auto it = plan.begin();
  while (it != plan.end()) {
    switch (*it) {
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
      mapaConPlan[cst.f][cst.c] = 2;
      break;
    case TURN_SR:
      cst.brujula = (cst.brujula + 1) % 8;
      break;
    }
    it++;
  }
}
EstadoA NextCasillaA(const EstadoA &st) {
  EstadoA siguiente = st;
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
bool CasillaAccesibleA(const EstadoA &st,
                              const vector<vector<unsigned char>> &terreno,
                              const vector<vector<unsigned char>> &altura) {
  EstadoA next = NextCasillaA(st);
  bool check1 = false, check2 = false, check3 = false;
  check1 = terreno[next.f][next.c] != 'P' and terreno[next.f][next.c] != 'M';
  check2 = terreno[next.f][next.c] != 'B' or
           (terreno[next.f][next.c] == 'B' and st.zapatillas);
  check3 = abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 1;
  return check1 and check2 and check3;
}
EstadoA applyA(Action accion, const EstadoA &st,
               const vector<vector<unsigned char>> &terreno,
               const vector<vector<unsigned char>> &altura) {
  EstadoA next = st;
  switch (accion) {
  case WALK:
    if (CasillaAccesibleA(st, terreno, altura)) {
      next = NextCasillaA(st);
    }
    break;
  case TURN_SR:
    next.brujula = (next.brujula + 1) % 8;
    break;
  }
  return next;
}
void ComportamientoAuxiliar::PintaPlan(const list<Action> &plan, bool zap) {
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
int CalcularCosteA(Action accion, const EstadoA &actual, const EstadoA &destino,
                   const vector<vector<unsigned char>> &terreno,
                   const vector<vector<unsigned char>> &altura) {
  int coste = 0;
  int diferencia = altura[destino.f][destino.c] - altura[actual.f][destino.c];
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
  }
  return coste;
}

int HeuristicaAestrella(const EstadoA &a, const EstadoA &b) {
  return std::max(abs(a.f - b.f), abs(a.c - b.c));
}

list<Action> ComportamientoAuxiliar::AestrellaA(
    const EstadoA &inicio, const EstadoA &final,
    const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura) {

  struct ComparadorCoste {
    EstadoA final;

    ComparadorCoste(EstadoA final) : final(final) {}

    bool operator()(const NodoA &a, const NodoA &b) const {
      return a.coste + HeuristicaAestrella(a.estado, final) >
             b.coste + HeuristicaAestrella(b.estado, final);
    }
  };
  priority_queue<NodoA, std::vector<NodoA>, ComparadorCoste> frontier(
      (ComparadorCoste(final)));

  // Mapa para registrar el menor coste encontrado para cada estado
  map<EstadoA, int> mejor_coste;

  // Nodo inicial
  NodoA actual;
  actual.estado = inicio;
  actual.coste = 0;
  frontier.push(actual);
  mejor_coste[actual.estado] = 0;
  if (terreno[actual.estado.f][actual.estado.c] == 'D') {
    actual.estado.zapatillas = true;
  }

  frontier.push(actual);
  while (!frontier.empty()) {
    // Extraemos el nodo con menor coste
    actual = frontier.top();
    frontier.pop();

    // Si llegamos al estado final, devolvemos la secuencia de acciones
    if (actual.estado.f == final.f && actual.estado.c == final.c) {
      return actual.secuencia;
    }

    if (terreno[actual.estado.f][actual.estado.c] == 'D') {
      actual.estado.zapatillas = true;
    }

    // Generamos los nodos hijos para cada acción posible
    for (const auto &accion : genera_acciones) {
      NodoA hijo;
      hijo.secuencia = actual.secuencia;
      hijo.coste = actual.coste;
      hijo.estado = actual.estado;
      hijo.estado = applyA(accion, actual.estado, terreno, altura);
      if (terreno[hijo.estado.f][hijo.estado.c] == 'D') {
        hijo.estado.zapatillas = true;
      }
      // Calculamos el coste de la acción
      double coste_accion =
          CalcularCosteA(accion, actual.estado, hijo.estado, terreno, altura);
      hijo.coste = actual.coste + coste_accion;

      // Si encontramos un camino más barato al estado hijo, lo procesamos
      auto it = mejor_coste.find(hijo.estado);
      if (it == mejor_coste.end() || it->second > hijo.coste) {
        mejor_coste[hijo.estado] = hijo.coste;
        hijo.secuencia.push_back(accion);
        frontier.push(hijo);
      }
    }
  }

  // Si no se encuentra solución, devolvemos una lista vacía
  return {};
}

/*
list<Action> ComportamientoAuxiliar::AestrellaA(
    const EstadoA &inicio, const EstadoA &final,
    const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura) {

  struct ComparadorCoste {
    EstadoA final;

    ComparadorCoste(EstadoA final) : final(final) {}

    bool operator()(const NodoA &a, const NodoA &b) const {
      return a.coste + HeuristicaAestrella(a.estado, final) >
             b.coste + HeuristicaAestrella(b.estado, final);
    }
  };
  priority_queue<NodoA, std::vector<NodoA>, ComparadorCoste> frontier(
      (ComparadorCoste(final)));

  map<EstadoA, int> explorados;
  NodoA inicial;
  inicial.estado = inicio;
  inicial.coste = 0;
  frontier.push(inicial);
  explorados[inicio] = 0;


  while (!frontier.empty()) {

    // Selecciono el nodo no explorado con menor coste

    NodoA actual = frontier.top();
    frontier.pop();
    explorados[actual.estado] = actual.coste;

    if (actual.estado.f == final.f && actual.estado.c == final.c) {
      return actual.secuencia;
    }

    if (terreno[actual.estado.f][actual.estado.c] == 'D')
      actual.estado.zapatillas = true;

    for (int i = 0; i < genera_acciones.size(); i++) {
      NodoA hijo = actual;
      hijo.estado = applyA(genera_acciones[i], actual.estado, terreno, altura);
      int coste_accion = CalcularCosteA(genera_acciones[i], actual.estado,
                                        hijo.estado, terreno, altura);
      hijo.coste = hijo.coste + coste_accion;
      hijo.secuencia.push_back(genera_acciones[i]);

      if (explorados.find(hijo.estado) == explorados.end() ||
          (hijo.coste < explorados[hijo.estado])) {
        explorados[hijo.estado] = hijo.coste;
        frontier.push(hijo);
      }
    }
  }

  return {};
}
*/
Action
ComportamientoAuxiliar::ComportamientoAuxiliarNivel_3(Sensores sensores) {
  Action accion = IDLE;
  if (!hayPlan) {
    // Invocar al método de búsqueda
    EstadoA inicio, fin;
    inicio.f = sensores.posF;
    inicio.c = sensores.posC;
    inicio.brujula = sensores.rumbo;
    inicio.zapatillas = zapatillas;
    fin.f = sensores.destinoF;
    fin.c = sensores.destinoC;
    plan = AestrellaA(inicio, fin, mapaResultado, mapaCotas);
    VisualizaPlan(inicio, plan);
    hayPlan = plan.size() != 0;
  }
  if (hayPlan and plan.size() > 0) {
    accion = plan.front();
    plan.pop_front();
  }
  if (plan.size() == 0) {
    hayPlan = false;
  }
  return accion;
}

////////////////////////////////////////////////////////////////////////////////
/// codigo para el nivel 4 ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
Action
ComportamientoAuxiliar::ComportamientoAuxiliarNivel_4(Sensores sensores) {}
