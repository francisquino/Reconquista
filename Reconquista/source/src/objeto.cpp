/*
 * objeto.cpp
 *
 *  Created on: 27/10/2016
 *      Author: francis
 */

#include "level.h"
#include "objeto.h"
#include "graphics.h"
#include "maquinaestados.h"
#include "estadosayuntamiento.h"
#include "estadosmina.h"
#include "estadosbosque.h"
#include "estadoscampesino.h"

namespace object_constants {
    const float WALK_SPEED = 0.2f;
    //const float JUMP_SPEED = 0.7f;

    //const float GRAVITY = 0.002f;
    //const float GRAVITY_CAP = 0.8f;
}

extern Level _level;

//Clase base objeto
Objeto::Objeto() {}

Objeto::Objeto(Graphics &graphics, tipoObjeto::TipoObjeto tipo, std::string filePath, int sourceX, int sourceY, int width, int height,
				sf::Vector2i spawnPoint, int maxHealth, int timeToUpdate) :
                    AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height,
                            spawnPoint.x, spawnPoint.y, timeToUpdate),
					_tipo(tipo),
					_dx(0),
					_dy(0),
					_ruta(),
                    _facing(LEFT),
					_seleccionado(false),
                    _maxHealth(maxHealth),
                    _currentHealth(maxHealth),
            		_unidades(),
            		_materiales()
{}

void Objeto::update(int elapsedTime) {
	//Move by dx
	this->_x += this->_dx * elapsedTime;
	//Move by dy
	this->_y += this->_dy * elapsedTime;

	AnimatedSprite::update(elapsedTime);
}

void Objeto::draw(Graphics &graphics) {
	//Si el objeto está seleccionado, dibujamos su BoundingBox
	if (this->_seleccionado) {
	    sf::VertexArray _box(sf::LinesStrip, 5);
		_box[0].position = sf::Vector2f(this->getBoundingBox().getLeft(), this->getBoundingBox().getTop());
		_box[0].color = sf::Color::Green;
		_box[1].position = sf::Vector2f(this->getBoundingBox().getRight(), this->getBoundingBox().getTop());
		_box[1].color = sf::Color::Green;
		_box[2].position = sf::Vector2f(this->getBoundingBox().getRight(), this->getBoundingBox().getBottom());
		_box[2].color = sf::Color::Green;
		_box[3].position = sf::Vector2f(this->getBoundingBox().getLeft(), this->getBoundingBox().getBottom());
		_box[3].color = sf::Color::Green;
		_box[4].position = sf::Vector2f(this->getBoundingBox().getLeft(), this->getBoundingBox().getTop());
		_box[4].color = sf::Color::Green;

		graphics.getWindow().setView(*graphics.getView(Juego));
		graphics.getWindow().draw(_box);
	}

    AnimatedSprite::draw(graphics, this->_x, this->_y);
}

bool Objeto::manejarMensaje(const Telegrama& msg) {}

void Objeto::animationDone(std::string currentAnimation) {}

void Objeto::setupAnimations() {}

void Objeto::moveLeft() {
    this->_dx = -object_constants::WALK_SPEED;
    this->playAnimation("RunLeft");
    this->_facing = LEFT;
}

void Objeto::moveRight() {
    this->_dx = object_constants::WALK_SPEED;
    this->playAnimation("RunRight");
    this->_facing = RIGHT;
}

void Objeto::moveDown() {
    this->_dy = object_constants::WALK_SPEED;
    this->playAnimation("RunDown");
    this->_facing = DOWN;
}

void Objeto::moveUp() {
	this->_dy = -object_constants::WALK_SPEED;
	this->playAnimation("RunUp");
	this->_facing = UP;
}

void Objeto::stopMoving() {
    this->_dx = 0.0f;
    this->_dy = 0.0f;
    if (this->_facing == RIGHT) this->playAnimation("IdleRight");
    else if (this->_facing == LEFT) this->playAnimation("IdleLeft");
    else if (this->_facing == UP) this->playAnimation("IdleUp");
    else this->playAnimation("IdleDown");
}

void Objeto::setDestino(const sf::Vector2i destino) { //const float destinoX, const float destinoY) {
	//this->_destinoX = destinoX;
	//this->_destinoY = destinoY;
	this->_ruta.push_back(destino);
}

void Objeto::setSeleccionado(const bool seleccionado) {
	this->_seleccionado = seleccionado;
}

