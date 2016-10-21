#ifndef ANIMATEDTILE_H
#define ANIMATEDTILE_H

#include <vector>

#include "tile.h"
#include "globals.h"

class AnimatedTile : public Tile {
public:
    AnimatedTile(std::vector<sf::Vector2i> tilesetPositions, int duration, sf::Texture* tileset, sf::Vector2i size, sf::Vector2i position);
    void update(int elapsedTime);
    void draw(Graphics &graphics);
protected:
    int _amountOfTime = 0;
    bool _notDone = false;
private:
    sf::Sprite _animatedTileSprite;
    std::vector<sf::Vector2i> _tilesetPositions;
    int _tileToDraw;
    int _duration;
};

struct AnimatedTileInfo {
public:
    int TilesetFirstGid;
    int StartTileId;
    std::vector<int> TileIds;
    int Duration;
};

#endif // ANIMATEDTILE_H
