#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>

// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"

bool Find(const stateN0 &item, const list<nodeN0> &lista);
bool Find(const stateN0 &item, const list<stateN0> &lista);
void PintaPlan(const list<Action> &plan);
list<Action> AnchuraSoloJugador(const stateN0 &inicio, const ubicacion &final,const vector<vector<unsigned char>> &mapa);
list<Action> AnchuraColaborador(const stateN1 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);
list<Action> costeUniforme(const stateN2 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);
stateN1 applyN1(const Action &a, const stateN1 &st, const vector<vector<unsigned char>> &mapa);
stateN2 applyN2(const Action &action, const stateN2 &st, int &coste, const vector<vector<unsigned char>> &mapa);
stateN3 applyN3(const Action &a, const stateN3 &st, int &coste, const vector<vector<unsigned char>> &mapa);

list<Action> aEstrella(const stateN3 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);




/* METODOS DE BUSQUEDA DEL COLABORADOR*/
bool VeoSonambulo(ubicacion jugador, ubicacion colaborador, Orientacion & orientacion_jugador);

bool ColaboradorALaVista(ubicacion &jugador, ubicacion &colaborador);


Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;
	
	
	if(sensores.nivel !=4){
		if (!hayPlan)
		{
			cout<<"Calculamos un nuevo plan"<<endl;
			
			c_state.jugador.f = sensores.posF;
			c_state.jugador.c = sensores.posC;
			c_state.jugador.brujula = sensores.sentido;
			c_state.colaborador.f = sensores.CLBposF;
			c_state.colaborador.c = sensores.CLBposC;
			c_state.colaborador.brujula = sensores.CLBsentido;
			c_state.ultimaOrdenColaborador = act_CLB_STOP;
			goal.f=sensores.destinoF;
			goal.c=sensores.destinoC;


			
			c_stateN1.ultimaOrdenColaborador = act_CLB_STOP;
			c_stateN1.jugador.f = sensores.posF;
			c_stateN1.jugador.c = sensores.posC;
			c_stateN1.jugador.brujula = sensores.sentido;
			c_stateN1.colaborador.f = sensores.CLBposF;
			c_stateN1.colaborador.c = sensores.CLBposC;
			c_stateN1.colaborador.brujula = sensores.CLBsentido;
			goal.f = sensores.destinoF;
			goal.c = sensores.destinoC;

			c_stateN2.jugador.f = sensores.posF;
			c_stateN2.jugador.c = sensores.posC;
			c_stateN2.jugador.brujula = sensores.sentido;
			c_stateN2.colaborador.f = sensores.CLBposF;
			c_stateN2.colaborador.c = sensores.CLBposC;
			c_stateN2.colaborador.brujula = sensores.CLBsentido;
			c_stateN2.tiene_bikini = false;
			c_stateN2.tiene_zapatillas = false;

			c_stateN3.ultimaOrdenColaborador = act_CLB_STOP;
			c_stateN3.jugador.f = sensores.posF;
			c_stateN3.jugador.c = sensores.posC;
			c_stateN3.jugador.brujula = sensores.sentido;
			c_stateN3.colaborador.f = sensores.CLBposF;
			c_stateN3.colaborador.c = sensores.CLBposC;
			c_stateN3.colaborador.brujula = sensores.CLBsentido;
			c_stateN3.tiene_bikini = false;
			c_stateN3.tiene_zapatillas = false;
			c_stateN3.tiene_bikini_colaborador = false;
			c_stateN3.tiene_zapatillas_colaborador = false;
			

			switch(sensores.nivel){
				case 0: plan = AnchuraSoloJugador(c_state,goal,mapaResultado);
				break;
				case 1: plan = AnchuraColaborador(c_stateN1,goal,mapaResultado);
				break;
				case 2: plan = costeUniforme(c_stateN2,goal,mapaResultado);
				break;
				case 3: plan = aEstrella(c_stateN3,goal,mapaResultado);
				break;
			}
			
			if(plan.size() >0) {
			VisualizaPlan(c_state,plan);
			hayPlan=true;
			}
		}

	if (hayPlan and plan.size()>0){
		accion = plan.front();
		plan.pop_front();
	}
	if (plan.size()== 0){
		cout<<"Se completó el plan"<<endl;
		hayPlan = false;
	}

	}

		

	return accion;

	// Incluir aquí el comportamiento del agente jugador


}


int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}




/*                                METODOS GENERALES                                           */

/** Devuelve si una ubicación en el mapa es transitable para el agente*/
bool CasillaTransitable(const ubicacion &x, const vector<vector<unsigned char> > &mapa){
	return (mapa[x.f][x.c] != 'P' and mapa[x.f][x.c] != 'M' );
}

/** Devuelve la ubicación siguiente según el avance del agente*/
ubicacion NextCasilla(const ubicacion &pos){
	ubicacion next = pos;
	switch (pos.brujula)
	{
	case norte:
		next.f = pos.f - 1;
		break;
	case noreste:
		next.f = pos.f - 1;
		next.c = pos.c + 1;
		break;
	case este:
		next.c = pos.c + 1;
		break;
	case sureste:
		next.f = pos.f + 1;
		next.c = pos.c + 1;
		break;
	case sur:
		next.f = pos.f + 1;
		break;
	case suroeste:
		next.f = pos.f + 1;
		next.c = pos.c - 1;
		break;
	case oeste:
		next.c = pos.c - 1;
		break;
	case noroeste:
		next.f = pos.f - 1;
		next.c = pos.c - 1;
		break;
	}

	return next;
}

/** pone a cero todos los elementos de una matriz */
void AnulaMatriz(vector<vector<unsigned char> > &matriz){
	for (int i = 0; i < matriz.size(); i++)
		for (int jugador = 0; jugador < matriz[0].size(); jugador++)
			matriz[i][jugador] = 0;
}


/** Permite pintar sobre el mapa del simulador el plan partiendo desde el estado st*/
void ComportamientoJugador::VisualizaPlan(const stateN0 &st, const list<Action> &plan)
{
	AnulaMatriz(mapaConPlan);
	stateN0 cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{

		
		if ((*it != act_CLB_WALK) and (*it != act_CLB_TURN_SR) and (*it != act_CLB_STOP))
		{
			switch (cst.ultimaOrdenColaborador)
			{
			case act_CLB_WALK:
				cst.colaborador = NextCasilla(cst.colaborador);
				mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
				break;
			case act_CLB_TURN_SR:
				cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
				break;
			}
		}
		
		switch (*it)
		{
		case actRUN:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 3;
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actWALK:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actTURN_SR:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 1) % 8);
			break;
		case actTURN_L:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
			break;
		case act_CLB_WALK:
			cst.colaborador = NextCasilla(cst.colaborador);
			cst.ultimaOrdenColaborador = act_CLB_WALK;
			mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
			break;
		case act_CLB_TURN_SR:
			cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
			cst.ultimaOrdenColaborador = act_CLB_TURN_SR;
			break;
		case act_CLB_STOP:
			cst.ultimaOrdenColaborador = act_CLB_STOP;
			break;
 		}
		it++;
	}
}


/** primera aproximación a la implementación de la búsqueda en anchura */





