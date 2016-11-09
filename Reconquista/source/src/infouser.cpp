/*
 * infouser.cpp
 *
 *  Created on: 02/11/2016
 *      Author: francis
 */

#include "infouser.h"

#include "graphics.h"

InfoUser::InfoUser() {}

InfoUser::InfoUser(Graphics &graphics) {
	//Iconos Ayuntamiento
    /*this->_aytoHealthBarSprite = Sprite(graphics, "Iconos.png", 0, 0, 0, 0, 0, 0);
    this->_aytoCurrentHealthBar = Sprite(graphics, "Iconos.png", 0, 0, 0, 0, 0, 0);
    this->_aytoMaderaNumber = Sprite(graphics, "Iconos.png", 0, 0, 0, 0, 0, 0);
    this->_aytoOroNumber = Sprite(graphics, "Iconos.png", 0, 0, 0, 0, 0, 0);*/
    this->_aytoEntrenarCampesino = Sprite(graphics, "content/sprites/Iconos.png", 0, 0, 32, 32, 0, 164);

    //Iconos Campesino
    this->_campRecolectar = Sprite(graphics, "content/sprites/Iconos.png", 0, 32, 32, 32, 0, 164);
}

void InfoUser::update(int elapsedTime) {
	/*
    this->_healthNumber1.setSourceRectX(8 * this->_player.getCurrentHealth());

    //Calculate the width of the health bar
    //100% = 39 px
    float num = (float)this->_player.getCurrentHealth() / this->_player.getMaxHealth();
    this->_currentHealthBar.setSourceRectW(std::floor(num * 39));
    */
}

void InfoUser::draw(Graphics &graphics) {

}

void InfoUser::drawAyuntamiento(Graphics &graphics) {
	graphics.getWindow().setView(*graphics.getView(Info));
    this->_aytoEntrenarCampesino.draw(graphics, this->_aytoEntrenarCampesino.getX(), this->_aytoEntrenarCampesino.getY());
}

void InfoUser::drawCampesino(Graphics &graphics) {
	graphics.getWindow().setView(*graphics.getView(Info));
    this->_campRecolectar.draw(graphics, this->_campRecolectar.getX(), this->_campRecolectar.getY());
}
