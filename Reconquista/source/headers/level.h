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
#include "nodo.h"

class Graphics;
class Ayuntamiento;
//class Enemy;
//class Player;
struct Tileset;



class Level {
public:
    Level ();
    Level(std::string mapName, Graphics &graphics);
    ~Level();
    void update(int elapsedTime);
    void draw(Graphics &graphics);

    void deseleccionarObjetos(Objeto* seleccionado);

    std::vector<Rectangle> checkTileCollisions(const Rectangle &other);
    std::vector<Slope> checkSlopeCollisions(const Rectangle &other);
    std::vector<Door> checkDoorCollisions(const Rectangle &other);
    //std::vector<Enemy*> checkEnemyCollisions(const Rectangle &other);

    //const sf::Vector2i getPlayerSpawnPoint() const;

    const sf::Vector2i getMapSize() const { return this->_size; }
    const sf::Vector2i getTileSize() const { return this->_tileSize; }

	// Algoritmo A-*
	// Devuelve una ruta formada por una cadena de los digitos de direccion
	std::string pathFind( const int & xStart, const int & yStart, const int & xFinish, const int & yFinish );

	//Transformar la ruta devuelta por pathFind a coordenadas
	std::vector<sf::Vector2i> rutaACoordenadas (std::string ruta, sf::Vector2i origen);

	//Simplificar la ruta de coordenadas, eliminando los desplazamientos irrelevantes
	std::vector<sf::Vector2i> simplificarRutaCoord (std::vector<sf::Vector2i> ruta,  sf::Vector2i origen);

    //Pasar de las coordenadas del terreno a las del mapa de obtencion de rutas
    sf::Vector2i coordAMapa(int x, int y);
    //Pasar de las coordenadas del mapa de obtencion de rutas a las del terreno
    sf::Vector2i mapaACoord(int x, int y);

    Ayuntamiento* _ayuntamiento;
    std::vector<Objeto*> _recursos;

    //Mapa para busqueda de rutas con A*
    int** mapa;

private:
    //Datos especificos de la busqueda de rutas con A*
    int nMapaBuscarRutas; // Tamaño horizontal del mapa
    int mMapaBuscarRutas; // Tamaño vertical del mapa

    //Datos especificos de la busqueda de rutas con A*


    std::string _mapName;
    //sf::Vector2i _spawnPoint;

    sf::Vector2i _size;
    sf::Vector2i _tileSize;

    //sf::Texture* _backgroundTexture;

    std::vector<Tile> _tileList;
    std::vector<Tileset> _tilesets;
    std::vector<Rectangle> _collisionRects;
    //std::vector<Slope> _slopes;

    std::vector<AnimatedTile> _animatedTileList;
    std::vector<AnimatedTileInfo> _animatedTileInfos;

    //std::vector<Door> _doorList;

    //std::vector<Enemy*> _enemies;


    /* void loadMap
     * Loads a map
     */
    void loadMap(std::string mapName, Graphics &graphics);

    sf::Vector2i getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight);

    //Rellenar el mapa para obtener rutas a partir de la posicion del tile y su id
    void rellenarMapa(int **mapa, int x, int y, int gid);

};

// Determinar prioridad (en la cola prioridad)
bool operator<(const Nodo & a, const Nodo & b);


//Tileset structure
struct Tileset {
    sf::Texture* Texture;
    int FirstGid;
    Tileset() {
    	this->Texture = NULL;
        this->FirstGid = -1;
    }
    Tileset(sf::Texture* texture, int firstGid) {
        this->Texture = texture;
        this->FirstGid = firstGid;
    }
};

#endif // LEVEL_H
