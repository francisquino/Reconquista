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

#include "player.h"
#include "level.h"
//#include "hud.h"
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

    Player _player;

    Level _level;

    //HUD _hud;

    Graphics _graphics;
};

#endif	/* GAME_H */

