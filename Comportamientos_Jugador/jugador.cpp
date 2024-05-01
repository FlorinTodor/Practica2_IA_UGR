#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>

// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"

bool Find(const stateN0 &item, const list<nodeN0> &lista);
bool Find(const stateN0 &item, const list<stateN0> &lista);
void PintaPlan(const list<Action> &plan);
list<Action> AnchuraSoloJugador(const stateN0 &inicio, const ubicacion &final,const vector<vector<unsigned char>> &mapa);
list<Action> AnchuraSonambulo(const stateN1 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);
stateN1 applyN1(const Action &a, const stateN1 &st, const vector<vector<unsigned char>> &mapa);


/* METODOS DE BUSQUEDA DEL COLABORADOR*/
bool estaEnCampoVision(const stateN1 &st);

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

			
		

			switch(sensores.nivel){
				case 0: plan = AnchuraSoloJugador(c_state,goal,mapaResultado);
				break;
				case 1: plan = AnchuraSonambulo(c_stateN1,goal,mapaResultado);
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


/** Devuelve el estado que se genera si el agente puede avanzar.
 * Si no puede avanzar, devuelve como salida el mismo estado de entrada
*/
stateN0 apply(const Action &a, const stateN0 &st, const vector<vector<unsigned char> > mapa){
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



/** pone a cero todos los elementos de una matriz */
void AnulaMatriz(vector<vector<unsigned char> > &matriz){
	for (int i = 0; i < matriz.size(); i++)
		for (int j = 0; j < matriz[0].size(); j++)
			matriz[i][j] = 0;
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


list<Action> AnchuraSonambulo(const stateN1 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
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

		
		if(estaEnCampoVision(current_node.st))
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

			// Generar hijo actWALK

				


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


			// GENERAR HIJO ACTRUN

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
				
				
				

				// Generar hijo actTurn_R
				nodeN1 child_turnsr = current_node;
				child_turnsr.st = applyN1(actTURN_SR, current_node.st, mapa);
				child_turnsr.secuencia.push_back(actTURN_SR);
				
				if (explored.find(child_turnsr) == explored.end())
				{
					
					frontier.push_back(child_turnsr);
				}
				
				// Generar hijo actTurn_L
				nodeN1 child_turnl = current_node;
				child_turnl.st = applyN1(actTURN_L, current_node.st, mapa);
				child_turnl.secuencia.push_back(actTURN_L);
				
				if (explored.find(child_turnl) == explored.end())
				{
					
					frontier.push_back(child_turnl);
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

bool estaEnCampoVision(const stateN1 &st)
{
	ubicacion ubicacion_colaborador = st.colaborador;
	ubicacion ubicacion_jugador = st.jugador;
	stateN1 c_state = st;

	Orientacion orientacion_jugador = ubicacion_jugador.brujula;
	int fil = ubicacion_jugador.f;
	int col = ubicacion_jugador.c;

	bool presente = false;

	switch (orientacion_jugador)
	{
		case norte:
			if ((ubicacion_colaborador.c == col) && ((fil - ubicacion_colaborador.f) <= 3) && ((fil - ubicacion_colaborador.f) > 0))
				presente = true;

			else if (((ubicacion_colaborador.c == col - 1) || (ubicacion_colaborador.c == col + 1)) && ((fil - ubicacion_colaborador.f) <= 3) && ((fil - ubicacion_colaborador.f) >= 1))
				presente = true;

			else if (((ubicacion_colaborador.c == col - 2) || (ubicacion_colaborador.c == col + 2)) && ((fil - ubicacion_colaborador.f) <= 3) && ((fil - ubicacion_colaborador.f) >= 2))
				presente = true;

			else if (((ubicacion_colaborador.c == col - 3) || (ubicacion_colaborador.c == col + 3)) && ((fil - ubicacion_colaborador.f) == 3))
				presente = true;

			else
				presente = false;
		break;

		case noreste:

			if ((ubicacion_colaborador.c == col) && ((fil - ubicacion_colaborador.f) <= 3) && ((fil - ubicacion_colaborador.f) > 0))
				presente = true;

			else if (( (ubicacion_colaborador.c == col + 1)) && ((fil - ubicacion_colaborador.f) <= 3) && ((fil - ubicacion_colaborador.f) >= 1))
				presente = true;

			else if (( (ubicacion_colaborador.c == col + 2)) && ((fil - ubicacion_colaborador.f) <= 3) && ((fil - ubicacion_colaborador.f) >= 2))
				presente = true;

			else if (( (ubicacion_colaborador.c == col + 3)) && ((fil - ubicacion_colaborador.f) == 3))
				presente = true;

			else
				presente = false;
			
		break;

		case noroeste:

		if ((ubicacion_colaborador.c == col) && ((fil - ubicacion_colaborador.f) <= 3) && ((fil - ubicacion_colaborador.f) > 0))
				presente = true;

			else if (((ubicacion_colaborador.c == col - 1)) && ((fil - ubicacion_colaborador.f) <= 3) && ((fil - ubicacion_colaborador.f) >= 1))
				presente = true;

			else if (((ubicacion_colaborador.c == col - 2) ) && ((fil - ubicacion_colaborador.f) <= 3) && ((fil - ubicacion_colaborador.f) >= 2))
				presente = true;

			else if (((ubicacion_colaborador.c == col - 3) ) && ((fil - ubicacion_colaborador.f) == 3))
				presente = true;

			else
				presente = false;
		break;
		

		case sur:
			if ((ubicacion_colaborador.c == col) && ((ubicacion_colaborador.f - fil) <= 3) && ((ubicacion_colaborador.f - fil) > 0))
				presente = true;

			else if (((ubicacion_colaborador.c == col - 1) || (ubicacion_colaborador.c == col + 1)) && ((ubicacion_colaborador.f - fil) <= 3) && ((ubicacion_colaborador.f - fil) >= 1))
				presente = true;

			else if (((ubicacion_colaborador.c == col - 2) || (ubicacion_colaborador.c == col + 2)) && ((ubicacion_colaborador.f - fil) <= 3) && ((ubicacion_colaborador.f - fil) >= 2))
				presente = true;

			else if (((ubicacion_colaborador.c == col - 3) || (ubicacion_colaborador.c == col + 3)) && ((ubicacion_colaborador.f - fil) == 3))
				presente = true;

			else
				presente = false;
			break;

		case sureste:

			if ((ubicacion_colaborador.c == col) && ((ubicacion_colaborador.f - fil) <= 3) && ((ubicacion_colaborador.f - fil) > 0))
				presente = true;

			else if (((ubicacion_colaborador.c == col + 1)) && ((ubicacion_colaborador.f - fil) <= 3) && ((ubicacion_colaborador.f - fil) >= 1))
				presente = true;

			else if (( (ubicacion_colaborador.c == col + 2)) && ((ubicacion_colaborador.f - fil) <= 3) && ((ubicacion_colaborador.f - fil) >= 2))
				presente = true;

			else if (((ubicacion_colaborador.c == col + 3)) && ((ubicacion_colaborador.f - fil) == 3))
				presente = true;

			else
				presente = false;
		
		break;

		case suroeste:

			if ((ubicacion_colaborador.c == col) && ((ubicacion_colaborador.f - fil) <= 3) && ((ubicacion_colaborador.f - fil) > 0))
				presente = true;

			else if (((ubicacion_colaborador.c == col - 1) ) && ((ubicacion_colaborador.f - fil) <= 3) && ((ubicacion_colaborador.f - fil) >= 1))
				presente = true;

			else if (((ubicacion_colaborador.c == col - 2)) && ((ubicacion_colaborador.f - fil) <= 3) && ((ubicacion_colaborador.f - fil) >= 2))
				presente = true;

			else if (((ubicacion_colaborador.c == col - 3) ) && ((ubicacion_colaborador.f - fil) == 3))
				presente = true;

			else
				presente = false;
		break;


		case este:
			if ((ubicacion_colaborador.f == fil) && ((ubicacion_colaborador.c - col) <= 3) && ((ubicacion_colaborador.c - col) > 0))
				presente = true;

			else if (((ubicacion_colaborador.f == fil + 1) || (ubicacion_colaborador.f == fil - 1)) && ((ubicacion_colaborador.c - col) <= 3) && ((ubicacion_colaborador.c - col) >= 1))
				presente = true;

			else if (((ubicacion_colaborador.f == fil + 2) || (ubicacion_colaborador.f == fil - 2)) && ((ubicacion_colaborador.c - col) <= 3) && ((ubicacion_colaborador.c - col) >= 2))
				presente = true;

			else if (((ubicacion_colaborador.f == fil + 3) || (ubicacion_colaborador.f == fil - 3)) && ((ubicacion_colaborador.c - col) == 3))
				presente = true;

			else
				presente = false;
			break;

		case oeste:
			if ((ubicacion_colaborador.f == fil) && ((col - ubicacion_colaborador.c) <= 3) && ((col - ubicacion_colaborador.c) > 0))
				presente = true;

			else if (((ubicacion_colaborador.f == fil - 1) || (ubicacion_colaborador.f == fil + 1)) && ((col - ubicacion_colaborador.c) <= 3) && ((col - ubicacion_colaborador.c) >= 1))
				presente = true;

			else if (((ubicacion_colaborador.f == fil - 2) || (ubicacion_colaborador.f == fil + 2)) && ((col - ubicacion_colaborador.c) <= 3) && ((col - ubicacion_colaborador.c) >= 2))
				presente = true;

			else if (((ubicacion_colaborador.f == fil - 3) || (ubicacion_colaborador.f == fil + 3)) && ((col - ubicacion_colaborador.c) == 3))
				presente = true;

			else
				presente = false;
			break;
	
	}

	return presente;
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