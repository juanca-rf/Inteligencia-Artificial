#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>
#include <list>
#include <tgmath.h>

void ComportamientoJugador::pintamapa(Sensores origen)
{
	int maxfil = mapaResultado.size();
	int maxcol = mapaResultado[0].size();
	
	switch (origen.sentido)
	{
	case 0: //Norte
		for(int i = 0; i <= 15; i++){
			if(i == 0)
				mapaResultado[origen.posF][origen.posC] = origen.terreno[i];
			if( i >= 1 && i <= 3 )
				mapaResultado[ origen.posF - 1 ][ origen.posC - 2 + i ] = origen.terreno[i];
			if( i >= 4 && i <= 8 )
				mapaResultado[ origen.posF - 2 ][ origen.posC - 2 + i - 4 ] = origen.terreno[i];
			if( i >= 9 && i <= 15 )
				mapaResultado[ origen.posF - 3 ][ origen.posC - 3 + i - 9 ] = origen.terreno[i];
		}
		break;

	case 2: //sur
		for(int i = 0; i <= 15; i++){
			if(i == 0) //Nivel 0
				mapaResultado[ origen.posF ][ origen.posC ] = origen.terreno[i];
			if( i >= 1 && i <= 3 )//Nivel 1
				mapaResultado[ origen.posF + 1 ][ origen.posC + 2 - i ] = origen.terreno[i];
			if( i >= 4 && i <= 8 )//Nivel 2
				mapaResultado[ origen.posF + 2 ][ origen.posC + 2 - i + 4 ] = origen.terreno[i];
			if( i >= 9 && i <= 15 )//Nivel 3
				mapaResultado[ origen.posF + 3 ][ origen.posC + 3 - i + 9  ] = origen.terreno[i];
		}
		break;

	case 1: //este
		for(int i = 0; i <= 15; i++){
			if(i == 0) 											
				mapaResultado[ origen.posF ][ origen.posC ] = origen.terreno[i];

			if( i >= 1 && i <= 3 )								
				mapaResultado[ origen.posF - 2 + i ][ origen.posC + 1 ] = origen.terreno[i];

			if( i >= 4 && i <= 8 )
				mapaResultado[ origen.posF - 2 + i - 4 ][ origen.posC + 2 ] = origen.terreno[i];

			if( i >= 9 && i <= 15 )
				mapaResultado[ origen.posF - 3 + i - 9 ][ origen.posC + 3 ] = origen.terreno[i];
	}
		break;

	case 3: //oeste
		for(int i = 0; i <= 15; i++){
			if(i == 0) 											
				mapaResultado[ origen.posF ][ origen.posC ] = origen.terreno[i];

			if( i >= 1 && i <= 3 )								
				mapaResultado[ origen.posF + 2 - i ][ origen.posC - 1 ] = origen.terreno[i];

			if( i >= 4 && i <= 8 )
				mapaResultado[ origen.posF + 2 - i + 4 ][ origen.posC - 2 ] = origen.terreno[i];

			if( i >= 9 && i <= 15 )
				mapaResultado[ origen.posF + 3 - i + 9 ][ origen.posC - 3 ] = origen.terreno[i];
		}
		break;

	default:
		break;
	}
}

/**
 * @brief Distancia entre los dos puntos @a a y @a b
 * 
 * @param a 
 * @param b 
 * @return int 
 */
int dist( estado a, estado b ){
	int x1 = a.fila; 
	int y1 = a.columna; 
	int x2 = b.fila; 
	int y2 = b.columna;
	return sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
}

/**
 * @brief Devuelve el objetivo mas cercano a @a actual y lo elimina de DESTINOS
 * 
 * @param destinos 
 * @param actual 
 * @return estado 
 */
estado extraeMejorObjetivo(list<estado> &destinos, estado actual){
	estado mejorEstado;
	list<estado>::iterator mejorit;

	int mejorDistancia = 1000000;
	for( list<estado>::iterator it = destinos.begin(); it != destinos.end(); it++ )
		if( dist( *it, actual ) < mejorDistancia ){
			mejorEstado = *it;
			mejorit = it;
		}
	destinos.erase(mejorit);
	return mejorEstado;
}

/**
 * @brief Devuele los destinos ordenador por menor distancia a actual
 * 
 * @param destinos 
 * @param actual 
 * @return list<estado> 
 */
list<estado> ordenaObjetivos(list<estado> destinos, estado actual){
	list<estado> ordenados;
	while( !destinos.empty() )
		ordenados.push_back( extraeMejorObjetivo(destinos,actual) );
	return ordenados;
}