bool VeoSonambulo(ubicacion jugador, ubicacion colaborador, Orientacion & orientacion_jugador){

  bool veo_colaborador = false;

  switch(orientacion_jugador){
    case norte:

      for(int i = 0; i < 3 and !veo_colaborador; i++){

        if((colaborador.f == jugador.f - 1) and (colaborador.c == jugador.c - 1 + i)){
          veo_colaborador = true;
        }
      }

      for(int i = 0; i < 5 and !veo_colaborador; i++){
        if((colaborador.f == jugador.f - 2) and (colaborador.c == jugador.c - 2 + i)){
          veo_colaborador = true;
        }
      }

      for(int i = 0; i < 7 and !veo_colaborador; i++){
        if((colaborador.f == jugador.f - 3) and (colaborador.c == jugador.c - 3 + i)){
          veo_colaborador = true;
        }
      }

      break;
    case sur:

      for(int i = 0; i < 3 and !veo_colaborador; i++){

        if((colaborador.f == jugador.f + 1) and (colaborador.c == jugador.c - 1 + i)){
          veo_colaborador = true;
        }
      }

      for(int i = 0; i < 5 and !veo_colaborador; i++){
        if((colaborador.f == jugador.f + 2) and (colaborador.c == jugador.c - 2 + i)){
          veo_colaborador = true;
        }
      }

      for(int i = 0; i < 7 and !veo_colaborador; i++){
        if((colaborador.f == jugador.f + 3) and (colaborador.c == jugador.c - 3 + i)){
          veo_colaborador = true;
        }
      }

      break;
    case este:
      for(int i = 0; i < 3 and !veo_colaborador; i++){

        if((colaborador.f == jugador.f - 1 + i) and (colaborador.c == jugador.c + 1)){
          veo_colaborador = true;
        }
      }

      for(int i = 0; i < 5 and !veo_colaborador; i++){

        if((colaborador.f == jugador.f - 2 + i) and (colaborador.c == jugador.c + 2)){
          veo_colaborador = true;
        }
      }

      for(int i = 0; i < 7 and !veo_colaborador; i++){

        if((colaborador.f == jugador.f - 3 + i) and (colaborador.c == jugador.c + 3)){
          veo_colaborador = true;
        }
      }
      break;
	  
    case oeste:
      for(int i = 0; i < 3 and !veo_colaborador; i++){

        if((colaborador.f == jugador.f - 1 + i) and (colaborador.c == jugador.c - 1)){
          veo_colaborador = true;
        }
      }

      for(int i = 0; i < 5 and !veo_colaborador; i++){

        if((colaborador.f == jugador.f - 2 + i) and (colaborador.c == jugador.c - 2)){
          veo_colaborador = true;
        }
      }

      for(int i = 0; i < 7 and !veo_colaborador; i++){

        if((colaborador.f == jugador.f - 3 + i) and (colaborador.c == jugador.c - 3)){
          veo_colaborador = true;
        }
      }
    break;


    case noreste:

		for (int i = 0; i < 4 and !veo_colaborador; i++)
		{
				if ((colaborador.f == jugador.f - i) and (colaborador.c == jugador.c)){
					veo_colaborador = true;
				}
				if ((colaborador.f == jugador.f - i) and (colaborador.c == jugador.c + 1)){
					veo_colaborador = true;
				} 
				if ((colaborador.f == jugador.f - i) and (colaborador.c == jugador.c + 2)){
					veo_colaborador = true;
				}
				if ((colaborador.f == jugador.f - i) and (colaborador.c == jugador.c + 3)){
					veo_colaborador = true;
				}          
		}
	

       
    break;

	case noroeste:
		for (int i = 0; i < 4 and !veo_colaborador; i++)
			{
					if ((colaborador.f == jugador.f - i) and (colaborador.c == jugador.c)){
						veo_colaborador = true;
					}
					if ((colaborador.f == jugador.f - i) and (colaborador.c == jugador.c - 1)){
						veo_colaborador = true;
					} 
					if ((colaborador.f == jugador.f - i) and (colaborador.c == jugador.c - 2 )){
						veo_colaborador = true;
					}
					if ((colaborador.f == jugador.f - i) and (colaborador.c == jugador.c -3 )){
						veo_colaborador = true;
					}          
			}

	break;

	case sureste:
	for (int i = 0; i < 4 and !veo_colaborador; i++)
		{
				if ((colaborador.f == jugador.f + i) and (colaborador.c == jugador.c)){
					veo_colaborador = true;
				}
				if ((colaborador.f == jugador.f + i) and (colaborador.c == jugador.c + 1)){
					veo_colaborador = true;
				} 
				if ((colaborador.f == jugador.f + i) and (colaborador.c == jugador.c + 2)){
					veo_colaborador = true;
				}
				if ((colaborador.f == jugador.f + i) and (colaborador.c == jugador.c + 3)){
					veo_colaborador = true;
				}          
		}


	break;


	case suroeste:

	for (int i = 0; i < 4 and !veo_colaborador; i++)
		{
				if ((colaborador.f == jugador.f + i) and (colaborador.c == jugador.c)){
					veo_colaborador = true;
				}
				if ((colaborador.f == jugador.f + i) and (colaborador.c == jugador.c - 1)){
					veo_colaborador = true;
				} 
				if ((colaborador.f == jugador.f + i) and (colaborador.c == jugador.c - 2)){
					veo_colaborador = true;
				}
				if ((colaborador.f == jugador.f + i) and (colaborador.c == jugador.c - 3)){
					veo_colaborador = true;
				}          
		}



	break;
	
  }

  return veo_colaborador;
}

