#include "../Comportamientos_Jugador/rescatador.hpp"
#include "comportamientos/comportamiento.hpp"
#include "motorlib/util.h"

Action ComportamientoRescatador::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoRescatadorNivel_0 (sensores);
		break;
	case 1:
		// accion = ComportamientoRescatadorNivel_1 (sensores);
		break;
	case 2:
		// accion = ComportamientoRescatadorNivel_2 (sensores);
		break;
	case 3:
		// accion = ComportamientoRescatadorNivel_3 (sensores);
		break;
	case 4:
		// accion = ComportamientoRescatadorNivel_4 (sensores);
		break;
	}

	return accion;
}

int ComportamientoRescatador::interact(Action accion, int valor)
{ 

	return 0;
}

int Casillainteresante(char i , char c , char d ,bool zap){
	if(c =='X') return 2;
	else if(i=='X') return 1;
	else if(d=='X') return 3;
	else if(!zap){
		if(c=='D')return 2;
		else if (i=='D') return 1;
		else if (d=='D') return 3;
	}
	if(c=='C') return 2;
	else if(i=='C') return 1;
	else if(d=='C') return 3;
	else return 0;
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_0(Sensores sensores)
{
	Action action ;
	
	if (sensores.superficie[0] == 'D') zapatillas =true;
	if(sensores.superficie[0] =='X'){
		action = IDLE;
	}
	else if(giroizq !=0){
		action = TURN_SR;
		giroizq--;
	}
	else{
		int pos = Casillainteresante(sensores.superficie[1], sensores.superficie[2], sensores.superficie[3],zapatillas);
		switch (pos){
			case 2:
				action =WALK;
				break;
			case 1:
				giroizq=1;
				action=TURN_L;
				break;
			case 3:
				action=TURN_SR;
				break;
			case 0:
				action = TURN_L;
				break;
		}
	}

	last_action = action;

	return action;

}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_1(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_2(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_3(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_4(Sensores sensores)
{
}
