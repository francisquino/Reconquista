#ifndef GLOBALS_H
#define GLOBALS_H

namespace globals {
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    const float SPRITE_SCALE = 2.0f;
}

namespace sides {
    enum Side {
        TOP,
        BOTTOM,
        LEFT,
        RIGHT,
        NONE
    };

    inline Side getOppositeSide(Side side) {
        return
                side == TOP ? BOTTOM :
                side == BOTTOM ? TOP :
                side == LEFT ? RIGHT :
                side == RIGHT ? LEFT :
                NONE;
    }
}

enum Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

/* struct Vector2 {}
    Usaremos la estructura definida en SFML.
*/

#endif // GLOBALS_H
