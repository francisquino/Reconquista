/*
 * animaciones.h
 *
 *  Created on: 07/12/2016
 *      Author: francis
 */

#ifndef ANIMACIONES_H
#define ANIMACIONES_H

#include "animatedsprite.h"

class Graphics;

class Animaciones:public AnimatedSprite {
	public:
		Animaciones();
		Animaciones(Graphics &graphics);
		void update(int elapsedTime);
		void draw(Graphics &graphics);

	    virtual void animationDone(std::string currentAnimation);
		virtual void setupAnimations();

		bool animacionFinalizada() { return this->_animacionFinalizada; }

	private:
		bool _animacionFinalizada;

};

#endif /* ANIMACIONES_H */
