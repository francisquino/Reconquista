/*
 * nodo.h
 *
 *  Created on: 29/11/2016
 *      Author: francis
 */

#ifndef NODO_H
#define NODO_H

// http://en.wikipedia.org/wiki/A*//
// Algoritmo de busqueda de rutas A*
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <math.h>
#include <ctime>


//Estructura usada para crear un mapa y poder calcular una ruta con el algoritmo A*

class Nodo
{
    // Posicion actual
    int xPos;
    int yPos;

    // Distancia total ya recorrida para alcanzar el nodo
    int distancia;

    // Prioridad=distancia + distancia restante estimada
    int prioridad;  // menor: prioridad mas alta

    public:
        Nodo(int xp, int yp, int d, int p)
            {xPos=xp; yPos=yp; distancia=d; prioridad=p;}

        int getxPos() const {return xPos;}
        int getyPos() const {return yPos;}
        int getDistancia() const {return distancia;}
        int getPrioridad() const {return prioridad;}

        void updatePrioridad(const int & xDest, const int & yDest)
        {
             prioridad=distancia+estimacion(xDest, yDest)*10; //A*
        }

        // Dar mejor prioridad a ir recto en lugar de diagonalmente
        // Dir indica las posibles opciones para avanzar:
        // dir=8 En el caso de que tengamos en cuenta las diagonales
        // dir=4 Solo podemos avanzar arriba, abajo, a izquierda o derecha
        void nextDistancia(const int dir, const int & i) // i: direction
        {
             distancia+=(dir==8?(i%2==0?10:14):10);
        }

        // Funcion de estimation para la distancia restante a la meta
        const int & estimacion(const int & xDest, const int & yDest) const
        {
            static int xd, yd, d;
            xd=xDest-xPos;
            yd=yDest-yPos;

            // Distancia euclidea
            d=static_cast<int>(sqrt(xd*xd+yd*yd));

            // Manhattan distance
            //d=abs(xd)+abs(yd);

            // Chebyshev distance
            //d=max(abs(xd), abs(yd));

            return(d);
        }
};


#endif /* NODO_H */
