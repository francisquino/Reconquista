/*
 * objeto.cpp
 *
 *  Created on: 27/10/2016
 *      Author: francis
 */

#include "objeto.h"
#include "graphics.h"

namespace object_constants {
    const float WALK_SPEED = 0.2f;
    //const float JUMP_SPEED = 0.7f;

    //const float GRAVITY = 0.002f;
    //const float GRAVITY_CAP = 0.8f;
}


//Clase base objeto
Objeto::Objeto() {}

Objeto::Objeto(Graphics &graphics, tipoObjeto::TipoObjeto tipo, std::string filePath, int sourceX, int sourceY, int width, int height,
				sf::Vector2i spawnPoint, int maxHealth, int timeToUpdate) :
                    AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height,
                            spawnPoint.x, spawnPoint.y, timeToUpdate),
					_tipo(tipo),
					_dx(0),
					_dy(0),
					_destinoX(-1),
					_destinoY(-1),
                    _facing(LEFT),
					_destinoAlcanzado(false),
                    _maxHealth(maxHealth),
                    _currentHealth(maxHealth),
            		_unidades(),
            		_recursos()
{}

void Objeto::update(int elapsedTime) {
	AnimatedSprite::update(elapsedTime);}

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

void Objeto::setDestino(const float destinoX, const float destinoY) {
	this->_destinoX = destinoX;
	this->_destinoY = destinoY;
}

void Objeto::setDestinoAlcanzado(const bool destino) {
	this->_destinoAlcanzado = destino;
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

void Objeto::gainHealth(int amount) {
    if ((this->_currentHealth += amount) < 0)
        this->_currentHealth = 0;
}


void Objeto::modificarRecurso(std::string recurso, int cantidad) {
	if ((this->_recursos[recurso] += cantidad) < 0)
		this->_recursos[recurso] = 0;
 }

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
    this->modificarRecurso("oro", 500);
    this->modificarRecurso("madera", 800);
 }

 void Ayuntamiento::update(int elapsedTime) {
	 Objeto::update(elapsedTime);
 }

 void Ayuntamiento::draw(Graphics &graphics) {
    Objeto::draw(graphics);
 }

 void Ayuntamiento::animationDone(std::string currentAnimation) {

 }

 void Ayuntamiento::setupAnimations() {
    this->addAnimation(1, 0, 0, "AyuntamientoInicial", 32, 32, sf::Vector2i(0,0));
 }

 //Clase Campesino
 Campesino::Campesino() {}

 Campesino::Campesino(Graphics&graphics, sf::Vector2i spawnPoint) :
 	Objeto(graphics, tipoObjeto::Campesino, "content/sprites/Tile-set-Toen's Medieval Strategy.png", 96, 192, 16, 16, spawnPoint, 3, 100)
 {
     graphics.loadImage("content/sprites/Tile-set-Toen's Medieval Strategy.png");
     this->setupAnimations();
     this->playAnimation("IdleRight");
 }

 void Campesino::update(int elapsedTime) {
	//Si hay un destino fijado, movernos hacia él.
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

	Objeto::update(elapsedTime);
 }

 void Campesino::draw(Graphics &graphics) {
    Objeto::draw(graphics);
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
