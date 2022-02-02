# Conecta 4 BOOM

La tercera práctica de la asignatura Inteligencia Artificial consiste en el diseño e implementación de alguna de las técnicas de búsqueda con adversario en un entorno de juegos. Al igual que en la práctica anterior, se trabajará con un simulador, pero en este caso adaptado para el juego CONECTA-4 que pasamos a describir. 

El juego CONECTA-4 BOOM se basa en el juego CONECTA-4 (también conocido como 4 en Raya). CONECTA-4 es un juego de mesa para dos jugadores distribuido por Hasbro, en el que se introducen fichas en un tablero vertical con el objetivo de alinear cuatro consecutivas de un mismo color. Fue creado en 1974 por Ned Strongin y Howard Wexler para Milton Bradley Company.

## Requisitos

Para poder realizar la práctica, es necesario que el alumno disponga de: 

1. Conocimientos básicos del lenguaje C/C++: tipos de datos, sentencias condicionales, sentencias repetitivas, funciones y procedimientos, clases, métodos de clases, constructores de clase. 

2. El entorno de programación **CodeBlocks** en el caso de trabajar bajo Sistema Operativo **Windows**, que tendrá que estar instalado en el computador donde vaya a realizar la práctica. Este software se puede descargar desde la siguiente URL: http://www.codeblocks.org/. Es importante recordar que debe bajarse la versión 17.12. Para versiones más actuales, el software que se proporciona no es compatible. 

3. En el caso de trabajar bajo entorno **Linux**, es necesario tener instalada la **librería “GLUT3”**, para los que tienen Ubuntu la instalación de esta biblioteca se hace poniendo la siguiente sentencia:

   `sudo apt-get install freeglut3-dev` 

   Para otras versiones de Linux, buscar en internet como instalar esta librería. 

4. El software para construir el simulador Conecta4Boom disponible en la plataforma docente de la asignatura.

## Objetivos

La práctica tiene como objetivo diseñar e implementar un agente deliberativo que pueda llevar a cabo un comportamiento inteligente dentro del juego CONECTA-4 BOOM que se explica a continuación. 

El objetivo de CONECTA-4 BOOM es alinear cuatro fichas sobre un tablero formado por siete filas y siete columnas (en el juego original, el tablero es de seis filas). Cada jugador dispone de 25 fichas de un color (en nuestro caso, verdes y azules). Las jugadas entre los jugadores son alternas, empezando siempre el jugador 1, y en cada jugada el jugador hace dos movimientos, **excepto el jugador 1 en la primera jugada en el que sólo hace un movimiento** . Un movimiento consiste en introducir una ficha en la columna que prefiera (de la 1 a la 7, numeradas de izquierda a derecha, siempre que no esté completa) y ésta caerá a la posición más baja.

Gana la partida el primero que consiga alinear cuatro fichas consecutivas de un mismo color en horizontal, vertical o diagonal. Si todas las columnas están ocupadas se produce un empate. 