bool Objeto::checkColision(const Rectangle &other) {
    if (this->getBoundingBox().collidesWith(other)) {
        return true;
    }
    else return false;
}

//Comprueba si este objeto choca con el objeto que le indicamos
bool Objeto::chocaConObjeto(Objeto* pObjeto) {
	if (this->checkColision(pObjeto->getBoundingBox())) return true;
	else return false;
}

//Comprueba si este objeto choca con cualquier otro objeto del nivel
Objeto* Objeto::chocaConAlgunObjeto() {
	//Recorremos todos los objetos del nivel:

	//Ayuntamiento
	if (this->chocaConObjeto(_level._ayuntamiento) && this!=_level._ayuntamiento) {
		return (_level._ayuntamiento);
	}

	//Unidades del ayuntamiento
	for (unsigned int i=0; i<_level._ayuntamiento->_unidades.size(); i++) {
		if (this->chocaConObjeto(_level._ayuntamiento->_unidades.at(i)) && this!=_level._ayuntamiento->_unidades.at(i)) {
			return (_level._ayuntamiento->_unidades.at(i));
		}
	}

	//Recursos del nivel
	for (unsigned int i=0; i<_level._recursos.size(); i++) {
		if (this->chocaConObjeto(_level._recursos.at(i)) && this!=_level._recursos.at(i)) {
			return (_level._recursos.at(i));
		}
	}

	//No esta sobre ningun objeto
	return NULL;
}

//Comprueba si este objeto se encuentra en la misma casilla del mapa o en una adyacente al objeto que le indicamos
bool Objeto::juntoAObjeto(Objeto* pObjeto) {
	bool juntos = false; //Inicialmente no estan juntos

	//Posicion de este objeto en coordenadas del mapa
	sf::Vector2i posicionMapa = _level.coordAMapa(this->getX(), this->getY());
	//Posicion del otro objeto a comprobar en coordenadas del mapa
	sf::Vector2i posicionOtro = _level.coordAMapa(pObjeto->getX(), pObjeto->getY());

	//Comprobamos la misma casilla y
	//las 8 casillas adyacentes a este objeto
	if ((posicionMapa.x == posicionOtro.x && posicionMapa.y == posicionOtro.y) ||
			(posicionMapa.x+1 == posicionOtro.x && posicionMapa.y == posicionOtro.y) ||
			(posicionMapa.x+1 == posicionOtro.x && posicionMapa.y+1 == posicionOtro.y) ||
			(posicionMapa.x == posicionOtro.x && posicionMapa.y+1 == posicionOtro.y) ||
			(posicionMapa.x-1 == posicionOtro.x && posicionMapa.y+1 == posicionOtro.y) ||
			(posicionMapa.x-1 == posicionOtro.x && posicionMapa.y == posicionOtro.y) ||
			(posicionMapa.x-1 == posicionOtro.x && posicionMapa.y-1 == posicionOtro.y) ||
			(posicionMapa.x == posicionOtro.x && posicionMapa.y-1 == posicionOtro.y) ||
			(posicionMapa.x+1 == posicionOtro.x && posicionMapa.y-1 == posicionOtro.y))
		juntos = true;
	return juntos;
}


void Objeto::gainHealth(int amount) {
    if ((this->_currentHealth += amount) < 0)
        this->_currentHealth = 0;
}


void Objeto::modificarCantidadMaterial(tipoMaterial::TipoMaterial material, int cantidad) {
	if ((this->_materiales[material] += cantidad) < 0)
		this->_materiales[material] = 0;
 }

tipoMaterial::TipoMaterial Objeto::getTipoMaterial() {}

void Objeto::sumarUnidad(Objeto* unidad) {
	this->_unidades.push_back(unidad);
}


//Clase Ayuntamiento
Ayuntamiento::Ayuntamiento() {}

