#include<iostream>

#include <SFML/Graphics.hpp>
//#include <SFML/Window.hpp>

#include "game.h"
#include "graphics.h"
#include "input.h"    //Lectura de eventos -> pulsacion de
#include "objeto.h"

/* Game class
 * This class holds all information for our main game loop
 */

namespace {
    const int FPS = 50;
    const int MAX_FRAME_TIME = 1000 / FPS;
}

bool isMouseBox = false;
bool playerSelected = false;
sf::VertexArray mouseBox(sf::LinesStrip, 5);
sf::VertexArray playerBox(sf::LinesStrip, 5);
sf::VertexArray ayuntamientoBox(sf::LinesStrip, 5);
//Posicion ratón
sf::Vector2i starting_position, current_position;
sf::Vector2f startingPositionWorldPos, currentPositionWorldPos;

Sprite destinoCruz;

enum tipoCursor {
	puntero,
	lupa
};

tipoCursor _tipoCursor;

Game::Game() {
    this->gameLoop();
}

Game::~Game() {
}

void Game::gameLoop() {
    Graphics graphics;
    Input input;
    sf::Event event;

    _tipoCursor = puntero;

    this->_level = Level("MapReconquista1", graphics);
    this->_player = Player(graphics, this->_level.getPlayerSpawnPoint());

    //this->_hud = HUD(graphics, this->_player);

    sf::Int32 LAST_UPDATE_TIME = this->clock.getElapsedTime().asMilliseconds();
    //std::cout << "LAST_UPDATE_TIME " << LAST_UPDATE_TIME << "\n";

    //Start the game loop
    while(true) {
        input.beginNewFrame();

        // Manejamos los eventos
        //while (graphics.getWindow().pollEvent(event))
        if (graphics.getWindow().pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed) {
                //if (sf::Keyboard::isKeyPressed == 0) {
                    input.keyDownEvent(event);
                //}
            }
            else if (event.type == sf::Event::KeyReleased) {
                input.keyUpEvent(event);
            }
            //Se pulsa un botón del ratón
            /*else if (event.type == sf::Event::MouseButtonPressed) {
            	input.mouseButtonDownEvent(event);
            }
            //Se libera un botón del ratón
            else if (event.type == sf::Event::MouseButtonReleased) {
            	input.mouseButtonUpEvent(event);
            }*/
            //Se desplaza el ratón
            if (event.type == sf::Event::MouseMoved) {
            	//Si el ratón está sobre un objeto, cambiar el cursor por una lupa
            	if (input.sobreObjeto(sf::Mouse::getPosition(graphics.getWindow()), (Objeto*) &this->_level._ayuntamiento)) {
            		_tipoCursor = lupa;

            	}
            }
            if( event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left )
            {
            	//Get the mouse position:
            	starting_position = sf::Mouse::getPosition(graphics.getWindow());
                // = event.mouseButton.x;
                //starting_position.y = event.mouseButton.y;

            	//Si se ha seleccionado al jugador. Al pulsar con el boton izquierdo, fijamos un destino.
            	//Ver si el cursor está dentro de la vista Juego
            	if (input.dentroVistaJuego(starting_position)) {
					if (playerSelected) {
						sf::Vector2i posicion_destino;
						posicion_destino = sf::Mouse::getPosition(graphics.getWindow());
						this->_player.setDestino(graphics.getWindow().mapPixelToCoords(posicion_destino).x, graphics.getWindow().mapPixelToCoords(posicion_destino).y);
						destinoCruz = Sprite(graphics, "content/sprites/Tile-set-Toen's Medieval Strategy.png", 48, 672, 16, 16, this->_player.getDestinoX(), this->_player.getDestinoY());
					}

					if (isMouseBox) {
						isMouseBox = false;
						//playerSelected = false;
					}
            	}

            	else if (input.dentroVistaMinimapa(starting_position)) {
            		printf ("Minimapa\n");
            	}
            	else if (input.dentroVistaInfo(starting_position)) {
            		printf ("Info\n");
            	}
            }

            //Pulsamos el botón izquierdo mientras desplazamos el ratón
            if( event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left) )
            {
            	current_position = sf::Mouse::getPosition(graphics.getWindow());
            	//printf ("Coords Antes %i, %i, %i, %i\n", starting_position.x, starting_position.y, current_position.x, current_position.y);

            	//Map Pixel to Coords:
            	startingPositionWorldPos = graphics.getWindow().mapPixelToCoords(starting_position);
            	currentPositionWorldPos = graphics.getWindow().mapPixelToCoords(current_position);
            	//printf ("Coords Despues %f, %f, %f, %f\n", startingPositionWorldPos.x, startingPositionWorldPos.y, currentPositionWorldPos.x, currentPositionWorldPos.y);

				mouseBox[0].position = startingPositionWorldPos;
				mouseBox[0].color = sf::Color::Red;
				mouseBox[1].position = sf::Vector2f(startingPositionWorldPos.x, currentPositionWorldPos.y);
				mouseBox[1].color = sf::Color::Red;
				mouseBox[2].position = currentPositionWorldPos;
				mouseBox[2].color = sf::Color::Red;
				mouseBox[3].position = sf::Vector2f(currentPositionWorldPos.x, startingPositionWorldPos.y);
				mouseBox[3].color = sf::Color::Red;
				mouseBox[4].position = startingPositionWorldPos;
				mouseBox[4].color = sf::Color::Red;

            	isMouseBox = true;
            }
            else if (event.type == sf::Event::Closed) {
                return;
            }
        }
        if (input.wasKeyPressed(sf::Keyboard::Escape) == true) {
            return;
        }
        else if (input.isKeyHeld(sf::Keyboard::Left) == true) {
            this->_player.moveLeft();
        }
        else if (input.isKeyHeld(sf::Keyboard::Right) == true) {
            this->_player.moveRight();
        }

        /*
        if (input.isKeyHeld(sf::Keyboard::Up) == true) {
            this->_player.lookUp();
        }
        else if (input.isKeyHeld(sf::Keyboard::Down) == true) {
            this->_player.lookDown();
        }

        if (input.wasKeyReleased(sf::Keyboard::Up) == true) {
            this->_player.stopLookingUp();
        }

        if (input.wasKeyReleased(sf::Keyboard::Down) == true) {
            this->_player.stopLookingDown();
        }

        if (input.isKeyHeld(sf::Keyboard::Z) == true) {
            this->_player.jump();
        }
        */
        if (!input.isKeyHeld(sf::Keyboard::Left) && !input.isKeyHeld(sf::Keyboard::Right)) {
            this->_player.stopMoving();
        }


        /*
            switch (event.type)
            {
                case sf::Event::Closed:             // La ventana es cerrada
                    return;
                    break;
                case sf::Event::KeyPressed:           // Tecla pulsada
                    if (event.key.code == sf::Keyboard::Escape) {
                        return;
                    }

//                    // Show statistics
//                    if (mEvent.key.code == sf::Keyboard::V) {
//                        mEnableStatistics = !mEnableStatistics;
//                        mDebugRendering = !mDebugRendering;
//                    }

                    // Propagate KeyPressed event to the scene
                    //mScene->handleEvent(eventWrapper);
                    break;

                case sf::Event::JoystickButtonPressed:

                    // Propagate JoystickButtonPressed event to the scene
                    //mScene->handleEvent(eventWrapper);
                    break;
                default:	// Otros eventos se los pasamos a la escena activa
                    break;
            } // switch (event.Type)
        } // while (m_window.GetEvent(event)) if
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            return;
        }
        // left key is pressed: move our character
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            this->_player.moveLeft();
        }
        // right key is pressed: move our character
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            this->_player.moveRight();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            this->_player.jump();
        }

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            this->_player.stopMoving();
        }*/

        const sf::Int32 CURRENT_TIME_MS = clock.getElapsedTime().asMilliseconds();
        //std::cout << "CURRENT_TIME_MS " << CURRENT_TIME_MS << "\n";
        sf::Int32 ELAPSED_TIME_MS = (CURRENT_TIME_MS - LAST_UPDATE_TIME);
        //std::cout << "ELAPSED_TIME_MS " << ELAPSED_TIME_MS << "\n";

        this->_graphics = graphics;
        this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
        LAST_UPDATE_TIME = CURRENT_TIME_MS;

        this->draw(graphics);
    } //Game loop
}