bool ColaboradorALaVista(ubicacion &jugador, ubicacion &colaborador)
{

	switch(jugador.brujula){
		case norte:

			if(jugador.c == colaborador.c){
				if((jugador.f-1) == colaborador.f)	// posicion 2
					return true;
				if((jugador.f-2) == colaborador.f)	// posicion 6
					return true;
				if((jugador.f-3) == colaborador.f)	// posicion 12
					return true;
			}

			if(jugador.c == (colaborador.c-1)){
				if((jugador.f-1) == colaborador.f)	// posicion 1
					return true;
				if((jugador.f-2) == colaborador.f)	// posicion 5
					return true;
				if((jugador.f-3) == colaborador.f)	// posicion 11
					return true;
			}

			if(jugador.c == (colaborador.c-2)){
				if((jugador.f-2) == colaborador.f)	// posicion 4
					return true;
				if((jugador.f-3) == colaborador.f)	// posicion 10
					return true;
			}

			if(jugador.c == (colaborador.c-3)){
				if((jugador.f-3) == colaborador.f)	// posicion 9
					return true;
			}

			if(jugador.c == (colaborador.c+1)){
				if((jugador.f-1) == colaborador.f)	// posicion 3
					return true;
				if((jugador.f-2) == colaborador.f)	// posicion 7
					return true;
				if((jugador.f-3) == colaborador.f)	// posicion 13
					return true;
			}

			if(jugador.c == (colaborador.c+2)){
				if((jugador.f-2) == colaborador.f)	// posicion 8
					return true;
				if((jugador.f-3) == colaborador.f)	// posicion 14
					return true;
			}

			if(jugador.c == (colaborador.c+3)){
				if((jugador.f-3) == colaborador.f)	// posicion 15
					return true;
			}

		break;

		case noreste:
			
			if(jugador.f == colaborador.f){
				if((jugador.c+1) == colaborador.c)	// posicion 3
					return true;
				if((jugador.c+2) == colaborador.c)	// posicion 8
					return true;
				if((jugador.c+3) == colaborador.c)	// posicion 15
					return true;
			}

			if(jugador.f == (colaborador.f-1)){
				if(jugador.c == colaborador.c)		// posicion 1
					return true;
				if((jugador.c+1) == colaborador.c)	// posicion 2
					return true;
				if((jugador.c+2) == colaborador.c)	// posicion 7
					return true;
				if((jugador.c+3) == colaborador.c)	// posicion 14
					return true;
			}

			if(jugador.f == (colaborador.f-2)){
				if(jugador.c == colaborador.c)		// posicion 4
					return true;
				if((jugador.c+1) == colaborador.c)	// posicion 5
					return true;
				if((jugador.c+2) == colaborador.c)	// posicion 6
					return true;
				if((jugador.c+3) == colaborador.c)	// posicion 13
					return true;
			}

			if(jugador.f == (colaborador.f-3)){
				if(jugador.c == colaborador.c)		// posicion 9
					return true;
				if((jugador.c+1) == colaborador.c)	// posicion 10
					return true;
				if((jugador.c+2) == colaborador.c)	// posicion 11
					return true;
				if((jugador.c+3) == colaborador.c)	// posicion 12
					return true;
			}

		break;

		case este:

			if(jugador.f == colaborador.f){
				if((jugador.c+1) == colaborador.c)	// posicion 2
					return true;
				if((jugador.c+2) == colaborador.c)	// posicion 6
					return true;
				if((jugador.c+3) == colaborador.c)	// posicion 12
					return true;
			}

			if(jugador.f == (colaborador.f-1)){
				if((jugador.c+1) == colaborador.c)	// posicion 1
					return true;
				if((jugador.c+2) == colaborador.c)	// posicion 5
					return true;
				if((jugador.c+3) == colaborador.c)	// posicion 11
					return true;
			}

			if(jugador.f == (colaborador.f-2)){
				if((jugador.c+2) == colaborador.c)	// posicion 4
					return true;
				if((jugador.c+3) == colaborador.c)	// posicion 10
					return true;
			}

			if(jugador.f == (colaborador.f-3)){
				if((jugador.c+3) == colaborador.c)	// posicion 9
					return true;
			}

			if(jugador.f == (colaborador.f+1)){
				if((jugador.c+1) == colaborador.c)	// posicion 3
					return true;
				if((jugador.c+2) == colaborador.c)	// posicion 7
					return true;
				if((jugador.c+3) == colaborador.c)	// posicion 13
					return true;
			}

			if(jugador.f == (colaborador.f+2)){
				if((jugador.c+2) == colaborador.c)	// posicion 8
					return true;
				if((jugador.c+3) == colaborador.c)	// posicion 14
					return true;
			}

			if(jugador.f == (colaborador.f+3)){
				if((jugador.c+3) == colaborador.c)	// posicion 15
					return true;
			}
			
		break;

		case sureste:
			
			if(jugador.f == colaborador.f){
				if((jugador.c+1) == colaborador.c)	// posicion 3
					return true;
				if((jugador.c+2) == colaborador.c)	// posicion 8
					return true;
				if((jugador.c+3) == colaborador.c)	// posicion 15
					return true;
			}

			if(jugador.f == (colaborador.f+1)){
				if(jugador.c == colaborador.c)		// posicion 1
					return true;
				if((jugador.c+1) == colaborador.c)	// posicion 2
					return true;
				if((jugador.c+2) == colaborador.c)	// posicion 7
					return true;
				if((jugador.c+3) == colaborador.c)	// posicion 14
					return true;
			}

			if(jugador.f == (colaborador.f+2)){
				if(jugador.c == colaborador.c)		// posicion 4
					return true;
				if((jugador.c+1) == colaborador.c)	// posicion 5
					return true;
				if((jugador.c+2) == colaborador.c)	// posicion 6
					return true;
				if((jugador.c+3) == colaborador.c)	// posicion 13
					return true;
			}

			if(jugador.f == (colaborador.f+3)){
				if(jugador.c == colaborador.c)		// posicion 9
					return true;
				if((jugador.c+1) == colaborador.c)	// posicion 10
					return true;
				if((jugador.c+2) == colaborador.c)	// posicion 11
					return true;
				if((jugador.c+3) == colaborador.c)	// posicion 12
					return true;
			}

		break;

		case sur:

			if(jugador.c == colaborador.c){
				if((jugador.f+1) == colaborador.f)	// posicion 2
					return true;
				if((jugador.f+2) == colaborador.f)	// posicion 6
					return true;
				if((jugador.f+3) == colaborador.f)	// posicion 12
					return true;
			}

			if(jugador.c == (colaborador.c-1)){
				if((jugador.f+1) == colaborador.f)	// posicion 3
					return true;
				if((jugador.f+2) == colaborador.f)	// posicion 7
					return true;
				if((jugador.f+3) == colaborador.f)	// posicion 13
					return true;
			}

			if(jugador.c == (colaborador.c-2)){
				if((jugador.f+2) == colaborador.f)	// posicion 8
					return true;
				if((jugador.f+3) == colaborador.f)	// posicion 14
					return true;
			}

			if(jugador.c == (colaborador.c-3)){
				if((jugador.f+3) == colaborador.f)	// posicion 15
					return true;
			}

			if(jugador.c == (colaborador.c+1)){
				if((jugador.f+1) == colaborador.f)	// posicion 1
					return true;
				if((jugador.f+2) == colaborador.f)	// posicion 5
					return true;
				if((jugador.f+3) == colaborador.f)	// posicion 11
					return true;
			}

			if(jugador.c == (colaborador.c+2)){
				if((jugador.f+2) == colaborador.f)	// posicion 4
					return true;
				if((jugador.f+3) == colaborador.f)	// posicion 10
					return true;
			}

			if(jugador.c == (colaborador.c+3)){
				if((jugador.f+3) == colaborador.f)	// posicion 9
					return true;
			}
			
		break;

		case suroeste:
			
			if(jugador.f == colaborador.f){
				if((jugador.c-1) == colaborador.c)	// posicion 3
					return true;
				if((jugador.c-2) == colaborador.c)	// posicion 8
					return true;
				if((jugador.c-3) == colaborador.c)	// posicion 15
					return true;
			}

			if(jugador.f == (colaborador.f+1)){
				if(jugador.c == colaborador.c)		// posicion 1
					return true;
				if((jugador.c-1) == colaborador.c)	// posicion 2
					return true;
				if((jugador.c-2) == colaborador.c)	// posicion 7
					return true;
				if((jugador.c-3) == colaborador.c)	// posicion 14
					return true;
			}

			if(jugador.f == (colaborador.f+2)){
				if(jugador.c == colaborador.c)		// posicion 4
					return true;
				if((jugador.c-1) == colaborador.c)	// posicion 5
					return true;
				if((jugador.c-2) == colaborador.c)	// posicion 6
					return true;
				if((jugador.c-3) == colaborador.c)	// posicion 13
					return true;
			}

			if(jugador.f == (colaborador.f+3)){
				if(jugador.c == colaborador.c)		// posicion 9
					return true;
				if((jugador.c-1) == colaborador.c)	// posicion 10
					return true;
				if((jugador.c-2) == colaborador.c)	// posicion 11
					return true;
				if((jugador.c-3) == colaborador.c)	// posicion 12
					return true;
			}

		break;

		case oeste:

			if(jugador.f == colaborador.f){
				if((jugador.c-1) == colaborador.c)	// posicion 2
					return true;
				if((jugador.c-2) == colaborador.c)	// posicion 6
					return true;
				if((jugador.c-3) == colaborador.c)	// posicion 12
					return true;
			}

			if(jugador.f == (colaborador.f-1)){
				if((jugador.c-1) == colaborador.c)	// posicion 3
					return true;
				if((jugador.c-2) == colaborador.c)	// posicion 7
					return true;
				if((jugador.c-3) == colaborador.c)	// posicion 13
					return true;
			}

			if(jugador.f == (colaborador.f-2)){
				if((jugador.c-2) == colaborador.c)	// posicion 8
					return true;
				if((jugador.c-3) == colaborador.c)	// posicion 14
					return true;
			}

			if(jugador.f == (colaborador.f-3)){
				if((jugador.c-3) == colaborador.c)	// posicion 15
					return true;
			}

			if(jugador.f == (colaborador.f+1)){
				if((jugador.c-1) == colaborador.c)	// posicion 
					return true;
				if((jugador.c-2) == colaborador.c)	// posicion 5
					return true;
				if((jugador.c-3) == colaborador.c)	// posicion 11
					return true;
			}

			if(jugador.f == (colaborador.f+2)){
				if((jugador.c-2) == colaborador.c)	// posicion 4
					return true;
				if((jugador.c-3) == colaborador.c)	// posicion 10
					return true;
			}

			if(jugador.f == (colaborador.f+3)){
				if((jugador.c-3) == colaborador.c)	// posicion 9
					return true;
			}
			
		break;
		
		case noroeste:
			
			if(jugador.f == colaborador.f){
				if((jugador.c-1) == colaborador.c)	// posicion 3
					return true;
				if((jugador.c-2) == colaborador.c)	// posicion 8
					return true;
				if((jugador.c-3) == colaborador.c)	// posicion 15
					return true;
			}

			if(jugador.f == (colaborador.f-1)){
				if(jugador.c == colaborador.c)		// posicion 1
					return true;
				if((jugador.c-1) == colaborador.c)	// posicion 2
					return true;
				if((jugador.c-2) == colaborador.c)	// posicion 7
					return true;
				if((jugador.c-3) == colaborador.c)	// posicion 14
					return true;
			}

			if(jugador.f == (colaborador.f-2)){
				if(jugador.c == colaborador.c)		// posicion 4
					return true;
				if((jugador.c-1) == colaborador.c)	// posicion 5
					return true;
				if((jugador.c-2) == colaborador.c)	// posicion 6
					return true;
				if((jugador.c-3) == colaborador.c)	// posicion 13
					return true;
			}

			if(jugador.f == (colaborador.f-3)){
				if(jugador.c == colaborador.c)		// posicion 9
					return true;
				if((jugador.c-1) == colaborador.c)	// posicion 10
					return true;
				if((jugador.c-2) == colaborador.c)	// posicion 11
					return true;
				if((jugador.c-3) == colaborador.c)	// posicion 12
					return true;
			}

		break;				

	}

	return false;
}


