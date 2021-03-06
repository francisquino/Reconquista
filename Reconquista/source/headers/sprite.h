/*
 * File:   sprite.h
 * Author: francis
 *
 * Created on 19 de septiembre de 2016, 13:55
 */

#ifndef SPRITE_H
#define	SPRITE_H

#include <SFML/Graphics.hpp>

#include <string>

#include "rectangle.h"
#include "globals.h"

class Graphics;

/* Sprite class
 * Holds all information for individual sprites
 */

class Sprite {
public:
    Sprite();
    Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
            float posX, float posY);
    virtual ~Sprite();
    virtual void update();
	void blitSurface(sf::Texture* source, sf::IntRect sourceRectangle);
    void draw(Graphics &graphics, int x , int y);

    const Rectangle getBoundingBox() const;
    const sides::Side getCollisionSide(Rectangle &other) const;

    const inline float getX() const { return this->_x; }
    const inline float getY() const { return this->_y; }

    const inline void setX(int x) { this->_x = x; }
    const inline void setY(int y) { this->_y = y; }

    void setSourceRectX(int value);
    void setSourceRectY(int value);
    void setSourceRectW(int value);
    void setSourceRectH(int value);

protected:
    //SDL_Rect _sourceRect;
    sf::IntRect _sourceRect;
    sf::Texture* _spriteSheet;
    sf::Sprite _sprite;

    Rectangle _boundingBox;

    float _x, _y;
private:
};

#endif	/* SPRITE_H */