void Game::draw(Graphics& graphics) {
    graphics.clear();

    //Dibujar level y player en _vistaJuego
    graphics.getWindow().setView(*graphics.getView(Juego));
    this->_level.draw(graphics);
    this->_player.draw(graphics);

    //Si hay destino fijado, dibujarlo
    if (this->_player.getDestinoX() != -1 && this->_player.getDestinoY() != -1) {
        graphics.getWindow().setView(*graphics.getView(Juego));
    	destinoCruz.draw(graphics, destinoCruz.getX(), destinoCruz.getY());
    }

    //Dibujar caja click & drag raton
    graphics.getWindow().setView(*graphics.getView(Juego));
    graphics.getWindow().draw(mouseBox);
    graphics.getWindow().draw(playerBox);
    graphics.getWindow().draw(ayuntamientoBox);

    //Posicion de player
    sf::Vector2f position(0, 0);
    position.x = this->_player.getX();
    position.y = this->_player.getY();

    //Obtener el punto medio de _vistaJuego
    float viewMitadX = graphics.getView(Juego)->getSize().x * 0.5;
    float viewMitadY = graphics.getView(Juego)->getSize().y * 0.5;

    //Obtener tamaño del mapa
    int sizeMapaX = this->_level.getMapSize().x * this->_level.getTileSize().x * globals::SPRITE_SCALE;
    int sizeMapaY = this->_level.getMapSize().y * this->_level.getTileSize().y * globals::SPRITE_SCALE;

    //Comparamos la posicion de player con el mapa,
    //tomando como extremos los bordes del mapa menos la mitad de la vista,
    //para luego centrar la vista.
    if (position.x < viewMitadX)
        position.x = viewMitadX;
    else if (position.x > sizeMapaX - viewMitadX)
        position.x = sizeMapaX - viewMitadX;

    if (position.y < viewMitadY)
        position.y = viewMitadY;
    else if (position.y > sizeMapaY - viewMitadY)
        position.y = sizeMapaY - viewMitadY;

    //printf ("Posicion player %f, %f\n", this->_player.getX(), this->_player.getY());
    //printf ("Posicion %f, %f\n", position.x, position.y);
    //printf ("viewMitadX %f Extremo %f\n", viewMitadX, sizeMapaX);
    graphics.getView(Juego)->setCenter(position.x, position.y);

    graphics.getWindow().setView(*graphics.getView(Completa));
    switch (_tipoCursor) {
    	case puntero:
    	    graphics.getCursorPuntero().draw(graphics, sf::Mouse::getPosition(graphics.getWindow()).x, sf::Mouse::getPosition(graphics.getWindow()).y);
    		break;
    	case lupa:
    	    graphics.getCursorLupa().draw(graphics, sf::Mouse::getPosition(graphics.getWindow()).x, sf::Mouse::getPosition(graphics.getWindow()).y);
    		break;
    }
    //this->_hud.draw(graphics);

    graphics.flip();
}