/**Definición del tipo nodo del nivel 0*/



/**Sobrecarga de la función Find para buscar en lista de nodeN0*/


/**Encuentra si el elemento item está en la lista */

bool Find(const stateN0 &item, const list<nodeN0> &lista){
	auto it = lista.begin();
	while (it != lista.end() and !(it->st == item))
		it++;

	return (!(it == lista.end()));
}


/**Encuentra si el elemento item está en la lista */

bool Find(const stateN0 &item, const list<stateN0> &lista){
	auto it = lista.begin();
	while (it != lista.end() and !((*it) == item))
		it++;

	return (!(it == lista.end()));
}

void PintaPlan(const list<Action> &plan)
{
	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == actWALK){
			cout << "W ";
		}
		else if (*it == actRUN){
			cout << "R ";
		}
		else if (*it == actTURN_SR){
			cout << "r ";
		}
		else if (*it == actTURN_L){
			cout << "L ";
		}
		else if (*it == act_CLB_WALK){
			cout << "cW ";
		}
		else if (*it == act_CLB_TURN_SR){
			cout << "cr ";
		}
		else if (*it == act_CLB_STOP){
			cout << "cS ";
		}
		else if (*it == actIDLE){
			cout << "I ";
		}
		else{
			cout << "-_ ";
		}
		it++;
	}
	cout << " (" << plan.size() << " acciones)\n";
}







/* 		NIVEL 0       */

stateN0 apply(const Action &a, const stateN0 &st, const vector<vector<unsigned char> > &mapa){
	stateN0 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2;
	switch (a)
	{
	case actWALK: //si prox casilla es transitable y no está ocupada por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (CasillaTransitable(sig_ubicacion, mapa) and 
			!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
				st_result.jugador = sig_ubicacion;
			}
		break;
	
	case actRUN: //si prox 2 casillas son transitables y no está ocupada por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (CasillaTransitable(sig_ubicacion, mapa) and 
			!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
				sig_ubicacion2 = NextCasilla(sig_ubicacion);
				if (CasillaTransitable(sig_ubicacion2, mapa) and 
					!(sig_ubicacion2.f == st.colaborador.f and sig_ubicacion2.c == st.colaborador.c)){
						st_result.jugador = sig_ubicacion2;
				}
			}
		break;

	case actTURN_L:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);
		break;

	case actTURN_SR:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);
		break;

	case actIDLE:
		st_result = st;
	}


	return st_result;
}


list<Action> AnchuraSoloJugador(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	nodeN0 current_node; //NO inicio
	current_node.st = inicio;
	list<nodeN0> frontier;
	//list<nodeN0> explored;
	set<nodeN0> explored;
	list<Action> plan; //Secuencia final guardada en el nodo objetivo
	frontier.push_back(current_node);
	bool SolutionFound = (current_node.st.jugador.f == final.f and
						  current_node.st.jugador.c == final.c);

	//frontier abierto, explorado cerrado
	while (!frontier.empty() and !SolutionFound){
		frontier.pop_front();
		explored.insert(current_node);

		// Generar hijo actWALK
		nodeN0 child_walk = current_node;
		child_walk.st = apply(actWALK, current_node.st, mapa);
		child_walk.secuencia.push_back(actWALK);
		//COmprobación
		if (child_walk.st.jugador.f == final.f and child_walk.st.jugador.c == final.c){
			current_node = child_walk;
			SolutionFound = true;
		}
		//PRIMERA VERSION DE ANCHURA, TENEMOS QUE HACER LO DE BUSCAR EN CERRADOS NO EN ABIERTOS PARA LA EFICIENCIA
		//MODIFICAR
		else if (explored.find(child_walk) == explored.end()){
			frontier.push_back(child_walk);
		}

		if (!SolutionFound){
			// Generar hijo actRUN
			nodeN0 child_run = current_node;
			child_run.st = apply(actRUN, current_node.st, mapa);
			child_run.secuencia.push_back(actRUN);
			if (child_run.st.jugador.f == final.f and child_run.st.jugador.c == final.c){
				current_node = child_run;
				SolutionFound = true;
			}
			else if (explored.find(child_run) == explored.end()){
				frontier.push_back(child_run);
			}
		}

		if (!SolutionFound){


			// Generar hijo actTURN_L
			nodeN0 child_turnl = current_node;
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			child_turnl.secuencia.push_back(actTURN_L);
			if (explored.find(child_turnl) == explored.end()){
				frontier.push_back(child_turnl);
			}		
			
			// Generar hijo actTURN_SR
			nodeN0 child_turnsr = current_node;
			child_turnsr.st = apply(actTURN_SR, current_node.st, mapa);
			child_turnsr.secuencia.push_back(actTURN_SR);
			if (explored.find(child_turnsr) == explored.end()){
				frontier.push_back(child_turnsr);
			}		
		}

		if (!SolutionFound and !frontier.empty())
			current_node = frontier.front();
			while(!frontier.empty() and explored.find(current_node) != explored.end()){
				frontier.pop_front();
				if(!frontier.empty()) current_node = frontier.front();
			}
	
	}
	

	if(SolutionFound){	
		plan = current_node.secuencia;
		cout << "Plan encontrado: ";
		PintaPlan(current_node.secuencia);
	}

	return plan;
}










/* NIVEL 1  */


