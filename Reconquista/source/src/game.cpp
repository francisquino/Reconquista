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
#include "tipoestadojuego.h"
#include "animaciones.h"

/* Game class
 * This class holds all information for our main game loop
 */

namespace {
    const int FPS = 50;
    const int MAX_FRAME_TIME = 1000 / FPS;
}

enum tipoCursor {
	puntero,
	lupa,
	recolectar
};


extern Level _level;
extern tipoEstadoJuego _estadoJuego;

Graphics graphics;

//Posicion para centrar la vista Juego
sf::Vector2f position(0, 0);

bool isMouseBox = false;
bool _aytoSeleccionado = false;
Objeto* objetoSeleccionado = NULL;

sf::VertexArray mouseBox(sf::LinesStrip, 5);
//Posicion ratón
sf::Vector2i starting_position, current_position;
sf::Vector2f startingPositionWorldPos, currentPositionWorldPos;

Sprite destinoCruz;

tipoCursor _tipoCursor = puntero;


Animaciones _animaciones;

Game::Game() {
    this->gameLoop();

    //Borrar el mapa usado para las busquedas de rutas en este nivel
    delete[] _level.mapa[0];
    delete[] _level.mapa;
}

Game::~Game() {
}

void Game::gameLoop() {
    Input input;
    sf::Event event;


    _estadoJuego = _estInactivo;

    _level = Level("MapReconquista1", graphics);

    destinoCruz = Sprite(graphics, "content/sprites/Tile-set-Toen's Medieval Strategy.png", 48, 672, 16, 16, 0, 0);

    //this->_player = Player(graphics, _level.getPlayerSpawnPoint());
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

				//Cursor dentro de Ventana Juego
				if (input.dentroVistaJuego(starting_position)) {

					//Si estado juego esperandoPosición o esperando Recurso
					if (_estadoJuego == _estEsperandoPosicion || _estadoJuego == _estEsperandoRecurso) _tipoCursor = recolectar;

					//Si estado juego _estInactivo o recolectando
					if (_estadoJuego == _estInactivo || _estadoJuego==_estRecolectando) {
						_tipoCursor = puntero;


						graphics.getWindow().setView(*graphics.getView(Juego));
						//Si el ratón está sobre un objeto de la vista Juego, cambiar el cursor por una lupa
						if (input.cursorSobreObjeto((sf::Vector2i) graphics.getWindow().mapPixelToCoords(starting_position), &_level)) {
							_tipoCursor = lupa;
						}
					}
				} //Ventana Juego

				//Cursor dentro de Ventana Info
				else if (input.dentroVistaInfo(starting_position)) {
					//El cursor siempre será de tipo Puntero
					_tipoCursor = puntero;
				}
				//Cursor dentro de Ventana Minimapa
				else if (input.dentroVistaMinimapa(starting_position)) {
					//El cursor puede ser de tipo Puntero o Posicion, según el estado del juego
					if (_estadoJuego == _estInactivo) _tipoCursor = puntero;
					else if (_estadoJuego == _estEsperandoPosicion) _tipoCursor = recolectar;
				}
            } //Se desplaza el raton

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
        			graphics.getWindow().setView(*graphics.getView(Juego));

        			//Estado del juego _estInactivo o Recolectando: podemos seleccionar un Objeto
        			if (_estadoJuego == _estInactivo || _estadoJuego == _estRecolectando) {
        				//Estamos sobre un objeto
        				if (input.cursorSobreObjeto((sf::Vector2i) graphics.getWindow().mapPixelToCoords(starting_position), &(_level))) {
            				objetoSeleccionado = input.cursorSobreObjeto((sf::Vector2i) graphics.getWindow().mapPixelToCoords(starting_position), &(_level));
    						objetoSeleccionado->setSeleccionado(true);

							//El resto de objetos se deseleccionan
							_level.deseleccionarObjetos(objetoSeleccionado);
						}
        				//Hemos pinchado sobre ningun objeto
        				else {
        					//Si tenemos un objeto seleccionado, lo deseleccionamos
        					if (objetoSeleccionado!=NULL) {
        						objetoSeleccionado->setSeleccionado(false);
    							//El resto de objetos tambien se deseleccionan
    							_level.deseleccionarObjetos(objetoSeleccionado);
    							objetoSeleccionado=NULL;
        					}
        				}

        			} //Estado del juego _estInactivo

            		//Estado del juego Esperando Posicion
        			else if (_estadoJuego == _estEsperandoPosicion) {
						sf::Vector2i posicionDestino = (sf::Vector2i) graphics.getWindow().mapPixelToCoords(sf::Mouse::getPosition(graphics.getWindow()));
						posicionDestino.x = posicionDestino.x - (posicionDestino.x % (_level.getTileSize().x * (int)globals::SPRITE_SCALE));
						posicionDestino.y = posicionDestino.y - (posicionDestino.y % (_level.getTileSize().y * (int)globals::SPRITE_SCALE));

						//Calculamos la ruta a recorrer con el algoritmo A*
						//Primero pasamos las coordenadas del terreno a las del mapa
						sf::Vector2i origenMapa = _level.coordAMapa(objetoSeleccionado->getX(), objetoSeleccionado->getY());
						sf::Vector2i destinoMapa = _level.coordAMapa(posicionDestino.x, posicionDestino.y);

						std::string ruta = _level.pathFind(origenMapa.x, origenMapa.y, destinoMapa.x, destinoMapa.y);
						if (ruta.compare("")!=0) { //Si ruta es distinto de ""
							std::vector<sf::Vector2i> rutaSimple = _level.simplificarRutaCoord (_level.rutaACoordenadas (ruta, origenMapa), origenMapa);

							for (unsigned int i=0; i<rutaSimple.size(); i++) {
								sf::Vector2i destinoCoord = _level.mapaACoord(rutaSimple[i].x, rutaSimple[i].y);
								//Cada desplazamiento lo almacenamos en la ruta del objeto
								objetoSeleccionado->setDestino(destinoCoord);
							}

							destinoCruz.setX(posicionDestino.x);
							destinoCruz.setY(posicionDestino.y);
							_estadoJuego = _estInactivo;
						}
        			} //Estado del juego Esperando Posicion

            		//Estado del juego Esperando Recurso
        			else if (_estadoJuego == _estEsperandoRecurso) {
        				//Estamos sobre un recurso
        				Objeto* pObjeto = input.cursorSobreObjeto((sf::Vector2i) graphics.getWindow().mapPixelToCoords(starting_position), &(_level));
        				if (pObjeto!=NULL && (pObjeto->getTipo()==tipoObjeto::Bosque || pObjeto->getTipo()==tipoObjeto::Mina)) {
							//Hacemos Downcasting para poder trabajar con el ObjetoSeleccionado como un Campesino
        					dynamic_cast<Campesino*>(objetoSeleccionado)->setRecursoRecolectar(pObjeto);
							//printf("Empezamos a recolectar %s\n", tipoObjetoToStr(pObjeto->getTipo()).c_str());

        					sf::Vector2i posicionDestino = sf::Vector2i (pObjeto->getX(), pObjeto->getY());
    						posicionDestino.x = posicionDestino.x - (posicionDestino.x % (_level.getTileSize().x * (int)globals::SPRITE_SCALE));
    						posicionDestino.y = posicionDestino.y - (posicionDestino.y % (_level.getTileSize().y * (int)globals::SPRITE_SCALE));

    						//Calculamos la ruta a recorrer con el algoritmo A*
    						//Primero pasamos las coordenadas del terreno a las del mapa
    						sf::Vector2i origenMapa = _level.coordAMapa(objetoSeleccionado->getX(), objetoSeleccionado->getY());
    						sf::Vector2i destinoMapa = _level.coordAMapa(posicionDestino.x, posicionDestino.y);
    						//printf("Origen [%i,%i] Destino [%i,%i]\n", origenMapa.x, origenMapa.y, destinoMapa.x, destinoMapa.y);

    						//Como el destino es una posicion del mapa que sobre la que no podemos pasar,
    						//buscamos como destino la posicion accesible mas cercana
    						sf::Vector2i destinoAlcanzable = _level.posicionAccesibleMasCercana(origenMapa.x, origenMapa.y, destinoMapa.x, destinoMapa.y);
    						//printf ("Destino alcanzable [%i,%i]\n", destinoAlcanzable.x, destinoAlcanzable.y);

    						std::string ruta = _level.pathFind(origenMapa.x, origenMapa.y, destinoAlcanzable.x, destinoAlcanzable.y);

    						if (ruta.compare("")!=0) { //Si ruta es distinto de ""
    							std::vector<sf::Vector2i> rutaSimple = _level.simplificarRutaCoord (_level.rutaACoordenadas (ruta, origenMapa), origenMapa);

    							for (unsigned int i=0; i<rutaSimple.size(); i++) {
    								sf::Vector2i destinoCoord = _level.mapaACoord(rutaSimple[i].x, rutaSimple[i].y);
    								//Cada desplazamiento lo almacenamos en la ruta del objeto
    								objetoSeleccionado->setDestino(destinoCoord);
    							}
    	        			    destinoCruz.setX(destinoAlcanzable.x);
    	        			    destinoCruz.setY(destinoAlcanzable.y);
    	        			    _estadoJuego = _estRecolectando;
    						}
        				} //If (pObjeto sobre Bosque o Mina)
        				//No estamos sobre un recurso
        				else {
        					//Nos dirigimos al punto marcado
    						sf::Vector2i posicionDestino = (sf::Vector2i) graphics.getWindow().mapPixelToCoords(sf::Mouse::getPosition(graphics.getWindow()));
    						posicionDestino.x = posicionDestino.x - (posicionDestino.x % (_level.getTileSize().x * (int)globals::SPRITE_SCALE));
    						posicionDestino.y = posicionDestino.y - (posicionDestino.y % (_level.getTileSize().y * (int)globals::SPRITE_SCALE));

    						//Calculamos la ruta a recorrer con el algoritmo A*
    						//Primero pasamos las coordenadas del terreno a las del mapa
    						sf::Vector2i origenMapa = _level.coordAMapa(objetoSeleccionado->getX(), objetoSeleccionado->getY());
    						sf::Vector2i destinoMapa = _level.coordAMapa(posicionDestino.x, posicionDestino.y);

    						std::string ruta = _level.pathFind(origenMapa.x, origenMapa.y, destinoMapa.x, destinoMapa.y);
    						if (ruta.compare("")!=0) { //Si ruta es distinto de ""
    							std::vector<sf::Vector2i> rutaSimple = _level.simplificarRutaCoord (_level.rutaACoordenadas (ruta, origenMapa), origenMapa);

    							for (unsigned int i=0; i<rutaSimple.size(); i++) {
    								sf::Vector2i destinoCoord = _level.mapaACoord(rutaSimple[i].x, rutaSimple[i].y);
    								//Cada desplazamiento lo almacenamos en la ruta del objeto
    								objetoSeleccionado->setDestino(destinoCoord);
    							}

    							destinoCruz.setX(posicionDestino.x);
    							destinoCruz.setY(posicionDestino.y);
    						}
        				}
        			} //Estado del juego Esperando Recurso


            		/*
                	//Campesino seleccionado. Al pulsar con el boton izquierdo, fijamos un destino.
					if (objetoSeleccionado!=NULL && objetoSeleccionado->getTipo()==tipoObjeto::Campesino) {

						//Comprobar que objetos estan dentro de la mouseBox
						//if (isMouseBox) {
							//printf("Player %i, %i, %i, %i\n", this->_player.getBoundingBox().getLeft(), this->_player.getBoundingBox().getTop(), this->_player.getBoundingBox().getRight(), this->_player.getBoundingBox().getBottom());
							//printf ("Coords Antes %i, %i, %i, %i\n", starting_position.x, starting_position.y, current_position.x, current_position.y);
							//printf ("Coords Despues %f, %f, %f, %f\n", startingPositionWorldPos.x, startingPositionWorldPos.y, currentPositionWorldPos.x, currentPositionWorldPos.y);

							//if (this->_player.checkColisionObjetos(Rectangle (startingPositionWorldPos.x, startingPositionWorldPos.y,
							//		currentPositionWorldPos.x - startingPositionWorldPos.x, currentPositionWorldPos.y- startingPositionWorldPos.y))) {

								//this->_player.handleSeleccion();
							//}
						//}

					}

					//if (isMouseBox) {
					//	isMouseBox = false;
					//}
					 *
					 */
            	} //Cursor dentro de vista Juego

            	//
            	//Cursor dentro de la vista Minimapa
            	//
            	else if (input.dentroVistaMinimapa(starting_position)) {
            		printf ("Minimapa\n");
            	} //Cursor dentro de vista Minimapa

            	//
            	//Cursor dentro de la vista Info
            	//
            	else if (input.dentroVistaInfo(starting_position)) {
					graphics.getWindow().setView(*graphics.getView(Info));
					//
            		//Ayuntamiento seleccionado:
					//
					if (_level._ayuntamiento->getSeleccionado()) {
                		//Si pulsamos sobre la acción "Entrenar Campesino"
            			if (input.sobre((sf::Vector2i) graphics.getWindow().mapPixelToCoords(sf::Mouse::getPosition(graphics.getWindow())),
								this->_info.getIconoAytoEntrenarCampesino()->getBoundingBox()))
            			{
            				//Hacemos Upcasting, para que al llamar al update ejecute la función de Campesino y no la de Objeto.
                            Objeto* campesino = new Campesino(graphics, sf::Vector2i(_level._ayuntamiento->getX()+32*_level._ayuntamiento->_unidades.size(),
                            														_level._ayuntamiento->getY()-32));
            				_level._ayuntamiento->sumarUnidad(campesino);

            				//Comprobar que cumnplimos con requisitos (recursos)
            				//Si se puede, lanzar CrearCampesino para este Ayto.
            				//Borrar iconos acciones de ayto.
            				//Dibujar la barra de progreso de creación
            			} //Entrenar campesino

            		} //Ayuntamiento seleccionado
					//
            		//Campesino seleccionado:
					//
					else if (objetoSeleccionado!=NULL && objetoSeleccionado->getTipo()==tipoObjeto::Campesino) {
            			//Acción "Ir a"
            			if (input.sobre((sf::Vector2i) graphics.getWindow().mapPixelToCoords(sf::Mouse::getPosition(graphics.getWindow())),
								this->_info.getIconoCampIrA()->getBoundingBox())) {
            				//El estado del juego pasa a Esperando Posicion
            				_estadoJuego = _estEsperandoPosicion;
            				//Enviamos un mensaje al Campesino para que entre en estado de Ir a un destino
            				Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY, //time delay
            										NULL,				 //Objeto* sender
													objetoSeleccionado,   //Objeto* recipient
													_msjIrA,			//the message
													NULL);  			//Informacion extra
            			} //Ir a

                		//Si pulsamos sobre la acción "Recolectar"
            			else if (input.sobre((sf::Vector2i) graphics.getWindow().mapPixelToCoords(sf::Mouse::getPosition(graphics.getWindow())),
								this->_info.getIconoCampRecolectar()->getBoundingBox()))
            			{
            				//El estado del juego pasa a Esperar Recurso
            				_estadoJuego = _estEsperandoRecurso;
            				//Enviamos un mensaje al Campesino para que entre en estado de recolectar
            				Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY, //time delay
            										NULL,				 //Objeto* sender
													objetoSeleccionado,   //Objeto* recipient
													_msjRecolectar,		//the message
													NULL);				//Informacion extra
            			} //Recolectar
            		} //Campesino seleccionado
            	} //Cursor dentro de vita Info
            } //Se pulsa el botón izquierdo

            //Pulsamos el botón izquierdo mientras desplazamos el ratón
            /*
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
            */
        }
        if (input.wasKeyPressed(sf::Keyboard::Escape) == true) {
            return;
        }
        else if (input.isKeyHeld(sf::Keyboard::Left) == true) { //Movemos la vista juego a la izq
            //this->_player.moveLeft();
            position.x-=1;
        }
        else if (input.isKeyHeld(sf::Keyboard::Right) == true) { //Movemos la vista juego a la der
            //this->_player.moveRight();
            position.x+=1;
        }
        if (input.isKeyHeld(sf::Keyboard::Up) == true) { //Movemos la vista juego arriba
            //this->_player.lookUp();
            position.y-=1;
        }
        else if (input.isKeyHeld(sf::Keyboard::Down) == true) { //Movemos la vista juego abajo
            //this->_player.lookDown();
            position.y+=1;
        }

        /*
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

    _level.draw(graphics);
    graphics.getWindow().setView(*graphics.getView(Juego));
    //this->_player.draw(graphics);

    //Si hay destino fijado, dibujarlo
    if (objetoSeleccionado && objetoSeleccionado->getDestinoX() && objetoSeleccionado->getDestinoY()) {
        graphics.getWindow().setView(*graphics.getView(Juego));
    	destinoCruz.draw(graphics, destinoCruz.getX(), destinoCruz.getY());
    }

    //Dibujar caja click & drag raton
    graphics.getWindow().setView(*graphics.getView(Juego));
    graphics.getWindow().draw(mouseBox);


    //Centrar vista Juego según la posición del ayuntamiento inicialmente
    if (position.x==0.f && position.y==0.f)
    	position = sf::Vector2f(_level._ayuntamiento->getX(), _level._ayuntamiento->getY());
    //else if (objetoSeleccionado!=NULL)
    //	position = sf::Vector2f(objetoSeleccionado->getX(), objetoSeleccionado->getY());

    //Obtener el punto medio de _vistaJuego
    float viewMitadX = graphics.getView(Juego)->getSize().x * 0.5;
    float viewMitadY = graphics.getView(Juego)->getSize().y * 0.5;
    //Obtener tamaño del mapa
    int sizeMapaX = _level.getMapSize().x * _level.getTileSize().x * globals::SPRITE_SCALE;
    int sizeMapaY = _level.getMapSize().y * _level.getTileSize().y * globals::SPRITE_SCALE;
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

    _animaciones.draw(graphics);

    graphics.getWindow().setView(*graphics.getView(Juego));

    graphics.flip();
}

void Game::update(float elapsedTime) {
    //this->_player.update(elapsedTime);

	_level.update(elapsedTime);
    this->_info.update(elapsedTime);

    _animaciones.update(elapsedTime);

    /*if (isMouseBox && _level._ayuntamiento.checkColision(Rectangle (startingPositionWorldPos.x, startingPositionWorldPos.y,
				currentPositionWorldPos.x - startingPositionWorldPos.x, currentPositionWorldPos.y- startingPositionWorldPos.y))) {

		//printf("Dibujar iconos de acciones del ayuntamiento en vista Info\n");
	}*/


    /*
    //Check collisions
    std::vector<Rectangle> others;
    if((others = _level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0) {
        //Player collided with at least one tile. Handle it.
        this->_player.handleTileCollisions(others);
    }
    //Check slopes
    std::vector<Slope> otherSlopes;
    if ((otherSlopes = _level.checkSlopeCollisions(this->_player.getBoundingBox())).size() > 0) {
        this->_player.handleSlopeCollisions(otherSlopes);
    }
    //Check doors
    std::vector<Door> otherDoors;
    if ((otherDoors = _level.checkDoorCollisions(this->_player.getBoundingBox())).size() > 0) {
        this->_player.handleDoorCollisions(otherDoors, _level, this->_graphics);
    }
    //Check enemies
    std::vector<Enemy*> otherEnemies;
    if ((otherEnemies = _level.checkEnemyCollisions(this->_player.getBoundingBox())).size() > 0) {
        this->_player.handleEnemyCollisions(otherEnemies);
    }
    */

    //dispatch any delayed messages
    Dispatcher->DispatchDelayedMessages();

}
