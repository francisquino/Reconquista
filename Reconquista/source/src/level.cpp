#include "level.h"
#include "graphics.h"
#include "globals.h"
#include "utils.h"
#include "player.h"
#include "enemy.h"
//#include "ayuntamiento.h"

#include "tinyxml2.h"

#include <SFML/Graphics.hpp>

#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

using namespace tinyxml2;

Level::Level() {}

Level::Level(std::string mapName, Graphics &graphics) :
        _mapName(mapName),
        _size(sf::Vector2i(0,0))
{
    this->loadMap(mapName, graphics);
}

Level::~Level() {
}

void Level::loadMap(std::string mapName, Graphics &graphics) {
//    //TEMPORARY CODE TO LOAD THE BACKGROUND
//    this->_backgroundTexture = graphics.loadImage("bkBlue.png");
//    this->_size = sf::Vector2i(1280, 960);

    //Parse the .tmx file
    XMLDocument doc;
    std::stringstream ss;

    //En CodeBlocks están todos los ficheros en el mismo directorio
    //ss << mapName << ".tmx"; //Pass Map 1, we get Map 1.tmx
    ss << "content/maps/" << mapName << ".tmx"; //PassMap 1, we get content/maps/Map 1.tmx
    doc.LoadFile(ss.str().c_str());
    //printf("%s\n", ss.str().c_str());

    XMLElement* mapNode = doc.FirstChildElement("map");

    //Get the width and the height of the whole map and store it in _size
    int width, height;
    mapNode->QueryIntAttribute("width", &width);
    mapNode->QueryIntAttribute("height", &height);
    this->_size = sf::Vector2i(width, height);

    //Get the width and the height of the tiles and store it in _tileSize
    int tileWidth, tileHeight;
    mapNode->QueryIntAttribute("tilewidth", &tileWidth);
    mapNode->QueryIntAttribute("tileheight", &tileHeight);
    this->_tileSize = sf::Vector2i(tileWidth, tileHeight);

    //Loading the tilesets
    XMLElement* pTileset = mapNode->FirstChildElement("tileset");
    if(pTileset != NULL) {
        while (pTileset) {
            int firstgid;
            const char* source = pTileset->FirstChildElement("image")->Attribute("source");
            std::stringstream ss;

            ss << source;
            pTileset->QueryAttribute("firstgid", &firstgid);
            sf::Texture* tex = graphics.loadImage(ss.str());
            this->_tilesets.push_back(Tileset(tex, firstgid));

            //Get all of the animations for that tileset
            XMLElement* pTileA = pTileset->FirstChildElement("tile");
            if (pTileA != NULL) {
                while (pTileA) {
                    AnimatedTileInfo ati;
                    ati.StartTileId = pTileA->IntAttribute("id") + firstgid;
                    ati.TilesetFirstGid = firstgid;
                    XMLElement* pAnimation = pTileA->FirstChildElement("animation");
                    if (pAnimation != NULL) {
                        while (pAnimation) {
                            XMLElement* pFrame = pAnimation->FirstChildElement("frame");
                            if (pFrame != NULL) {
                                while (pFrame) {
                                    ati.TileIds.push_back(pFrame->IntAttribute("tileid") + firstgid);
                                    ati.Duration = pFrame->IntAttribute("duration");
                                    pFrame = pFrame->NextSiblingElement("frame");
                                }
                            }
                            pAnimation = pAnimation->NextSiblingElement("animation");
                        }
                    }
                    //this->_animatedTilesInfos.push_back(ati);
                    pTileA = pTileA->NextSiblingElement("tile");
                }
            }
            pTileset = pTileset->NextSiblingElement("tileset");
        }
    }

    //Loading the layers
    XMLElement* pLayer = mapNode->FirstChildElement("layer");
    if (pLayer != NULL) {
        while (pLayer) {
            //Loading the data element
            XMLElement* pData = pLayer->FirstChildElement("data");
            if (pData != NULL) {
                while (pData) {
                    //Loading the tile element
                	//Formato XML
					XMLElement* pTile = pData->FirstChildElement("tile");
					if (pTile != NULL) {
						int tileCounter = 0;
						while (pTile) {
							//Build each individual tile here
							//If gid is 0, no tile should be drawn. Continue loop
							if (pTile->IntAttribute("gid") == 0) {
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}

							//Get the tileset for this specific gid
							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							int closest = 0;
							for (int i = 0; i < this->_tilesets.size(); i++) {
								if (this->_tilesets[i].FirstGid <= gid) {
									if (this->_tilesets[i].FirstGid > closest) {
										closest = this->_tilesets[i].FirstGid;
										tls = this->_tilesets.at(i);
									}
								}
							}

							if (tls.FirstGid == -1) {
								//No tileset was found for this gid
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}

							//Get the position of the tile in the level
							int xx = 0;
							int yy = 0;
							xx = tileCounter % width;
							xx *= tileWidth;
							yy += tileHeight * (tileCounter / width);
							sf::Vector2i finalTilePosition = sf::Vector2i(xx, yy);

							//Calculate the position of the tile in the tileset
							sf::Vector2i finalTilesetPosition = this->getTilesetPosition(tls, gid, tileWidth, tileHeight);

							//Build the actual tile and add it to the level's tile list
							bool isAnimatedTile = false;
							AnimatedTileInfo ati;
							for (int i = 0; i < this->_animatedTileInfos.size(); i++) {
								if (this->_animatedTileInfos.at(i).StartTileId == gid) {
									ati = this->_animatedTileInfos.at(i);
									isAnimatedTile = true;
									break;
								}
							}
							if (isAnimatedTile == true) {
								std::vector<sf::Vector2i> tilesetPositions;
								for (int i = 0; i < ati.TileIds.size(); i++) {
									tilesetPositions.push_back(this->getTilesetPosition(tls, ati.TileIds.at(i),
											tileWidth, tileHeight));
								}
								AnimatedTile tile(tilesetPositions, ati.Duration,
										tls.Texture, sf::Vector2i(tileWidth, tileHeight), finalTilePosition);
								this->_animatedTileList.push_back(tile);
							}
							else {
								Tile tile(tls.Texture, sf::Vector2i(tileWidth, tileHeight),
										finalTilesetPosition, finalTilePosition);
								this->_tileList.push_back(tile);
							}

	                        tileCounter++;
	                        pTile = pTile->NextSiblingElement("tile"); //El fichero tiene una estrucutra diferente
	                    }
					}

					/*
                    //Formato CSV

                    //XMLElement* pTile = pData->GetText();
                    std::stringstream csvss(pData->FirstChild()->Value());
                    //printf("csvss %s\n", csvss.str().c_str());

                    int tileCounter = 0;
                    unsigned int gid = 0;
                    while (csvss >> gid)
                    {
                        //printf ("gid %u\n", gid);

                        if (csvss.peek() == ',' || csvss.peek() == '\n')
                        {
                            csvss.ignore();
                        }

                        //Build each individual tile here
                        //If gid is 0, no tile should be drawn. Continue loop
                        if (gid == 0) {
                            tileCounter++;
                            //printf("Gid 0. Continue. tileCounter %i\n", tileCounter);
                            continue;
                        }

                        //printf ("**** gid %u\n", gid);
                        //Get the tileset for this specific gid
                        Tileset tls;
                        int closest = 0;
                        for (int i=0; i<this->_tilesets.size(); i++) {
                            if (this->_tilesets[i].FirstGid <= gid) {
                                if(this->_tilesets[i].FirstGid > closest) {
                                    closest = this->_tilesets[i].FirstGid;
                                    tls = this->_tilesets.at(i);
                                }
                            }
                        }

                        if (tls.FirstGid == -1) {
                            //No tileset was found for this gid
                            tileCounter++;
                            continue;
                        }

                        //Get the position of the tile in the level
                        int xx = 0;
                        int yy = 0;

                        xx = tileCounter % width;
                        xx *= tileWidth;
                        yy += tileHeight * (tileCounter / width);
                        sf::Vector2i finalTilePosition (xx, yy);
                        //printf("finalTilePosition %i, %i\n", xx, yy);

                        //Calculate the position of the tile in the tileset
                        sf::Vector2i finalTilesetPosition = this->getTilesetPosition(tls, gid, tileWidth, tileHeight);

                        //Build the actual tile and add it to the level's tile list
                        bool isAnimatedTile = false;
                        AnimatedTileInfo ati;
                        for (int i=0; i<this->_animatedTilesInfos.size(); i++) {
                            if (this->_animatedTilesInfos.at(i).StartTileId == gid) {
                                ati = this->_animatedTilesInfos.at(i);
                                isAnimatedTile = true;
                                break;
                            }
                        }
                        if (isAnimatedTile) {
                            std::vector<sf::Vector2i> tilesetPositions;
                            for (int i=0; i<ati.TileIds.size(); i++) {
                                tilesetPositions.push_back(this->getTilesetPosition(tls, ati.TileIds.at(i),
                                        tileWidth, tileHeight));
                            }
                            AnimatedTile tile(tilesetPositions, ati.Duration,
                                    tls.Texture, sf::Vector2i(tileWidth, tileHeight), finalTilePosition);
                            this->_animatedTileList.push_back(tile);
                        }
                        else {
                            Tile tile(tls.Texture, sf::Vector2i(tileWidth, tileHeight),
                                    finalTilesetPosition, finalTilePosition);
                            this->_tileList.push_back(tile);
                        }
                        tileCounter++;
                        //pTile = pTile->NextSiblingElement("tile"); //El fichero tiene una estrucutra diferente
                    }
                    */ //Fin formato CSV

                    pData = pData->NextSiblingElement("data");
                }
            }

            pLayer = pLayer->NextSiblingElement("layer");
        }
    }

    //Parse out the collisions
    XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
    if (pObjectGroup != NULL){
        while(pObjectGroup) {
            const char* name = pObjectGroup->Attribute("name");
            std::stringstream ss;
            ss << name;
            if (ss.str() == "collisions") {
                    XMLElement* pObject = pObjectGroup->FirstChildElement("object");
                    if (pObject != NULL){
                        while(pObject) {
                            float x, y, width, height;
                            x = pObject->FloatAttribute("x");
                            y = pObject->FloatAttribute("y");
                            width = pObject->FloatAttribute("width");
                            height = pObject->FloatAttribute("height");
                            this->_collisionRects.push_back(Rectangle(
                                std::ceil(x) * globals::SPRITE_SCALE,
                                std::ceil(y) * globals::SPRITE_SCALE,
                                std::ceil(width) * globals::SPRITE_SCALE,
                                std::ceil(height) * globals::SPRITE_SCALE
                            ));

                            pObject = pObject->NextSiblingElement("object");
                        }
                    }
            }
            //Other objectgroups go here with an else if (ss.str() == "whatever")
            else if (ss.str() == "slopes") {
                XMLElement* pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL){
                    while(pObject) {
                        std::vector<sf::Vector2i> points;
                        sf::Vector2i p1;
                        p1 = sf::Vector2i(std::ceil(pObject->FloatAttribute("x")), std::ceil(pObject->FloatAttribute("y")));

                        XMLElement* pPolyLine = pObject->FirstChildElement("polyline");
                        if (pPolyLine != NULL) {
                            std::vector<std::string> pairs;
                            const char* pointString = pPolyLine->Attribute("points");

                            std::stringstream ss;
                            ss << pointString;
                            Utils::split(ss.str(), pairs, ' ');
                            //Now we have each of the pairs. Loop through the list of pairs
                            //and split them into Vector2s and then store them in our points vector
                            for (int i=0; i<pairs.size(); i++) {
                                std::vector<std::string> ps;
                                Utils::split(pairs.at(i), ps, ',');
                                //points.push_back(sf::Vector2i(std::stoi(ps.at(0)), std::stoi(ps.at(1))));
                                points.push_back(sf::Vector2i(std::atoi(ps.at(0).c_str()), std::atoi(ps.at(1).c_str())));
                            }

                        }

                        for(int i=0; i<points.size(); i+=2) {
                            /*this->_slopes.push_back(Slope(
                                    sf::Vector2f((p1.x + points.at(i<2 ? i : i-1).x) * globals::SPRITE_SCALE,
                                                 (p1.y + points.at(i<2 ? i : i-1).y) * globals::SPRITE_SCALE),
                                    sf::Vector2f((p1.x + points.at(i<2 ? i+1 : i).x) * globals::SPRITE_SCALE,
                                                 (p1.y + points.at(i<2 ? i+1 : i).y) * globals::SPRITE_SCALE)
                            ));*/
                        }

                        pObject = pObject->NextSiblingElement("object");
                    }
                }
            }
            else if (ss.str() == "spawn points") {
                XMLElement* pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL){
                    while(pObject) {
                        float x = pObject->FloatAttribute("x");
                        float y = pObject->FloatAttribute("y");
                        const char* name = pObject->Attribute("name");
                        std::stringstream ss;
                        ss << name;
                        if(ss.str() == "player") {
                            this->_spawnPoint = sf::Vector2i(std::ceil(x) * globals::SPRITE_SCALE,
                                    std::ceil(y) * globals::SPRITE_SCALE);
                        }
                        if(ss.str() == "ayuntamiento") {
                            this->_ayuntamiento = Ayuntamiento(graphics, sf::Vector2i(std::floor(x) * globals::SPRITE_SCALE,
                                    std::floor(y) * globals::SPRITE_SCALE));
                        }

                        pObject = pObject->NextSiblingElement("object");
                    }
                }
            }

            else if (ss.str() == "doors") {
                XMLElement* pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL){
                    while(pObject) {
                        float x = pObject->FloatAttribute("x");
                        float y = pObject->FloatAttribute("y");
                        float w = pObject->FloatAttribute("width");
                        float h = pObject->FloatAttribute("height");
                        Rectangle rect = Rectangle(x, y, w, h);

                        XMLElement* pProperties = pObject->FirstChildElement("properties");
                        if (pProperties != NULL){
                            while(pProperties) {
                                XMLElement* pProperty = pProperties->FirstChildElement("property");
                                if (pProperty != NULL){
                                    while(pProperty) {

                                        const char* name = pProperty->Attribute("name");
                                        std::stringstream ss;
                                        ss << name;
                                        if(ss.str() == "destination") {
                                            const char*  value = pProperty->Attribute("value");
                                            std::stringstream ss2;
                                            ss2 << value;
                                            Door door = Door(rect, ss2.str());
                                            //this->_doorList.push_back(door);
                                        }

                                        pProperty = pProperty->NextSiblingElement("property");
                                    }
                                }

                                pProperties = pProperties->NextSiblingElement("properties");
                            }
                        }

                        pObject = pObject->NextSiblingElement("object");
                    }
                }
            }

            else if (ss.str() == "enemies") {
                float x, y;
                XMLElement* pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL) {
                    while (pObject) {
                        x = pObject->FloatAttribute("x");
                        y = pObject->FloatAttribute("y");
                        const char* name = pObject->Attribute("name");
                        std::stringstream ss;
                        ss << name;
                        if (ss.str() == "bat") {
                            this->_enemies.push_back(new Bat(graphics, sf::Vector2i(std::floor(x) * globals::SPRITE_SCALE,
                                    std::floor(y) * globals::SPRITE_SCALE)));
                        }

                        pObject = pObject->NextSiblingElement("object");
                    }
                }
            }

            pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
        }
    }
}

