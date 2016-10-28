/*
 * objeto.cpp
 *
 *  Created on: 27/10/2016
 *      Author: francis
 */

#include "objeto.h"

//Clase base objeto
Objeto::Objeto() {}

Objeto::Objeto(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height,
        sf::Vector2i spawnPoint, int timeToUpdate) :
                    AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height,
                            spawnPoint.x, spawnPoint.y, timeToUpdate),
                    _direction(LEFT),
                    _maxHealth(0),
                    _currentHealth(0)
{}

void Objeto::update(int elapsedTime) {
    AnimatedSprite::update(elapsedTime);
}

void Objeto::draw(Graphics &graphics) {
    AnimatedSprite::draw(graphics, this->_x, this->_y);
}

//Clase Ayuntamiento
Ayuntamiento::Ayuntamiento() {}

Ayuntamiento::Ayuntamiento(Graphics &graphics, sf::Vector2i spawnPoint) :
        Objeto(graphics, "content/sprites/Ayuntamientos.png", 0, 0, 32, 32, spawnPoint, 100),
        _startingX(spawnPoint.x),
        _startingY(spawnPoint.y),
		_unidades(),
		_recursos()
 {
    this->setupAnimations();
    this->playAnimation("AyuntamientoInicial");
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

