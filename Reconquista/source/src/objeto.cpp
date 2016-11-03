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

Objeto::Objeto(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height,
				sf::Vector2i spawnPoint, int maxHealth, int timeToUpdate) :
                    AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height,
                            spawnPoint.x, spawnPoint.y, timeToUpdate),
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
    AnimatedSprite::update(elapsedTime);
}

void Objeto::draw(Graphics &graphics) {
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

const float Objeto::getX() const {
    return this->_x;
}

const float Objeto::getY() const {
    return this->_y;
}

void Objeto::setDestino(const float destinoX, const float destinoY) {
	this->_destinoX = destinoX;
	this->_destinoY = destinoY;
}

void Objeto::setDestinoAlcanzado(const bool destino) {
	this->_destinoAlcanzado = destino;
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

void Objeto::sumarUnidad(Objeto unidad) {
	this->_unidades.push_back(unidad);
}


//Clase Ayuntamiento
Ayuntamiento::Ayuntamiento() {}

Ayuntamiento::Ayuntamiento(Graphics &graphics, sf::Vector2i spawnPoint) :
        Objeto(graphics, "content/sprites/Ayuntamientos.png", 0, 0, 32, 32, spawnPoint, 10, 100)
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
 	Objeto(graphics, "content/sprites/Tile-set-Toen's Medieval Strategy.png", 96, 192, 16, 16, spawnPoint, 3, 100)
 {
     graphics.loadImage("content/sprites/Tile-set-Toen's Medieval Strategy.png");
     this->setupAnimations();
     this->playAnimation("IdleRight");
 }

 void Campesino::update(int elapsedTime) {
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
