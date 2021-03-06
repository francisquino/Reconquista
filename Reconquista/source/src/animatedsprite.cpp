#include "animatedsprite.h"
#include "graphics.h"
#include "sprite.h"

/* AnimatedSprite class
 * Animates our sprites
 */

AnimatedSprite::AnimatedSprite() {}

AnimatedSprite::AnimatedSprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
            float posX, float posY, float timeToUpdate) :
                Sprite(graphics, filePath, sourceX, sourceY, width, height, posX, posY),
                _frameIndex(0),
                _timeElapsed(0),
                _timeToUpdate(timeToUpdate),
                _visible(true),
                _currentAnimationOnce(false),
                _currentAnimation("")
{}

void AnimatedSprite::addAnimation(int frames, int x, int y, std::string name, int width, int height, sf::Vector2i offset) {
    std::vector<sf::IntRect> rectangles;

    for (int i=0; i<frames; i++) {
        sf::IntRect newRect((i+x)*width, y*height, width, height);
        rectangles.push_back(newRect);
    }
    this->_animations.insert(std::pair<std::string, std::vector<sf::IntRect> >(name, rectangles));
    this->_offsets.insert(std::pair<std::string, sf::Vector2i>(name, offset));
}

void AnimatedSprite::resetAnimations() {
    this->_animations.clear();
    this->_offsets.clear();
}

void AnimatedSprite::playAnimation(std::string animation, bool once) {
    this->_currentAnimationOnce = once;
    if (this->_currentAnimation != animation) {
            this->_currentAnimation = animation;
            this->_frameIndex = 0;
    }
}

void AnimatedSprite::setVisible(bool visible) {
    this->_visible = visible;
}

void AnimatedSprite::stopAnimation() {
    this->_frameIndex = 0;
    this->animationDone(this->_currentAnimation);
}

void AnimatedSprite::update(int elapsedTime) {
    Sprite::update();

    this->_timeElapsed += elapsedTime;
    if (this->_timeElapsed > this->_timeToUpdate) {
        this->_timeElapsed -= this->_timeToUpdate;
        if (this->_frameIndex < this->_animations[this->_currentAnimation].size() - 1) {
            this->_frameIndex++;
        }
        else {
            if (this->_currentAnimationOnce == true) {
                this->setVisible(false);
            }
            this->stopAnimation();
        }
    }
}

void AnimatedSprite::draw(Graphics& graphics, int x, int y) {
    if (this->_visible) {
        /*		SDL_Rect destinationRectangle;
		destinationRectangle.x = x + this->_offsets[this->_currentAnimation].x;
		destinationRectangle.y = y + this->_offsets[this->_currentAnimation].y;
		destinationRectangle.w = this->_sourceRect.w * globals::SPRITE_SCALE;
		destinationRectangle.h = this->_sourceRect.h * globals::SPRITE_SCALE;

		SDL_Rect sourceRect = this->_animations[this->_currentAnimation][this->_frameIndex];
        graphics.blitSurface(this->_spriteSheet, &sourceRect, &destinationRectangle);*/
        int posX, posY;

        posX = x + this->_offsets[this->_currentAnimation].x;
        posY = y + this->_offsets[this->_currentAnimation].y;

        sf::IntRect sourceRect = this->_animations[this->_currentAnimation][this->_frameIndex];
        this->blitSurface(this->_spriteSheet, sourceRect);
        this->_sprite.setPosition(posX, posY);
        graphics.getWindow().draw(this->_sprite);
    }
}

//void AnimatedSprite::setupAnimations() {
//    this->addAnimation(3, 0, 0, "RunLeft", 16, 16, sf::Vector2i(0,0));
//    this->addAnimation(3, 0, 16, "RunRight", 16, 16, sf::Vector2i(0,0));
//}
