#include <iostream>
#include <typeinfo>

#include <SFML/Graphics.hpp>
//#include <SFML/Window.hpp>

#include "game.h"
#include "graphics.h"
#include "input.h"    //Lectura de eventos
#include "objeto.h"
#include "telegrama.h"
#include "messagedispatcher.h"
#include "tipomensaje.h"

/* Game class
 * This class holds all information for our main game loop
 */

namespace {
    const int FPS = 50;
    const int MAX_FRAME_TIME = 1000 / FPS;
}

bool isMouseBox = false;
bool _aytoSeleccionado = false;
Objeto* objetoSeleccionado = NULL;

sf::VertexArray mouseBox(sf::LinesStrip, 5);
//Posicion ratón
sf::Vector2i starting_position, current_position;
sf::Vector2f startingPositionWorldPos, currentPositionWorldPos;

Sprite destinoCruz;

enum tipoCursor {
	puntero,
	lupa,
	recolectar
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
    //this->_player = Player(graphics, this->_level.getPlayerSpawnPoint());
    this->_info = InfoUser(graphics);
    //this->_hud = HUD(graphics, this->_player);


    sf::Int32 LAST_UPDATE_TIME = this->clock.getElapsedTime().asMilliseconds();
    //std::cout << "LAST_UPDATE_TIME " << LAST_UPDATE_TIME << "\n";

