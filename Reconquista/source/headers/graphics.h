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

    //Implementar view
    sf::View* getMiView() { return &_view; }
    sf::View* getFixed() { return &_fixed; }
    //Fin implementar view


private:
    sf::RenderWindow* _window;
    //Implementar view
    sf::View _fixed;
    sf::View _view;
    //Fin implementar view

    std::map<std::string, sf::Texture*> _spriteSheets;
};

#endif	/* GRAPHICS_H */

