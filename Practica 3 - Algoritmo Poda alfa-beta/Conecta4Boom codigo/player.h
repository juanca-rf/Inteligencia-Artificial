#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"


// struct Nodo{
//    Environment entorno;
//    Nodo *hijos;
//    int num_hijos;
//    int valoracion;
// };

// struct ValorLastAction
// {
//    double valor;
//    double alfa;
//    double beta;
//    Environment::ActionType lastAction;
// };

class Player{
    public:
      Player(int jug);
      Environment::ActionType Think();
      void Perceive(const Environment &env);
      // double minimax( Nodo &actual, int profundidad, int jugador );
      // ValorLastAction minimax2(Environment &actual, int profundidad, int jugador);
      // ValorLastAction alfabeta(Environment &actual, int profundidad, double alfa, double beta, int jugador);
      pair<double, Environment::ActionType> alfabeta(Environment &actual, int profundidad, double alfa, double beta, int jugador);
    private:
      int jugador_;
      Environment actual_;
};
#endif
