#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <SFML/System/Vector2.hpp>

#include <map>
#include <string>

#include "sprite.h"
#include "globals.h"

class Graphics;

/* AnimatedSprite class
 * Holds logic for animating sprites
 */

class AnimatedSprite : public Sprite {
public:
    AnimatedSprite();
    AnimatedSprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
            float posX, float posY, float timeToUpdate);

    /* void playAnimation
     * Plays the animation provided if it's not already playing
     */
    void playAnimation(std::string animation, bool once = false);

    /* void update
     * Updates the animated sprite (timer)
     */
    void update(int elapsedTime);

    /* void draw
     * Draws the sprite to the screen
     */
    void draw(Graphics &graphics, int x, int y);

protected:
    double _timeToUpdate;
    bool _currentAnimationOnce;
    std::string _currentAnimation;

    /* void addAnimation
     * Adds an animation to the map of animations for the sprite
     * x: posicion del sprite en el fichero .png, empezando desde 0, coordenada x
     * y: posicion del sprite en el fichero .png, empezando desde 0, coordenada y
     * por ejemplo: x=2, y=3. Sprite en la tercera columna de la cuarta fila.
     */
    void addAnimation(int frames, int x, int y, std::string name, int width, int height, sf::Vector2i offset);

    /* void resetAnimations
     * Reset all animations asociated with this sprite
     */
    void resetAnimations();

    /* void stopAnimation
     * Stops the current animation
     */
    void stopAnimation();

    /* void setVisible
     * Changes the visibility of the animated sprite
     */
    void setVisible(bool visible);

    /* void animationDone
     * Logic that happens when an animation ends
     */
    virtual void animationDone(std::string currentAnimation) = 0;

    /* void setupAnimations
     * A required function that sets up all animations for a sprite
     */
    virtual void setupAnimations() = 0;

private:
    std::map<std::string, std::vector<sf::IntRect> > _animations;
    std::map<std::string, sf::Vector2i> _offsets;

    int _frameIndex;
    //sf::Int32 _timeElapsed;
    double _timeElapsed;
    bool _visible;
};

#endif // ANIMATED_SPRITE_H
