/*
 * estadosmina.cpp
 *
 *  Created on: 10/11/2016
 *      Author: francis
 */

#include "estadosmina.h"
#include "estado.h"
#include "objeto.h"
#include "tipomensaje.h"

#include <iostream>

//--------------------------------------- Metodos para minaEstadoGlobal
minaEstadoGlobal* minaEstadoGlobal::Instance() {
	static minaEstadoGlobal instance;

	return &instance;
}

void minaEstadoGlobal::entrar(Mina* pMina) {
}

void minaEstadoGlobal::ejecutar(Mina* pMina) {
}

void minaEstadoGlobal::salir(Mina* pMina) {
}

bool minaEstadoGlobal::OnMessage(Mina* pMina, const Telegrama& msg)
{
	return false;
}

//--------------------------------------- Metodos para minaEstadoInactivo
minaEstadoInactivo* minaEstadoInactivo::Instance() {
	static minaEstadoInactivo instance;

	return &instance;
}

void minaEstadoInactivo::entrar(Mina* pMina) {
	//No se hace nada
	//std::cout << "\nObjeto tipo " << pMina->getTipo() << ": "
	//<< "En espera de orden";
}

void minaEstadoInactivo::ejecutar(Mina* pMina) {
	//std::cout << "\nObjeto tipo " << pMina->getTipo() << ": "
	//<< "No hace nada";
}

void minaEstadoInactivo::salir(Mina* pMina) {
	//std::cout << "\nObjeto tipo " << pMina->getTipo() << ": "
	//<< "Deja de estar inactivo";
}

bool minaEstadoInactivo::OnMessage(Mina* pMina, const Telegrama& msg)
{
	switch(msg._msg)
	{
		case _msjRecolectarMaterial:
			Objeto* pCampesino = msg._emisor; //Campesino que recolecta

			pMina->modificarCantidadMaterial(tipoMaterial::Oro, -1*pCampesino->getCantidadMaterial(tipoMaterial::Oro));

			return true;

	}//end switch

	//send msg to global message handler
	return false;
}