Ayuntamiento::Ayuntamiento(Graphics &graphics, sf::Vector2i spawnPoint) :
        Objeto(graphics, tipoObjeto::Ayuntamiento, "content/sprites/Ayuntamientos.png", 0, 0, 32, 32, spawnPoint, 10, 100)
 {
	graphics.loadImage("content/sprites/Ayuntamientos.png");
	this->setupAnimations();
    this->playAnimation("AyuntamientoInicial");
    this->modificarCantidadMaterial(tipoMaterial::Oro, 500);
    this->modificarCantidadMaterial(tipoMaterial::Madera, 800);

	//Inicializar la maquina de estados
	this->_apMaquinaEstados = new maquinaEstados<Ayuntamiento>(this);
	this->_apMaquinaEstados->setEstadoActual(ayuntamientoEstadoInactivo::Instance());
 }

 void Ayuntamiento::update(int elapsedTime) {
	 Objeto::update(elapsedTime);
	 this->_apMaquinaEstados->update();
 }

 void Ayuntamiento::draw(Graphics &graphics) {
    Objeto::draw(graphics);
 }

 bool Ayuntamiento::manejarMensaje(const Telegrama& msg) {
	 return _apMaquinaEstados->manejarMensaje(msg);
 }

 void Ayuntamiento::animationDone(std::string currentAnimation) {

 }

 void Ayuntamiento::setupAnimations() {
    this->addAnimation(1, 0, 0, "AyuntamientoInicial", 32, 32, sf::Vector2i(0,0));
 }

 tipoMaterial::TipoMaterial Ayuntamiento::getTipoMaterial() {}


//class Mina
Mina::Mina() {}

Mina::Mina(Graphics &graphics, sf::Vector2i spawnPoint, int cantidad) :
                Objeto(graphics, tipoObjeto::Mina, "content/sprites/Recursos.png", 0, 0, 16, 16, spawnPoint, 8, 100)
{
	graphics.loadImage("content/sprites/Recursos.png");
	this->setupAnimations();
    this->playAnimation("MinaInicial");
    this->modificarCantidadMaterial(tipoMaterial::Oro, cantidad);

	//Inicializar la maquina de estados
	this->_mpMaquinaEstados = new maquinaEstados<Mina>(this);
	this->_mpMaquinaEstados->setEstadoActual(minaEstadoInactivo::Instance());
}

void Mina::update(int elapsedTime) {
	 Objeto::update(elapsedTime);

	 this->_mpMaquinaEstados->update();
}

void Mina::draw(Graphics &graphics) {
    Objeto::draw(graphics);
}

bool Mina::manejarMensaje(const Telegrama& msg)
{
	 return _mpMaquinaEstados->manejarMensaje(msg);
}

void Mina::animationDone(std::string currentAnimation) {
}

void Mina::setupAnimations() {
    this->addAnimation(1, 0, 0, "MinaInicial", 16, 16, sf::Vector2i(0,0));
}

 //class Bosque
Bosque::Bosque() {}

Bosque::Bosque(Graphics &graphics, sf::Vector2i spawnPoint, int cantidad) :
                Objeto(graphics, tipoObjeto::Bosque, "content/sprites/Recursos.png", 16, 0, 16, 16, spawnPoint, 0, 100)
{
	graphics.loadImage("content/sprites/Recursos.png");
	this->setupAnimations();
    this->playAnimation("BosqueInicial");
    this->modificarCantidadMaterial(tipoMaterial::Madera, cantidad);

	//Inicializar la maquina de estados
	this->_bpMaquinaEstados = new maquinaEstados<Bosque>(this);
	this->_bpMaquinaEstados->setEstadoActual(bosqueEstadoInactivo::Instance());
}

void Bosque::update(int elapsedTime) {
	 Objeto::update(elapsedTime);
	 this->_bpMaquinaEstados->update();
}

void Bosque::draw(Graphics &graphics) {
    Objeto::draw(graphics);
}

bool Bosque::manejarMensaje(const Telegrama& msg)
{
	 return _bpMaquinaEstados->manejarMensaje(msg);
}

void Bosque::animationDone(std::string currentAnimation) {
}

void Bosque::setupAnimations() {
    this->addAnimation(1, 1, 0, "BosqueInicial", 16, 16, sf::Vector2i(0,0));
}


//Clase Campesino
Campesino::Campesino() {}

