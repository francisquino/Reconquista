/*
 * infouser.h
 *
 *  Created on: 02/11/2016
 *      Author: francis
 */

#ifndef INFOUSER_H
#define INFOUSER_H

#include "sprite.h"

class Graphics;

class InfoUser {
	public:
		InfoUser();
		InfoUser(Graphics &graphics);
		void update(int elapsedTime);
		void draw(Graphics &graphics);

		//Dibujar iconos e informacion de Ayuntamiento
		void drawAyuntamiento(Graphics &graphics);
		Sprite* getIconoAytoEntrenarCampesino() { return &this->_aytoEntrenarCampesino; };

		//Dibujar iconos e informacion de Campesino
		void drawCampesino(Graphics &graphics);
		Sprite* getIconoCampIrA() { return &this->_campIrA; };
		Sprite* getIconoCampRecolectar() { return &this->_campRecolectar; };

	private:
		//Health sprites
		Sprite _aytoHealthBarSprite;
		Sprite _aytoCurrentHealthBar;

		//Recursos sprites
		Sprite _aytoMaderaNumber;
		Sprite _aytoOroNumber;

		//Acciones Ayuntamiento
		Sprite _aytoEntrenarCampesino;

		//Acciones Campesino
		Sprite _campRecolectar;
		Sprite _campIrA;
};



#endif /* INFOUSER_H */