    //Start the game loop
    while(true) {
    	input.beginNewFrame();

        graphics.getWindow().setView(*graphics.getView(Completa));
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

            //
            //Se desplaza el raton
            //
            if (event.type == sf::Event::MouseMoved) {
            	starting_position = sf::Mouse::getPosition(graphics.getWindow());
            	//_tipoCursor = puntero;
            	graphics.getWindow().setView(*graphics.getView(Juego));
            	//Si el ratón está sobre un objeto de la vista Juego, cambiar el cursor por una lupa
				if (input.sobre((sf::Vector2i) graphics.getWindow().mapPixelToCoords(starting_position),
										this->_level._ayuntamiento->getBoundingBox())) {
            		_tipoCursor = lupa;
            	}
				else
				{
					//Recorrer las unidades del ayuntamiento
					for (unsigned int i=0; i<this->_level._ayuntamiento->_unidades.size(); i++) {
						if (input.sobre((sf::Vector2i) graphics.getWindow().mapPixelToCoords(starting_position),
										this->_level._ayuntamiento->_unidades.at(i)->getBoundingBox())) {
							_tipoCursor = lupa;
							break;
						}
					}
            	}
            }

            //
            //Se pulsa el boton izquierdo del raton
            //
            if( event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left )
            {
            	graphics.getWindow().setView(*graphics.getView(Completa));
            	starting_position = sf::Mouse::getPosition(graphics.getWindow());

            	//
            	//Cursor dentro de la vista Juego
            	//
            	if (input.dentroVistaJuego(starting_position)) {
            		//Si el cursor es Lupa (estamos sobre un objeto)
            		if (_tipoCursor == lupa) {
            			graphics.getWindow().setView(*graphics.getView(Juego));
            			//Estamos sobre Ayuntamiento
            			if (input.sobre((sf::Vector2i) graphics.getWindow().mapPixelToCoords(starting_position),
                    							this->_level._ayuntamiento->getBoundingBox())) {
            				//Mostrar en vista Info los datos del Ayuntamiento y Acciones

            				objetoSeleccionado = this->_level._ayuntamiento;
            				objetoSeleccionado->setSeleccionado(true);

            				//El resto de objetos se deseleccionan
							for (unsigned int i=0; i<this->_level._ayuntamiento->_unidades.size(); i++) {
								this->_level._ayuntamiento->_unidades.at(i)->setSeleccionado(false);
							}
            			}

            			//Estamos sobre otro objeto
            			else {
							//Recorrer las unidades del ayuntamiento
							for (unsigned int i=0; i<this->_level._ayuntamiento->_unidades.size(); i++) {
								if (input.sobre((sf::Vector2i) graphics.getWindow().mapPixelToCoords(starting_position),
												this->_level._ayuntamiento->_unidades.at(i)->getBoundingBox())) {
		            				objetoSeleccionado = this->_level._ayuntamiento->_unidades.at(i);
		            				objetoSeleccionado->setSeleccionado(true);
									break;
								}
								//El resto de unidades se deseleccionan
								else {
		            				this->_level._ayuntamiento->_unidades.at(i)->setSeleccionado(false);
								}
							}
							//El ayuntamiento también se deselecciona
            				this->_level._ayuntamiento->setSeleccionado(false);
            			}

            		}
            		//Cursor es Buscando recurso para recolectar y hay seleccionado un Campesino
            		else if (_tipoCursor==recolectar && objetoSeleccionado!=NULL && objetoSeleccionado->getTipo()==tipoObjeto::Campesino) {
            			//Recorrer los recursos de este nivel
						for (unsigned int i=0; i<this->_level._recursos.size(); i++) {
							if (input.sobre((sf::Vector2i) graphics.getWindow().mapPixelToCoords(starting_position),
											this->_level._recursos.at(i)->getBoundingBox())) {
								//El cursor está sobre un recurso. Fijarlo como destino para recolectar
								//Hacemos Downcasting para poder trabajar con el ObjetoSeleccionado como un Campesino
								//dynamic_cast<Campesino*>(objetoSeleccionado)->setRecolectando(true);
								//dynamic_cast<Campesino*>(objetoSeleccionado)->setDestinoRecoleccion(this->_level._recursos.at(i));
								printf("Empezamos a recolectar %i\n", this->_level._recursos.at(i)->getTipo());
								break;
							}
						}
						//Si no estamos sobre ningún recurso, parar la recoleccion.
            		}

                	//Campesino seleccionado. Al pulsar con el boton izquierdo, fijamos un destino.
					if (objetoSeleccionado!=NULL && objetoSeleccionado->getTipo()==tipoObjeto::Campesino) {
						sf::Vector2i posicion_destino;
						graphics.getWindow().setView(*graphics.getView(Juego));
						posicion_destino = sf::Mouse::getPosition(graphics.getWindow());
						objetoSeleccionado->setDestino(graphics.getWindow().mapPixelToCoords(posicion_destino).x, graphics.getWindow().mapPixelToCoords(posicion_destino).y);
						destinoCruz = Sprite(graphics, "content/sprites/Tile-set-Toen's Medieval Strategy.png", 48, 672, 16, 16, objetoSeleccionado->getDestinoX(), objetoSeleccionado->getDestinoY());
						graphics.getWindow().setView(*graphics.getView(Completa));


						//Comprobar que objetos estan dentro de la mouseBox
						/*
						if (isMouseBox) {
							//printf("Player %i, %i, %i, %i\n", this->_player.getBoundingBox().getLeft(), this->_player.getBoundingBox().getTop(), this->_player.getBoundingBox().getRight(), this->_player.getBoundingBox().getBottom());
							//printf ("Coords Antes %i, %i, %i, %i\n", starting_position.x, starting_position.y, current_position.x, current_position.y);
							//printf ("Coords Despues %f, %f, %f, %f\n", startingPositionWorldPos.x, startingPositionWorldPos.y, currentPositionWorldPos.x, currentPositionWorldPos.y);

							if (this->_player.checkColisionObjetos(Rectangle (startingPositionWorldPos.x, startingPositionWorldPos.y,
									currentPositionWorldPos.x - startingPositionWorldPos.x, currentPositionWorldPos.y- startingPositionWorldPos.y))) {

								//this->_player.handleSeleccion();
							}
						}*/

					}

					if (isMouseBox) {
						isMouseBox = false;
					}
            	}

            	//
            	//Cursor dentro de la vista Minimapa
            	//
            	else if (input.dentroVistaMinimapa(starting_position)) {
            		printf ("Minimapa\n");
            	}
            	//
            	//Cursor dentro de la vista Info
            	//
            	else if (input.dentroVistaInfo(starting_position)) {
					graphics.getWindow().setView(*graphics.getView(Info));
					//
            		//Ayuntamiento seleccionado:
					//
					if (this->_level._ayuntamiento->getSeleccionado()) {
                		//Si pulsamos sobre la acción "Entrenar Campesino"
            			if (input.sobre((sf::Vector2i) graphics.getWindow().mapPixelToCoords(sf::Mouse::getPosition(graphics.getWindow())),
								this->_info.getIconoAytoEntrenarCampesino()->getBoundingBox()))
            			{
            				//Hacemos Upcasting, para que al llamar al update ejecute la función de Campesino y no la de Objeto.
                            Objeto* campesino = new Campesino(graphics, sf::Vector2i(this->_level._ayuntamiento->getX()+32*this->_level._ayuntamiento->_unidades.size(),
                            														this->_level._ayuntamiento->getY()-32));
            				this->_level._ayuntamiento->sumarUnidad(campesino);

            				//Comprobar que cumnplimos con requisitos (recursos)
            				//Si se puede, lanzar CrearCampesino para este Ayto.
            				//Borrar iconos acciones de ayto.
            				//Dibujar la barra de progreso de creación
            			} //Entrenar campesino

            		}
					//
            		//Campesino seleccionado:
					//
					else if (objetoSeleccionado!=NULL && objetoSeleccionado->getTipo()==tipoObjeto::Campesino) {
                		//Si pulsamos sobre la acción "Recolectar"
            			if (input.sobre((sf::Vector2i) graphics.getWindow().mapPixelToCoords(sf::Mouse::getPosition(graphics.getWindow())),
								this->_info.getIconoCampRecolectar()->getBoundingBox()))
            			{

            				_tipoCursor=recolectar;

            				//Enviamos un mensaje al Campesino para que entre en estado de recolectar
            				Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY, //time delay
            										NULL,				 //Objeto* sender
													objetoSeleccionado,   //Objeto* recipient
													_msjRecolectarRecurso,//the message
													NULL);  //Informacion extra
            			} //Recolectar

            		}

            	}
            }

