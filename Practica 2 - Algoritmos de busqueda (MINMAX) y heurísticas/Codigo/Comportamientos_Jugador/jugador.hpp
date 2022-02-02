#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>
#include <vector>

struct estado { //nivel 3 iran aqui objetivos
  //estado():bikini(0),zapatillas(0){}
  int fila;
  int columna;
  int orientacion;
  bool bikini;
	bool zapatillas;
  vector<estado> visitados;
  int nvisitados=0;
  bool visited[3]={};
};



class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {

    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    void pintamapa(Sensores origen);

    int interact(Action accion, int valor);
    void VisualizaPlan(const estado &st, const list<Action> &plan);
    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

  private:
    // Declarar Variables de Estado
    estado actual;
    list<estado> objetivos;
    list<Action> plan;
    bool hayPlan;
    bool visto;

    // Métodos privados de la clase
    bool pathFinding(int level, const estado &origen, const list<estado> &destino, list<Action> &plan);
    bool pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Aestrella(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_A3(const estado &origen, const list<estado> destinos, list<Action> &plan);

    void PintaPlan(list<Action> plan);
    bool HayObstaculoDelante(estado &st);
    friend class supernodo;

};

#endif
