/*
 * estadoscampesino.cpp
 *
 *  Created on: 10/11/2016
 *      Author: francis
 */

#include "tipoestadojuego.h"
#include "estadoscampesino.h"
#include "estado.h"
#include "objeto.h"
#include "tipomensaje.h"
#include "level.h"

#include <iostream>


extern Level _level;
extern tipoEstadoJuego _estadoJuego;

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

	//Aquí se pueden recibir los mensajes de Parar y RecibirAtaque por ejemplo.
	return false;
}

//--------------------------------------- Metodos para campesinoEstadoInactivo
campesinoEstadoInactivo* campesinoEstadoInactivo::Instance() {
	static campesinoEstadoInactivo instance;

	return &instance;
}

void campesinoEstadoInactivo::entrar(Campesino* pCampesino) {
	//std::cout << "\nObjeto tipo " << pCampesino->getTipo() << ": "
	//<< "EstadoInactivo esperando orden\n";
}

void campesinoEstadoInactivo::ejecutar(Campesino* pCampesino) {
	//std::cout << "\nObjeto tipo " << pCampesino->getTipo() << ": "
	//<< "No hace nada";
}

void campesinoEstadoInactivo::salir(Campesino* pCampesino) {
	//std::cout << "\nObjeto tipo " << pCampesino->getTipo() << ": "
	//<< "Deja de estar inactivo";
}

bool campesinoEstadoInactivo::OnMessage(Campesino* pCampesino, const Telegrama& msg)
{
	switch(msg._msg)
	{
		case _msjIrA:
			pCampesino->GetFSM()->cambiarEstado(campesinoEstadoIrDestino::Instance());

			return true;
		case _msjRecolectar:
			pCampesino->GetFSM()->cambiarEstado(campesinoEstadoRecolectar::Instance());

			return true;
	}//end switch

	//send msg to global message handler
	return false;
}


//--------------------------------------- Metodos para campesinoEstadoIrDestino
campesinoEstadoIrDestino* campesinoEstadoIrDestino::Instance() {
	static campesinoEstadoIrDestino instance;

	return &instance;
}

void campesinoEstadoIrDestino::entrar(Campesino* pCampesino) {
	//std::cout << "\nObjeto tipo " << pCampesino->getTipo() << ": "
	//<< "Fijamos destino";
}

void campesinoEstadoIrDestino::ejecutar(Campesino* pCampesino) {
	//Si hay una ruta, movernos al primer punto.
	if (pCampesino->getDestinoX() && pCampesino->getDestinoY()) {
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
			pCampesino->deleteDestino();

			//Si no quedan puntos en la ruta, nos paramos y pasamos al estado previo
			if (!pCampesino->getDestinoX() && !pCampesino->getDestinoY()) {
				//printf("DESTINO ALCANZADO\n");
				pCampesino->stopMoving();
				//pCampesino->setDestino(-1, -1);
				pCampesino->GetFSM()->cambiarAEstadoPrevio();
			}
		}
	}
}

void campesinoEstadoIrDestino::salir(Campesino* pCampesino) {
	//std::cout << "\nObjeto tipo " << pCampesino->getTipo() << ": "
	//<< "Sale de IrDestino\n";
}


bool campesinoEstadoIrDestino::OnMessage(Campesino* pCampesino, const Telegrama& msg)
{
	switch(msg._msg)
	{
		case _msjDestinoFijado:
			printf ("Destino fijado\n");
			//Obtener coordenadas del destino en el telegrama
			sf::Vector2i* destino = (sf::Vector2i*)msg._extraInfo;

			//pCampesino->setDestino(destino->x, destino->y);

			return true; //_msjDestinoParcial
	}//end switch
  //send msg to global message handler
  return false;
}

//--------------------------------------- Metodos para campesinoEstadoRecolectar
campesinoEstadoRecolectar* campesinoEstadoRecolectar::Instance() {
	static campesinoEstadoRecolectar instance;

	return &instance;
}

