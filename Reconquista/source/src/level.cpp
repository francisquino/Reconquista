#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "level.h"
#include "graphics.h"
#include "globals.h"
#include "utils.h"
#include "input.h"
#include "tinyxml2.h"

using namespace tinyxml2;

const int dir=4; // Numero de posibles direcciones para desplazarse desde cualquier posicion. 8 con diagonales. 4 sin diagonales
// if dir==4
static int dx[dir]={1, 0, -1, 0};
static int dy[dir]={0, 1, 0, -1};
// if dir==8
//static int dx[dir]={1, 1, 0, -1, -1, -1, 0, 1};
//static int dy[dir]={0, 1, 1, 1, 0, -1, -1, -1};

//Sumamos el valor de dx y dy a las cooordenadas para desplazarnos en el sentido del reloj
// [     ][     ][     ]		[     ][     ][     ]		[     ][     ][     ]	...	[-1,-1][ 0,-1][ 1,-1]
// [     ]       [ 1, 0]		[     ]       [ 1, 0]		[     ]       [ 1, 0]	...	[-1, 0]       [ 1, 0]
// [     ][     ][     ]		[     ][     ][ 1, 1]		[     ][ 0, 1][ 1, 1]	...	[-1, 1][ 0, 1][ 1, 1]





Level::Level() {}

