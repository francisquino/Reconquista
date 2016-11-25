/*
 * estadosbosque.cpp
 *
 *  Created on: 10/11/2016
 *      Author: francis
 */

#include "estadosbosque.h"
#include "estado.h"
#include "objeto.h"

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
	std::cout << "\nObjeto tipo " << pBosque->getTipo() << ": "
	<< "En espera de orden";
}

void bosqueEstadoInactivo::ejecutar(Bosque* pBosque) {
	//std::cout << "\nObjeto tipo " << pBosque->getTipo() << ": "
	//<< "No hace nada";
}

void bosqueEstadoInactivo::salir(Bosque* pBosque) {
	std::cout << "\nObjeto tipo " << pBosque->getTipo() << ": "
	<< "Deja de estar inactivo";
}

bool bosqueEstadoInactivo::OnMessage(Bosque* pBosque, const Telegrama& msg)
{
	/*
	SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

	switch(msg.Msg)
	{
		case Msg_StewReady:

			cout << "\nMessage handled by " << GetNameOfEntity(pBosque->ID())
			<< " at time: " << Clock->GetCurrentTime();

			SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

			cout << "\n" << GetNameOfEntity(pBosque->ID())
						  << ": Okay Hun, ahm a comin'!";

			pBosque->GetFSM()->ChangeState(EatStew::Instance());

			return true;

	}//end switch
	*/

	//send msg to global message handler
	return false;
}
