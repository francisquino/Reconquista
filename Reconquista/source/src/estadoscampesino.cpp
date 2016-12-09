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
#include "animaciones.h"

#include <iostream>


extern Graphics graphics;
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
				printf("DESTINO ALCANZADO\n");
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
		printf("\nCampesino espera como destino un recurso para recolectar\n");
		pCampesino->GetFSM()->cambiarEstado(campesinoEstadoIrDestino::Instance());
	}
	else {
		Objeto* recurso = pCampesino->getRecursoRecolectar(); //Recurso que estamos explotando
		Objeto* ayuntamiento = _level._ayuntamiento; //Ayuntamiento al que pertenece el Campesino

		//Si estamos situados junto al recurso, recolectamos el material
		if (pCampesino->juntoAObjeto(recurso))
			printf("\nCampesino empieza a recolectar %s %p\n", materialToStr(recurso->getTipoMaterial()).c_str(), (void*)recurso);
		//Si estamos situados junto al ayuntamiento, descargamos y volvemos al recurso
		else if (pCampesino->juntoAObjeto(ayuntamiento)) {
			//Enviar un mensaje al ayuntamiento para aumentar sus recursos
			std::map<tipoMaterial::TipoMaterial, int> *materialYTipoRecolectado;

			(*materialYTipoRecolectado)[pCampesino->getTipoMaterial()] = pCampesino->getCantidadMaterial(pCampesino->getTipoMaterial());
			Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY, 		//time delay
									pCampesino,					//Objeto* sender
									ayuntamiento,		   		//Objeto* recipient
									_msjDescargarMaterial,		//the message
									materialYTipoRecolectado);	//Informacion extra

			printf("Campesino deja en Ayuntamiento %i %s\n", pCampesino->getCantidadMaterial(pCampesino->getTipoMaterial()), materialToStr(pCampesino->getTipoMaterial()).c_str());
			pCampesino->modificarCantidadMaterial(pCampesino->getTipoMaterial(), -1*pCampesino->getCantidadMaterial(pCampesino->getTipoMaterial()));

			//Creamos una nueva animacion secundaria de "+100 puntos"
			Animaciones* as = new Animaciones(graphics);
			printf("Nueva animacion +100Puntos %p\n", (void*)as);
			as->playAnimation("mas100Puntos", true); //Once = true
			as->setX(ayuntamiento->getX());
			as->setY(ayuntamiento->getY());
			_level._animacionesSecundarias.push_back(as);


			printf("Regresar a %s\n", tipoObjetoToStr(recurso->getTipo()).c_str());
			sf::Vector2i posicionDestino = sf::Vector2i (recurso->getX(), recurso->getY());
			posicionDestino.x = posicionDestino.x - (posicionDestino.x % (_level.getTileSize().x * (int)globals::SPRITE_SCALE));
			posicionDestino.y = posicionDestino.y - (posicionDestino.y % (_level.getTileSize().y * (int)globals::SPRITE_SCALE));

			//Calculamos la ruta a recorrer con el algoritmo A*
			//Primero pasamos las coordenadas del terreno a las del mapa
			sf::Vector2i origenMapa = _level.coordAMapa(pCampesino->getX(), pCampesino->getY());
			sf::Vector2i destinoMapa = _level.coordAMapa(posicionDestino.x, posicionDestino.y);
			//printf("Origen [%i,%i] Destino [%i,%i]\n", origenMapa.x, origenMapa.y, destinoMapa.x, destinoMapa.y);

			//Como el destino es una posicion del mapa que sobre la que no podemos pasar,
			//buscamos como destino la posicion accesible mas cercana
			sf::Vector2i destinoAlcanzable = _level.posicionAccesibleMasCercana(origenMapa.x, origenMapa.y, destinoMapa.x, destinoMapa.y);
			//printf ("Destino alcanzable [%i,%i]\n", destinoAlcanzable.x, destinoAlcanzable.y);

			std::string ruta = _level.pathFind(origenMapa.x, origenMapa.y, destinoAlcanzable.x, destinoAlcanzable.y);

			if (ruta.compare("")!=0) { //Si ruta es distinto de ""
				std::vector<sf::Vector2i> rutaSimple = _level.simplificarRutaCoord (_level.rutaACoordenadas (ruta, origenMapa), origenMapa);

				for (unsigned int i=0; i<rutaSimple.size(); i++) {
					sf::Vector2i destinoCoord = _level.mapaACoord(rutaSimple[i].x, rutaSimple[i].y);
					//Cada desplazamiento lo almacenamos en la ruta del campesino
					pCampesino->setDestino(destinoCoord);
				}

				Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY, 	//time delay
										NULL,					//Objeto* sender
										pCampesino,			   	//Objeto* recipient
										_msjDestinoFijado,		//the message
										NULL);					//Informacion extra
			}
		}
	}
}