stateN1 applyN1(const Action &action, const stateN1 &st, const vector<vector<unsigned char>> &mapa)
{
	stateN1 st_result = st;
	stateN1 principio = st;
	ubicacion sig_ubicacion, sig_ubicacion2;


	switch (action)
	{
		case actWALK: // si casilla delante es transitable y no está ocupada por el sonámbulo
			sig_ubicacion = NextCasilla(st.jugador);
			if (CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c))
			{
				st_result.jugador = sig_ubicacion;
			}
		break;

		case actRUN: //si prox 2 casillas son transitables y no está ocupada por el colaborador
			sig_ubicacion = NextCasilla(st.jugador);
			if (CasillaTransitable(sig_ubicacion, mapa) and 
				!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
					sig_ubicacion2 = NextCasilla(sig_ubicacion);
					if (CasillaTransitable(sig_ubicacion2, mapa) and 
						!(sig_ubicacion2.f == st.colaborador.f and sig_ubicacion2.c == st.colaborador.c)){
							st_result.jugador = sig_ubicacion2;
					}
				}
		break;
		
		case actTURN_L:
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 6) % 8);
		break;

		case actTURN_SR:
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 1) % 8);
		break;

		case act_CLB_TURN_SR:
			st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula + 1) % 8);
			st_result.ultimaOrdenColaborador = act_CLB_TURN_SR;
		break;

		case act_CLB_WALK:
			sig_ubicacion = NextCasilla(st.colaborador);
			if (CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.jugador.f && sig_ubicacion.c == st.jugador.c))
			{
				st_result.colaborador = sig_ubicacion;
				st_result.ultimaOrdenColaborador = act_CLB_WALK;
			}
		break;

		case act_CLB_STOP:
			st_result.colaborador = principio.colaborador;
			st_result.ultimaOrdenColaborador = act_CLB_STOP;
		break;

		case actIDLE:
			st_result = st;
		break;

	}

	if ( action != act_CLB_STOP && action != act_CLB_WALK && action != act_CLB_TURN_SR){
		switch(st.ultimaOrdenColaborador){

			case act_CLB_TURN_SR:
				st_result.colaborador.brujula = static_cast<Orientacion>((st.colaborador.brujula + 1) % 8);
				st_result.ultimaOrdenColaborador = act_CLB_TURN_SR;
			break;

			case act_CLB_WALK:
				sig_ubicacion = NextCasilla(st.colaborador);
				if (CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.jugador.f && sig_ubicacion.c == st.jugador.c))
				{
					st_result.colaborador = sig_ubicacion;
					st_result.ultimaOrdenColaborador = act_CLB_WALK;
				}
				
			break;

			case act_CLB_STOP:
				st_result.colaborador = principio.colaborador;
				st_result.ultimaOrdenColaborador = act_CLB_STOP;
			break;

		}
	}

	return st_result;
}


list<Action> AnchuraColaborador(const stateN1 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	nodeN1 current_node;
	list<nodeN1> frontier;
	set<nodeN1> explored;
	list<Action> plan;
	current_node.st = inicio;

	bool SolutionFound = (current_node.st.colaborador.f == final.f && current_node.st.colaborador.c == final.c);
	frontier.push_back(current_node);

	while (!frontier.empty() && !SolutionFound)
	{
		frontier.pop_front();
		explored.insert(current_node);

		
		if(VeoSonambulo(current_node.st.jugador,current_node.st.colaborador,current_node.st.jugador.brujula))
		
		{
				//Si vemos al colaborador

				// Generar hijo act_CLB_STOP
				nodeN1 child_clb_stop = current_node;
				child_clb_stop.st = applyN1(act_CLB_STOP, current_node.st, mapa);
				child_clb_stop .secuencia.push_back(act_CLB_STOP);

				if(explored.find(child_clb_stop ) == explored.end())
				{
					// Añadir hijo a la lista de nodos por explorar
					
					frontier.push_back(child_clb_stop );
				}

			nodeN1 child_clb_walk = current_node;
			//stateN1 prueba = child_clb_walk.st;
			
			child_clb_walk.st = applyN1(act_CLB_WALK, current_node.st, mapa);
			child_clb_walk.secuencia.push_back(act_CLB_WALK);

			
			if(child_clb_walk.st.colaborador.f == final.f 
			and child_clb_walk.st.colaborador.c == final.c)
			{
				
				current_node = child_clb_walk;
				SolutionFound = true;
			}    
			else if(explored.find(child_clb_walk) == explored.end())
			{
				// Añadir hijo a la lista de nodos por explora
				frontier.push_back(child_clb_walk);
			}

			
			// Generar hijo act_CLB_TURN_SR
				nodeN1 child_clb_turnsr = current_node;
				child_clb_turnsr.st = applyN1(act_CLB_TURN_SR, current_node.st, mapa);
				child_clb_turnsr.secuencia.push_back(act_CLB_TURN_SR);

				if(explored.find(child_clb_turnsr) == explored.end())
				{
					// Añadir hijo a la lista de nodos por explorar
					
					frontier.push_back(child_clb_turnsr);
				}

				
				

		}

			if(!SolutionFound){

				// Generar hijo actIDLE
				nodeN1 child_idle = current_node;
				child_idle.st = applyN1(actIDLE, current_node.st, mapa);
				child_idle.secuencia.push_back(actIDLE);
				
				if (explored.find(child_idle)== explored.end())
				{
					
					frontier.push_back(child_idle);
				}
				else if (child_idle.st.colaborador.f == final.f and child_idle.st.colaborador.c == final.c){
					current_node = child_idle;
					SolutionFound = true;
				}

			// Generar hijo actWALK

				

			if(!SolutionFound){
				nodeN1 child_walk = current_node;
				child_walk.st = applyN1(actWALK, current_node.st, mapa);
				child_walk.secuencia.push_back(actWALK);
				
				if (explored.find(child_walk) == explored.end())
				{
					
					frontier.push_back(child_walk);
				}

				else if(child_walk.st.colaborador.f == final.f and child_walk.st.colaborador.c == final.c)
				{
					
					current_node = child_walk;
					SolutionFound = true;
				}    

			}
			// GENERAR HIJO ACTRUN

				if(!SolutionFound){
				nodeN1 child_run = current_node;
				child_run.st = applyN1(actRUN, current_node.st, mapa);
				child_run.secuencia.push_back(actRUN);

				if (explored.find(child_run) == explored.end()){
					
					frontier.push_back(child_run);
				}
				else if (child_run.st.colaborador.f == final.f and child_run.st.colaborador.c == final.c){
					current_node = child_run;
					SolutionFound = true;
				}
				}
				
				

			if(!SolutionFound){
				// Generar hijo actTurn_SR
				nodeN1 child_turnsr = current_node;
				child_turnsr.st = applyN1(actTURN_SR, current_node.st, mapa);
				child_turnsr.secuencia.push_back(actTURN_SR);
				
				if (explored.find(child_turnsr) == explored.end())
				{
					
					frontier.push_back(child_turnsr);
				}

				else if (child_turnsr.st.colaborador.f == final.f and child_turnsr.st.colaborador.c == final.c){
					current_node = child_turnsr;
					SolutionFound = true;
				}
			}

			if(!SolutionFound){
				// Generar hijo actTurn_L
				nodeN1 child_turnl = current_node;
				child_turnl.st = applyN1(actTURN_L, current_node.st, mapa);
				child_turnl.secuencia.push_back(actTURN_L);
				
				if (explored.find(child_turnl) == explored.end())
				{
					
					frontier.push_back(child_turnl);
				}
				else if (child_turnl.st.colaborador.f == final.f and child_turnl.st.colaborador.c == final.c){
					current_node = child_turnl;
					SolutionFound = true;
				}

			}

			}
			

		if(!SolutionFound and !frontier.empty()){
			current_node = frontier.front();
			while(!frontier.empty() and explored.find(current_node) != explored.end()){
					frontier.pop_front();
					if(!frontier.empty())
					current_node = frontier.front();
			}
		}
		

	}

	if(SolutionFound)
		plan = current_node.secuencia;
		cout << "Plan encontrado: ";
		PintaPlan(current_node.secuencia);

	return plan;
}




/* 			NIVEL 2  */

