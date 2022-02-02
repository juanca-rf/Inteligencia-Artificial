#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"

#define bomba(a) ((char)(a + 3)) //Macro que devuelve la casilla bomba correspondiente al jugador a
#define mp(a,b) make_pair(a,b)

using namespace std;

const double masinf = 99999999.0, menosinf = -99999999.0;

const int tamDiagonal = 7;
const pair<int, int> inicioDerechas[tamDiagonal] = {mp(3,0), mp(2,0), mp(1,0), mp(0,0), mp(0,1), mp(0,2), mp(0,3)};
const pair<int, int> inicioIzquierdas[tamDiagonal] = {mp(3,0), mp(4,0), mp(5,0), mp(6,0), mp(6,1), mp(6,2), mp(6,3)};
const int TAMDIAG[tamDiagonal] = {4, 5, 6, 7, 6, 5, 4};
// Constructor
Player::Player(int jug)
{
   jugador_ = jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment &env)
{
   actual_ = env;
}

double Puntuacion(int jugador, const Environment &estado)
{
   double suma = 0;

   for (int i = 0; i < 7; i++)
      for (int j = 0; j < 7; j++)
      {
         if (estado.See_Casilla(i, j) == jugador)
         {
            if (j < 3)
               suma += j;
            else
               suma += (6 - j);
         }
      }

   return suma;
}

// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador)
{
   int ganador = estado.RevisarTablero();

   if (ganador == jugador)
      return 99999999.0; // Gana el jugador que pide la valoracion
   else if (ganador != 0)
      return -99999999.0; // Pierde el jugador que pide la valoracion
   else if (estado.Get_Casillas_Libres() == 0)
      return 0; // Hay un empate global y se ha rellenado completamente el tablero
   else
      return Puntuacion(jugador, estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar

/**
 * @brief Valora las posibilidades de ganar de @a jugador de la lina del mazo pasada 
 * 
 * @param linea 
 * @param ocupacion 
 * @param jugador 
 * @return double 
 */
double evaluaLinea(char *linea, int ocupacion, int jugador)
{
   char cjugador = jugador;
   char contrincante = (jugador == 1) ? 2 : 1; //invierte jugador
   char casilla;                               //Variable auxiliar para hacer solo un acceso a memoria
   int valoracion = 0;

   for (int i = 0; i < ocupacion; i++)
   {
      casilla = linea[i];
      if (casilla == (char)jugador)
      {
         if (valoracion < 0)
            valoracion = (valoracion*-1)+(7 - ocupacion); //VALORAR POSITIVAMENTE QUITAR CASILLAS CONTRINCANTE SEGUIDAS en funcion de la ocupacion
         valoracion++;
      }
      else
      {
         if (valoracion > 0)
            valoracion = (valoracion*-1)-(7 - ocupacion);
         valoracion --;
      }
   }
   return valoracion;
}

/**
 * @brief Evalua las posibilidades de ganar de forma linea vertical de @a juagdor_actual respecto el tablero @a estado
 * 
 * @param estado Mazo
 * @param jugador_actual 
 * @return double Valoracion
 */
double posibilidadesVerticales2(const Environment &estado, int jugador_actual)
{
   int valoracion = 0;
   int ocupacion;
   int iterator = 0;
   char casilla;

   for (int col = 0; col < 7; col++) //Recorro las columnas
   {
      ocupacion = estado.Get_Ocupacion_Columna(col);

      if(ocupacion == 7 && estado.Last_Action(jugador_actual)==col)
         return 0;

      if (ocupacion < 7 && ocupacion != 0) //Si esta llena  o vacia no se evalua
      {
         char columna[ocupacion];
         
         for (int fil = 0; fil <= ocupacion; fil++)
           columna[fil] = estado.See_Casilla(fil,col);
         
         valoracion += evaluaLinea(columna, ocupacion + 1, jugador_actual);
      }else valoracion=0;
   }
   return valoracion;
}

/**
 * @brief Evalua las posibilidades de ganar de forma linea horizontal de @a jugador_actual respecto el tablero @a estado
 * 
 * @param estado 
 * @param jugador_actual 
 * @return double 
 */
double posibilidadesHorizontales(const Environment &estado, int jugador_actual) 
{
   int valoracion = 0;
   char casilla;
   char fila[7];
   char fila2[7]; //En el otro sentido
   int iteradorReverse = 6;

   for (int fil = 6; fil >= 0; fil--)
   {
      for (int col = 0; col < 7; col++)
      {
         casilla = estado.See_Casilla(fil, col);
         fila[col] = casilla;
         fila2[iteradorReverse] = casilla;
      }
      valoracion += evaluaLinea(fila, 7, jugador_actual);
      valoracion += evaluaLinea(fila2, 7, jugador_actual);
   }
   return valoracion;
}

/**
 * @brief Calcula las posibilidades de ganar o perde el @a jugador_actual en dos tipos de diagonales: las que ascienden a derechas y las de izquierdas. Estas tienen un inicio predefinido y un tamaño precalculado.
 * 
 * @param estado 
 * @param jugador_actual 
 * @return double 
 */
double posibilidadesDiagonales(const Environment &estado, int jugador_actual){
   int valoracion = 0;
   bool vacio = true;
   int fil;
   int col;
   char aux;
   int j;
   int ij;//Iterador invertido

   //Evaluar Derechas
   for( int i = 0; i < tamDiagonal; i++ ){
      char casillas[TAMDIAG[i]];
      char icasillas[TAMDIAG[i]]; //invertidas
      
      fil = inicioDerechas[i].first;
      col = inicioDerechas[i].second;

      vacio = true;
      for( j=0, ij=TAMDIAG[i]-1; j < TAMDIAG[i]; j++, ij-- ){
         aux = estado.See_Casilla(fil+j,col+j);
         casillas[j] = aux;
         icasillas[ij] = aux;
         if( aux != 0 ) vacio = false;
      }

      if( !vacio ){
         valoracion += evaluaLinea(casillas,TAMDIAG[i], jugador_actual);
         valoracion += evaluaLinea(icasillas,TAMDIAG[i], jugador_actual);
      }
   }

   //Evaluar Izquierdas
   for( int i = 0; i < tamDiagonal; i++ ){
      char casillas[TAMDIAG[i]];
      char icasillas[TAMDIAG[i]]; //invertidas
      
      fil = inicioIzquierdas[i].first;
      col = inicioIzquierdas[i].second;

      vacio = true;
      for( j=0, ij=TAMDIAG[i]-1; j <TAMDIAG[i]; j++, ij-- ){
         aux = estado.See_Casilla(fil-j,col-j);
         casillas[j] = aux;
         icasillas[ij] = aux;
         if( aux != 0 ) vacio = false;
      }
       if( !vacio ){
         valoracion += evaluaLinea(casillas,TAMDIAG[i], jugador_actual);
         valoracion += evaluaLinea(icasillas,TAMDIAG[i], jugador_actual);
      }
   }
   
   return valoracion;
}

double Valoracion(const Environment &estado, int jugador)
{
   double valor = 0;
   if (estado.JuegoTerminado())
   {
      valor = estado.RevisarTablero();
      if (valor == 0.0)
         return 0;
      else 
      if (valor == jugador)
         return masinf;
      else
         return menosinf;
   }
   else
   {
      if (estado.Get_Casillas_Libres() == 0)
         return 0;
      else
      {
         //Valorar verticales
         valor += posibilidadesVerticales2(estado, jugador);
         // cout << "                     valor VERTICAL " << valor << endl;

         //valorar horizontales
         valor += posibilidadesHorizontales(estado, jugador);
         // cout << "                     valor HORIZONTAL " << valor << endl;

         //valorar diagonales
         valor += posibilidadesDiagonales(estado, jugador);
         // cout << "                     valor DIAGONAL " << valor << endl;

         valor += Puntuacion(jugador,estado);
      }
   }
   //cout<< valor << endl;
   return valor;
}

// // Esta funcion no se puede usar en la version entregable
// // Aparece aqui solo para ILUSTRAR el comportamiento del juego
// // ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
// void JuegoAleatorio(bool aplicables[], int opciones[], int &j)
// {
//    j = 0;
//    for (int i = 0; i < 8; i++)
//    {
//       if (aplicables[i])
//       {
//          opciones[j] = i;
//          j++;
//       }
//    }
// }

// /**
//  * @brief Funcion minimax ANTIGUA, Segmentation fault por no eliminar hijos del heap
//  *
//  * @param actual
//  * @param profundidad
//  * @param jugador
//  * @return double
//  */
// double Player::minimax(Nodo &actual, int profundidad, int jugador)
// {
//    int valor = 0;
//    Environment hijos[8];
//    //cout << "   CALCULO MINIMAX1 JUGADOR "<< jugador_ << endl;
//    actual.valoracion = masinf;

//    if (profundidad == 0 || actual.entorno.JuegoTerminado())
//       return Valoracion(actual.entorno, jugador);

//    actual.hijos = new Nodo[8];
//    actual.num_hijos = actual.entorno.GenerateAllMoves(hijos);
//    for (int i = 0; i < actual.num_hijos; i++)
//       actual.hijos[i].entorno = hijos[i];

//    if (jugador == jugador_)
//    { //maximizar
//       double maximo_valor = menosinf;
//       for (int i = 0; i < actual.num_hijos; i++)
//       {
//          valor = minimax(actual.hijos[i], (profundidad - 1), (jugador == 1) ? 2 : 1);

//          if (valor > maximo_valor)
//             maximo_valor = valor;
//       }
//       actual.valoracion = maximo_valor;
//       return maximo_valor;
//    }
//    else
//    { //minimizar
//       double maximo_valor = masinf;
//       for (int i = 0; i < actual.num_hijos; i++)
//       {
//          valor = minimax(actual.hijos[i], (profundidad - 1), (jugador == 1) ? 1 : 2);
//          if (valor < maximo_valor) //maxmimovalor == minimovalor
//             maximo_valor = valor;
//       }
//       actual.valoracion = maximo_valor;
//       return maximo_valor;
//    }
// }

// // struct ValorLastAction
// // {
// //    double valor;
// //    Environment::ActionType lastAction;
// // };

// ValorLastAction Player::minimax2(Environment &actual, int profundidad, int jugador)
// {
//    int numHijos;
//    Environment hijos[8];
//    ValorLastAction mejorPar;
//    cout << "   CALCULO MINIMAX2 JUGADOR " << jugador_ << " profundidad " << profundidad << endl;

//    if (profundidad == 0 || actual.JuegoTerminado())
//    {
//       mejorPar.lastAction = static_cast<Environment::ActionType>(actual.Last_Action(jugador));
//       mejorPar.valor = Valoracion(actual, jugador);
//       cout << mejorPar.valor << "VALORACION #############################" << endl;
//       return mejorPar;
//    }

//    numHijos = actual.GenerateAllMoves(hijos);
//    cout << "Numero de hijos generado y profundidad actual y jugador " << numHijos << " " << profundidad << " " << jugador << endl;

//    if (jugador == jugador_)
//    { //maximizar
//       mejorPar.valor = menosinf;
//       double valor;
//       for (int i = 0; i < numHijos; i++)
//       {
//          valor = (minimax2(hijos[i], (profundidad - 1), (jugador == 1) ? 2 : 1)).valor;
//          cout << "                        MAXIMIZAR VALOR > MAXIMO VALOR " << valor << " " << mejorPar.valor << endl;
//          if (valor > mejorPar.valor)
//          {
//             mejorPar.valor = valor;
//             mejorPar.lastAction = static_cast<Environment::ActionType>(hijos[i].Last_Action(jugador));
//          }
//       }
//       return mejorPar;
//    }
//    else
//    { //minimizar
//       mejorPar.valor = masinf;
//       double valor;
//       for (int i = 0; i < numHijos; i++)
//       {
//          valor = (minimax2(hijos[i], (profundidad - 1), jugador)).valor;
//          cout << "                        Minimizar VALOR < Minimo VALOR " << valor << " " << mejorPar.valor << endl;

//          if (valor < mejorPar.valor)
//          {
//             mejorPar.valor = valor;
//             mejorPar.lastAction = static_cast<Environment::ActionType>(hijos[i].Last_Action(jugador));
//          }
//       }
//       return mejorPar;
//    }
// }

// ValorLastAction Player::alfabeta(Environment &actual, int profundidad, double alfa, double beta, int jugador)
// {
//    int numHijos;
//    Environment hijos[8];
//    ValorLastAction mejorPar;
//    cout << "   CALCULO ALFABETA JUGADOR " << jugador_ << " profundidad " << profundidad << endl;

//    if (profundidad == 0 || actual.JuegoTerminado())
//    {
//       mejorPar.lastAction = static_cast<Environment::ActionType>(actual.Last_Action(jugador));
//       mejorPar.valor = Valoracion(actual, jugador);
//       cout << mejorPar.valor << "VALORACION #############################" << endl;
//       return mejorPar;
//    }

//    numHijos = actual.GenerateAllMoves(hijos);
//    cout << "Numero de hijos generado y profundidad actual y jugador " << numHijos << " " << profundidad << " " << jugador << endl;

//    if (jugador == jugador_)
//    { //maximizar
//       mejorPar.valor = menosinf;
//       double valor;
//       for (int i = 0; i < numHijos; i++)
//       {
//          valor = (alfabeta(hijos[i], (profundidad - 1), alfa, beta, (jugador == 1) ? 2 : 1)).alfa;
//          cout << "                        MAXIMIZAR VALOR > MAXIMO VALOR " << valor << " " << mejorPar.valor << endl;
//          if (valor > alfa)
//          {
//             alfa = valor;
//             mejorPar.alfa = valor;
//             mejorPar.valor = valor;
//             mejorPar.lastAction = static_cast<Environment::ActionType>(hijos[i].Last_Action(jugador));
//             if (beta <= alfa)
//                i = numHijos; //break
//          }
//       }
//       return mejorPar;
//    }
//    else
//    { //minimizar
//       mejorPar.valor = masinf;
//       double valor;
//       for (int i = 0; i < numHijos; i++)
//       {
//          valor = (alfabeta(hijos[i], (profundidad - 1), alfa, beta, jugador)).beta;
//          cout << "                        Minimizar VALOR < Minimo VALOR " << valor << " " << mejorPar.valor << endl;
//          if (valor < beta)
//          {
//             beta = valor;
//             mejorPar.beta = valor;
//             mejorPar.valor = valor;
//             mejorPar.lastAction = static_cast<Environment::ActionType>(hijos[i].Last_Action(jugador));
//             if (beta <= alfa)
//                i = numHijos; //break
//          }
//       }
//       return mejorPar;
//    }
// }

pair<double, Environment::ActionType> Player::alfabeta(Environment &actual, int profundidad, double alfa, double beta, int jugador)
{
   Environment hijos[8];
   pair<double, Environment::ActionType> valor;
   pair<double, Environment::ActionType> mejorValor;

   if (actual.JuegoTerminado() || profundidad == 0)
   {
      Environment::ActionType lastAction;
      lastAction = static_cast<Environment::ActionType>(actual.Last_Action(jugador));
      pair<double, Environment::ActionType> valorado = make_pair(Valoracion(actual, jugador), lastAction);
      return valorado;
   }

   int numHijos = actual.GenerateAllMoves(hijos);

   if (jugador == actual.JugadorActivo())
   { //Maximizar
      mejorValor = make_pair( alfa, static_cast<Environment::ActionType>(actual.Last_Action(jugador)) );
      for (int i = 0; i < numHijos && beta > alfa; i++)
      {
         valor = alfabeta(hijos[i], profundidad - 1, alfa, beta, jugador );
         if (valor.first > alfa)
         {
            alfa = valor.first;
            mejorValor = valor;
         }
         if( beta <= alfa )
            return mejorValor;
      }
      return mejorValor;
   }
   else
   { //Minimizar
      mejorValor = make_pair( beta, static_cast<Environment::ActionType>(actual.Last_Action(jugador)) );
      for (int i = 0; i < numHijos && beta > alfa; i++)
      {
         valor = alfabeta(hijos[i], profundidad - 1, alfa, beta, jugador);
         if (valor.first < beta)
         {
            beta = valor.first;
            mejorValor = valor;
         }
         if( beta <= alfa )
            return mejorValor;
      }
      return mejorValor;
   }
}

// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think()
{
   const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
   const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

   Environment::ActionType accion; // acci�n que se va a devolver
   bool aplicables[8];             // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                                   // aplicables[0]==true si PUT1 es aplicable
                                   // aplicables[1]==true si PUT2 es aplicable
                                   // aplicables[2]==true si PUT3 es aplicable
                                   // aplicables[3]==true si PUT4 es aplicable
                                   // aplicables[4]==true si PUT5 es aplicable
                                   // aplicables[5]==true si PUT6 es aplicable
                                   // aplicables[6]==true si PUT7 es aplicable
                                   // aplicables[7]==true si BOOM es aplicable

   int n_act; //Acciones posibles en el estado actual

   n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
   int opciones[10];

   cout << " Acciones aplicables ";
   (jugador_ == 1) ? cout << "Verde: " : cout << "Azul: ";
   for (int t = 0; t < 8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr(static_cast<Environment::ActionType>(t));
   cout << endl;

   //Pinta tablero
   cout << "TABLERO\n"
        << endl;
   for (int i = 0; i < 7; i++)
   {
      for (int j = 0; j < 7; j++)
         cout << (int)actual_.See_Casilla(i, j);
      cout << endl;
   }
   pair<double, Environment::ActionType> accionvalor;
   if( actual_.N_Jugada()!=1 ){
      accionvalor = alfabeta(actual_, PROFUNDIDAD_ALFABETA, menosinf, masinf, jugador_);
      accion = accionvalor.second;
      cout << "ACCION A RALIZAR "<< (int)accion  << " con valor " << accionvalor.first<< endl;
   }  
   else 
      accion = Environment::ActionType::PUT4;

   return accion;
}
