/*
 * recursos.h
 *
 *  Created on: 20/10/2016
 *      Author: francis
 */

#ifndef RECURSOS_H
#define RECURSOS_H

#include "animatedsprite.h"
#include "globals.h"
#include "player.h"

#include <string>

class Graphics;

class Recurso : public AnimatedSprite {
public:
    Recurso();
    Recurso(Graphics &graphics, std::string filePath, int sourceX, int sourceY,
            int width, int height, sf::Vector2i spawnPoint, int timeToUpdate);
    virtual void update(int elapsedTime);
    virtual void draw(Graphics &graphics);
    virtual void touchPlayer(Player* player) = 0;

    const inline int getMaxProducto() const { return this->_maxProducto; }
    const inline int getCurrentProducto() const { return this->_currentProducto; }

protected:
    int _maxProducto;
    int _currentProducto;
};



#endif /* RECURSOS_H */