stateN2 applyN2(const Action &action, const stateN2 &st, int &coste, const vector<vector<unsigned char>> &mapa)
{
	stateN2 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2;
	switch (action)
	{
	case actWALK: // si casilla delante es transitable y no está ocupada por el sonámbulo
		sig_ubicacion = NextCasilla(st.jugador);
		if (CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c))
		{

			if (mapa[sig_ubicacion.f][sig_ubicacion.c] == 'K')
			{
				st_result.tiene_bikini = true;
				if (st_result.tiene_zapatillas)
					st_result.tiene_zapatillas = false;
			}

			else if (mapa[sig_ubicacion.f][sig_ubicacion.c] == 'D')
			{
				st_result.tiene_zapatillas = true;
				if (st_result.tiene_bikini)
					st_result.tiene_bikini = false;
			}

			// Actualiza el coste acumulado
			switch (mapa[st.jugador.f][st.jugador.c])
			{
			case 'A':
				if (!st.tiene_bikini)
					coste += 100;
				else
					coste += 10;
				break;

			case 'B':
				if (!st.tiene_zapatillas)
					coste += 50;
				else
					coste += 15;
				break;

			case 'T':
				coste += 2;
				break;

			default:
				coste += 1;
				break;
			}
			st_result.jugador = sig_ubicacion;
		}

		break;

	case actRUN:

		sig_ubicacion = NextCasilla(st.jugador);
			if (CasillaTransitable(sig_ubicacion, mapa) and 
				!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
					sig_ubicacion2 = NextCasilla(sig_ubicacion);
					if (CasillaTransitable(sig_ubicacion2, mapa) and 
						!(sig_ubicacion2.f == st.colaborador.f and sig_ubicacion2.c == st.colaborador.c)){
							st_result.jugador = sig_ubicacion2;




							if (mapa[sig_ubicacion2.f][sig_ubicacion2.c] == 'K')
							{
								st_result.tiene_bikini = true;
								if (st_result.tiene_zapatillas)
									st_result.tiene_zapatillas = false;
							}

							else if (mapa[sig_ubicacion2.f][sig_ubicacion2.c] == 'D')
							{
								st_result.tiene_zapatillas = true;
								if (st_result.tiene_bikini)
									st_result.tiene_bikini = false;
							}
					}
				}


			

		switch (mapa[st.jugador.f][st.jugador.c])
		{

		
			
			case 'A':
				if (!st.tiene_bikini)
					coste += 150;
				else
					coste += 15;
				break;

			case 'B':
				if (!st.tiene_zapatillas)
					coste += 75;
				else
					coste += 25;
				break;

			case 'T':
				coste += 3;
				break;

			default:
				coste += 1;
				break;
			
			
			st_result.jugador = sig_ubicacion;
		}


	break;


	case actTURN_L:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 6) % 8);

		switch (mapa[st.jugador.f][st.jugador.c])
		{
		case 'A':
			if (!st.tiene_bikini)
				coste += 30;
			else
				coste += 5;
			break;

		case 'B':
			if (!st.tiene_zapatillas)
				coste += 7;
			else
				coste += 1;
			break;

		case 'T':
			coste += 2;
			break;

		default:
			coste += 1;
			break;
		}

		break;

	case actTURN_SR:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 1) % 8);

		switch (mapa[st.jugador.f][st.jugador.c])
		{
		case 'A':
			if (!st.tiene_bikini)
				coste += 10;
			else
				coste += 2;
			break;

		case 'B':
			if (!st.tiene_zapatillas)
				coste += 5;
			else
				coste += 1;
			break;

		case 'T':
			coste += 1;
			break;

		default:
			coste += 1;
			break;
		}

		break;
	case actIDLE:

		st_result = st;

	break;
	}

	
	return st_result;
}


list<Action> costeUniforme(const stateN2 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	nodeN2 current_node;
	priority_queue<nodeN2> frontier;
	set<stateN2> explored;
	list<Action> plan;
	current_node.st = inicio;
	current_node.coste = 0;

	if (mapa[current_node.st.jugador.f][current_node.st.jugador.c] == 'K')
		current_node.st.tiene_bikini = true;
	if (mapa[current_node.st.jugador.f][current_node.st.jugador.c] == 'D')
		current_node.st.tiene_zapatillas = true;

	bool SolutionFound = (current_node.st.jugador.f == final.f && current_node.st.jugador.c == final.c);
	frontier.push(current_node);

	while (!frontier.empty() && !SolutionFound)
	{
			frontier.pop();

			explored.insert(current_node.st);

			nodeN2 child_walk = current_node;
			child_walk.st = applyN2(actWALK, current_node.st, child_walk.coste, mapa);
			
			if (explored.find(child_walk.st) == explored.end())
			{
				child_walk.secuencia.push_back(actWALK);
				frontier.push(child_walk);
			}
			else if (child_walk.st.jugador.f == final.f && child_walk.st.jugador.c == final.c)
			{
				SolutionFound = true;
				current_node = child_walk;
			}

			
					// Generar hijo actRUN
				nodeN2 child_run = current_node;
				child_run.st = applyN2(actRUN, current_node.st,child_run.coste, mapa);
				
				
				if (explored.find(child_run.st)== explored.end())
				{
					child_run.secuencia.push_back(actRUN);
					frontier.push(child_run);
				}
				else if (child_run.st.jugador.f == final.f and child_run.st.jugador.c == final.c){
					SolutionFound = true;
					current_node = child_run;
					
				}


			

			if(!SolutionFound)
			{
					// Generar hijo actIDLE
				nodeN2 child_idle = current_node;
				child_idle.st = applyN2(actIDLE, current_node.st,child_idle.coste, mapa);
				child_idle.secuencia.push_back(actIDLE);
				
				if (explored.find(child_idle.st)== explored.end())
				{
					
					frontier.push(child_idle);
				}
				else if (child_idle.st.jugador.f == final.f and child_idle.st.jugador.c == final.c){
					current_node = child_idle;
					SolutionFound = true;
				}


			}
			if (!SolutionFound)
			{

				// Genera hijo actTURN_L
				nodeN2 child_turnl = current_node;
				child_turnl.st = applyN2(actTURN_L, current_node.st, child_turnl.coste, mapa);
				child_turnl.secuencia.push_back(actTURN_L);
				if (explored.find(child_turnl.st) == explored.end())
				{
					
					frontier.push(child_turnl);
				}
				else if (child_turnl.st.jugador.f == final.f and child_turnl.st.jugador.c == final.c){
					current_node = child_turnl;
					SolutionFound = true;
				}

				// Genera hijo actTURN_SR
				nodeN2 child_turnsr = current_node;
				child_turnsr.st = applyN2(actTURN_SR, current_node.st, child_turnsr.coste, mapa);
				child_turnsr.secuencia.push_back(actTURN_SR);
				if (explored.find(child_turnsr.st) == explored.end())
				{
					
					frontier.push(child_turnsr);
				}
				else if (child_turnsr.st.jugador.f == final.f and child_turnsr.st.jugador.c == final.c){
					current_node = child_turnsr;
					SolutionFound = true;
				}
			}
		



		if(!frontier.empty() and !SolutionFound){
			current_node = frontier.top();	
			while(!frontier.empty() and explored.find(current_node.st) != explored.end()){
				frontier.pop();
				if(!frontier.empty())
					current_node = frontier.top();
			}
		}



	}

	if (SolutionFound){
		plan = current_node.secuencia;
		
		cout<<"Coste Total: "<<current_node.coste<<endl;
		cout<< "TAM  de Frontier: "<<frontier.size()<<endl;
		cout<< "TAM  de Explored: "<<explored.size()<<endl;
		cout << "Plan encontrado: ";
		PintaPlan(current_node.secuencia);}


	return plan;
}









/*       NIVEL 3        */

