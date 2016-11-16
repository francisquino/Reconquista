/*
 * estadosayuntamiento.cpp
 *
 *  Created on: 10/11/2016
 *      Author: francis
 */

#include "estadosayuntamiento.h"
#include "estado.h"
#include "objeto.h"

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
	std::cout << "\nObjeto tipo " << pAyuntamiento->getTipo() << ": "
	<< "No hace nada";
}

void ayuntamientoEstadoInactivo::salir(Ayuntamiento* pAyuntamiento) {
	std::cout << "\nObjeto tipo " << pAyuntamiento->getTipo() << ": "
	<< "Deja de estar inactivo";
}

bool ayuntamientoEstadoInactivo::OnMessage(Ayuntamiento* pAyuntamiento, const Telegrama& msg)
{
	/*
	SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

	switch(msg.Msg)
	{
		case Msg_StewReady:

			cout << "\nMessage handled by " << GetNameOfEntity(pAyuntamiento->ID())
			<< " at time: " << Clock->GetCurrentTime();

			SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

			cout << "\n" << GetNameOfEntity(pAyuntamiento->ID())
						  << ": Okay Hun, ahm a comin'!";

			pAyuntamiento->GetFSM()->ChangeState(EatStew::Instance());

			return true;

	}//end switch
	*/

	//send msg to global message handler
	return false;
}