void campesinoEstadoRecolectar::ejecutar(Campesino* pCampesino) {
	Objeto* recurso = pCampesino->getRecursoRecolectar();
	tipoMaterial::TipoMaterial material = recurso->getTipoMaterial();

	//Si queda material en el recurso, cargamos el material
	//Si ya no queda material, borramos el recurso del campesino.
	if (recurso->getCantidadMaterial(material)>0) {

		//Si la cantidad de material que queda es menor que la carga maxima que puede transportar el campesino,
		//recolectamos esa cantidad
		int cantidadRecolectada=pCampesino->cargaMaterialMaxima(material);
		if (recurso->getCantidadMaterial(material) < cantidadRecolectada) {
			cantidadRecolectada=recurso->getCantidadMaterial(material);
		}
		pCampesino->modificarCantidadMaterial(material, cantidadRecolectada);
		printf("Campesino recoje %i %i %s\n", cantidadRecolectada, pCampesino->getCantidadMaterial(material), materialToStr(pCampesino->getTipoMaterial()).c_str());

		//Enviar un mensaje al recurso para disminuir su material
		Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY, 	//time delay
								pCampesino,				//Objeto* sender
								recurso,			   	//Objeto* recipient
								_msjRecolectarMaterial,	//the message
								NULL);					//Informacion extra

		//Creamos una nueva animacion secundaria de "+100 puntos"
		Animaciones* as = new Animaciones(graphics);
		printf("Nueva animacion +100Puntos %p\n", (void*)as);
		as->playAnimation("mas100Puntos", true); //Once = true
		as->setX(recurso->getX());
		as->setY(recurso->getY());
		_level._animacionesSecundarias.push_back(as);

		printf("%s disponible %i\n", materialToStr(recurso->getTipoMaterial()).c_str(), recurso->getCantidadMaterial(material));
	}

	//Si ya no queda material, borramos el recurso del campesino.
	if (recurso->getCantidadMaterial(material)<=0) {
		pCampesino->setRecursoRecolectar(NULL);
		printf("Recurso agotado\n");

		//Si el recurso es un bosque, buscar si hay uno cerca y fijarlo como nuevo objetivo

		pCampesino->GetFSM()->cambiarEstado(campesinoEstadoInactivo::Instance());

		_estadoJuego=_estInactivo;
	}

	//Si hemos recolectado algo de material, ir al ayuntamiento a depositarlo
	if (pCampesino->getCantidadMaterial(material)>0) {
		printf("Ir a Ayuntamiento a depositar el material\n");

		sf::Vector2i posicionDestino = sf::Vector2i (_level._ayuntamiento->getX(), _level._ayuntamiento->getY());
		posicionDestino.x = posicionDestino.x - (posicionDestino.x % (_level.getTileSize().x * (int)globals::SPRITE_SCALE));
		posicionDestino.y = posicionDestino.y - (posicionDestino.y % (_level.getTileSize().y * (int)globals::SPRITE_SCALE));

		//Calculamos la ruta a recorrer con el algoritmo A*
		//Primero pasamos las coordenadas del terreno a las del mapa
		sf::Vector2i origenMapa = _level.coordAMapa(pCampesino->getX(), pCampesino->getY());
		sf::Vector2i destinoMapa = _level.coordAMapa(posicionDestino.x, posicionDestino.y);
		//printf("Origen [%i,%i] Destino [%i,%i]\n", origenMapa.x, origenMapa.y, destinoMapa.x, destinoMapa.y);

		//Como el destino es una posicion del mapa que sobre la que no podemos pasar,
		//buscamos como destino la posicion accesible mas cercana
		sf::Vector2i destinoAlcanzable = _level.posicionAccesibleMasCercana(origenMapa.x, origenMapa.y, destinoMapa.x, destinoMapa.y);
		//printf ("Destino alcanzable [%i,%i]\n", destinoAlcanzable.x, destinoAlcanzable.y);

		std::string ruta = _level.pathFind(origenMapa.x, origenMapa.y, destinoAlcanzable.x, destinoAlcanzable.y);

		if (ruta.compare("")!=0) { //Si ruta es distinto de ""
			std::vector<sf::Vector2i> rutaSimple = _level.simplificarRutaCoord (_level.rutaACoordenadas (ruta, origenMapa), origenMapa);

			for (unsigned int i=0; i<rutaSimple.size(); i++) {
				sf::Vector2i destinoCoord = _level.mapaACoord(rutaSimple[i].x, rutaSimple[i].y);
				//Cada desplazamiento lo almacenamos en la ruta del campesino
				pCampesino->setDestino(destinoCoord);
			}

			Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY, 	//time delay
									pCampesino,				//Objeto* sender
									pCampesino,			   	//Objeto* recipient
									_msjDestinoFijado,		//the message
									NULL);					//Informacion extra
		}
	}
}

void campesinoEstadoRecolectar::salir(Campesino* pCampesino) {
	//std::cout << "\nObjeto tipo " << tipoObjetoToStr(pCampesino->getTipo()) << ": "
	//<< "Saliendo de EstadoRecolectar\n";
	printf ("Saliendo de EstadoRecolectar\n");
}

bool campesinoEstadoRecolectar::OnMessage(Campesino* pCampesino, const Telegrama& msg)
{
	switch(msg._msg)
	{
		case _msjDestinoFijado:
			//printf("Campesino pasa a estado temporal Ir Destino: (%i, %i)\n", destino->x, destino->y);
			pCampesino->GetFSM()->cambiarEstado(campesinoEstadoIrDestino::Instance());

			return true;
	}//end switch

  //send msg to global message handler
  return false;
}
