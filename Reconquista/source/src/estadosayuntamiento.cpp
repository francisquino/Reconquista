/*
 * estadosayuntamiento.cpp
 *
 *  Created on: 10/11/2016
 *      Author: francis
 */

#include <string>

#include "estadosayuntamiento.h"
#include "estado.h"
#include "objeto.h"
#include "tipomensaje.h"

#include <iostream>

//--------------------------------------- Metodos para ayuntamientoEstadoGlobal
ayuntamientoEstadoGlobal* ayuntamientoEstadoGlobal::Instance() {
	static ayuntamientoEstadoGlobal instance;

	return &instance;
}

void ayuntamientoEstadoGlobal::entrar(Ayuntamiento* pAyuntamiento) {
}

void ayuntamientoEstadoGlobal::ejecutar(Ayuntamiento* pAyuntamiento) {
}

void ayuntamientoEstadoGlobal::salir(Ayuntamiento* pAyuntamiento) {
}

bool ayuntamientoEstadoGlobal::OnMessage(Ayuntamiento* pAyuntamiento, const Telegrama& msg)
{
	return false;
}

//--------------------------------------- Metodos para ayuntamientoEstadoInactivo
ayuntamientoEstadoInactivo* ayuntamientoEstadoInactivo::Instance() {
	static ayuntamientoEstadoInactivo instance;

	return &instance;
}

void ayuntamientoEstadoInactivo::entrar(Ayuntamiento* pAyuntamiento) {
	//No se hace nada
	std::cout << "\nObjeto tipo " << pAyuntamiento->getTipo() << ": "
	<< "En espera de orden";
}

void ayuntamientoEstadoInactivo::ejecutar(Ayuntamiento* pAyuntamiento) {
	//std::cout << "\nObjeto tipo " << pAyuntamiento->getTipo() << ": "
	//<< "No hace nada";
}

void ayuntamientoEstadoInactivo::salir(Ayuntamiento* pAyuntamiento) {
	std::cout << "\nObjeto tipo " << pAyuntamiento->getTipo() << ": "
	<< "Deja de estar inactivo";
}

bool ayuntamientoEstadoInactivo::OnMessage(Ayuntamiento* pAyuntamiento, const Telegrama& msg)
{
	switch(msg._msg)
	{
		case _msjDescargarMaterial:
			std::map<tipoMaterial::TipoMaterial, int> *materialYTipo = (std::map<tipoMaterial::TipoMaterial, int>*) msg._extraInfo;

			//for (std::map<tipoMaterial::TipoMaterial, int>::iterator it=materialYTipo->begin(); it!=materialYTipo->end(); ++it)
			//    std::cout << it->first << " => " << it->second << '\n';
			pAyuntamiento->modificarCantidadMaterial(materialYTipo->begin()->first, materialYTipo->begin()->second);

			return true;

	}//end switch

	//send msg to global message handler
	return false;
}
