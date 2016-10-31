#ifndef GLOBALS_H
#define GLOBALS_H

namespace globals {
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    const float SPRITE_SCALE = 2.0f;

    //Coordenadas de vistas
    const int vistaJuegoX1 = 161;
    const int vistaJuegoX2 = 640;
    const int vistaJuegoY1 = 0;
    const int vistaJuegoY2 = 480;

    const int vistaMinimapaX1 = 0;
    const int vistaMinimapaX2 = 160;
    const int vistaMinimapaY1 = 0;
    const int vistaMinimapaY2 = 120;

    const int vistaInfoX1 = 0;
    const int vistaInfoX2 = 160;
    const int vistaInfoY1 = 121;
    const int vistaInfoY2 = 480;
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