/**
 * @brief Busca la ubicacion de bateria
 * 
 * @param mapaResultado 
 * @return estado 
 */
bool buscaBateria(vector<vector<unsigned char>> mapaResultado, estado &bateria){
	bool done = false;
	for( int i = 0; i < mapaResultado.size() && !done; i++ )
		for( int j = 0; j < mapaResultado[0].size() && !done; j++ ) 
			if( mapaResultado[i][j] == 'X' ){
				done = true;
				bateria.fila = i;
				bateria.columna = j;
			}
	return done;
}

// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
Action ComportamientoJugador::think(Sensores sensores)
{	
	Action sigAction;
	estado stbateria;
	bool found, bateria;
	actual.fila = sensores.posF;
	actual.columna = sensores.posC;
	actual.orientacion = sensores.sentido;

	if( sensores.nivel >=0 and sensores.nivel <= 3 ){
		objetivos.clear();
		for (int i = 0; i < sensores.num_destinos; i++)
		{
			estado aux;
			aux.fila = sensores.destino[2 * i];
			aux.columna = sensores.destino[2 * i + 1];
			objetivos.push_back(aux);
		}

		if (!hayPlan)
		{
			hayPlan = pathFinding(sensores.nivel, actual, objetivos, plan);
		}

		if (hayPlan && plan.size() > 0)//Si hay plan no vacio
		{ 
				sigAction = plan.front();
				plan.erase(plan.begin());
		}
		else
			cout << "no se puede encontrar plan";	
	}
	else{ //NIVEL 4
		pintamapa(sensores);

		//Compruebo si estoy en objetivo
		for (list<estado>::iterator it = objetivos.begin(); it != objetivos.end() && !found; it++){ 
			if (actual.fila == it->fila and actual.columna == it->columna )
			{ //Comparo si la casilla actual es un destino
				it = objetivos.erase(it);
				hayPlan=false;
				found = true;
				if( sensores.bateria < 500 ) //Si tengo poca bateria busco la casilla para recargarla 
					if( buscaBateria(mapaResultado, stbateria) ) //Si la encuentro la meto como primer objetivo siguiente
						objetivos.push_front(stbateria);					
			}
		}
		

		//Si paso por recarga cojo bateria
		if( actual.fila==stbateria.fila && actual.columna==stbateria.columna && sensores.bateria < 500 ){
			sigAction = actIDLE;
			bateria = true;
		}
		if( sensores.bateria > 200 and bateria) bateria = false;//Si me he recargado lo suficiente paro


		// Capturo los destinos
		if( objetivos.empty() ){
			objetivos.clear();
			for (int i = 0; i < sensores.num_destinos; i++)
			{
				estado aux;
				aux.fila = sensores.destino[2 * i];
				aux.columna = sensores.destino[2 * i + 1];
				objetivos.push_back(aux);
			}
			
			//Los ordeno
			objetivos = ordenaObjetivos( objetivos, actual );
		}

		estado aux = actual;
		//Si no hay plan, se construye
		if (!hayPlan or (HayObstaculoDelante(aux) and plan.front()==actFORWARD))
		{
			hayPlan = pathFinding(sensores.nivel, actual, objetivos, plan);
		}
	
		if (hayPlan && plan.size() > 0  && !bateria)//Si hay plan no vacio y no quiero bateria
		{ 
			if(sensores.superficie[2] == 'a'){
				sigAction = actIDLE;		
			}else{
				sigAction = plan.front();
				plan.erase(plan.begin());
			}
		}
		else
			cout << "no se puede encontrar plan";	
	}
	return sigAction;
}

// Llama al algoritmo de busqueda que se usara en cada comportamiento del agente
// Level representa el comportamiento en el que fue iniciado el agente.
bool ComportamientoJugador::pathFinding(int level, const estado &origen, const list<estado> &destino, list<Action> &plan)
{
	estado un_objetivo;
	switch (level)
	{
	case 0:
		cout << "Demo\n";
		un_objetivo = objetivos.front();
		cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
		return pathFinding_Profundidad(origen, un_objetivo, plan);
		break;

	case 1:
		cout << "Optimo numero de acciones\n";
		// Incluir aqui la llamada al busqueda en anchura
		un_objetivo = objetivos.front();
		cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
		return pathFinding_Anchura(origen, un_objetivo, plan);
		break;
	case 2:
		cout << "Optimo en coste 1 Objetivo\n";
		// Incluir aqui la llamada al busqueda de costo uniforme o A*
		un_objetivo = objetivos.front();
		cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
		return pathFinding_Aestrella(origen, un_objetivo, plan);
		break;
	case 3:
		cout << "Optimo en coste 3 Objetivos\n";
		// Incluir aqui la llamada al algoritmo de busqueda para 3 objetivos
		return pathFinding_A3(origen, destino, plan);
		break;
	case 4:
		cout << "Algoritmo de busqueda usado en el reto\n";
		// Incluir aqui la llamada al algoritmo de busqueda usado en el nivel 2
		un_objetivo = objetivos.front();
		return pathFinding_Aestrella(origen, un_objetivo, plan);
		break;
	}
	return false;
}

