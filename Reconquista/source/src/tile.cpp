#include "tile.h"
#include "graphics.h"

#include <SFML/System.hpp>

Tile::Tile() {}

Tile::Tile(sf::Texture* tileset, sf::Vector2i size, sf::Vector2i tilesetPosition, sf::Vector2i position) :
        _tileset(tileset),
        _size(size),
        _tilesetPosition(tilesetPosition),
        _position(sf::Vector2i(position.x*globals::SPRITE_SCALE, position.y*globals::SPRITE_SCALE))
{}

void Tile::update(int elapsedTime) {}

void Tile::draw(Graphics &graphics) {
    sf::IntRect sourceRect(this->_tilesetPosition.x, this->_tilesetPosition.y, this->_size.x, this->_size.y);

    this->_tileSprite.setTexture(*this->_tileset);
    this->_tileSprite.setTextureRect(sourceRect);
    this->_tileSprite.setScale(sf::Vector2f(globals::SPRITE_SCALE, globals::SPRITE_SCALE)); // factor relative to the current scale

    this->_tileSprite.setPosition(this->_position.x, this->_position.y);
    graphics.getWindow().draw(this->_tileSprite);
}
