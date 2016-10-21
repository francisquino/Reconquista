#ifndef ENEMY_H
#define ENEMY_H

#include "animatedsprite.h"
#include "globals.h"
#include "player.h"

#include <string>

class Graphics;

class Enemy : public AnimatedSprite {
public:
    Enemy();
    Enemy(Graphics &graphics, std::string filePath, int sourceX, int sourceY,
            int width, int height, sf::Vector2i spawnPoint, int timeToUpdate);
    virtual void update(int elapsedTime, Player &player);
    virtual void draw(Graphics &graphics);
    virtual void touchPlayer(Player* player) = 0;

    const inline int getMaxHealth() const { return this->_maxHealth; }
    const inline int getCurrentHealth() const { return this->_currentHealth; }

protected:
    Direction _direction;

    int _maxHealth;
    int _currentHealth;
};

class Bat : public Enemy {
public:
    Bat();
    Bat(Graphics &graphics, sf::Vector2i spawnPoint);
    void update(int elapsedTime, Player &player);
    void draw(Graphics &graphics);
    void touchPlayer(Player* player);

    void animationDone(std::string currentAnimation);
    void setupAnimations();

private:
    float _startingX, _startingY;
    bool _shouldMoveUp;
};

#endif // ENEMY_H
