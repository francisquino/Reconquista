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
    /*this->_aytoHealthBarSprite = Sprite(graphics, "Iconos.png", 0, 0, 0, 0, 0, 0);
    this->_aytoCurrentHealthBar = Sprite(graphics, "Iconos.png", 0, 0, 0, 0, 0, 0);
    this->_maderaNumber = Sprite(graphics, "Iconos.png", 0, 0, 0, 0, 0, 0);
    this->_oroNumber = Sprite(graphics, "Iconos.png", 0, 0, 0, 0, 0, 0);*/
    this->_entrenarCampesino = Sprite(graphics, "content/sprites/Iconos.png", 0, 0, 32, 32, 0, 164);
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

void InfoUser::drawAyto(Graphics &graphics) {
	graphics.getWindow().setView(*graphics.getView(Info));
    this->_entrenarCampesino.draw(graphics, this->_entrenarCampesino.getX(), this->_entrenarCampesino.getY());
}
