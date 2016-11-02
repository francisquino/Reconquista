#include "input.h"
#include "globals.h"

/* Input class
 * Keeps track of keyboard state
 */

//This function gets called at the beginning of each new frame
//to reset the keys that are no longer relevant
void Input::beginNewFrame() {
    this->_pressedKeys.clear();
    this->_releasedKeys.clear();

    this->_pressedMouseButtons.clear();
    this->_releasedMouseButtons.clear();
}

//This gets called when a key has been pressed
void Input::keyDownEvent(const sf::Event& event) {
    this->_pressedKeys[event.key.code] = true;
    this->_heldKeys[event.key.code] = true;
}

//This gets called when a key is released
void Input::keyUpEvent(const sf::Event& event) {
    this->_releasedKeys[event.key.code] = true;
    this->_heldKeys[event.key.code] = false;
}

//This gets called when a mouse button has been pressed
void Input::mouseButtonDownEvent(const sf::Event& event) {
    this->_pressedMouseButtons [event.mouseButton.button] = true;
    this->_heldMouseButtons[event.mouseButton.button] = true;
}

//This gets called when a mouse button is released
void Input::mouseButtonUpEvent(const sf::Event& event) {
    this->_releasedMouseButtons[event.mouseButton.button] = true;
    this->_heldMouseButtons[event.mouseButton.button] = false;
}

//Check if a certain key was pressed during the current frame
bool Input::wasKeyPressed(sf::Keyboard::Key key) {
    return this->_pressedKeys[key];
}

//Check if a certain key was released during the current frame
bool Input::wasKeyReleased(sf::Keyboard::Key key) {
    return this->_releasedKeys[key];
}

//Check if a certain key is currently being held
bool Input::isKeyHeld(sf::Keyboard::Key key) {
    return this->_heldKeys[key];
}

//Check if a certain mouse button was pressed during the current frame
bool Input::wasMouseButtonPressed(sf::Mouse::Button boton) {
	return this->_pressedMouseButtons[boton];
}

//Check if a certain mouse button was released during the current frame
bool Input::wasMouseButtonReleased(sf::Mouse::Button boton) {
    return this->_releasedMouseButtons[boton];
}

//Check if a certain mouse button is currently being held
bool Input::isMouseButtonHeld(sf::Mouse::Button boton) {
    return this->_heldMouseButtons[boton];
}

//Comprobar si coordenadas estan dentro de vista Minimapa
bool Input::dentroVistaMinimapa(const sf::Vector2i coord) {
	if (coord.x >= globals::vistaMinimapaX1 && coord.x <= globals::vistaMinimapaX2 &&
		coord.y >= globals::vistaMinimapaY1 && coord.y <= globals::vistaMinimapaY2)
		return true;
	else return false;
}

//Comprobar si coordenadas estan dentro de vista Info
bool Input::dentroVistaInfo(const sf::Vector2i coord) {
	if (coord.x >= globals::vistaInfoX1 && coord.x <= globals::vistaInfoX2 &&
		coord.y >= globals::vistaInfoY1 && coord.y <= globals::vistaInfoY2)
		return true;
	else return false;
}

//Comprobar si coordenadas estan dentro de vista Juego
bool Input::dentroVistaJuego(const sf::Vector2i coord) {
	if (coord.x >= globals::vistaJuegoX1 && coord.x <= globals::vistaJuegoX2 &&
		coord.y >= globals::vistaJuegoY1 && coord.y <= globals::vistaJuegoY2)
		return true;
	else return false;
}

//Devuelve true si las coordenadas están sobre un objeto
bool Input::sobre(const sf::Vector2i coord, Rectangle boundingBox) {
	if (coord.x >= boundingBox.getLeft() && coord.x <= boundingBox.getRight() &&
		coord.y >= boundingBox.getTop() && coord.y <= boundingBox.getBottom())
		return true;
	else return false;
}
