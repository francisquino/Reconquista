/*
 * estadosbosque.cpp
 *
 *  Created on: 10/11/2016
 *      Author: francis
 */

#include "estadosbosque.h"
#include "estado.h"
#include "objeto.h"
#include "tipomensaje.h"

#include <iostream>

//--------------------------------------- Metodos para bosqueEstadoGlobal
bosqueEstadoGlobal* bosqueEstadoGlobal::Instance() {
	static bosqueEstadoGlobal instance;

	return &instance;
}

void bosqueEstadoGlobal::entrar(Bosque* pBosque) {
}

void bosqueEstadoGlobal::ejecutar(Bosque* pBosque) {
}

void bosqueEstadoGlobal::salir(Bosque* pBosque) {
}

bool bosqueEstadoGlobal::OnMessage(Bosque* pBosque, const Telegrama& msg)
{
	return false;
}

//--------------------------------------- Metodos para bosqueEstadoInactivo
bosqueEstadoInactivo* bosqueEstadoInactivo::Instance() {
	static bosqueEstadoInactivo instance;

	return &instance;
}

void bosqueEstadoInactivo::entrar(Bosque* pBosque) {
	//No se hace nada
	//std::cout << "\nObjeto tipo " << pBosque->getTipo() << ": "
	//<< "En espera de orden";
}

void bosqueEstadoInactivo::ejecutar(Bosque* pBosque) {
	//std::cout << "\nObjeto tipo " << pBosque->getTipo() << ": "
	//<< "No hace nada";
}

void bosqueEstadoInactivo::salir(Bosque* pBosque) {
	//std::cout << "\nObjeto tipo " << pBosque->getTipo() << ": "
	//<< "Deja de estar inactivo";
}

bool bosqueEstadoInactivo::OnMessage(Bosque* pBosque, const Telegrama& msg)
{
	switch(msg._msg)
	{
		case _msjRecolectarMaterial:
			Objeto* pCampesino = msg._emisor; //Campesino que recolecta

			pBosque->modificarCantidadMaterial(tipoMaterial::Madera, -1*pCampesino->getCantidadMaterial(tipoMaterial::Madera));

			return true;

	}//end switch

	//send msg to global message handler
	return false;
}