//---------------------- Implementación de la busqueda en profundidad ---------------------------

// Dado el codigo en caracter de una casilla del mapa dice si se puede
// pasar por ella sin riegos de morir o chocar.
bool EsObstaculo(unsigned char casilla)
{
	if (casilla == 'P' or casilla == 'M')
		return true;
	else
		return false;
}

// Comprueba si la casilla que hay delante es un obstaculo. Si es un
// obstaculo devuelve true. Si no es un obstaculo, devuelve false y
// modifica st con la posición de la casilla del avance.
bool ComportamientoJugador::HayObstaculoDelante(estado &st)
{
	int fil = st.fila, col = st.columna;

	// calculo cual es la casilla de delante del agente
	switch (st.orientacion)
	{
	case 0:
		fil--;
		break;
	case 1:
		col++;
		break;
	case 2:
		fil++;
		break;
	case 3:
		col--;
		break;
	}

	// Compruebo que no me salgo fuera del rango del mapa
	if (fil < 0 or fil >= mapaResultado.size())
		return true;
	if (col < 0 or col >= mapaResultado[0].size())
		return true;

	// Miro si en esa casilla hay un obstaculo infranqueable
	if (!EsObstaculo(mapaResultado[fil][col]))
	{
		// No hay obstaculo, actualizo el parametro st poniendo la casilla de delante.
		st.fila = fil;
		st.columna = col;
		return false;
	}
	else
	{
		return true;
	}
}

struct nodo
{
	estado st;
	list<Action> secuencia;
};

class supernodo
{
public:
	estado st;
	list<Action> secuencia;
	int gasto;

	supernodo()
	{
		gasto = 0;
	}

	bool operator>(const supernodo &otro) const
	{
		return this->gasto > otro.gasto;
	}

	void calcularGasto(unsigned char casilla, Action movimiento)
	{ // mapaResultado[st.fil][st.col] actFordward

		switch (casilla)
		{
		case 'A':
			if (movimiento == actFORWARD)
				gasto += st.bikini ? 10 : 200;
			else if (movimiento == actTURN_L || movimiento == actTURN_R)
				gasto += st.bikini ? 5 : 500;
			break;

		case 'B':
			if (movimiento == actFORWARD)
				gasto += st.zapatillas ? 15 : 100;
			else if (movimiento == actTURN_L || movimiento == actTURN_R)
				gasto += st.zapatillas ? 1 : 3;
			break;

		case 'T':
			gasto += 2;
			break;

		case 'K': //bikini
			st.bikini = true;
			if (st.zapatillas)
				st.zapatillas = false;
			break;

		case 'D': //zapatillas
			st.zapatillas = true;
			if (st.bikini)
				st.bikini = false;
			break;

		case 'X':
			//gasto += 2;
		break;

		default:
			gasto += 1;
			break;
		}
	}
};

struct ComparaFilsCols
{
	bool operator()(const estado &a, const estado &n) const
	{
		if (a.fila == n.fila and a.columna == n.columna)
			return false;
		else
			return true;
	}
};

