/*
 * objeto.h
 *
 *  Created on: 27/10/2016
 *      Author: francis
 */

#ifndef OBJETO_H
#define OBJETO_H

#include <cmath>
#include <vector>
#include <map>
#include <string>

#include <SFML/Graphics.hpp>

#include "animatedsprite.h"
#include "globals.h"

class Graphics;

//Los distintos entes
namespace tipoObjeto {
    enum TipoObjeto {
        Ayuntamiento,
        Barracones,
        Mina,
        Bosque,
        Campesino,
        Caballero,
        NoDefinido
    };
}

//Diferentes recursos materiales
namespace tipoMaterial {
	enum TipoMaterial {
		Oro,
		Madera
	};
}

class Objeto : public AnimatedSprite {
public:
    Objeto();
    Objeto(Graphics &graphics, tipoObjeto::TipoObjeto tipo, std::string filePath, int sourceX, int sourceY,
            int width, int height, sf::Vector2i spawnPoint, int maxHealth, int timeToUpdate);

    virtual ~Objeto() {}

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

    bool getSeleccionado() const { return this->_seleccionado; }
    void setSeleccionado(const bool seleccionado);

    bool checkColision(const Rectangle &other);

    const inline int getMaxHealth() const { return this->_maxHealth; }
    const inline int getCurrentHealth() const { return this->_currentHealth; }

    void gainHealth(int amount);

    void modificarCantidadMaterial(tipoMaterial::TipoMaterial material, int cantidad);
    int getCantidadMaterial(tipoMaterial::TipoMaterial material) { return this->_materiales[material];}

    void sumarUnidad(Objeto* unidad);


    std::vector<Objeto*> _unidades;


protected:
    tipoObjeto::TipoObjeto _tipo;

    float _dx, _dy, _destinoX, _destinoY;

    Direction _facing;

    bool _destinoAlcanzado;
    bool _seleccionado;

    int _maxHealth;
    int _currentHealth;

    std::map<tipoMaterial::TipoMaterial, int> _materiales;
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

class Barracones : public Objeto {
public:
	Barracones();
	Barracones(Graphics &graphics, sf::Vector2i spawnPoint);
    void update(int elapsedTime);
    void draw(Graphics &graphics);

    void animationDone(std::string currentAnimation);
    void setupAnimations();

}; //class Barracones

class Mina : public Objeto {
public:
	Mina();
	Mina(Graphics &graphics, sf::Vector2i spawnPoint, int cantidad);
    void update(int elapsedTime);
    void draw(Graphics &graphics);

    void animationDone(std::string currentAnimation);
    void setupAnimations();

}; //class Mina

class Bosque : public Objeto {
public:
	Bosque();
	Bosque(Graphics &graphics, sf::Vector2i spawnPoint, int cantidad);
    void update(int elapsedTime);
    void draw(Graphics &graphics);

    void animationDone(std::string currentAnimation);
    void setupAnimations();

}; //class Bosque

class Campesino : public Objeto {
public:
    Campesino();
    Campesino(Graphics &graphics, sf::Vector2i spawnPoint);
    void update(int elapsedTime);
    void draw(Graphics &graphics);

    void animationDone(std::string currentAnimation);
    void setupAnimations();

    void setRecolectando(const bool recolectando) { this->_recolectando = recolectando; }
    void setCantidadMaterialRecoleccion(const int cantidad, const tipoMaterial::TipoMaterial material) { this->_cantidadRecolectada += cantidad; this->_materialRecolectado = material; }
    void setDestinoRecoleccion(Objeto* recurso) { this->_destinoRecoleccion = recurso; }

    /*
    void handleTileCollisions(std::vector<Rectangle> &others);
    void handleSlopeCollisions(std::vector<Slope> &others);
    void handleDoorCollisions(std::vector<Door> &others, Level &level, Graphics &graphics);
    void handleEnemyCollisions(std::vector<Enemy*> &others);
    */

private:
    bool _recolectando;

    int _cantidadRecolectada;
    tipoMaterial::TipoMaterial _materialRecolectado;

    Objeto* _destinoRecoleccion;

}; //class Campesino


#endif /* OBJETO_H */
