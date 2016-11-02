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
    void drawAyto(Graphics &graphics);

    Sprite* getIconoEntrenarCampesino() { return &this->_entrenarCampesino; };

private:
    //Health sprites
    Sprite _aytoHealthBarSprite;
    Sprite _aytoCurrentHealthBar;

    //Recursos sprites
    Sprite _maderaNumber;
    Sprite _oroNumber;

    //Acciones
    Sprite _entrenarCampesino;
};



#endif /* INFOUSER_H */
