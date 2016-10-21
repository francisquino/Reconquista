#ifndef INPUT_H
#define INPUT_H

#include <SFML/Window.hpp>
#include <map>

class Input {
public:
    void beginNewFrame();
    void keyUpEvent(const sf::Event& event);
    void keyDownEvent(const sf::Event& event);

	void mouseButtonDownEvent(const sf::Event& event);
	void mouseButtonUpEvent(const sf::Event& event);
	sf::Vector2f mouseMovedEvent(const sf::Event& event);

    bool wasKeyPressed(sf::Keyboard::Key key);
    bool wasKeyReleased(sf::Keyboard::Key key);
    bool isKeyHeld(sf::Keyboard::Key key);

    bool wasMouseButtonPressed(sf::Mouse::Button boton);
    bool wasMouseButtonReleased(sf::Mouse::Button boton);
    bool isMouseButtonHeld(sf::Mouse::Button boton);

private:
    std::map<sf::Keyboard::Key, bool> _heldKeys;
    std::map<sf::Keyboard::Key, bool> _pressedKeys;
    std::map<sf::Keyboard::Key, bool> _releasedKeys;

    std::map<sf::Mouse::Button, bool> _heldMouseButtons;
    std::map<sf::Mouse::Button, bool> _pressedMouseButtons;
    std::map<sf::Mouse::Button, bool> _releasedMouseButtons;

};

#endif // INPUT_H