Level::Level(std::string mapName, Graphics &graphics) :
        _mapName(mapName),
        _size(sf::Vector2i(0,0)),
        _ayuntamiento(NULL)
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

    //Tamaño del mapa para buscar rutas
    nMapaBuscarRutas = width;
    mMapaBuscarRutas = height;

	mapa = new int*[nMapaBuscarRutas];
	mapa[0] = new int[nMapaBuscarRutas * mMapaBuscarRutas];
	for (int i = 1; i < nMapaBuscarRutas; i++) {
		mapa[i] = mapa[i-1] + mMapaBuscarRutas;
		//Inicializamos el mapa a cero: en principio, todas las casillas son accesibles
		for (int j = 1; j < mMapaBuscarRutas; j++)
			mapa[i][j]=0;
	}

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
							for (unsigned int i = 0; i < this->_tilesets.size(); i++) {
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
							for (unsigned int i = 0; i < this->_animatedTileInfos.size(); i++) {
								if (this->_animatedTileInfos.at(i).StartTileId == gid) {
									ati = this->_animatedTileInfos.at(i);
									isAnimatedTile = true;
									break;
								}
							}
							if (isAnimatedTile == true) {
								std::vector<sf::Vector2i> tilesetPositions;
								for (unsigned int i = 0; i < ati.TileIds.size(); i++) {
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

							//Indicamos en el mapa utilizado para obtener las rutas con A* si la casilla correspondiente a este tile
							//es accesible o no.
							// (Mapa, x, y, gid)
							this->rellenarMapa(mapa, tileCounter%width, tileCounter/width, gid);

	                        tileCounter++;
	                        pTile = pTile->NextSiblingElement("tile"); //El fichero tiene una estructura diferente
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
                            for (unsigned int i=0; i<pairs.size(); i++) {
                                std::vector<std::string> ps;
                                Utils::split(pairs.at(i), ps, ',');
                                //points.push_back(sf::Vector2i(std::stoi(ps.at(0)), std::stoi(ps.at(1))));
                                points.push_back(sf::Vector2i(std::atoi(ps.at(0).c_str()), std::atoi(ps.at(1).c_str())));
                            }

                        }

                        for(unsigned int i=0; i<points.size(); i+=2) {
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
                        int x = std::floor(pObject->FloatAttribute("x"));
                        int y = std::floor(pObject->FloatAttribute("y"));
                    	x = (x - (x % tileWidth));	//La posicion se ajusta a la esquina superior izquierda
                    	y = (y - (y % tileHeight));
                        const char* name = pObject->Attribute("name");
                        std::stringstream ss;
                        ss << name;
                        if(ss.str() == "ayuntamiento") {
                            this->_ayuntamiento = new Ayuntamiento(graphics, sf::Vector2i(x * globals::SPRITE_SCALE, y * globals::SPRITE_SCALE));
                            //Rellenamos a mano el mapa de obtencion de rutas
                            //El ayto ocupa 4 casillas, que no se pueden atravesar
                            mapa[x/tileWidth][y/tileHeight]=1;
                            mapa[(x/tileWidth)+1][y/tileHeight]=1;
                            mapa[x/tileWidth][(y/tileHeight)+1]=1;
                            mapa[(x/tileWidth)+1][(y/tileHeight)+1]=1;
                        }
                        else if(ss.str() == "campesino") {
                            this->_ayuntamiento->_unidades.push_back(new Campesino(graphics, sf::Vector2i(x * globals::SPRITE_SCALE, y * globals::SPRITE_SCALE)));
                        }

                        pObject = pObject->NextSiblingElement("object");
                    }
                }
            }

            else if (ss.str() == "recursos") {
                XMLElement* pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL){
                    while(pObject) {
                        int x = std::floor(pObject->FloatAttribute("x"));
                        int y = std::floor(pObject->FloatAttribute("y"));
                    	x = (x - (x % tileWidth));	//La posicion se ajusta a la esquina superior izquierda
                    	y = (y - (y % tileHeight));
                        const char* name = pObject->Attribute("name");
                                                std::stringstream ss1;
                                                ss1 << name;
						XMLElement* pProperties = pObject->FirstChildElement("properties");
						if (pProperties != NULL){
							while(pProperties) {
								XMLElement* pProperty = pProperties->FirstChildElement("property");
								if (pProperty != NULL){
									while(pProperty) {

										const char* name = pProperty->Attribute("name");
										std::stringstream ss2;
										ss2 << name;
										if(ss2.str() == "cantidad") {
											const char* value = pProperty->Attribute("value");
											std::stringstream ss3;
											ss3 << value;
											if (ss1.str() == "mina") {
												this->_recursos.push_back(new Mina(graphics, sf::Vector2i(x * globals::SPRITE_SCALE, y * globals::SPRITE_SCALE), atoi(ss3.str().c_str())));
											}
											else if (ss1.str() == "bosque") {
												this->_recursos.push_back(new Bosque(graphics, sf::Vector2i(x * globals::SPRITE_SCALE, y * globals::SPRITE_SCALE), atoi(ss3.str().c_str())));
											}
				                            //Rellenamos a mano el mapa de obtencion de rutas
				                            //Cada recurso ocupa 1 casilla, que no se puede atravesar
				                            mapa[x/tileWidth][y/tileHeight]=1;
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

            /*
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
            }*/

            pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
        }
    }

    //Dibujar mapar de rutas

	for (int j = 0; j < mMapaBuscarRutas; j++) {
		for (int i = 0; i < nMapaBuscarRutas; i++)
			printf ("%i",mapa[i][j]);
		printf ("\n");
	}

}

void Level::update(int elapsedTime) {
	this->_ayuntamiento->update(elapsedTime);

    for (unsigned int i=0; i<this->_ayuntamiento->_unidades.size(); i++) {
        this->_ayuntamiento->_unidades.at(i)->update(elapsedTime);
    }

    for (unsigned int i=0; i<this->_recursos.size(); i++) {
        this->_recursos.at(i)->update(elapsedTime);
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

    for (unsigned int i=0; i<this->_tileList.size(); i++) {
        ////Dibujamos todos en la vista vista Minimapa
        //graphics.getWindow().setView(*graphics.getView(Minimapa)); //Establecer vista Minimapa
        //this->_tileList.at(i).draw(graphics);

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
	this->_ayuntamiento->draw(graphics);
    //Dibujar ayuntamiento en vista Juego
    graphics.getWindow().setView(*graphics.getView(Juego)); //Establecer la vista Juego
	this->_ayuntamiento->draw(graphics);

	//Dibujar las unidades del ayuntamiento.
	for (unsigned int i=0; i<this->_ayuntamiento->_unidades.size(); i++) {
	    graphics.getWindow().setView(*graphics.getView(Minimapa)); //Establecer vista Minimapa
		this->_ayuntamiento->_unidades.at(i)->draw(graphics);
	    graphics.getWindow().setView(*graphics.getView(Juego)); //Establecer la vista Juego
		this->_ayuntamiento->_unidades.at(i)->draw(graphics);
	}

	//Dibujar los recursos
    for (unsigned int i=0; i<this->_recursos.size(); i++) {
	    graphics.getWindow().setView(*graphics.getView(Minimapa)); //Establecer vista Minimapa
        this->_recursos.at(i)->draw(graphics);
    	graphics.getWindow().setView(*graphics.getView(Juego)); //Establecer la vista Juego
        this->_recursos.at(i)->draw(graphics);
    }

    /*
    for (int i=0; i<this->_animatedTileList.size(); i++) {
        this->_animatedTileList.at(i).draw(graphics);
    }
    for (int i=0; i<this->_enemies.size(); i++) {
        this->_enemies.at(i)->draw(graphics);
    }*/

}

// Recorre todos los objetos de este nivel y los marca como No Seleccionado, excepto uno.
void Level::deseleccionarObjetos(Objeto* seleccionado) {
	//Ayuntamiento
	if (this->_ayuntamiento != seleccionado) {
		this->_ayuntamiento->setSeleccionado(false);
	}

	//Unidades del ayuntamiento
	for (unsigned int i=0; i<this->_ayuntamiento->_unidades.size(); i++) {
		if (this->_ayuntamiento->_unidades.at(i) != seleccionado) {
			this->_ayuntamiento->_unidades.at(i)->setSeleccionado(false);
		}
	}

	//Recursos del nivel
	for (unsigned int i=0; i<this->_recursos.size(); i++) {
		if (this->_recursos.at(i) != seleccionado) {
			this->_recursos.at(i)->setSeleccionado(false);
		}
	}
} //deseleccionarObjetos


std::vector<Rectangle> Level::checkTileCollisions(const Rectangle &other) {
    std::vector<Rectangle> others;
    for (unsigned int i=0; i<this->_collisionRects.size(); i++) {
        if (this->_collisionRects.at(i).collidesWith(other)) {
            others.push_back(this->_collisionRects.at(i));
        }
    }
    return others;
}

/*
std::vector<Slope> Level::checkSlopeCollisions(const Rectangle &other) {
    std::vector<Slope> others;
    for (unsigned int i=0; i<this->_slopes.size(); i++) {
        if (this->_slopes.at(i).collidesWith(other)) {
            others.push_back(this->_slopes.at(i));
        }
    }
    return others;
}

std::vector<Door> Level::checkDoorCollisions(const Rectangle &other) {
    std::vector<Door> others;
    for (unsigned int i=0; i<this->_doorList.size(); i++) {
        if (this->_doorList.at(i).collidesWith(other)) {
            others.push_back(this->_doorList.at(i));
        }
    }
    return others;
}

std::vector<Enemy*> Level::checkEnemyCollisions(const Rectangle &other) {
    std::vector<Enemy*> others;
    for (unsigned int i=0; i<this->_enemies.size(); i++) {
        if (this->_enemies.at(i)->getBoundingBox().collidesWith(other)) {
            others.push_back(this->_enemies.at(i));
        }
    }
    return others;
}
*/

/*const sf::Vector2i Level::getPlayerSpawnPoint() const {
    return this->_spawnPoint;
}*/

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

//Rellenar el mapa para obtener rutas a partir de la posicion del tile y su id
void Level::rellenarMapa(int **mapa, int x, int y, int gid) {
	//Segun el id del tile, se indicara que por esta casilla se puede pasar (0) o no (1)
	switch (gid) {
		//0, no se dibuja nada. No se hace nada.
		case 0:
			break;
		//Montañas: no se puede
		case 11 ... 13:
		//Laguna: no se puede
		case 19 ... 21:
		//Rio: no se puede
		case 92 ... 96:
		case 99 ... 103:
		case 106 ... 108:
		//Mar: no se puede
		case 169 ... 172:
		//Bordes terreno: no se puede
		case 176 ... 181:
		case 183:
		case 185:
		case 186:
		case 188:
		case 190 ... 195:
		case 197 ... 202:
		case 204:
		case 206:
		case 207:
		case 209:
		case 211 ... 216:
			mapa[x][y]=1;
			break;
		//Terreno: se puede
		case 1 ... 4:
		//Puente: se puede
		case 241 ... 244:
			mapa[x][y]=0;
			break;
		default:
			break;
	}
}

bool operator<(const Nodo & a, const Nodo & b) {
	 return a.getPrioridad() > b.getPrioridad();
}

// Algoritmo A-*
// Devuelve una ruta formada por una cadena de los digitos de direccion
std::string Level::pathFind( const int & xStart, const int & yStart,
                 const int & xFinish, const int & yFinish ) {
    static std::priority_queue<Nodo> pq[2]; //Lista de nodos abiertos (todavia no evaluados)
    int** mapaNodosCerrados;
    int** mapaNodosAbiertos;
    int** mapaDir;
    static int pqi; //Indice de pq
    static Nodo* n0;
    static Nodo* m0;
    static int i, j, x, y, xdx, ydy;
    static char c;

    pqi=0;

    // Inicia los nodos de los mapas. Se vuelve tan complicado al ser array 2D sin conocer el tamaño inicialmente
    mapaNodosCerrados = new int*[nMapaBuscarRutas];
    mapaNodosCerrados[0] = new int[nMapaBuscarRutas * mMapaBuscarRutas];
    for (int i = 1; i < nMapaBuscarRutas; i++) {
    	mapaNodosCerrados[i] = mapaNodosCerrados[i-1] + mMapaBuscarRutas;
    	for (int j = 1; j < mMapaBuscarRutas; j++)
            mapaNodosCerrados[i][j]=0;
    }

	mapaNodosAbiertos = new int*[nMapaBuscarRutas];
	mapaNodosAbiertos[0] = new int[nMapaBuscarRutas * mMapaBuscarRutas];
	for (int i = 1; i < nMapaBuscarRutas; i++) {
		mapaNodosAbiertos[i] = mapaNodosAbiertos[i-1] + mMapaBuscarRutas;
		for (int j = 1; j < mMapaBuscarRutas; j++)
			mapaNodosAbiertos[i][j]=0;
	}

	mapaDir = new int*[nMapaBuscarRutas];
	mapaDir[0] = new int[nMapaBuscarRutas * mMapaBuscarRutas];
	for (int i = 1; i < nMapaBuscarRutas; i++) {
		mapaDir[i] = mapaDir[i-1] + mMapaBuscarRutas;
		for (int j = 1; j < mMapaBuscarRutas; j++)
			mapaDir[i][j]=0;
	}

    // Crear el nodo inicial e insertarlo en la lista de nodos abiertos
    n0=new Nodo(xStart, yStart, 0, 0);
    n0->updatePrioridad(xFinish, yFinish);
    pq[pqi].push(*n0);
    mapaNodosAbiertos[x][y]=n0->getPrioridad(); // Marcarlo en el mapa de nodos abiertos
	delete n0;

    // Busqueda A*
    while(!pq[pqi].empty()) // Mientras haya algun nodo en la lista de nodos abiertos
    {
        // Obtener el actual nodo con la mas alta prioridad de la lista de nodos abiertos
        n0=new Nodo( pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
                     pq[pqi].top().getDistancia(), pq[pqi].top().getPrioridad());

        x=n0->getxPos(); y=n0->getyPos();

        pq[pqi].pop(); // Eliminar el nodo de la lista abierta
        mapaNodosAbiertos[x][y]=0;
        // Marcarlo en el mapa de nodos cerrados
        mapaNodosCerrados[x][y]=1;

        // Parar de buscar cuando la meta se alcanza
        //if((*n0).estimate(xFinish, yFinish) == 0)
        if(x==xFinish && y==yFinish)
        {
            // Generar la ruta desde el fin al inicio siguiendo las direcciones
            std::string ruta="";
            while(!(x==xStart && y==yStart))
            {
                j=mapaDir[x][y];
                c='0'+(j+dir/2)%dir;
                ruta=c+ruta;
                x+=dx[j];
                y+=dy[j];
            }

            // Recolectar basura
            delete n0;
            // Vaciar los nodos sobrantes
            while(!pq[pqi].empty()) pq[pqi].pop();

            delete[] mapaDir[0];
            delete[] mapaDir;
            delete[] mapaNodosAbiertos[0];
            delete[] mapaNodosAbiertos;
            delete[] mapaNodosCerrados[0];
            delete[] mapaNodosCerrados;

            return ruta;
        } //Parar de buscar al alcanzar la meta

        // Generar movimientos (nodos hijos) en todas las direcciones posibles
        for(i=0;i<dir;i++)
        {
        	//xdx es la coord. x del nuevo nodo (x + el desplazamiento x) ydy es la coord. y del nuevo nodo (y + el desplazamiento y)
            xdx=x+dx[i];
            ydy=y+dy[i];

            //Si las coordenadas del nodo hijo (xdx, ydy) estan dentro de las coordenadas del mapa,
            //la casilla del mapa en esas coordenadas no es 1,
            //y la casilla del mapa de nodos cerrados (ya evaluados) en esas coordenadas tampoco es 1:
            if(!(xdx<0 || xdx>nMapaBuscarRutas-1 || ydy<0 || ydy>mMapaBuscarRutas-1 || mapa[xdx][ydy]==1
                || mapaNodosCerrados[xdx][ydy]==1))
            {
                // Generar un nuevo nodo hijo, calculando la distancia y la prioridad
                m0=new Nodo( xdx, ydy, n0->getDistancia(),
                             n0->getPrioridad());
                m0->nextDistancia(dir, i);
                m0->updatePrioridad(xFinish, yFinish);

                // Si no esta en la lista abierta (no evaluados), añadirlo
                if(mapaNodosAbiertos[xdx][ydy]==0)
                {
                    mapaNodosAbiertos[xdx][ydy]=m0->getPrioridad();
                    pq[pqi].push(*m0);
                    delete m0; // Only <-- new added by commenter // mark its parent Nodo direction
                    mapaDir[xdx][ydy]=(i+dir/2)%dir;
                }
                // Ya esta en el mapa de nodos abiertos
                else if(mapaNodosAbiertos[xdx][ydy]>m0->getPrioridad())
                {
                    // Actualizar la informacion de prioridad
                    mapaNodosAbiertos[xdx][ydy]=m0->getPrioridad();
                    // Actualizar la informacion de direccion del padre
                    mapaDir[xdx][ydy]=(i+dir/2)%dir;

                    // Sustituir el nodo
                    // vaciando una cola de prioridad a la otra    		by emptying one pq to the other one
                    // excepto el nodo a ser sutituido sera ignorado	except the Nodo to be replaced will be ignored
                    // y el nuevo nodo sera insertado en su lugar		and the new Nodo will be pushed in instead
                    while(!(pq[pqi].top().getxPos()==xdx &&
                           pq[pqi].top().getyPos()==ydy))
                    {
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pq[pqi].pop(); // Remover el nodo buscado			remove the wanted Nodo

                    // Vaciar el mayor tamaño pq al mas pequeño			empty the larger size pq to the smaller one
                    if(pq[pqi].size()>pq[1-pqi].size()) pqi=1-pqi;
                    while(!pq[pqi].empty())
                    {
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pqi=1-pqi;
                    pq[pqi].push(*m0);	// Solo el segundo elemento añadido nuevo			only 2nd item added new
                    delete m0; // Añadir el mejor nodo en su lugar			add the better Nodo instead
                }
                else delete m0; // Recoleccion de basura
            }
        }
        delete n0; // Recoleccion de basura
    }

    delete[] mapaDir[0];
    delete[] mapaDir;
    delete[] mapaNodosAbiertos[0];
    delete[] mapaNodosAbiertos;
    delete[] mapaNodosCerrados[0];
    delete[] mapaNodosCerrados;

    return ""; // No encontrada ruta
} //pathFind


// Devuelve la posicion accesible mas cercana, en una ruta, a un punto que no es accesible
// Vamos a utilizar el algoritmo A*, pero haciendo el recorrido inverso: desde el final al inicio
sf::Vector2i Level::posicionAccesibleMasCercana( const int & xStart, const int & yStart, const int & xFinish, const int & yFinish ) {
    static std::priority_queue<Nodo> pq[2]; //Lista de nodos abiertos (todavia no evaluados)
    int** mapaNodosCerrados;
    int** mapaNodosAbiertos;
    int** mapaDir;
    static int pqi; //Indice de pq
    static Nodo* n0;
    static Nodo* m0;
    static int i, x, y, xdx, ydy;
    bool accesible = false; //Condicion de finalizacion de busqueda
    sf::Vector2i posicion;

    pqi=0;

    // Inicia los nodos de los mapas. Se vuelve tan complicado al ser array 2D sin conocer el tamaño inicialmente
    mapaNodosCerrados = new int*[nMapaBuscarRutas];
    mapaNodosCerrados[0] = new int[nMapaBuscarRutas * mMapaBuscarRutas];
    for (int i = 1; i < nMapaBuscarRutas; i++) {
    	mapaNodosCerrados[i] = mapaNodosCerrados[i-1] + mMapaBuscarRutas;
    	for (int j = 1; j < mMapaBuscarRutas; j++)
            mapaNodosCerrados[i][j]=0;
    }

	mapaNodosAbiertos = new int*[nMapaBuscarRutas];
	mapaNodosAbiertos[0] = new int[nMapaBuscarRutas * mMapaBuscarRutas];
	for (int i = 1; i < nMapaBuscarRutas; i++) {
		mapaNodosAbiertos[i] = mapaNodosAbiertos[i-1] + mMapaBuscarRutas;
		for (int j = 1; j < mMapaBuscarRutas; j++)
			mapaNodosAbiertos[i][j]=0;
	}

	mapaDir = new int*[nMapaBuscarRutas];
	mapaDir[0] = new int[nMapaBuscarRutas * mMapaBuscarRutas];
	for (int i = 1; i < nMapaBuscarRutas; i++) {
		mapaDir[i] = mapaDir[i-1] + mMapaBuscarRutas;
		for (int j = 1; j < mMapaBuscarRutas; j++)
			mapaDir[i][j]=0;
	}

    // Crear el nodo inicial e insertarlo en la lista de nodos abiertos. Tomamos la posicion final como inicio
    n0=new Nodo(xFinish, yFinish, 0, 0);
    n0->updatePrioridad(xStart, yStart);
    pq[pqi].push(*n0);
    mapaNodosAbiertos[x][y]=n0->getPrioridad(); // Marcarlo en el mapa de nodos abiertos
	delete n0;

    // Busqueda A*
    while(!pq[pqi].empty()) // Mientras haya algun nodo en la lista de nodos abiertos
    {
        // Obtener el actual nodo con la mas alta prioridad de la lista de nodos abiertos
        n0=new Nodo( pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
                     pq[pqi].top().getDistancia(), pq[pqi].top().getPrioridad());

        x=n0->getxPos(); y=n0->getyPos();

        pq[pqi].pop(); // Eliminar el nodo de la lista abierta
        mapaNodosAbiertos[x][y]=0;
        // Marcarlo en el mapa de nodos cerrados
        mapaNodosCerrados[x][y]=1;

        // Parar de buscar cuando hemos alcanzado algún nodo que es accesible
        if(accesible)
        {
        	//Sacamos de la lista de nodos abiertos hasta que cogemos uno accesible
        	while (mapa[x][y]!=0) {
                n0=new Nodo( pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
                             pq[pqi].top().getDistancia(), pq[pqi].top().getPrioridad());

                x=n0->getxPos(); y=n0->getyPos();

                pq[pqi].pop(); // Eliminar el nodo de la lista abierta
        	}

        	posicion.x = x;
        	posicion.y = y;
            // Recolectar basura
            delete n0;
            // Vaciar los nodos sobrantes
            while(!pq[pqi].empty()) pq[pqi].pop();

            delete[] mapaDir[0];
            delete[] mapaDir;
            delete[] mapaNodosAbiertos[0];
            delete[] mapaNodosAbiertos;
            delete[] mapaNodosCerrados[0];
            delete[] mapaNodosCerrados;

            // Solo devolvemos una posición. Tomar Generar la ruta desde el fin al inicio siguiendo las direcciones
            return posicion;
        } //Parar de buscar al alcanzar la meta

        // Generar movimientos (nodos hijos) en todas las direcciones posibles
        for(i=0;i<dir;i++)
        {
        	//xdx es la coord. x del nuevo nodo (x + el desplazamiento x) ydy es la coord. y del nuevo nodo (y + el desplazamiento y)
            xdx=x+dx[i];
            ydy=y+dy[i];

            //Si la casilla que estamos evaluando ya es accesible, evaluamos las del for, pero no evaluamos mas
            if (mapa[xdx][ydy]==0) accesible = true;

            //Si las coordenadas del nodo hijo (xdx, ydy) estan dentro de las coordenadas del mapa
            if(!(xdx<0 || xdx>nMapaBuscarRutas-1 || ydy<0 || ydy>mMapaBuscarRutas-1))
            {
                // Generar un nuevo nodo hijo, calculando la distancia y la prioridad
                m0=new Nodo( xdx, ydy, n0->getDistancia(),
                             n0->getPrioridad());
                m0->nextDistancia(dir, i);
                m0->updatePrioridad(xFinish, yFinish);

                // Si no esta en la lista abierta (no evaluados), añadirlo
                if(mapaNodosAbiertos[xdx][ydy]==0)
                {
                    mapaNodosAbiertos[xdx][ydy]=m0->getPrioridad();
                    pq[pqi].push(*m0);
                    delete m0; // Only <-- new added by commenter // mark its parent Nodo direction
                    mapaDir[xdx][ydy]=(i+dir/2)%dir;
                }
                // Ya esta en el mapa de nodos abiertos
                else if(mapaNodosAbiertos[xdx][ydy]>m0->getPrioridad())
                {
                    // Actualizar la informacion de prioridad
                    mapaNodosAbiertos[xdx][ydy]=m0->getPrioridad();
                    // Actualizar la informacion de direccion del padre
                    mapaDir[xdx][ydy]=(i+dir/2)%dir;

                    // Sustituir el nodo
                    // vaciando una cola de prioridad a la otra    		by emptying one pq to the other one
                    // excepto el nodo a ser sutituido sera ignorado	except the Nodo to be replaced will be ignored
                    // y el nuevo nodo sera insertado en su lugar		and the new Nodo will be pushed in instead
                    while(!(pq[pqi].top().getxPos()==xdx &&
                           pq[pqi].top().getyPos()==ydy))
                    {
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pq[pqi].pop(); // Remover el nodo buscado			remove the wanted Nodo

                    // Vaciar el mayor tamaño pq al mas pequeño			empty the larger size pq to the smaller one
                    if(pq[pqi].size()>pq[1-pqi].size()) pqi=1-pqi;
                    while(!pq[pqi].empty())
                    {
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pqi=1-pqi;
                    pq[pqi].push(*m0);	// Solo el segundo elemento añadido nuevo			only 2nd item added new
                    delete m0; // Añadir el mejor nodo en su lugar			add the better Nodo instead
                }
                else delete m0; // Recoleccion de basura
            }
        }
        delete n0; // Recoleccion de basura
    }

    delete[] mapaDir[0];
    delete[] mapaDir;
    delete[] mapaNodosAbiertos[0];
    delete[] mapaNodosAbiertos;
    delete[] mapaNodosCerrados[0];
    delete[] mapaNodosCerrados;

    return posicion; // No encontrada posicion
} //posicionAccesibleMasCercana


//Transformar la ruta devuelta por pathFind a coordenadas
std::vector<sf::Vector2i> Level::rutaACoordenadas (std::string ruta, sf::Vector2i origen) {
	std::vector<sf::Vector2i> listaCoordenadas;
	int dir;
	char c;
	int x=origen.x;
	int y=origen.y;

	for(unsigned int i=0;i<ruta.length();i++)
	{
		c =ruta.at(i);
		dir=c-'0';  // original --> j=atoi(&c);
		x = x+dx[dir];
		y = y+dy[dir];
		listaCoordenadas.push_back(sf::Vector2i(x, y));
		printf("Ruta [%i, %i]\n", x, y);
	}

	return listaCoordenadas;
}


//Simplificar la ruta de coordenadas, eliminando los desplazamientos irrelevantes
std::vector<sf::Vector2i> Level::simplificarRutaCoord (std::vector<sf::Vector2i> ruta,  sf::Vector2i origen) {
	std::vector<sf::Vector2i> rutaSimple;
	unsigned int j;

	sf::Vector2i dir1, dir2;
	sf::Vector2i puntoA = origen;
	sf::Vector2i puntoB;
	sf::Vector2i puntoC;

	if (ruta.size()>=2) {
		j=0;
		for (unsigned int i=j+1; i<ruta.size(); i++) {
			puntoB = ruta[j];
			puntoC = ruta[i];

			//Obtenemos la direccion de cada par de puntos (de A a B y de B a C)
			dir1 = puntoB - puntoA;
			dir2 = puntoC - puntoB;
			//Si cambia la direccion, guardamos el punto B
			if (dir1 != dir2) {
				rutaSimple.push_back(puntoB);
				printf ("Ruta simplificada [%i,%i]\n", puntoB.x, puntoB.y);
			}
			puntoA = puntoB;
			j++;
		}
		rutaSimple.push_back(puntoC);
		printf ("Ruta simplificada [%i,%i]\n", puntoC.x, puntoC.y);
	}
	return rutaSimple;
}


//Pasar de las coordenadas del terreno a las del mapa de obtencion de rutas
sf::Vector2i Level::coordAMapa(int x, int y) {
	sf::Vector2i mapa;

	mapa.x = x / this->getTileSize().x / (int)globals::SPRITE_SCALE;
	mapa.y = y / this->getTileSize().y / (int)globals::SPRITE_SCALE;
	return (mapa);
}

//Pasar de las coordenadas del mapa de obtencion de rutas a las del terreno
sf::Vector2i Level::mapaACoord(int x, int y) {
	sf::Vector2i coord;

	coord.x = x * this->getTileSize().x * globals::SPRITE_SCALE;
	coord.y = y * this->getTileSize().y * globals::SPRITE_SCALE;

	return (coord);
}


