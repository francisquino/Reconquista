/*
 * estadoscampesino.cpp
 *
 *  Created on: 10/11/2016
 *      Author: francis
 */

#include "estadoscampesino.h"
#include "estado.h"
#include "objeto.h"

#include <iostream>

//--------------------------------------- Metodos para campesinoEstadoGlobal
campesinoEstadoGlobal* campesinoEstadoGlobal::Instance() {
	static campesinoEstadoGlobal instance;

	return &instance;
}

void campesinoEstadoGlobal::entrar(Campesino* pCampesino) {
}

void campesinoEstadoGlobal::ejecutar(Campesino* pCampesino) {
}

void campesinoEstadoGlobal::salir(Campesino* pCampesino) {
}

bool campesinoEstadoGlobal::OnMessage(Campesino* pCampesino, const Telegrama& msg)
{
	return false;
}

//--------------------------------------- Metodos para campesinoEstadoInactivo
campesinoEstadoInactivo* campesinoEstadoInactivo::Instance() {
	static campesinoEstadoInactivo instance;

	return &instance;
}

void campesinoEstadoInactivo::entrar(Campesino* pCampesino) {
	//No se hace nada
	std::cout << "\nObjeto tipo " << pCampesino->getTipo() << ": "
	<< "En espera de orden";
}

void campesinoEstadoInactivo::ejecutar(Campesino* pCampesino) {
	std::cout << "\nObjeto tipo " << pCampesino->getTipo() << ": "
	<< "No hace nada";
}

void campesinoEstadoInactivo::salir(Campesino* pCampesino) {
	std::cout << "\nObjeto tipo " << pCampesino->getTipo() << ": "
	<< "Deja de estar inactivo";
}

bool campesinoEstadoInactivo::OnMessage(Campesino* pCampesino, const Telegrama& msg)
{
	/*
	SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

	switch(msg.Msg)
	{
		case Msg_StewReady:

			cout << "\nMessage handled by " << GetNameOfEntity(pCampesino->ID())
			<< " at time: " << Clock->GetCurrentTime();

			SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

			cout << "\n" << GetNameOfEntity(pCampesino->ID())
						  << ": Okay Hun, ahm a comin'!";

			pCampesino->GetFSM()->ChangeState(EatStew::Instance());

			return true;

	}//end switch
	*/

	//send msg to global message handler
	return false;
}

//--------------------------------------- Metodos para campesinoEstadoIrDestino
campesinoEstadoIrDestino* campesinoEstadoIrDestino::Instance() {
	static campesinoEstadoIrDestino instance;

	return &instance;
}

void campesinoEstadoIrDestino::entrar(Campesino* pCampesino) {
	//Fijamos como destino del objeto, la posicion del objeto destino
	std::cout << "\nObjeto tipo " << pCampesino->getTipo() << ": "
	<< "Fijamos destino";
	//pCampesino->setDestino(pDestino->getX(), pDestino->getY()); //Fijar destino (mina o bosque);
}

void campesinoEstadoIrDestino::ejecutar(Campesino* pCampesino) {
	//Si hay un destino fijado, movernos hacia él.
	if (pCampesino->getDestinoX() != -1 && pCampesino->getDestinoY() != 1) {
		if(pCampesino->getX() < pCampesino->getDestinoX()) {
			pCampesino->moveRight();
		}
		if(pCampesino->getX() > pCampesino->getDestinoX()) {
			pCampesino->moveLeft();
		}
		if(pCampesino->getY() < pCampesino->getDestinoY()) {
			pCampesino->moveDown();
		}
		if(pCampesino->getY() > pCampesino->getDestinoY()) {
			pCampesino->moveUp();
		}

		//Si hemos llegado al destino, inicializamos. Al usar números decimales, damos un margen de 2.0f
		if(abs(floor(pCampesino->getX() - pCampesino->getDestinoX())) < 2.0f &&
			abs(floor(pCampesino->getY() - pCampesino->getDestinoY())) < 2.0f) {
			//printf("DESTINO ALCANZADO\n");
			pCampesino->stopMoving();
			pCampesino->setDestinoAlcanzado(true);
			pCampesino->setDestino(-1, -1);
		}
	}
}