Campesino::Campesino(Graphics&graphics, sf::Vector2i spawnPoint) :
 	Objeto(graphics, tipoObjeto::Campesino, "content/sprites/Tile-set-Toen's Medieval Strategy.png", 96, 192, 16, 16, spawnPoint, 3, 100),
 	_recursoRecoleccion(NULL)
{
	graphics.loadImage("content/sprites/Tile-set-Toen's Medieval Strategy.png");
	this->setupAnimations();
	this->playAnimation("IdleRight");
	this->_cargaMaxima[tipoMaterial::Madera] = 100;
	this->_cargaMaxima[tipoMaterial::Oro] = 100;

	//Inicializar la maquina de estados
	this->_cpMaquinaEstados = new maquinaEstados<Campesino>(this);
	this->_cpMaquinaEstados->setEstadoActual(campesinoEstadoInactivo::Instance());
	this->_cpMaquinaEstados->setEstadoGlobal(campesinoEstadoGlobal::Instance());
}

 void Campesino::update(int elapsedTime) {
	//Si hay un destino fijado, movernos hacia él.
	 /*
	if (this->getDestinoX() != -1 && this->getDestinoY() != -1) {
		if(this->getX() < this->getDestinoX()) {
			this->moveRight();
		}
		if(this->getX() > this->getDestinoX()) {
			this->moveLeft();
		}
		if(this->getY() < this->getDestinoY()) {
			this->moveDown();
		}
		if(this->getY() > this->getDestinoY()) {
			this->moveUp();
		}

		//Si hemos llegado al destino, inicializamos. Al usar números decimales, damos un margen de 2.0f
		if(abs(floor(this->getX() - this->getDestinoX())) < 2.0f &&
			abs(floor(this->getY() - this->getDestinoY())) < 2.0f) {
			//printf("DESTINO ALCANZADO\n");
			this->stopMoving();
			this->setDestinoAlcanzado(true);
			this->setDestino(-1, -1);
		}
	}

	//Move by dx
	this->_x += this->_dx * elapsedTime;
	//Move by dy
	this->_y += this->_dy * elapsedTime;
	*/

	Objeto::update(elapsedTime);

	this->_cpMaquinaEstados->update();
 }

 void Campesino::draw(Graphics &graphics) {
    Objeto::draw(graphics);
 }

 bool Campesino::manejarMensaje(const Telegrama& msg)
 {
	 return _cpMaquinaEstados->manejarMensaje(msg);
 }

 void Campesino::animationDone(std::string currentAnimation) {}

 void Campesino::setupAnimations() {
     this->addAnimation(1, 6, 12, "IdleRight", 16, 16, sf::Vector2i(0,0));
     this->addAnimation(1, 6, 12, "IdleLeft", 16, 16, sf::Vector2i(0,0));
     this->addAnimation(1, 6, 12, "IdleUp", 16, 16, sf::Vector2i(0,0));
     this->addAnimation(1, 6, 12, "IdleDown", 16, 16, sf::Vector2i(0,0));
     this->addAnimation(1, 6, 12, "RunRight", 16, 16, sf::Vector2i(0,0));
     this->addAnimation(1, 6, 12, "RunLeft", 16, 16, sf::Vector2i(0,0));
     this->addAnimation(1, 6, 12, "RunUp", 16, 16, sf::Vector2i(0,0));
     this->addAnimation(1, 6, 12, "RunDown", 16, 16, sf::Vector2i(0,0));
     //this->addAnimation(1, 96, 192, "IdleLeft", 16, 16, sf::Vector2i(0,0));
     /*this->addAnimation(3, 0, 0, "RunLeft", 16, 16, sf::Vector2i(0,0));
     this->addAnimation(3, 0, 16, "RunRight", 16, 16, sf::Vector2i(0,0));
     this->addAnimation(1, 3, 0, "IdleLeftUp", 16, 16, sf::Vector2i(0,0));
     this->addAnimation(1, 3, 16, "IdleRightUp", 16, 16, sf::Vector2i(0,0));
     this->addAnimation(3, 3, 0, "RunLeftUp", 16, 16, sf::Vector2i(0,0));
     this->addAnimation(3, 3, 16, "RunRightUp", 16, 16, sf::Vector2i(0,0));
     this->addAnimation(1, 6, 0, "LookDownLeft", 16, 16, sf::Vector2i(0,0));
     this->addAnimation(1, 6, 16, "LookDownRight", 16, 16, sf::Vector2i(0,0));
     this->addAnimation(1, 7, 0, "LookBackwardsLeft", 16, 16, sf::Vector2i(0,0));
     this->addAnimation(1, 7, 16, "LookBackwardsRight", 16, 16, sf::Vector2i(0,0));*/
 }

 tipoMaterial::TipoMaterial Campesino::getTipoMaterial() {
	 //Devuelve el material del que transportamos alguna cantidad
	 if (this->getCantidadMaterial(tipoMaterial::Madera) > 0) return tipoMaterial::Madera;
	 else return tipoMaterial::Oro;
 }
