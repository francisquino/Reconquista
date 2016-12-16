#include "graphics.h"
#include "globals.h"


/* Graphics class
 * Guarda toda la informacion relativa a los graficos para el juego.
 */

Graphics::Graphics() {
    _window = new sf::RenderWindow(sf::VideoMode(640, 480), "Reconquista", sf::Style::Close);
    _window->setMouseCursorVisible(false); // Ocultar cursor

    //Preparar cursor para el ratón
    _spriteCursorLupa = Sprite(*this, "content/sprites/Cursores.png", 0, 0, 16, 16, 0, 0);
    _spriteCursorPuntero = Sprite(*this, "content/sprites/Cursores.png", 16, 0, 16, 16, 0, 0);
    _spriteCursorRecolectar = Sprite(*this, "content/sprites/Cursores.png", 32, 0, 16, 16, 0, 0);

    //Definir las vistas
    _vistaCompleta = _window->getView();

    _vistaJuego.reset(sf::FloatRect(0, 0, globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT));
    _vistaJuego.setViewport(sf::FloatRect(0.25f, 0, 1.f, 1.f));

    //_vistaInJuego.reset(sf::FloatRect(0.25f, 0, globals::SCREEN_WIDTH*.75f, globals::SCREEN_HEIGHT));
    //_vistaInJuego.reset(sf::FloatRect(176, 16, 448, 448));
    _vistaInJuego.reset(sf::FloatRect(132, 12, 336, 336));
    //_vistaInJuego.reset(sf::FloatRect(88, 8, 224, 224));
    _vistaInJuego.setViewport(sf::FloatRect(0.25f+0.05f, 0+0.067f, 0.75f-(0.05f*2), 1.f-(0.067f*2))); //0.05f -> Ancho tile de 16 px. 0.067 -> Alto

    _vistaMinimapa.reset(sf::FloatRect(0, 0, 50*16*globals::SPRITE_SCALE, 50*16*globals::SPRITE_SCALE)); //(0, 0, 50 tiles x 16 x factor escalado, 50 tiles x 16 x factor escalado)
    _vistaMinimapa.setViewport(sf::FloatRect(0, 0, 0.25f, 0.25f));

    _vistaInfo.reset(sf::FloatRect(0, 0, 120*globals::SPRITE_SCALE, 340*globals::SPRITE_SCALE));
    _vistaInfo.setViewport(sf::FloatRect(0, 0.25f, 0.25f, 1.f));
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

sf::View* Graphics::getView(const Vista vista) {
	switch (vista) {
		case Completa:
			return &_vistaCompleta;
			break;
		case Juego:
			return &_vistaJuego;
			break;
		case InJuego:
			return &_vistaInJuego;
			break;
		case Minimapa:
			return &_vistaMinimapa;
			break;
		case Info:
			return &_vistaInfo;
			break;
		default:
			return NULL;
			break;
	}
}