void campesinoEstadoRecolectar::entrar(Campesino* pCampesino) {
	//printf("\nEntramos en Estado Recolectar\n");
	//Si el objeto no esta ya localizado sobre un recurso se debe esperar un destino
	if (pCampesino->getRecursoRecolectar() == NULL) {
		//printf("\nCampesino espera como destino un recurso para recolectar\n");
		pCampesino->GetFSM()->cambiarEstado(campesinoEstadoIrDestino::Instance());
	}
	else {
		Objeto* recurso = pCampesino->getRecursoRecolectar(); //Recurso que estamos explotando
		Objeto* ayuntamiento = _level._ayuntamiento; //Ayuntamiento al que pertenece el Campesino

		//Objeto sobre el que se encuentra situado el Campesino, el ayuntamiento o un recurso
		Objeto* pObjeto = pCampesino->chocaConAlgunObjeto();

		//Si estamos situados sobre el recurso, recolectamos el material
		if (pObjeto!=NULL && (pObjeto->getTipo()==tipoObjeto::Bosque || pObjeto->getTipo()==tipoObjeto::Mina))
			printf("\nCampesino empieza a recolectar\n");
		//Si estamos situados sobre el ayuntamiento, nos desplazamos al recurso
		else if (pObjeto!=NULL && pObjeto->getTipo()==tipoObjeto::Ayuntamiento) {
			//Enviar un mensaje al ayuntamiento para aumentar sus recursos

			printf("Campesino deja en Ayuntamiento %i %s\n", pCampesino->getCantidadMaterial(pCampesino->getTipoMaterial()), materialToStr(pCampesino->getTipoMaterial()).c_str());
			pCampesino->modificarCantidadMaterial(pCampesino->getTipoMaterial(), -1*pCampesino->getCantidadMaterial(pCampesino->getTipoMaterial()));
			printf("Regresar a %s\n", tipoObjetoToStr(recurso->getTipo()).c_str());
			sf::Vector2i posicion_destino = sf::Vector2i (recurso->getX(), recurso->getY());
			Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY, 	//time delay
									ayuntamiento,			//Objeto* sender
									pCampesino,			   	//Objeto* recipient
									_msjDestinoFijado,		//the message
									&posicion_destino);		//Informacion extra
		}
	}
}

void campesinoEstadoRecolectar::ejecutar(Campesino* pCampesino) {
	Objeto* recurso = pCampesino->getRecursoRecolectar();
	tipoMaterial::TipoMaterial material = recurso->getTipoMaterial();

	//Si queda material en el recurso, cargamos el material
	//Si ya no queda material, borramos el recurso del campesino.
	if (recurso->getCantidadMaterial(material)>0) {
		pCampesino->modificarCantidadMaterial(material, pCampesino->cargaMaterialMaxima(material));
		recurso->modificarCantidadMaterial(material, -1*pCampesino->cargaMaterialMaxima(material));
		printf("%s disponible %i\n", materialToStr(recurso->getTipoMaterial()).c_str(), recurso->getCantidadMaterial(material));
	}

	//Si ya no queda material, borramos el recurso del campesino.
	if (recurso->getCantidadMaterial(material)<=0) {
		pCampesino->setRecursoRecolectar(NULL);
		printf("Recurso agotado\n");
		pCampesino->GetFSM()->cambiarEstado(campesinoEstadoInactivo::Instance());

		_estadoJuego=_estInactivo;
	}

	//Si hemos recolectado algo de material, ir al ayuntamiento a depositarlo
	if (pCampesino->getCantidadMaterial(material)>0) {
		printf("Ir a Ayuntamiento a depositar el material\n");
		sf::Vector2i posicion_destino = sf::Vector2i (_level._ayuntamiento->getX(), _level._ayuntamiento->getY());
		Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY, 	//time delay
								pCampesino,				//Objeto* sender
								pCampesino,			   	//Objeto* recipient
								_msjDestinoFijado,		//the message
								&posicion_destino);		//Informacion extra
	}

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
	//std::cout << "\nObjeto tipo " << tipoObjetoToStr(pCampesino->getTipo()) << ": "
	//<< "Saliendo de EstadoRecolectar\n";
}

bool campesinoEstadoRecolectar::OnMessage(Campesino* pCampesino, const Telegrama& msg)
{
	switch(msg._msg)
	{
		case _msjDestinoFijado:
			sf::Vector2i* destino = (sf::Vector2i*)msg._extraInfo;
			pCampesino->setDestino(*destino);
			//printf("Campesino pasa a estado temporal Ir Destino: (%i, %i)\n", destino->x, destino->y);
			pCampesino->GetFSM()->cambiarEstado(campesinoEstadoIrDestino::Instance());

			return true;
	}//end switch

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