stateN3 applyN3(const Action &action, const stateN3 &st, int &coste, const vector<vector<unsigned char>> &mapa){
	stateN3 principio = st;
	stateN3 st_result = st;
	ubicacion sig_ubicacion,sig_ubicacion2,sig_ubicacion3;
	switch (action)
	{
	case actWALK: 
		sig_ubicacion = NextCasilla(st.jugador);
		sig_ubicacion2 = NextCasilla(st.colaborador);
		if (CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c))
		{
			if((st_result.ultimaOrdenColaborador == act_CLB_WALK && CasillaTransitable(sig_ubicacion2,mapa) and !(sig_ubicacion2.f == st.jugador.f and sig_ubicacion2.c == st.jugador.c)) 
			or st_result.ultimaOrdenColaborador == act_CLB_TURN_SR or st_result.ultimaOrdenColaborador == act_CLB_STOP)
			{

			

				if (mapa[sig_ubicacion.f][sig_ubicacion.c] == 'K')
				{
					st_result.tiene_bikini = true;
					if (st_result.tiene_zapatillas)
						st_result.tiene_zapatillas = false;
				}

				else if (mapa[sig_ubicacion.f][sig_ubicacion.c] == 'D')
				{
					st_result.tiene_zapatillas = true;
					if (st_result.tiene_bikini)
						st_result.tiene_bikini = false;
				}

				// Actualiza el coste acumulado
				switch (mapa[st.jugador.f][st.jugador.c])
				{
				case 'A':
					if (!st.tiene_bikini)
						coste += 100;
					else
						coste += 10;
					break;

				case 'B':
					if (!st.tiene_zapatillas)
						coste += 50;
					else
						coste += 15;
					break;

				case 'T':
					coste += 2;
					break;

				default:
					coste += 1;
					break;
				}
				st_result.jugador = sig_ubicacion;
		}
	}

	break;

	case actRUN:

		sig_ubicacion3 = NextCasilla(st.colaborador);
		sig_ubicacion = NextCasilla(st.jugador);
			if (CasillaTransitable(sig_ubicacion, mapa) and  !(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c))
			{
				sig_ubicacion2 = NextCasilla(sig_ubicacion);
					if (CasillaTransitable(sig_ubicacion2, mapa) and !(sig_ubicacion2.f == st.colaborador.f and sig_ubicacion2.c == st.colaborador.c))
					{
						st_result.jugador = sig_ubicacion2;
						if((st_result.ultimaOrdenColaborador == act_CLB_WALK && CasillaTransitable(sig_ubicacion3,mapa) and !(sig_ubicacion3.f == st_result.jugador.f and sig_ubicacion3.c == st_result.jugador.c)) 
						or st_result.ultimaOrdenColaborador == act_CLB_TURN_SR or st_result.ultimaOrdenColaborador == act_CLB_STOP)
						{

							st_result.jugador = sig_ubicacion2;

								if (mapa[sig_ubicacion2.f][sig_ubicacion2.c] == 'K')
								{
									st_result.tiene_bikini = true;
									if (st_result.tiene_zapatillas)
										st_result.tiene_zapatillas = false;
								}

								else if (mapa[sig_ubicacion2.f][sig_ubicacion2.c] == 'D')
								{
									st_result.tiene_zapatillas = true;
									if (st_result.tiene_bikini)
										st_result.tiene_bikini = false;
								}

							switch (mapa[st.jugador.f][st.jugador.c])
							{

							case 'A':
								if (!st.tiene_bikini)
									coste += 150;
								else
									coste += 15;
								break;

							case 'B':
								if (!st.tiene_zapatillas)
									coste += 75;
								else
									coste += 25;
								break;

							case 'T':
								coste += 3;
								break;

							default:
								coste += 1;
								break;
						
						
						
							}

						}
						else {
							st_result = st;
						}
					}
		}
	break;


	case actTURN_L:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 6) % 8);

		switch (mapa[st.jugador.f][st.jugador.c])
		{
		case 'A':
			if (!st.tiene_bikini)
				coste += 30;
			else
				coste += 5;
			break;

		case 'B':
			if (!st.tiene_zapatillas)
				coste += 7;
			else
				coste += 1;
			break;

		case 'T':
			coste += 2;
			break;

		default:
			coste += 1;
			break;
		}

		break;

	case actTURN_SR:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 1) % 8);

		switch (mapa[st.jugador.f][st.jugador.c])
		{
		case 'A':
			if (!st.tiene_bikini)
				coste += 10;
			else
				coste += 2;
			break;

		case 'B':
			if (!st.tiene_zapatillas)
				coste += 5;
			else
				coste += 1;
			break;

		case 'T':
			coste += 1;
			break;

		default:
			coste += 1;
			break;
		}

	break;

	case actIDLE:

		st_result = st;

	break;
	
	case act_CLB_WALK:
		sig_ubicacion = NextCasilla(st.colaborador);
		
		
		if (CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.jugador.f && sig_ubicacion.c == st.jugador.c)){



			st_result.colaborador = sig_ubicacion;
			st_result.ultimaOrdenColaborador = act_CLB_WALK;

			if(mapa[sig_ubicacion.f][sig_ubicacion.c] == 'K'){
				st_result.tiene_bikini_colaborador = true;
				if(st_result.tiene_zapatillas_colaborador)
					st_result.tiene_zapatillas_colaborador = false;
			}

			if(mapa[sig_ubicacion.f][sig_ubicacion.c] == 'D'){
				st_result.tiene_zapatillas_colaborador = true;
				if(st_result.tiene_bikini_colaborador)
					st_result.tiene_bikini_colaborador = false;
			}

			switch (mapa[st.colaborador.f][st.colaborador.c])
			{
			case 'A':
				if (!st.tiene_bikini_colaborador)
					coste += 100;
				else
					coste += 10;
				break;

			case 'B':
				if (!st.tiene_zapatillas_colaborador)
					coste += 50;
				else
					coste += 15;
				break;

			case 'T':
				coste += 2;
				break;

			default:
				coste += 1;
				break;
			}
			
			

		}
		
	break;


	case act_CLB_TURN_SR:
		st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula + 1) % 8);
		st_result.ultimaOrdenColaborador = act_CLB_TURN_SR;

		switch (mapa[st.colaborador.f][st.colaborador.c])
		{
		case 'A':
			if (!st.tiene_bikini_colaborador)
				coste += 10;
			else
				coste += 2;
			break;

		case 'B':
			if (!st.tiene_zapatillas_colaborador)
				coste += 5;
			else
				coste += 1;
			break;

		case 'T':
			coste += 1;
			break;

		default:
			coste += 1;
			break;
		}

	break;

	case act_CLB_STOP:
			st_result.colaborador = principio.colaborador;
			st_result.ultimaOrdenColaborador = act_CLB_STOP;
	break;

	}


	if ( action != act_CLB_STOP && action != act_CLB_WALK && action != act_CLB_TURN_SR){
		switch(st.ultimaOrdenColaborador){

			case act_CLB_TURN_SR:
				st_result.colaborador.brujula = static_cast<Orientacion>((st.colaborador.brujula + 1) % 8);
				st_result.ultimaOrdenColaborador = act_CLB_TURN_SR;
				switch (mapa[st.colaborador.f][st.colaborador.c])
				{
					case 'A':
						if (!st.tiene_bikini_colaborador)
							coste += 10;
						else
							coste += 2;
						break;

					case 'B':
						if (!st.tiene_zapatillas_colaborador)
							coste += 5;
						else
							coste += 1;
						break;

					case 'T':
						coste += 1;
						break;

					default:
						coste += 1;
					break;
				}

				
			break;

			case act_CLB_WALK:
				sig_ubicacion = NextCasilla(st.colaborador);
				
				if (CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.jugador.f && sig_ubicacion.c == st.jugador.c))
				{
					st_result.colaborador = sig_ubicacion;
					st_result.ultimaOrdenColaborador = act_CLB_WALK;
					

					if(mapa[sig_ubicacion.f][sig_ubicacion.c] == 'K'){
						st_result.tiene_bikini_colaborador = true;
					if(st_result.tiene_zapatillas_colaborador)
						st_result.tiene_zapatillas_colaborador = false;
					}

					if(mapa[sig_ubicacion.f][sig_ubicacion.c] == 'D'){
						st_result.tiene_zapatillas_colaborador = true;
					if(st_result.tiene_bikini_colaborador)
						st_result.tiene_bikini_colaborador = false;
					}

					switch (mapa[st.colaborador.f][st.colaborador.c])
					{
					case 'A':
						if (!st.tiene_bikini_colaborador)
							coste += 100;
						else
							coste += 10;
						break;

					case 'B':
						if (!st.tiene_zapatillas_colaborador)
							coste += 50;
						else
							coste += 15;
						break;

					case 'T':
						coste += 2;
						break;

					default:
						coste += 1;
						break;
					}

					
				}

				
				
			break;

			case act_CLB_STOP:
				st_result.colaborador = principio.colaborador;
				st_result.ultimaOrdenColaborador = act_CLB_STOP;
			break;

		}
	}


	return st_result;
}


