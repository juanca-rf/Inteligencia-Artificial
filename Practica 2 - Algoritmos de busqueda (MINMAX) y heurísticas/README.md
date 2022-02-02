# Extraños mundos de Belkan

## Introducción

La segunda práctica de la asignatura Inteligencia Artificial consiste en el diseño e implementación de un agente reactivo/deliberativo, capaz de percibir el ambiente y actuar considerando una representación de las consecuencias de sus acciones y siguiendo un proceso de búsqueda. Se trabajará con un simulador software. Para ello, se proporciona al alumno un entorno de programación, junto con el software necesario para simular el entorno.

En esta práctica se diseñará e implementará un agente reactivo y deliberativo basado en los ejemplos del libro Stuart Russell, Peter Norvig, “Inteligencia Artificial: Un enfoque Moderno”, Prentice Hall, Segunda Edición, 2004. El simulador que utilizaremos fue inicialmente desarrollado por el profesor Tsung-Che Chiang de la NTNU (National Taiwan Normal University of Science and Technology), pero la versión sobre la que se va a trabajar ha sido desarrollada por los profesores de la asignatura.

## Escenario

Este juego se desarrolla sobre un mapa cuadrado bidimensional discreto que contiene como máximo 100 filas y 100 columnas. El mapa representa los accidentes geográficos de la superficie de un terreno que son considerados como inmutables, es decir, los elementos dentro del mapa que no cambian durante el desarrollo del juego. 

Representaremos dicha superficie usando una matriz donde la primera componente representa la fila y la segunda representa la columna dentro de nuestro mapa. Como ejemplo usaremos un mapa de tamaño 100x100 de caracteres. 

Teniendo en cuenta las consideraciones anteriores, diremos que un elemento móvil dentro del mapa va hacia el NORTE, si en su movimiento se decrementa el valor de la fila. Extendiendo este convenio, irá al SUR si incrementa su valor en la fila, irá al ESTE si incrementa su valor en las columnas, y por último irá al OESTE si decrementa su valor en columnas.

Los elementos permanentes en el terreno son los siguientes: 

- **Árboles o Bosque**, codificado con el carácter ‘B’ y se representan en el mapa como casillas de color verde. 

- **Agua**, codificado con el carácter ‘A’ y tiene asociado el color azul. 

- **Precipicios**, codificado con el carácter ‘P’ y tiene asociado el color negro. Estas casillas se consideran intransitables.

- **Suelo Pedregoso**, codificado con el carácter ‘S’ y tiene asociado el color gris.

- **Suelo Arenoso**, codificado con el carácter ‘T’ y tiene asociado el color marrón. 

- **Muros**, codificado con el carácter ‘M’ y son rojo oscuro. 

- **Bikini**, codificado con el carácter ‘K’ y se muestra en amarillo. Esta es una casilla especial que cuando el jugador pasa por ella adquiere el objeto “bikini”. Este objeto le permite reducir el consumo de batería en sus desplazamientos por el agua. 

- **Zapatillas**, codificado con el carácter ‘D’ y son moradas. 

  Esta es una casilla especial y al igual que la anterior, el jugador adquiere, en este caso el objeto “zapatillas” simplemente al posicionarse sobre ella. Las “Zapatillas” le permiten al jugador reducir el consumo de batería en los bosques. 

- **Recarga**, codificado con el carácter 'X' y de color rosa. Esta casilla especial permite al jugador cargar su batería. Por cada instante de simulación aumenta en 10 el nivel de su batería. Cuando la batería alcanza su nivel máximo de carga (3000), estar en esta casilla no incrementa la carga. 

- **Casilla aún desconocida**, se codifica con el carácter ‘?’ y se muestra en blanco (representa la parte del mundo que aún no has explorado).

## Compilación y Ejecución

```
sh install.sh
make
./Belkan
```