void Level::update(int elapsedTime) {
	this->_ayuntamiento.update(elapsedTime);

    for (unsigned int i=0; i<this->_ayuntamiento._unidades.size(); i++) {
        this->_ayuntamiento._unidades.at(i).update(elapsedTime);
    }
	/*
    for (int i=0; i<this->_animatedTileList.size(); i++) {
        this->_animatedTileList.at(i).update(elapsedTime);
    }
    for (int i=0; i<this->_enemies.size(); i++) {
        this->_enemies.at(i)->update(elapsedTime, player);
    }
    */
}

void Level::draw(Graphics &graphics) {
//    //Draw the background
//    sf::IntRect sourceRect(0, 0, 64, 64);
//
//    for (int x=0; x<this->_size.x/64; x++) {
//        for(int y=0; y<this->_size.y/64; y++) {
//            sf::Sprite sprite;
//
//            sprite.setPosition(x*64*globals::SPRITE_SCALE, y*64*globals::SPRITE_SCALE);
//            sprite.setTexture(*this->_backgroundTexture);
//            sprite.setTextureRect(sourceRect);
//            sprite.setScale(sf::Vector2f(globals::SPRITE_SCALE, globals::SPRITE_SCALE)); // factor relative to the current scale
//            graphics.getWindow().draw(sprite);
//        }
//    }

    //Calcular el rectangulo de la vista Juego. Le aumentamos el tamaño de un tile en todos los bordes para evitar
	//que al dibujar no aparezcan dentro de la vista tiles que si lo están y se produzcan espacios vacios.
    sf::FloatRect screenRect(sf::Vector2f((graphics.getView(Juego)->getCenter().x - (graphics.getView(Juego)->getSize().x)/2) - this->_tileList.at(0).getSize().x,
                                        (graphics.getView(Juego)->getCenter().y - (graphics.getView(Juego)->getSize().y)/2) - this->_tileList.at(0).getSize().y) ,
    						sf::Vector2f(graphics.getView(Juego)->getSize().x + this->_tileList.at(0).getSize().x,
    									graphics.getView(Juego)->getSize().y + this->_tileList.at(0).getSize().y));

    for (int i=0; i<this->_tileList.size(); i++) {
        //Dibujamos todos en la vista vista Minimapa
        graphics.getWindow().setView(*graphics.getView(Minimapa)); //Establecer vista Minimapa
        this->_tileList.at(i).draw(graphics);

        //Obtenemos la caja de colisión del tile
        sf::FloatRect tileBounds(sf::Vector2f(this->_tileList.at(i).getPosition().x, this->_tileList.at(i).getPosition().y),
                                    sf::Vector2f(this->_tileList.at(i).getSize().x, this->_tileList.at(i).getSize().y));
        //Si el tile está dentro del rectangulo de la vista Juego lo dibujamos
        if(screenRect.intersects(tileBounds)) {
            graphics.getWindow().setView(*graphics.getView(Juego));  //Establecer la vista Juego
            this->_tileList.at(i).draw(graphics);
        }
        /*else {
            printf("screenRect %f, %f, %f, %f\n", screenRect.left, screenRect.top, screenRect.left+screenRect.width, screenRect.top+screenRect.height);
            printf("getBounds %f, %f, %f, %f\n", tileBounds.left, tileBounds.top,tileBounds.left+tileBounds.width, tileBounds.top+tileBounds.height);
            printf ("Tile fuera de view\n");
        }*/
    }

    //Dibujamos ayuntamiento en vista Minimapa
    graphics.getWindow().setView(*graphics.getView(Minimapa)); //Establecer vista Minimapa
	this->_ayuntamiento.draw(graphics);
    //Dibujar ayuntamiento en vista Juego
    graphics.getWindow().setView(*graphics.getView(Juego)); //Establecer la vista Juego
	this->_ayuntamiento.draw(graphics);

	//Dibujar las unidades del ayuntamiento.
	for (int i=0; i<this->_ayuntamiento._unidades.size(); i++) {
	    graphics.getWindow().setView(*graphics.getView(Minimapa)); //Establecer vista Minimapa
		this->_ayuntamiento._unidades.at(i).draw(graphics);
	    graphics.getWindow().setView(*graphics.getView(Juego)); //Establecer la vista Juego
		this->_ayuntamiento._unidades.at(i).draw(graphics);
	}

    /*
    for (int i=0; i<this->_animatedTileList.size(); i++) {
        this->_animatedTileList.at(i).draw(graphics);
    }
    for (int i=0; i<this->_enemies.size(); i++) {
        this->_enemies.at(i)->draw(graphics);
    }*/

}