int Heuristica_Chebysev(int f, int c, const ubicacion &final){


	int dist_filas = abs(f - final.f);
	int dist_columnas = abs(c - final.c);
	int resultado = max (dist_filas,dist_columnas);

	

	

	return resultado;
}



list<Action> aEstrella(const stateN3 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	nodeN3 current_node;
	priority_queue<nodeN3> frontier;
	set<stateN3> explored;
	// unordered_set<stateN3> explored;
	/*
		Iba a intentarlo, pero poco tiempo para aprender como crear el hash xd
	
	*/
	list<Action> plan;
	current_node.st = inicio;
	current_node.coste = 0;
	current_node.heuristica = Heuristica_Chebysev(current_node.st.jugador.f,current_node.st.jugador.c, final);
	current_node.total = current_node.coste + current_node.heuristica;

	if (mapa[current_node.st.jugador.f][current_node.st.jugador.c] == 'K')
		current_node.st.tiene_bikini = true;
	if (mapa[current_node.st.jugador.f][current_node.st.jugador.c] == 'D')
		current_node.st.tiene_zapatillas = true;

	if (mapa[current_node.st.colaborador.f][current_node.st.colaborador.c] == 'K')
		current_node.st.tiene_bikini_colaborador = true;
	if (mapa[current_node.st.colaborador.f][current_node.st.colaborador.c] == 'D')
		current_node.st.tiene_zapatillas_colaborador = true;

	bool SolutionFound = (current_node.st.colaborador.f == final.f && current_node.st.colaborador.c == final.c);


	frontier.push(current_node);

	while (!frontier.empty() && !SolutionFound)
	{
			frontier.pop();

			explored.insert(current_node.st);
			if (current_node.st.colaborador.f == final.f and current_node.st.colaborador.c == final.c){
     		SolutionFound = true;
   			}


			if(VeoSonambulo(current_node.st.jugador,current_node.st.colaborador,current_node.st.jugador.brujula)){
			
				// Generar hijo act_CLB_WALK
				nodeN3 child_clb_walk = current_node;
				child_clb_walk.st = applyN3(act_CLB_WALK, current_node.st, child_clb_walk.coste, mapa);
				child_clb_walk.heuristica = Heuristica_Chebysev(child_clb_walk.st.colaborador.f,child_clb_walk.st.colaborador.c, final);
				child_clb_walk.total = child_clb_walk.heuristica + child_clb_walk.coste;
				
				child_clb_walk.secuencia.push_back(act_CLB_WALK);
				if(explored.find(child_clb_walk.st) == explored.end()){
					
					
					frontier.push(child_clb_walk);
				}

				//Genera hijo atc_CLB_TURN_SR
					nodeN3 child_clb_turnsr = current_node;
					child_clb_turnsr.st = applyN3(act_CLB_TURN_SR, current_node.st, child_clb_turnsr.coste, mapa);
					child_clb_turnsr.heuristica = Heuristica_Chebysev(child_clb_turnsr.st.colaborador.f,child_clb_walk.st.colaborador.c, final);
					child_clb_turnsr.total = child_clb_turnsr.heuristica + child_clb_turnsr.coste;
					

					 child_clb_turnsr.secuencia.push_back(act_CLB_TURN_SR);
				if(explored.find(child_clb_turnsr.st) == explored.end())
				{
						 
						frontier.push(child_clb_turnsr);
				}
				

				// Generar hijo act_CLB_STOP				
				nodeN3 child_clb_stop = current_node;

				child_clb_stop.st = applyN3(act_CLB_STOP, current_node.st, child_clb_stop.coste, mapa);
				child_clb_stop.heuristica = Heuristica_Chebysev(child_clb_stop.st.colaborador.f,child_clb_stop.st.colaborador.c, final);
				child_clb_stop.total = child_clb_stop.heuristica + child_clb_stop.coste;
				
				

				child_clb_stop.secuencia.push_back(act_CLB_STOP);
				if(explored.find(child_clb_stop.st ) == explored.end())
				{
					// Añadir hijo a la lista de nodos por explorar
					
					frontier.push(child_clb_stop );
				}	
				
					
			}
				
			if(!SolutionFound){

			// Generar hijo actIDLE
				nodeN3 child_idle = current_node;
				child_idle.st = applyN3(actIDLE, current_node.st, child_idle.coste,mapa);
				child_idle.heuristica = Heuristica_Chebysev(child_idle .st.colaborador.f,child_idle.st.colaborador.c, final);
				child_idle.total = child_idle.heuristica + child_idle.coste;
				
				child_idle.secuencia.push_back(actIDLE);
				if (explored.find(child_idle.st)== explored.end())
				{
					
					frontier.push(child_idle);
				}
				

				nodeN3 child_run = current_node;
						child_run.st = applyN3(actRUN, current_node.st,child_run.coste, mapa);
						child_run.heuristica = Heuristica_Chebysev(child_run.st.colaborador.f,child_run.st.colaborador.c, final);
						child_run.total = child_run.heuristica + child_run.coste;
						
						child_run.secuencia.push_back(actRUN);
						if (explored.find(child_run.st) == explored.end()){
							
							frontier.push(child_run);
						}
						
				//GENERAR HIJO ACT_WALK
					nodeN3 child_walk = current_node;
					child_walk.st = applyN3(actWALK, current_node.st, child_walk.coste, mapa);
					child_walk.heuristica = Heuristica_Chebysev(child_walk.st.colaborador.f,child_walk.st.colaborador.c, final);
					child_walk.total = child_walk.heuristica + child_walk.coste;
					
					
					child_walk.secuencia.push_back(actWALK);
					if (explored.find(child_walk.st)== explored.end())
					{
						
						frontier.push(child_walk);
					}	

					// Genera hijo actTURN_SR
					nodeN3 child_turnsr = current_node;
					child_turnsr.st = applyN3(actTURN_SR, current_node.st, child_turnsr.coste, mapa);
					child_turnsr.heuristica = Heuristica_Chebysev(child_turnsr.st.colaborador.f,child_turnsr.st.colaborador.c, final);
					child_turnsr.total = child_turnsr.heuristica + child_turnsr.coste;
					
					child_turnsr.secuencia.push_back(actTURN_SR);
					if (explored.find(child_turnsr.st) == explored.end())
					{
						
						frontier.push(child_turnsr);
					}	

					// Genera hijo actTURN_L
					nodeN3 child_turnl = current_node;
					child_turnl.st = applyN3(actTURN_L, current_node.st, child_turnl.coste, mapa);
					child_turnl.heuristica = Heuristica_Chebysev(child_turnl.st.colaborador.f,child_turnl.st.colaborador.c, final);
					child_turnl.total = child_turnl.heuristica + child_turnl.coste;
					
					child_turnl.secuencia.push_back(actTURN_L);
					if (explored.find(child_turnl.st) == explored.end())
					{
						
						frontier.push(child_turnl);
					}

				
						
			}
			
					
			
		
					
		if(!frontier.empty() and !SolutionFound){
			current_node = frontier.top();	
			while(!frontier.empty() and explored.find(current_node.st) != explored.end()){
				frontier.pop();
				if (current_node.st.colaborador.f == final.f and current_node.st.colaborador.c == final.c){
     			SolutionFound = true;
   				}
				if(!frontier.empty())
					current_node = frontier.top();
			}
		}
	
	}
	if (SolutionFound)
	{
		plan = current_node.secuencia;
		cout<<"Coste Total: "<<current_node.coste<<endl;
		cout<< "TAM  de Frontier: "<<frontier.size()<<endl;
		cout<< "TAM  de Explored: "<<explored.size()<<endl;
		cout << "Plan encontrado: ";
		PintaPlan(current_node.secuencia);
		
	}
		
	return plan;
}