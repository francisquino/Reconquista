/*
 * estadosObjeto.cpp
 *
 *  Created on: 10/11/2016
 *      Author: francis
 */

#include "estadosObjeto.h"
#include "estado.h"
#include "objeto.h"

#include <iostream>

//--------------------------------------- Metodos para estadoInactivo
estadoInactivo* estadoInactivo::Instance() {
	static estadoInactivo instance;

	return &instance;
}

void estadoInactivo::entrar(Objeto* pObjeto, Objeto *pDestino) {
	//No se hace nada
	std::cout << "\nObjeto tipo " << pObjeto->getTipo() << ": "
	<< "En espera de orden";
}

void estadoInactivo::ejecutar(Objeto* pObjeto, Objeto *pDestino) {
	std::cout << "\nObjeto tipo " << pObjeto->getTipo() << ": "
	<< "No hace nada";
}

void estadoInactivo::salir(Objeto* pObjeto) {
	std::cout << "\nObjeto tipo " << pObjeto->getTipo() << ": "
	<< "Deja de estar inactivo";
}

//--------------------------------------- Metodos para estadoRecolectar
estadoRecolectar* estadoRecolectar::Instance() {
	static estadoRecolectar instance;

	return &instance;
}

void estadoRecolectar::entrar(Objeto* pObjeto, Objeto *pDestino) {
	//Si el objeto no esta ya localizado en la mina de oro, o el bosque se debe
	//cambiar la localizacion
	if (pObjeto->getDestinoX() != pDestino->getX() && pObjeto->getDestinoY() != pDestino->getY()) {
		std::cout << "\nObjeto tipo " << pObjeto->getTipo() << ": "
		<< "Dirigiendose a la mina o el bosque";
		pObjeto->setDestino(pDestino->getX(), pDestino->getY()); //Cambiar destino (mina o bosque);
	}
}

void estadoRecolectar::ejecutar(Objeto* pObjeto, Objeto *pDestino) {
	//El objeto (campesino) recolecta oro o madera hasta que llega al ĺímite que puede transportar.
	pObjeto->modificarCantidadMaterial(pDestino->getTipoMaterial(), 1);
	pDestino->modificarCantidadMaterial(pDestino->getTipoMaterial(), -1);
	std::cout << "\nObjeto tipo " << pObjeto->getTipo() << ": "
	<< "Cogiendo material" << pDestino->getTipoMaterial();
	//Si hay suficiente material recolectado, almacenarlo al ayuntamiento
	if (pObjeto->cargaMaterialMaxima(pDestino->getTipoMaterial()))
	{
		pObjeto->cambiarEstado(estadoVisitarAyuntamientoYDepositarMaterial::Instance());
	}
}

void estadoRecolectar::salir(Objeto* pObjeto) {
	std::cout << "\nObjeto tipo " << pObjeto->getTipo() << ": "
	<< "Abandonando la mina o el bosque con la carga completa";
}

//--------------------------------------- Metodos para estadoVisitarAyuntamientoYDepositarMaterial
estadoVisitarAyuntamientoYDepositarMaterial* estadoVisitarAyuntamientoYDepositarMaterial::Instance() {
	static estadoVisitarAyuntamientoYDepositarMaterial instance;

	return &instance;
}

void estadoVisitarAyuntamientoYDepositarMaterial::entrar(Objeto* pObjeto, Objeto *pDestino) {
	//Si el objeto no esta ya localizado en el ayuntamiento se debe
	//cambiar la localizacion
	if (pObjeto->getDestinoX() != pDestino->getX() && pObjeto->getDestinoY() != pDestino->getY()) {
		std::cout << "\nObjeto tipo " << pObjeto->getTipo() << ": "
		<< "Dirigiendose al ayuntamiento";
		pObjeto->setDestino(pDestino->getX(), pDestino->getY()); //Cambiar destino ayuntamiento;
	}
}

void estadoVisitarAyuntamientoYDepositarMaterial::ejecutar(Objeto* pObjeto, Objeto *pDestino) {
	//El objeto (campesino) deposita el material recolectado
	pObjeto->modificarCantidadMaterial(pObjeto->getTipoMaterial(), -1*pObjeto->getCantidadMaterial(pObjeto->getTipoMaterial()));
	pDestino->modificarCantidadMaterial(pObjeto->getTipoMaterial(), pObjeto->getCantidadMaterial(pObjeto->getTipoMaterial()));
	std::cout << "\nObjeto tipo " << pObjeto->getTipo() << ": "
	<< "Depositando material" << pDestino->getTipoMaterial();
	//Volver al recurso a obtener mas material
	pObjeto->cambiarEstado(estadoRecolectar::Instance());
}

void estadoVisitarAyuntamientoYDepositarMaterial::salir(Objeto* pObjeto) {
	std::cout << "\nObjeto tipo " << pObjeto->getTipo() << ": "
	<< "Abandonando del ayuntamiento";
}