struct ComparaEstados
{
	bool operator()(const estado &a, const estado &n) const
	{
		ComparaFilsCols distintos;
		if (!a.visitados.empty() and !n.visitados.empty())
		{
			if ((a.fila > n.fila) or
				(a.fila == n.fila and a.columna > n.columna) or
				(a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion) or
				(a.fila == n.fila and a.columna == n.columna and a.orientacion == n.orientacion and a.bikini < n.bikini) or
				(a.fila == n.fila and a.columna == n.columna and a.orientacion == n.orientacion and a.bikini == n.bikini and a.zapatillas < n.zapatillas) or
				(a.fila == n.fila and a.columna == n.columna and a.orientacion == n.orientacion and a.bikini == n.bikini and a.zapatillas == n.zapatillas and a.visited[0] < n.visited[0]) or
				(a.fila == n.fila and a.columna == n.columna and a.orientacion == n.orientacion and a.bikini == n.bikini and a.zapatillas == n.zapatillas and a.visited[0] == n.visited[0] and a.visited[1] < n.visited[1]) or
				(a.fila == n.fila and a.columna == n.columna and a.orientacion == n.orientacion and a.bikini == n.bikini and a.zapatillas == n.zapatillas and a.visited[0] == n.visited[0] and a.visited[1] == n.visited[1] and a.visited[2] < n.visited[2]))
				return true;
			else
				return false;
		}
		else if (a.visitados.empty() and n.visitados.empty())
		{
			if ((a.fila > n.fila) or
				(a.fila == n.fila and a.columna > n.columna) or
				(a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion) or
				(a.fila == n.fila and a.columna == n.columna and a.orientacion == n.orientacion and a.bikini < n.bikini) or
				(a.fila == n.fila and a.columna == n.columna and a.orientacion == n.orientacion and a.bikini == n.bikini and a.zapatillas < n.zapatillas))
				return true;
			else
				return false;
		}
		else
			return true;
	}
};