void Game::update(float elapsedTime) {
    //std::cout << elapsedTime << "\n";

    this->_player.update(elapsedTime);
	this->_level.update(elapsedTime, this->_player);
    //this->_hud.update(elapsedTime, this->_player);

	//Comprobar que objetos estan dentro de la mouseBox
	std::vector<Player> objetosSeleccionados;

	if (isMouseBox) {
		//printf("Player %i, %i, %i, %i\n", this->_player.getBoundingBox().getLeft(), this->_player.getBoundingBox().getTop(), this->_player.getBoundingBox().getRight(), this->_player.getBoundingBox().getBottom());
		//printf ("Coords Antes %i, %i, %i, %i\n", starting_position.x, starting_position.y, current_position.x, current_position.y);
		//printf ("Coords Despues %f, %f, %f, %f\n", startingPositionWorldPos.x, startingPositionWorldPos.y, currentPositionWorldPos.x, currentPositionWorldPos.y);

		if (this->_player.checkColisionObjetos(Rectangle (startingPositionWorldPos.x, startingPositionWorldPos.y,
				currentPositionWorldPos.x - startingPositionWorldPos.x, currentPositionWorldPos.y- startingPositionWorldPos.y))) {

			//this->_player.handleSeleccion();
			playerSelected = true;
			playerBox[0].position = sf::Vector2f(this->_player.getBoundingBox().getLeft(), this->_player.getBoundingBox().getTop());
			playerBox[0].color = sf::Color::Green;
			playerBox[1].position = sf::Vector2f(this->_player.getBoundingBox().getRight(), this->_player.getBoundingBox().getTop());
			playerBox[1].color = sf::Color::Green;
			playerBox[2].position = sf::Vector2f(this->_player.getBoundingBox().getRight(), this->_player.getBoundingBox().getBottom());
			playerBox[2].color = sf::Color::Green;
			playerBox[3].position = sf::Vector2f(this->_player.getBoundingBox().getLeft(), this->_player.getBoundingBox().getBottom());
			playerBox[3].color = sf::Color::Green;
			playerBox[4].position = sf::Vector2f(this->_player.getBoundingBox().getLeft(), this->_player.getBoundingBox().getTop());
			playerBox[4].color = sf::Color::Green;
		}
	}

	if (isMouseBox && this->_level._ayuntamiento.checkColision(Rectangle (startingPositionWorldPos.x, startingPositionWorldPos.y,
				currentPositionWorldPos.x - startingPositionWorldPos.x, currentPositionWorldPos.y- startingPositionWorldPos.y))) {
		ayuntamientoBox[0].position = sf::Vector2f(this->_level._ayuntamiento.getBoundingBox().getLeft(), this->_level._ayuntamiento.getBoundingBox().getTop());
		ayuntamientoBox[0].color = sf::Color::Green;
		ayuntamientoBox[1].position = sf::Vector2f(this->_level._ayuntamiento.getBoundingBox().getRight(), this->_level._ayuntamiento.getBoundingBox().getTop());
		ayuntamientoBox[1].color = sf::Color::Green;
		ayuntamientoBox[2].position = sf::Vector2f(this->_level._ayuntamiento.getBoundingBox().getRight(), this->_level._ayuntamiento.getBoundingBox().getBottom());
		ayuntamientoBox[2].color = sf::Color::Green;
		ayuntamientoBox[3].position = sf::Vector2f(this->_level._ayuntamiento.getBoundingBox().getLeft(), this->_level._ayuntamiento.getBoundingBox().getBottom());
		ayuntamientoBox[3].color = sf::Color::Green;
		ayuntamientoBox[4].position = sf::Vector2f(this->_level._ayuntamiento.getBoundingBox().getLeft(), this->_level._ayuntamiento.getBoundingBox().getTop());
		ayuntamientoBox[4].color = sf::Color::Green;

		printf("Dibujar iconos de acciones del ayuntamiento en vista Info\n");
	}


    /*
    //Check collisions
    std::vector<Rectangle> others;
    if((others = this->_level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0) {
        //Player collided with at least one tile. Handle it.
        this->_player.handleTileCollisions(others);
    }
    //Check slopes
    std::vector<Slope> otherSlopes;
    if ((otherSlopes = this->_level.checkSlopeCollisions(this->_player.getBoundingBox())).size() > 0) {
        this->_player.handleSlopeCollisions(otherSlopes);
    }
    //Check doors
    std::vector<Door> otherDoors;
    if ((otherDoors = this->_level.checkDoorCollisions(this->_player.getBoundingBox())).size() > 0) {
        this->_player.handleDoorCollisions(otherDoors, this->_level, this->_graphics);
    }
    //Check enemies
    std::vector<Enemy*> otherEnemies;
    if ((otherEnemies = this->_level.checkEnemyCollisions(this->_player.getBoundingBox())).size() > 0) {
        this->_player.handleEnemyCollisions(otherEnemies);
    }
    */
}
