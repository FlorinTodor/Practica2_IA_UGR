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



struct stateN1{
  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador;

  bool operator== (const stateN1 &x) const{
    if(jugador == x.jugador && colaborador.f == x.colaborador.f && colaborador.c == x.colaborador.c && ultimaOrdenColaborador == x.ultimaOrdenColaborador)  return true;
    else {return false;}
  }
  
};

struct stateN2{
  ubicacion jugador;
  ubicacion colaborador;
  bool tiene_zapatillas, tiene_bikini;

  bool operator== (const stateN2 &x) const{
    if(jugador == x.jugador && colaborador.f == x.colaborador.f && colaborador.c == x.colaborador.c)  return true;
    else return false;
  }

  bool operator<(const stateN2 &st) const{
    
      if(jugador.f < st.jugador.f){ return true;}
          else if (jugador.f == st.jugador.f && jugador.c < st.jugador.c) { return true;}
          else if  (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula < st.jugador.brujula) { return true;}
          else if (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula && tiene_zapatillas < st.tiene_zapatillas) { return true;}
          else if (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula && tiene_zapatillas == st.tiene_zapatillas && tiene_bikini < st.tiene_bikini){ return true;}
      else{
          return false;
          }
  }
};

struct stateN3{
  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador;
  bool tiene_zapatillas, tiene_bikini, tiene_zapatillas_colaborador, tiene_bikini_colaborador;

  bool operator== (const stateN3 &x) const{
    if(jugador == x.jugador && colaborador.f == x.colaborador.f && colaborador.c == x.colaborador.c &&  tiene_zapatillas == x.tiene_zapatillas && tiene_bikini == x.tiene_bikini && tiene_zapatillas_colaborador == x.tiene_zapatillas_colaborador && tiene_bikini_colaborador == x.tiene_bikini_colaborador && ultimaOrdenColaborador == x.ultimaOrdenColaborador)  return true;
    else return false;
  }

  bool operator<(const stateN3 &st) const{
          if(jugador.f < st.jugador.f){ return true;}
          else if (jugador.f == st.jugador.f && jugador.c < st.jugador.c) { return true;}
          else if (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula < st.jugador.brujula) { return true;}
          else if (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula && tiene_zapatillas < st.tiene_zapatillas) { return true;}
          else if (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula && tiene_zapatillas == st.tiene_zapatillas && tiene_bikini < st.tiene_bikini){ return true;}
          else if (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula && tiene_zapatillas == st.tiene_zapatillas && tiene_bikini == st.tiene_bikini && colaborador.f < st.colaborador.f) { return true;}
          else if (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula && tiene_zapatillas == st.tiene_zapatillas && tiene_bikini == st.tiene_bikini && colaborador.f == st.colaborador.f && colaborador.c < st.colaborador.c) { return true;}
          else if (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula && tiene_zapatillas == st.tiene_zapatillas && tiene_bikini == st.tiene_bikini  && colaborador.f == st.colaborador.f && colaborador.c == st.colaborador.c && colaborador.brujula < st.colaborador.brujula) { return true;}
          else if (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula &&  tiene_zapatillas == st.tiene_zapatillas && tiene_bikini == st.tiene_bikini && colaborador.f == st.colaborador.f && colaborador.c == st.colaborador.c && colaborador.brujula == st.colaborador.brujula && tiene_zapatillas_colaborador < st.tiene_zapatillas_colaborador) { return true;}
          else if (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula &&  tiene_zapatillas == st.tiene_zapatillas && tiene_bikini == st.tiene_bikini && colaborador.f == st.colaborador.f && colaborador.c == st.colaborador.c && colaborador.brujula == st.colaborador.brujula && tiene_zapatillas_colaborador == st.tiene_zapatillas_colaborador && tiene_bikini_colaborador < st.tiene_bikini_colaborador ) { return true;}
          else if (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula &&  tiene_zapatillas == st.tiene_zapatillas && tiene_bikini == st.tiene_bikini && colaborador.f == st.colaborador.f && colaborador.c == st.colaborador.c && colaborador.brujula == st.colaborador.brujula && tiene_zapatillas_colaborador == st.tiene_zapatillas_colaborador && tiene_bikini_colaborador == st.tiene_bikini_colaborador && ultimaOrdenColaborador < st.ultimaOrdenColaborador){ return true;}
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


struct nodeN1{
  stateN1 st;
  list<Action> secuencia;

  bool operator==(const nodeN1 &nd) const{
    return (st == nd.st);
  }

  bool operator<(const nodeN1 &b) const{

    if ( st.jugador.f < b.st.jugador.f){ return true;}
    else if ( st.jugador.f == b.st.jugador.f && st.jugador.c <b.st.jugador.c){ return true;}
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c  == b.st.jugador.c && st.jugador.brujula < b.st.jugador.brujula) {return true;}
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c  == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula && st.colaborador.f < b.st.colaborador.f){ return true;}
    else if ( st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula && st.colaborador.f == b.st.colaborador.f &&  st.colaborador.c < b.st.colaborador.c){return true;}
    else if ( st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula && st.colaborador.f == b.st.colaborador.f &&  st.colaborador.c == b.st.colaborador.c && st.colaborador.brujula < b.st.colaborador.brujula){return true;}
    else if ( st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula && st.colaborador.f == b.st.colaborador.f &&  st.colaborador.c == b.st.colaborador.c && st.colaborador.brujula ==  b.st.colaborador.brujula && st.ultimaOrdenColaborador < b.st.ultimaOrdenColaborador){return true;}
    else{ return false;}
}
};

struct nodeN2{
  stateN2 st;
  list<Action> secuencia;
  int coste;

  bool operator==(const nodeN2 &nd) const{
    return (st == nd.st);
  }

  bool operator<(const nodeN2 &nd) const
  {
        if (coste > nd.coste) { return true;}
        else{
          return false;
        }
  }
};

struct nodeN3{
  stateN3 st;
  list<Action> secuencia;
  int coste;
  int heuristica;
  int total;

  bool operator==(const nodeN3 &nd) const{
    return (st == nd.st);
  }

  bool operator<(const nodeN3 &nd) const{
    if (total > nd.total) { return true;}
        else{
          return false;
    }
  }
};

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
       hayPlan = false;
       
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
   
    ubicacion goal;

    stateN0 c_state;
    stateN1 c_stateN1;
    stateN2 c_stateN2;
    stateN3 c_stateN3;




};

#endif