            //Pulsamos el botón izquierdo mientras desplazamos el ratón
            if( event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left) )
            {
            	graphics.getWindow().setView(*graphics.getView(Juego));
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
        /*else if (input.isKeyHeld(sf::Keyboard::Left) == true) {
            this->_player.moveLeft();
        }
        else if (input.isKeyHeld(sf::Keyboard::Right) == true) {
            this->_player.moveRight();
        }
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

        if (!input.isKeyHeld(sf::Keyboard::Left) && !input.isKeyHeld(sf::Keyboard::Right)) {
            this->_player.stopMoving();
        }
        */

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

    //Si hemos seleccionado el ayuntamiento, dibujar su estado y las acciones en la vista Info
    if (objetoSeleccionado!=NULL && objetoSeleccionado->getTipo()==tipoObjeto::Ayuntamiento) {
    	this->_info.drawAyuntamiento(graphics);
    }
    //Si hemos seleccionado un campesino, dibujar su estado y las acciones en la vista Info
    else if (objetoSeleccionado!=NULL && objetoSeleccionado->getTipo()==tipoObjeto::Campesino) {
    	this->_info.drawCampesino(graphics);
    }

    this->_level.draw(graphics);
    graphics.getWindow().setView(*graphics.getView(Juego));
    //this->_player.draw(graphics);

    //Si hay destino fijado, dibujarlo
    if (objetoSeleccionado!=NULL && objetoSeleccionado->getDestinoX() != -1 && objetoSeleccionado->getDestinoY() != -1) {
        graphics.getWindow().setView(*graphics.getView(Juego));
    	destinoCruz.draw(graphics, destinoCruz.getX(), destinoCruz.getY());
    }

    //Dibujar caja click & drag raton
    graphics.getWindow().setView(*graphics.getView(Juego));
    graphics.getWindow().draw(mouseBox);


    //Centrar vista Juego según la posición del ayuntamiento inicialmente, luego según el objeto seleccionado
    sf::Vector2f position(0, 0);
    if (objetoSeleccionado==NULL)
    	position = sf::Vector2f(this->_level._ayuntamiento->getX(), this->_level._ayuntamiento->getY());
    else
    	position = sf::Vector2f(objetoSeleccionado->getX(), objetoSeleccionado->getY());
    //Obtener el punto medio de _vistaJuego
    float viewMitadX = graphics.getView(Juego)->getSize().x * 0.5;
    float viewMitadY = graphics.getView(Juego)->getSize().y * 0.5;
    //Obtener tamaño del mapa
    int sizeMapaX = this->_level.getMapSize().x * this->_level.getTileSize().x * globals::SPRITE_SCALE;
    int sizeMapaY = this->_level.getMapSize().y * this->_level.getTileSize().y * globals::SPRITE_SCALE;
    //Comparamos la posicion de ayuntamiento con el mapa,
    //tomando como extremos los bordes del mapa menos la mitad de la vista,
    //para luego centrar la vista.
    if (position.x < viewMitadX) position.x = viewMitadX;
    else if (position.x > sizeMapaX - viewMitadX) position.x = sizeMapaX - viewMitadX;
    if (position.y < viewMitadY) position.y = viewMitadY;
    else if (position.y > sizeMapaY - viewMitadY) position.y = sizeMapaY - viewMitadY;
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
    	case recolectar:
    	    graphics.getCursorRecolectar().draw(graphics, sf::Mouse::getPosition(graphics.getWindow()).x, sf::Mouse::getPosition(graphics.getWindow()).y);
    		break;
    }
    //this->_hud.draw(graphics);

    graphics.flip();
}

void Game::update(float elapsedTime) {
    //this->_player.update(elapsedTime);

	this->_level.update(elapsedTime);
    this->_info.update(elapsedTime);

    /*if (isMouseBox && this->_level._ayuntamiento.checkColision(Rectangle (startingPositionWorldPos.x, startingPositionWorldPos.y,
				currentPositionWorldPos.x - startingPositionWorldPos.x, currentPositionWorldPos.y- startingPositionWorldPos.y))) {

		//printf("Dibujar iconos de acciones del ayuntamiento en vista Info\n");
	}*/


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

    //dispatch any delayed messages
    Dispatcher->DispatchDelayedMessages();

}
