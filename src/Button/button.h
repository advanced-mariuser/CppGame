#pragma once

#include "../Utils/utils.h"
#include <SFML/Graphics.hpp>

struct Button {
    sf::Sprite button;
    sf::Texture buttonTexture;

    sf::Font newasletic;
    sf::Text buttonText;
};

void initializeButton(Button &button, const std::string &text, sf::Vector2f positionText, const std::string &filePath,
                      sf::Vector2f position);