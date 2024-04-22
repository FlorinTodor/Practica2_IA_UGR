#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H
#include "comportamientos/comportamiento.hpp"

#include <list>

/*

struct ubicacion{
  int f;
  int c;
  Orientacion brujula;
  bool operator==(const ubicacion & ub) const{
    return (f == ub.f and c == ub.c and brujula== ub.brujula);
  }
}*/

struct stateN0{
  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador;

  bool operator==(const stateN0 &x)const{
    if(jugador == x.jugador and colaborador.f == x.colaborador.f and colaborador.c == x.colaborador.c){ return true;}
    else{ return false;}
  }

};

struct nodeN0{
  stateN0 st;
  list<Action> secuencia;

  bool operator==(const nodeN0 &b) const{
    return ( st == b.st);
  }

  bool operator<(const nodeN0 &b) const{
    if(st.jugador.f < b.st.jugador.f){
      return true;
    }
    else if(st.jugador.f  == b.st.jugador.f  and st.jugador.c < b.st.jugador.c){return true;}
    else if( st.jugador.f  == b.st.jugador.f  and st.jugador.c == b.st.jugador.c and st.jugador.brujula < b.st.jugador.brujula ){ return true;}
    else{ return false;}
  }
};



class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      /*
      hayPlan = false;
      c_state.jugador.f == 99;
      c_state.jugador.c == 99;
      c_state.jugador.brujula = norte;
      c_state.colaborador.f = 99;
      c_state.colaborador.c = 99;
      c_state.colaborador.brujula = norte;
      c_state.ultimaOrdenColaborador= actIDLE;
      goal.brujula = norte;
      goal.f = 99;
      goal.c = 99;

*/
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      hayPlan = false;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);

  void VisualizaPlan(const stateN0 &st, const list<Action> &plan);


  private:
    // Declarar Variables de Estado
    list<Action> plan;
    bool hayPlan;
    stateN0 c_state;
    ubicacion goal;




};

#endif
