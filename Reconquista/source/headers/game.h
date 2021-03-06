/*
 * File:   game.h
 * Author: francis
 *
 * Created on 16 de septiembre de 2016, 10:18
 */

#ifndef GAME_H
#define	GAME_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

//#include "level.h"
#include "infouser.h"
#include "graphics.h"

class Game {
public:
    Game();
    ~Game();
private:
    sf::Clock clock;    //Inicializar reloj

    void gameLoop();
    void draw(Graphics &graphics);
    void update(float elapsedTime);

    //Level _level;
    InfoUser _info;
    Graphics _graphics;
};

#endif	/* GAME_H */

