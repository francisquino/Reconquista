/*
 * estadoscaballero.cpp
 *
 *  Created on: 10/11/2016
 *      Author: francis
 */

#include "tipoestadojuego.h"
#include "estadoscaballero.h"
#include "estado.h"
#include "objeto.h"
#include "tipomensaje.h"
#include "level.h"
#include "animaciones.h"

#include <iostream>


extern Level _level;



//--------------------------------------- Metodos para caballeroEstadoGlobal
caballeroEstadoGlobal* caballeroEstadoGlobal::Instance() {
	static caballeroEstadoGlobal instance;

	return &instance;
}

void caballeroEstadoGlobal::entrar(Caballero* pCaballero) {
}

void caballeroEstadoGlobal::ejecutar(Caballero* pCaballero) {
}

void caballeroEstadoGlobal::salir(Caballero* pCaballero) {
}

bool caballeroEstadoGlobal::OnMessage(Caballero* pCaballero, const Telegrama& msg)
{
	//Aquí se pueden recibir los mensajes de Parar y RecibirAtaque por ejemplo.
	switch(msg._msg)
	{
		case _msjParar:
			//Borrar la ruta, si hay
			while (pCaballero->getDestinoX() && pCaballero->getDestinoY()) {
				pCaballero->deleteDestino();
			}
			pCaballero->stopMoving();
			pCaballero->GetFSM()->cambiarEstado(caballeroEstadoInactivo::Instance());
			return true; //_msjParar
	}//end switch
	return false;
}

//--------------------------------------- Metodos para caballeroEstadoInactivo
caballeroEstadoInactivo* caballeroEstadoInactivo::Instance() {
	static caballeroEstadoInactivo instance;

	return &instance;
}

void caballeroEstadoInactivo::entrar(Caballero* pCaballero) {
	//std::cout << "\nObjeto tipo " << pCaballero->getTipo() << ": "
	//<< "EstadoInactivo esperando orden\n";
}

void caballeroEstadoInactivo::ejecutar(Caballero* pCaballero) {
	//std::cout << "\nObjeto tipo " << pCaballero->getTipo() << ": "
	//<< "No hace nada";
}

void caballeroEstadoInactivo::salir(Caballero* pCaballero) {
	//std::cout << "\nObjeto tipo " << pCaballero->getTipo() << ": "
	//<< "Deja de estar inactivo";
}

bool caballeroEstadoInactivo::OnMessage(Caballero* pCaballero, const Telegrama& msg)
{
	switch(msg._msg)
	{
		case _msjIrA:
			pCaballero->GetFSM()->cambiarEstado(caballeroEstadoIrDestino::Instance());

			return true;
	}//end switch

	//send msg to global message handler
	return false;
}


//--------------------------------------- Metodos para caballeroEstadoIrDestino
caballeroEstadoIrDestino* caballeroEstadoIrDestino::Instance() {
	static caballeroEstadoIrDestino instance;

	return &instance;
}

void caballeroEstadoIrDestino::entrar(Caballero* pCaballero) {
	//std::cout << "\nObjeto tipo " << pCaballero->getTipo() << ": "
	//<< "Fijamos destino";
}

void caballeroEstadoIrDestino::ejecutar(Caballero* pCaballero) {
	//Si hay una ruta, movernos al primer punto.
	if (pCaballero->getDestinoX() && pCaballero->getDestinoY()) {
		if(pCaballero->getX() < pCaballero->getDestinoX()) {
			pCaballero->moveRight();
		}
		if(pCaballero->getX() > pCaballero->getDestinoX()) {
			pCaballero->moveLeft();
		}
		if(pCaballero->getY() < pCaballero->getDestinoY()) {
			pCaballero->moveDown();
		}
		if(pCaballero->getY() > pCaballero->getDestinoY()) {
			pCaballero->moveUp();
		}

		//Si hemos llegado al destino, inicializamos. Al usar números decimales, damos un margen de 0.2f
		//printf ("Posicion [%f,%f] -- Destino [%i,%i]\n", pCaballero->getX(), pCaballero->getY(), pCaballero->getDestinoX(), pCaballero->getDestinoY());

		//Si hemos llegado al destino, inicializamos.
		/*if(abs(floor(pCaballero->getX() - pCaballero->getDestinoX())) < 0.2f &&
			abs(floor(pCaballero->getY() - pCaballero->getDestinoY())) < 0.2f) {*/
		if(abs(pCaballero->getX() - pCaballero->getDestinoX()) < 0.2f &&
			abs(pCaballero->getY() - pCaballero->getDestinoY()) < 0.2f) {

			//Para eliminar esa pequeña diferencia
			pCaballero->setX(pCaballero->getDestinoX());
			pCaballero->setY(pCaballero->getDestinoY());

			pCaballero->deleteDestino();

			//Si no quedan puntos en la ruta, nos paramos y pasamos al estado previo
			if (!pCaballero->getDestinoX() && !pCaballero->getDestinoY()) {
				printf("DESTINO ALCANZADO [%i,%i]\n\n", _level.coordAMapa(pCaballero->getX(), pCaballero->getY()).x, _level.coordAMapa(pCaballero->getX(), pCaballero->getY()).y);
				pCaballero->stopMoving();
				//pCaballero->setDestino(-1, -1);
				pCaballero->GetFSM()->cambiarAEstadoPrevio();
			}
		}
	}
}

void caballeroEstadoIrDestino::salir(Caballero* pCaballero) {
	//std::cout << "\nObjeto tipo " << pCaballero->getTipo() << ": "
	//<< "Sale de IrDestino\n";
}


bool caballeroEstadoIrDestino::OnMessage(Caballero* pCaballero, const Telegrama& msg)
{
	switch(msg._msg)
	{
		case _msjDestinoFijado:
			return true; //_msjDestinoParcial
	}//end switch
  //send msg to global message handler
  return false;
}

