#include "graphics.h"


/* Graphics class
 * Guarda toda la informacion relativa a los graficos para el juego.
 */

Graphics::Graphics() {
    _window = new sf::RenderWindow(sf::VideoMode(640, 480), "Reconquista", sf::Style::Close);

    //Implementar view
    _fixed = _window->getView();
    //_view.reset(sf::FloatRect(0, 0, (_window->getSize().x * 0.5), (_window->getSize().y) * 0.5));
    _view.reset(sf::FloatRect(0, 0, 640, 480));
    _view.setViewport(sf::FloatRect(0, 0, 1.f, 1.f));
    //Fin implementar view
}

Graphics::~Graphics() {
    _window->close();
}

sf::Texture* Graphics::loadImage(const std::string &filePath) {
    if (this->_spriteSheets.count(filePath) == 0) {
        this->_spriteSheets[filePath] = new sf::Texture;
        this->_spriteSheets[filePath]->loadFromFile(filePath.c_str());
    }
    return this->_spriteSheets[filePath];
}

/*
 * Lo vamos a sustituir por una función de la clase Sprite, que dibujará desde una textura en ese sprite.
void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
	SDL_RenderCopy(this->_renderer, texture, sourceRectangle, destinationRectangle);
}
*/

void Graphics::flip() {
    _window->display();
}

void Graphics::clear() {
    _window->clear();
}
