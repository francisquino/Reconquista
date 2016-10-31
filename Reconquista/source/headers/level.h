#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "globals.h"
#include "tile.h"
#include "rectangle.h"
#include "slope.h"
#include "animatedtile.h"
#include "door.h"
#include "objeto.h"

class Graphics;
//class Ayuntamiento;
class Enemy;
class Player;
struct Tileset;

class Level {
public:
    Level ();
    Level(std::string mapName, Graphics &graphics);
    ~Level();
    void update(int elapsedTime, Player &player);
    void draw(Graphics &graphics);

    std::vector<Rectangle> checkTileCollisions(const Rectangle &other);
    std::vector<Slope> checkSlopeCollisions(const Rectangle &other);
    std::vector<Door> checkDoorCollisions(const Rectangle &other);
    std::vector<Enemy*> checkEnemyCollisions(const Rectangle &other);

    const sf::Vector2i getPlayerSpawnPoint() const;

    const sf::Vector2i getMapSize() const { return this->_size; }
    const sf::Vector2i getTileSize() const { return this->_tileSize; }

    Ayuntamiento _ayuntamiento;

private:
    std::string _mapName;
    sf::Vector2i _spawnPoint;

    sf::Vector2i _size;
    sf::Vector2i _tileSize;

    sf::Texture* _backgroundTexture;

    std::vector<Tile> _tileList;
    std::vector<Tileset> _tilesets;
    std::vector<Rectangle> _collisionRects;
    std::vector<Slope> _slopes;

    std::vector<AnimatedTile> _animatedTileList;
    std::vector<AnimatedTileInfo> _animatedTileInfos;

    std::vector<Door> _doorList;

    std::vector<Enemy*> _enemies;

    /* void loadMap
     * Loads a map
     */
    void loadMap(std::string mapName, Graphics &graphics);

    sf::Vector2i getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight);
};

//Tileset structure
struct Tileset {
    sf::Texture* Texture;
    int FirstGid;
    Tileset() {
        this->FirstGid = -1;
    }
    Tileset(sf::Texture* texture, int firstGid) {
        this->Texture = texture;
        this->FirstGid = firstGid;
    }
};

#endif // LEVEL_H
