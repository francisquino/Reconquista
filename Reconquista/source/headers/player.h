#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"
#include "globals.h"
#include "slope.h"
#include "level.h"
#include "enemy.h"

class Graphics;

class Player : public AnimatedSprite {
public:
    Player();
    Player(Graphics &graphics, sf::Vector2i spawnPoint);
    void draw(Graphics &graphics);
    void update(float elapsedTime);

    /* void moveLeft
     * Moves the player left by -dx
     */
    void moveLeft();

    /* void moveRight
     * Moves the player right by dx
     */
    void moveRight();

    /* void moveDown
     * Moves the player down by dy
     */
    void moveDown();

    /* void moveUp
     * Moves the player up by -dy
     */
    void moveUp();

    /* void stopMoving
     * Stops moving the player
     */
    void stopMoving();

    /* void lookUp
     * The player looks up
     */
    void lookUp();

    /* void stopLookingUp
     * The player stops looking up
     */
    void stopLookingUp();

    /* void lookDown
     * The player looks down
     */
    void lookDown();

    /* void stopLookingDown
     * The player stops looking down
     */
    void stopLookingDown();

    /* void jump
     * Starts jumping
     */
    void jump();

    virtual void animationDone(std::string currentAnimation);
    virtual void setupAnimations();

    void handleTileCollisions(std::vector<Rectangle> &others);
    void handleSlopeCollisions(std::vector<Slope> &others);
    void handleDoorCollisions(std::vector<Door> &others, Level &level, Graphics &graphics);
    void handleEnemyCollisions(std::vector<Enemy*> &others);


    //Hay que definir Objeto y chequear colision con cualquier objeto (enemigos, construcciones, ...)
    bool checkColisionObjetos(const Rectangle &other);


    const float getX() const;
    const float getY() const;

    const inline int getMaxHealth() const { return this->_maxHealth; }
    const inline int getCurrentHealth() { return this->_currentHealth; }

    void gainHealth(int amount);

private:
    float _dx, _dy;

    Direction _facing;

    bool _grounded;

    bool _lookingUp;
    bool _lookingDown;

    int _maxHealth;
    int _currentHealth;
};

#endif // PLAYER_H
