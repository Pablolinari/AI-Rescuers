#ifndef COMPORTAMIENTORESCATADOR_H
#define COMPORTAMIENTORESCATADOR_H

#include "comportamientos/comportamiento.hpp"
#include <chrono>
#include <cstddef>
#include <list>
#include <regex>
#include <thread>
#include <time.h>
#include <utility>
#include <vector>
struct EstadoR {
  int f;
  int c;
  int brujula;
  bool zapatillas;
  bool operator==(const EstadoR &st) const {
    return f == st.f && c == st.c && brujula == st.brujula &&
           zapatillas == st.zapatillas;
  }
  bool operator<(const EstadoR &st) const {
    if (f < st.f)
      return true;
    else if (f == st.f && c < st.c)
      return true;
    else if (f == st.f && c == st.c && brujula < st.brujula)
      return true;
    else if (f == st.f && c == st.c && brujula == st.brujula &&
             zapatillas < st.zapatillas)
      return true;
    else
      return false;
  }
  EstadoR &operator=(const EstadoR &st) {
    if (this != &st) {
      f = st.f;
      c = st.c;
      brujula = st.brujula;
      zapatillas = st.zapatillas;
    }
    return *this;
  }
};
struct NodoR {
  EstadoR estado;
  vector<Action> secuencia;
  int coste;
  NodoR() {
    estado = EstadoR();
    coste = 0;
    secuencia = {};
  }
  bool operator==(const NodoR &nodo) const {
    return estado == nodo.estado && nodo.coste == coste;
  }
  bool operator<(const NodoR &node) const {
    if (coste != node.coste) {
      return coste < node.coste;
    } else {
      return estado < node.estado;
    }
  }
};
class ComportamientoRescatador : public Comportamiento {

public:
  ComportamientoRescatador(unsigned int size = 0) : Comportamiento(size) {
    // Inicializar Variables de Estado Niveles 0,1,4
    this->giro180 = 0;
    this->orientar = 0;
    this->giroizq = 0;
    this->zapatillas = false;
    this->next_action = IDLE;
    this->camino_opcional = 0;
    this->extraturnd = false;
    this->extraturni = false;
    this->extrawalk = false;
    this->memoria = vector<vector<int>>(128, vector<int>(128, 0));
    this->hayPlan = false;
    this->plan = {};
  }
  ComportamientoRescatador(std::vector<std::vector<unsigned char>> mapaR,
                           std::vector<std::vector<unsigned char>> mapaC)
      : Comportamiento(mapaR, mapaC) {
    // Inicializar Variables de Estado Niveles 2,3
    this->hayPlan = false;
    this->zapatillas = false;
    this->plan = {};
  }
  ComportamientoRescatador(const ComportamientoRescatador &comport)
      : Comportamiento(comport) {}
  ~ComportamientoRescatador() {}

  Action think(Sensores sensores);
  int interact(Action accion, int valor);
  Action ComportamientoRescatadorNivel_0(Sensores sensores);
  Action ComportamientoRescatadorNivel_1(Sensores sensores);
  Action ComportamientoRescatadorNivel_2(Sensores sensores);
  Action ComportamientoRescatadorNivel_3(Sensores sensores);
  Action ComportamientoRescatadorNivel_4(Sensores sensores);
  int MenosPisadaR(Sensores sensores);
  int MenosPisadaR1(Sensores sensores);
  int SectorInteresanteR(int posf, int posc);
  int CasillainteresanteR(Sensores sensores);
  int CasillainteresanteR1(Sensores sensores);
  void VisualizaPlan(const EstadoR &st, const vector<Action> &plan);
  void PintaPlan(const vector<Action> &plan, bool zap);
  vector<Action>
  DijkstraRescatadornew(const EstadoR &inicio, const EstadoR &final,
                        const vector<vector<unsigned char>> &terreno,
                        const vector<vector<unsigned char>> &altura);
  vector<Action> AestrellaR4(const EstadoR &inicio, const EstadoR &final,
                             const vector<vector<unsigned char>> &terreno,
                             const vector<vector<unsigned char>> &altura);

private:
  // Variables de Estado
  Action next_action;
  int giroizq;
  int giro180;
  bool zapatillas;
  int orientar;
  bool hayPlan;
  bool extrawalk;
  bool extraturni;
  bool extraturnd;
  int camino_opcional;
  vector<Action> plan;
  std::vector<std::vector<int>> memoria;
};

#endif