std::vector<Rectangle> Level::checkTileCollisions(const Rectangle &other) {
    std::vector<Rectangle> others;
    for (int i=0; i<this->_collisionRects.size(); i++) {
        if (this->_collisionRects.at(i).collidesWith(other)) {
            others.push_back(this->_collisionRects.at(i));
        }
    }
    return others;
}

/*
std::vector<Slope> Level::checkSlopeCollisions(const Rectangle &other) {
    std::vector<Slope> others;
    for (int i=0; i<this->_slopes.size(); i++) {
        if (this->_slopes.at(i).collidesWith(other)) {
            others.push_back(this->_slopes.at(i));
        }
    }
    return others;
}

std::vector<Door> Level::checkDoorCollisions(const Rectangle &other) {
    std::vector<Door> others;
    for (int i=0; i<this->_doorList.size(); i++) {
        if (this->_doorList.at(i).collidesWith(other)) {
            others.push_back(this->_doorList.at(i));
        }
    }
    return others;
}

std::vector<Enemy*> Level::checkEnemyCollisions(const Rectangle &other) {
    std::vector<Enemy*> others;
    for (int i=0; i<this->_enemies.size(); i++) {
        if (this->_enemies.at(i)->getBoundingBox().collidesWith(other)) {
            others.push_back(this->_enemies.at(i));
        }
    }
    return others;
}
*/

const sf::Vector2i Level::getPlayerSpawnPoint() const {
    return this->_spawnPoint;
}

sf::Vector2i Level::getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight) {
    sf::Vector2u tilesetSize = tls.Texture->getSize();
    //printf("tilesetSize %i x %i\n", tilesetSize.x, tilesetSize.y);

    int tsxx = gid % (tilesetSize.x / tileWidth) - 1;
    tsxx *= tileWidth;
    int tsyy = 0;
    int amt = ((gid - tls.FirstGid) / (tilesetSize.x / tileWidth));
    tsyy = tileHeight * amt;
    sf::Vector2i finalTilesetPosition (tsxx, tsyy);
    return finalTilesetPosition;
}
