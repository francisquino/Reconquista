/*
 * File:   graphics.h
 * Author: francis
 *
 * Created on 15 de septiembre de 2016, 14:41
 */

#ifndef GRAPHICS_H
#define	GRAPHICS_H

/* Graphics class
 * Guarda toda la informacion relativa a los graficos para el juego.
 */
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

#include "sprite.h"

enum Vista {
	Completa,
	Juego,
	Minimapa,
	Info
};


class Graphics {
public:
    Graphics();
    ~Graphics();

    /* sf::Texture* LoadImage()
     * Loads an image into the _spriteSheets map if it doesn't already exist.
     * As a result, each image will only ever be loaded once
     * Returns the image from the map regardless of whether or not it was just loaded
     */
    sf::Texture* loadImage(const std::string &filePath);


	/* void blitSurface
	 * Draws a texture to a certain part of the screen
	 */
	//void blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle);
    //Se usa para dibujar desde una imagen a pantalla, con unas coordenadas de la imagen origen y unas coordenadas en el destino.
    //Lo vamos a sustituir por una función de la clase Sprite, que dibujará desde una textura en ese sprite.

    /* void flip
     * Renders everything to the screen
     */
    void flip();

    /* void clear
     * Clear the screen
     */
    void clear();

    /* sf::RenderWindow& getWindow
     * Returns the renderer window
     */
    sf::RenderWindow& getWindow() { return *_window; }

    sf::View* getView(const Vista vista);

    Sprite getCursorPuntero() { return _spriteCursorPuntero; }
    Sprite getCursorLupa() { return _spriteCursorLupa; }

private:
    sf::RenderWindow* _window;

    Sprite _spriteCursorPuntero, _spriteCursorLupa;

    sf::View _vistaCompleta;
    sf::View _vistaJuego;
    sf::View _vistaMinimapa;
    sf::View _vistaInfo;

    std::map<std::string, sf::Texture*> _spriteSheets;
};

#endif	/* GRAPHICS_H */

