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

#include <cmath>
#include <vector>
#include <map>
#include <string>

class Graphics;

namespace tipoObjeto {
    enum TipoObjeto {
        Ayuntamiento,
        Mina,
        Bosque,
        Campesino,
        Caballero,
        NoDefinido
    };
}

class Objeto : public AnimatedSprite {
public:
    Objeto();
    Objeto(Graphics &graphics, tipoObjeto::TipoObjeto tipo, std::string filePath, int sourceX, int sourceY,
            int width, int height, sf::Vector2i spawnPoint, int maxHealth, int timeToUpdate);

    const inline tipoObjeto::TipoObjeto getTipo() const { return this->_tipo; }

    virtual void update(int elapsedTime);
    virtual void draw(Graphics &graphics);

    virtual void animationDone(std::string currentAnimation);
    virtual void setupAnimations();

    /* void moveLeft
     * Moves the object left by -dx
     */
    void moveLeft();

    /* void moveRight
     * Moves the object right by dx
     */
    void moveRight();

    /* void moveDown
     * Moves the object down by dy
     */
    void moveDown();

    /* void moveUp
     * Moves the object up by -dy
     */
    void moveUp();

    /* void stopMoving
     * Stops moving the object
     */
    void stopMoving();

    void setDestino(const float destinoX, const float destinoY);
    const float getDestinoX() const { return this->_destinoX; }
    const float getDestinoY() const { return this->_destinoY; }

    bool destinoAlcanzado() const { return this->_destinoAlcanzado; }
    void setDestinoAlcanzado(const bool destino);

    bool checkColision(const Rectangle &other);

    const inline int getMaxHealth() const { return this->_maxHealth; }
    const inline int getCurrentHealth() const { return this->_currentHealth; }

    void gainHealth(int amount);

    void modificarRecurso(std::string recurso, int cantidad);

    int getRecurso(std::string recurso) { return this->_recursos[recurso];}

    void sumarUnidad(Objeto unidad);


    std::vector<Objeto> _unidades;

    std::map<std::string, int> _recursos;


protected:
    tipoObjeto::TipoObjeto _tipo;

    float _dx, _dy, _destinoX, _destinoY;

    Direction _facing;

    bool _destinoAlcanzado;

    int _maxHealth;
    int _currentHealth;

}; //class Objeto

class Ayuntamiento : public Objeto {
public:
	Ayuntamiento();
	Ayuntamiento(Graphics &graphics, sf::Vector2i spawnPoint);
    void update(int elapsedTime);
    void draw(Graphics &graphics);

    void animationDone(std::string currentAnimation);
    void setupAnimations();

}; //class Ayuntamiento

class Campesino : public Objeto {
public:
    Campesino();
    Campesino(Graphics &graphics, sf::Vector2i spawnPoint);
    void update(int elapsedTime);
    void draw(Graphics &graphics);

    void animationDone(std::string currentAnimation);
    void setupAnimations();

    /*
    void handleTileCollisions(std::vector<Rectangle> &others);
    void handleSlopeCollisions(std::vector<Slope> &others);
    void handleDoorCollisions(std::vector<Door> &others, Level &level, Graphics &graphics);
    void handleEnemyCollisions(std::vector<Enemy*> &others);
    */

private:

}; //class Campesino


#endif /* OBJETO_H */
