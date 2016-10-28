/*
 * objeto.h
 *
 *  Created on: 27/10/2016
 *      Author: francis
 */

#ifndef OBJETO_H
#define OBJETO_H

#include "animatedsprite.h"
#include "globals.h"

#include <vector>
#include <map>
#include <string>

class Graphics;

class Objeto : public AnimatedSprite {
public:
    Objeto();
    Objeto(Graphics &graphics, std::string filePath, int sourceX, int sourceY,
            int width, int height, sf::Vector2i spawnPoint, int timeToUpdate);
    virtual void update(int elapsedTime);
    virtual void draw(Graphics &graphics);

    const inline int getMaxHealth() const { return this->_maxHealth; }
    const inline int getCurrentHealth() const { return this->_currentHealth; }

protected:
    Direction _direction;

    int _maxHealth;
    int _currentHealth;
};

class Ayuntamiento : public Objeto {
public:
	Ayuntamiento();
	Ayuntamiento(Graphics &graphics, sf::Vector2i spawnPoint);
    void update(int elapsedTime);
    void draw(Graphics &graphics);

    void animationDone(std::string currentAnimation);
    void setupAnimations();

private:
    float _startingX, _startingY;

    std::vector<Objeto*> _unidades;

    std::map<std::string, int> _recursos;

};


#endif /* OBJETO_H */
