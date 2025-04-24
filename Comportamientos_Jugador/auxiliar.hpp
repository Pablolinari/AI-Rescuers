#ifndef COMPORTAMIENTOAUXILIAR_H
#define COMPORTAMIENTOAUXILIAR_H
#include <chrono>
#include <list>
#include <thread>
#include <time.h>
#include <utility>

#include "comportamientos/comportamiento.hpp"
struct EstadoA {
  int f;
  int c;
  int brujula;
  bool zapatillas;
	  bool operator<(const EstadoA &st) const {
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
  bool operator==(const EstadoA&st) const {
    return f == st.f && c == st.c && brujula == st.brujula &&
           zapatillas == st.zapatillas;
  }
};
struct NodoA {
  EstadoA estado;
  list<Action> secuencia;
	int coste ;
  bool operator==(const NodoA &nodo) const { return estado == nodo.estado; }
};

class ComportamientoAuxiliar : public Comportamiento {

public:
  ComportamientoAuxiliar(unsigned int size = 0) : Comportamiento(size) {
    // Inicializar Variables de Estado Niveles 0,1,4
    this->giroizq = 0;
    this->giro180 = 0;
    this->zapatillas = false;
    this->last_action = IDLE;
		this->camino_alternativo=false;
    this->memoria = vector<vector<int>>(128, vector<int>(128, 0));
  }
  ComportamientoAuxiliar(std::vector<std::vector<unsigned char>> mapaR,
                         std::vector<std::vector<unsigned char>> mapaC)
      : Comportamiento(mapaR, mapaC) {
    // Inicializar Variables de Estado Niveles 2,3
  }
  ComportamientoAuxiliar(const ComportamientoAuxiliar &comport)
      : Comportamiento(comport) {

		this->hayPlan=false;
		this->zapatillas=false;
		this->plan={};
	}
  ~ComportamientoAuxiliar() {}

  Action think(Sensores sensores);

  int interact(Action accion, int valor);

  Action ComportamientoAuxiliarNivel_0(Sensores sensores);
  Action ComportamientoAuxiliarNivel_1(Sensores sensores);
  Action ComportamientoAuxiliarNivel_2(Sensores sensores);
  Action ComportamientoAuxiliarNivel_3(Sensores sensores);
  Action ComportamientoAuxiliarNivel_4(Sensores sensores);
  int MenosPisadaA(Sensores sensores);
  int MenosPisadaA1(Sensores sensores);
  int SectorInteresanteA(int posf, int posc);
  int CasillainteresanteA(Sensores sensores);
  int CasillainteresanteA1(Sensores sensores);
	void VisualizaPlan(const EstadoA &st,const list<Action>&plan);
	void PintaPlan(const list<Action>&plan , bool zap);
	list<Action> AestrellaA(const EstadoA & inicio,const EstadoA& final,const vector<vector<unsigned char>>&terreno,const vector<vector<unsigned char>>&altura);


private:
  // Definir Variables de Estado
  Action last_action;
  bool zapatillas;
  int giroizq;
  int giro180;
  std::vector<std::vector<int>> memoria;
  list<Action> plan;
  bool hayPlan;
  bool camino_alternativo;
};

#endif
