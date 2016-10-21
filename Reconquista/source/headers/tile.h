#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "globals.h"

class Graphics;

class Tile {
public:
    Tile();
    Tile(sf::Texture* tileset, sf::Vector2i size, sf::Vector2i tilesetPosition, sf::Vector2i position);
    void update(int elapsedTime);
    void draw(Graphics &Graphics);

    const inline sf::Vector2i getSize() const { return this->_size; }
    const inline sf::Vector2i getPosition() const { return this->_position; }

protected:
    sf::Sprite _tileSprite;
    sf::Texture* _tileset;
    sf::Vector2i _size;
    sf::Vector2i _tilesetPosition;
    sf::Vector2i _position;
};

#endif // TILE_H
