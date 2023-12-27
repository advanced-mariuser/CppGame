#pragma once

#include "../Button/button.h"
#include <SFML/Graphics.hpp>

struct Menu
{
    sf::Sprite mainMenu;
    sf::Texture mainMenuTexture;

    sf::Font arial;
    sf::Text gameOverLabel;

    Button empty;
    Button playButton;
    Button shopButton;
    Button buyButton;
};

extern const char MENU_TEXTURE[];

void initializeMenu(Menu &menu);
void initializeShop(Menu &menu);
void drawMenu(sf::RenderWindow &window, const Menu &menu);
