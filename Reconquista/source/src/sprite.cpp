#include "sprite.h"
#include "graphics.h"

Sprite::Sprite() {}

Sprite::Sprite(Graphics &graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height, float posX, float posY) :
_x(posX),
_y(posY)
{
    //Coordenadas del sprite en la hoja con todos los sprites.
    this->_sourceRect.left = sourceX;
    this->_sourceRect.top = sourceY;
    this->_sourceRect.width = width;
    this->_sourceRect.height = height;

    //Cargamos la imagen con todos los sprites desde un archivo
    //this->_spriteSheet = new sf::Texture;
    //this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
    //this->_spriteSheet->loadFromFile(filePath.c_str());
    this->_spriteSheet = graphics.loadImage(filePath);
    if (this->_spriteSheet == NULL) {
        printf ("\nError: Unable to load image\n");
    }

    this->_boundingBox = Rectangle(this->_x, this->_y, width * globals::SPRITE_SCALE, height * globals::SPRITE_SCALE);
}

Sprite::~Sprite() {}

void Sprite::blitSurface(sf::Texture* source, sf::IntRect sourceRectangle) {
    this->_sprite.setTexture(*source);
    this->_sprite.setTextureRect(sourceRectangle);
    this->_sprite.setScale(sf::Vector2f(globals::SPRITE_SCALE, globals::SPRITE_SCALE)); // factor relative to the current scale
}

void Sprite::draw(Graphics& graphics, int x, int y) {
	//SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * globals::SPRITE_SCALE,
	//		this->_sourceRect.h * globals::SPRITE_SCALE };
    //graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);

    this->blitSurface(this->_spriteSheet, this->_sourceRect);
    this->_sprite.setPosition(x, y);
    graphics.getWindow().draw(this->_sprite);
}

void Sprite::update() {
    this->_boundingBox = Rectangle(this->_x, this->_y,
            this->_sourceRect.width * globals::SPRITE_SCALE, this->_sourceRect.height * globals::SPRITE_SCALE);
}

const Rectangle Sprite::getBoundingBox() const {
    return this->_boundingBox;
}

//Side getCollisionSide
//Determine which side the collision happened on
const sides::Side Sprite::getCollisionSide(Rectangle &other) const {
    int amtRight, amtLeft, amtTop, amtBottom;
    amtRight = this->getBoundingBox().getRight() - other.getLeft();
    amtLeft = other.getRight() - this->getBoundingBox().getLeft();
    amtTop = other.getBottom() - this->getBoundingBox().getTop();
    amtBottom = this->getBoundingBox().getBottom() - other.getTop();

    int vals[4] = { abs(amtRight), abs(amtLeft), abs(amtTop), abs(amtBottom) };
    int lowest = vals[0];
    for (int i=0; i<4; i++) {
        if (vals[i] < lowest) {
            lowest = vals[i];
        }
    }

    return
            lowest == abs(amtRight) ? sides::RIGHT :
            lowest == abs(amtLeft) ? sides::LEFT :
            lowest == abs(amtTop) ? sides::TOP :
            lowest == abs(amtBottom) ? sides::BOTTOM :
            sides::NONE;
}

void Sprite::setSourceRectX(int value) {
    this->_sourceRect.left = value;
}

void Sprite::setSourceRectY(int value) {
    this->_sourceRect.top = value;
}

void Sprite::setSourceRectW(int value) {
    this->_sourceRect.width = value;
}

void Sprite::setSourceRectH(int value) {
    this->_sourceRect.height = value;
}
