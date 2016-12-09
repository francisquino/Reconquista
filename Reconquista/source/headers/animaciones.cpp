/*
 * animaciones.cpp
 *
 *  Created on: 07/12/2016
 *      Author: francis
 */

#include "animaciones.h"
#include "graphics.h"

Animaciones::Animaciones():
	_animacionFinalizada(false)
{}

Animaciones::Animaciones(Graphics& graphics) :
    AnimatedSprite(graphics, "content/sprites/TextBox.png", 0, 0, 8, 8, 0, 0, 100),
	_animacionFinalizada(false)
{
    graphics.loadImage("content/sprites/TextBox.png");
    this->setupAnimations();
    this->playAnimation("mas100Puntos");
}

void Animaciones::update(int elapsedTime) {
	AnimatedSprite::update(elapsedTime);
}

void Animaciones::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void Animaciones::animationDone(std::string currentAnimation) {
	//Si solo se hace una vez la animacion y ha terminado, lo indicamos
	if (this->_currentAnimationOnce) {
		printf("Finaliza animacion secundaria %s\n", currentAnimation.c_str());
		this->_animacionFinalizada=true;
	}
}


void Animaciones::setupAnimations() {
    this->addAnimation(6, 0, 0, "mas100Puntos", 32, 8, sf::Vector2i(0,-10));
}

