/*
 * estadosmina.cpp
 *
 *  Created on: 10/11/2016
 *      Author: francis
 */

#include "estadosmina.h"
#include "estado.h"
#include "objeto.h"

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
	std::cout << "\nObjeto tipo " << pMina->getTipo() << ": "
	<< "En espera de orden";
}

void minaEstadoInactivo::ejecutar(Mina* pMina) {
	std::cout << "\nObjeto tipo " << pMina->getTipo() << ": "
	<< "No hace nada";
}

void minaEstadoInactivo::salir(Mina* pMina) {
	std::cout << "\nObjeto tipo " << pMina->getTipo() << ": "
	<< "Deja de estar inactivo";
}

bool minaEstadoInactivo::OnMessage(Mina* pMina, const Telegrama& msg)
{
	/*
	SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

	switch(msg.Msg)
	{
		case Msg_StewReady:

			cout << "\nMessage handled by " << GetNameOfEntity(pMina->ID())
			<< " at time: " << Clock->GetCurrentTime();

			SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

			cout << "\n" << GetNameOfEntity(pMina->ID())
						  << ": Okay Hun, ahm a comin'!";

			pMina->GetFSM()->ChangeState(EatStew::Instance());

			return true;

	}//end switch
	*/

	//send msg to global message handler
	return false;
}