// Implementación de la busqueda en profundidad.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan)
{
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstados> Cerrados; // Lista de Cerrados
	stack<nodo> Abiertos;				  // Lista de Abiertos

	nodo current;
	current.st = origen;
	current.secuencia.empty();

	Abiertos.push(current);

	while (!Abiertos.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
	{

		Abiertos.pop();
		Cerrados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 1) % 4;
		if (Cerrados.find(hijoTurnR.st) == Cerrados.end())
		{
			hijoTurnR.secuencia.push_back(actTURN_R);
			Abiertos.push(hijoTurnR);
		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 3) % 4;
		if (Cerrados.find(hijoTurnL.st) == Cerrados.end())
		{
			hijoTurnL.secuencia.push_back(actTURN_L);
			Abiertos.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			if (Cerrados.find(hijoForward.st) == Cerrados.end())
			{
				hijoForward.secuencia.push_back(actFORWARD);
				Abiertos.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la Abiertos
		if (!Abiertos.empty())
		{
			current = Abiertos.top();
		}
	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}

// Implementación de la busqueda en anchura.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan)
{
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstados> Cerrados; // Lista de Cerrados
	queue<nodo> Abiertos;				  // Lista de Abiertos

	nodo current;
	current.st = origen;
	current.secuencia.empty();

	Abiertos.push(current);

	while (!Abiertos.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
	{

		Abiertos.pop();
		Cerrados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 1) % 4;
		if (Cerrados.find(hijoTurnR.st) == Cerrados.end())
		{
			hijoTurnR.secuencia.push_back(actTURN_R);
			Abiertos.push(hijoTurnR);
		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 3) % 4;
		if (Cerrados.find(hijoTurnL.st) == Cerrados.end())
		{
			hijoTurnL.secuencia.push_back(actTURN_L);
			Abiertos.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			if (Cerrados.find(hijoForward.st) == Cerrados.end())
			{
				hijoForward.secuencia.push_back(actFORWARD);
				Abiertos.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la Abiertos
		if (!Abiertos.empty())
		{
			current = Abiertos.front();
		}
	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}

// Implementación de la busqueda A*.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Aestrella(const estado &origen, const estado &destino, list<Action> &plan)
{
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstados> Cerrados;									   // Lista de Cerrados
	priority_queue<supernodo, vector<supernodo>, greater<supernodo>> Abiertos; // Lista de Abiertos

	supernodo current;
	current.st = origen;
	current.secuencia.empty();

	Abiertos.push(current);

	while (!Abiertos.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
	{

		Abiertos.pop();
		Cerrados.insert(current.st);

		// Generar descendiente de girar a la derecha
		supernodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 1) % 4;
		hijoTurnR.calcularGasto(mapaResultado[hijoTurnR.st.fila][hijoTurnR.st.columna], actTURN_R);
		hijoTurnR.secuencia.push_back(actTURN_R);
		Abiertos.push(hijoTurnR);

		// Generar descendiente de girar a la izquierda
		supernodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 3) % 4;
		hijoTurnL.calcularGasto(mapaResultado[hijoTurnL.st.fila][hijoTurnL.st.columna], actTURN_L);
		hijoTurnL.secuencia.push_back(actTURN_L);
		Abiertos.push(hijoTurnL);

		// Generar descendiente de avanzar
		supernodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			hijoForward.calcularGasto(mapaResultado[hijoForward.st.fila][hijoForward.st.columna], actFORWARD);
			hijoForward.secuencia.push_back(actFORWARD);
			Abiertos.push(hijoForward);
		}

		// Tomo el siguiente valor de la Abiertos
		if (!Abiertos.empty())
		{

			current = Abiertos.top();
			while (Cerrados.find(current.st) != Cerrados.end())
			{
				Abiertos.pop();
				current = Abiertos.top();
			}
		}
	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}

void pintaestados(supernodo a, estado b)
{
	cout << "  A B" << endl;
	cout << "F " << a.st.fila << " " << b.fila << endl;
	cout << "C " << a.st.columna << " " << b.columna << endl;
}

// Implementación de la busqueda A* para tres objetvos.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_A3(const estado &origen, const list<estado> destinos, list<Action> &plan)
{
	list<estado>::const_iterator it;
	int i;

	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstados> Cerrados;									   // Lista de Cerrados
	priority_queue<supernodo, vector<supernodo>, greater<supernodo>> Abiertos; // Lista de Abiertos

	estado destino;
	ComparaFilsCols comparo;

	supernodo current;
	current.st = origen;
	current.secuencia.empty();
	current.st.visitados.resize(3);

	Abiertos.push(current);
	cout << "Visitados al principio " << current.st.nvisitados << endl;

	while (!Abiertos.empty() and current.st.nvisitados < 3)
	{
		Abiertos.pop();
		Cerrados.insert(current.st);

		// Generar descendiente de girar a la derecha
		supernodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 1) % 4;
		hijoTurnR.calcularGasto(mapaResultado[hijoTurnR.st.fila][hijoTurnR.st.columna], actTURN_R);
		hijoTurnR.secuencia.push_back(actTURN_R);
		Abiertos.push(hijoTurnR);

		// Generar descendiente de girar a la izquierda
		supernodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 3) % 4;
		hijoTurnL.calcularGasto(mapaResultado[hijoTurnL.st.fila][hijoTurnL.st.columna], actTURN_L);
		hijoTurnL.secuencia.push_back(actTURN_L);
		Abiertos.push(hijoTurnL);

		// Generar descendiente de avanzar
		supernodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			hijoForward.calcularGasto(mapaResultado[hijoForward.st.fila][hijoForward.st.columna], actFORWARD);
			hijoForward.secuencia.push_back(actFORWARD);
			Abiertos.push(hijoForward);
		}

		// Tomo el siguiente valor de la Abiertos
		if (!Abiertos.empty())
		{
			current = Abiertos.top();
			while (Cerrados.find(current.st) != Cerrados.end())
			{
				Abiertos.pop();
				current = Abiertos.top();
			}
		}

		for (i = 0, it = destinos.cbegin(); it != destinos.cend(); it++, i++)
		{ //Recorro los destinos
			destino = *it;
			if (current.st.fila == destino.fila and current.st.columna == destino.columna && comparo(current.st.visitados[i], destino))
			{ //Comparo si la casilla actual es un destino y no esta visitado
				current.st.visitados[i] = destino;
				current.st.nvisitados++;
				current.st.visited[i] = 1;
			}
		}

		//cout << "Objetivos visitados del current " << current.st.nvisitados << " " << current.st.visited[0]<<current.st.visited[1]<<current.st.visited[2]<< endl;
	}

	cout << "Terminada la busqueda\n";

	if (current.st.visitados.size() >= 3)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}

// Sacar por la consola la secuencia del plan obtenido
void ComportamientoJugador::PintaPlan(list<Action> plan)
{
	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == actFORWARD)
		{
			cout << "A ";
		}
		else if (*it == actTURN_R)
		{
			cout << "D ";
		}
		else if (*it == actTURN_L)
		{
			cout << "I ";
		}
		else
		{
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}

// Funcion auxiliar para poner a 0 todas las casillas de una matriz
void AnularMatriz(vector<vector<unsigned char>> &m)
{
	for (int i = 0; i < m[0].size(); i++)
	{
		for (int j = 0; j < m.size(); j++)
		{
			m[i][j] = 0;
		}
	}
}

// Pinta sobre el mapa del juego el plan obtenido
void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan)
{
	AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == actFORWARD)
		{
			switch (cst.orientacion)
			{
			case 0:
				cst.fila--;
				break;
			case 1:
				cst.columna++;
				break;
			case 2:
				cst.fila++;
				break;
			case 3:
				cst.columna--;
				break;
			}
			mapaConPlan[cst.fila][cst.columna] = 1;
		}
		else if (*it == actTURN_R)
		{
			cst.orientacion = (cst.orientacion + 1) % 4;
		}
		else
		{
			cst.orientacion = (cst.orientacion + 3) % 4;
		}
		it++;
	}
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}
