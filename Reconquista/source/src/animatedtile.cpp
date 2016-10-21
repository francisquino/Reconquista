#include "animatedtile.h"
#include "graphics.h"

AnimatedTile::AnimatedTile(std::vector<sf::Vector2i> tilesetPositions, int duration,
        sf::Texture* tileset, sf::Vector2i size, sf::Vector2i position) :
        Tile(tileset, size, tilesetPositions.at(0), position),
        _tilesetPositions(tilesetPositions),
        _duration(duration),
        _tileToDraw(0)
{}

void AnimatedTile::update(int elapsedTime) {
    //Timer code
    if (this->_amountOfTime <= 0) {
        if (this->_tileToDraw == this->_tilesetPositions.size() -1) {
            this->_tileToDraw = 0;
        }
        else {
            this->_tileToDraw++;
        }
        this->_amountOfTime = this->_duration;
    }
    else {
        this->_amountOfTime -= elapsedTime;
    }

    Tile::update(elapsedTime);
}

void AnimatedTile::draw(Graphics &graphics) {
    sf::IntRect sourceRect(this->_tilesetPositions.at(this->_tileToDraw).x, this->_tilesetPositions.at(this->_tileToDraw).y, this->_size.x, this->_size.y);
    this->_animatedTileSprite.setPosition(this->_position.x, this->_position.y);
    this->_animatedTileSprite.setTexture(*this->_tileset);
    this->_animatedTileSprite.setTextureRect(sourceRect);
    this->_animatedTileSprite.setScale(sf::Vector2f(globals::SPRITE_SCALE, globals::SPRITE_SCALE)); // factor relative to the current scale

    graphics.getWindow().draw(this->_animatedTileSprite);
}