void campesinoEstadoIrDestino::salir(Campesino* pCampesino) {
	std::cout << "\nObjeto tipo " << pCampesino->getTipo() << ": "
	<< "Deja de estar inactivo";
}


bool campesinoEstadoIrDestino::OnMessage(Campesino* pCampesino, const Telegrama& msg)
{
  //send msg to global message handler
  return false;
}

//--------------------------------------- Metodos para campesinoEstadoRecolectar
campesinoEstadoRecolectar* campesinoEstadoRecolectar::Instance() {
	static campesinoEstadoRecolectar instance;

	return &instance;
}

void campesinoEstadoRecolectar::entrar(Campesino* pCampesino) {
	//Si el objeto no esta ya localizado en la mina de oro, o el bosque se debe
	//cambiar la localizacion
	if (pCampesino->getDestinoX() != pCampesino->getX() && pCampesino->getDestinoY() != pCampesino->getY()) {
		std::cout << "\nObjeto tipo " << pCampesino->getTipo() << ": "
		<< "Dirigiendose a la mina o el bosque";
	}
}

void campesinoEstadoRecolectar::ejecutar(Campesino* pCampesino) {
	/*
	//El objeto (campesino) recolecta oro o madera hasta que llega al ĺímite que puede transportar.
	pCampesino->modificarCantidadMaterial(pDestino->getTipoMaterial(), 1);
	pDestino->modificarCantidadMaterial(pDestino->getTipoMaterial(), -1);
	std::cout << "\nObjeto tipo " << pCampesino->getTipo() << ": "
	<< "Cogiendo material" << pDestino->getTipoMaterial();
	//Si hay suficiente material recolectado, almacenarlo al ayuntamiento
	if (pCampesino->cargaMaterialMaxima(pDestino->getTipoMaterial()))
	{
		pCampesino->cambiarEstado(campesinoEstadoVisitarAyuntamientoYDepositarMaterial::Instance());
	}
	*/
}

void campesinoEstadoRecolectar::salir(Campesino* pCampesino) {
	std::cout << "\nObjeto tipo " << pCampesino->getTipo() << ": "
	<< "Abandonando la mina o el bosque con la carga completa";
}

bool campesinoEstadoRecolectar::OnMessage(Campesino* pCampesino, const Telegrama& msg)
{
  //send msg to global message handler
  return false;
}

//--------------------------------------- Metodos para campesinoEstadoVisitarAyuntamientoYDepositarMaterial
campesinoEstadoVisitarAyuntamientoYDepositarMaterial* campesinoEstadoVisitarAyuntamientoYDepositarMaterial::Instance() {
	static campesinoEstadoVisitarAyuntamientoYDepositarMaterial instance;

	return &instance;
}

void campesinoEstadoVisitarAyuntamientoYDepositarMaterial::entrar(Campesino* pCampesino) {
	std::cout << "\nObjeto tipo " << pCampesino->getTipo() << ": "
	<< "Dirigiendose al ayuntamiento";
}

void campesinoEstadoVisitarAyuntamientoYDepositarMaterial::ejecutar(Campesino* pCampesino) {
	//El objeto (campesino) deposita el material recolectado
	/*
	pCampesino->modificarCantidadMaterial(pCampesino->getTipoMaterial(), -1*pCampesino->getCantidadMaterial(pCampesino->getTipoMaterial()));
	pDestino->modificarCantidadMaterial(pCampesino->getTipoMaterial(), pCampesino->getCantidadMaterial(pCampesino->getTipoMaterial()));
	*/
	std::cout << "\nObjeto tipo " << pCampesino->getTipo() << ": "
	<< "Depositando material"; // << pDestino->getTipoMaterial();
	//Volver al recurso a obtener mas material
	//pCampesino->cambiarEstado(campesinoEstadoRecolectar::Instance());
}

void campesinoEstadoVisitarAyuntamientoYDepositarMaterial::salir(Campesino* pCampesino) {
	std::cout << "\nObjeto tipo " << pCampesino->getTipo() << ": "
	<< "Abandonando del ayuntamiento";
}

bool campesinoEstadoVisitarAyuntamientoYDepositarMaterial::OnMessage(Campesino* pCampesino, const Telegrama& msg)
{
  //send msg to global message handler
  return false;
}
