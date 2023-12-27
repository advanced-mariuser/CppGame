#include "button.h"
#include <iostream>
#include <cassert>

void initializeButton(Button &button, const std::string &text, sf::Vector2f positionText, const std::string &filePath,
                      sf::Vector2f position) {
    bool succeed = button.newasletic.loadFromFile("../media/fonts/newasletic.ttf");
    if (!succeed) {
        assert(false);
        exit(1);
    }

    if (!button.buttonTexture.loadFromFile(filePath)) {
        exit(1);
    }

    button.button.setTexture(button.buttonTexture);
    button.button.setPosition(position.x, position.y);
    button.button.setScale(1.f, 1.f);

    if (positionText.x != 0.f && positionText.y != 0.f) {
        button.buttonText.setFont(button.newasletic);
        button.buttonText.setPosition(positionText);
    }

    if (!text.empty()) {
        button.buttonText.setFont(button.newasletic);
        button.buttonText.setString(text);
        button.buttonText.setFillColor(sf::Color::White);
    }
}