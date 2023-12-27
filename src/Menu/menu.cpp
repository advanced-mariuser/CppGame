#include "menu.h"
#include <iostream>

const char MENU_TEXTURE[] = "../media/menu/menu.png";

void initializeMenu(Menu &menu) {
    if (!menu.mainMenuTexture.loadFromFile(MENU_TEXTURE)) {
        exit(1);
    }

    menu.mainMenu.setTexture(menu.mainMenuTexture);
    menu.mainMenu.scale(1.f, 0.91f);
    menu.mainMenu.setPosition(0.f, 0.f);

    initializeButton(menu.empty, "Hello!", {770.f, 220.f}, "../media/menu/empty_but.png", {680.f, 200.f});
    menu.empty.button.setScale(1.2f, 0.8f);
    initializeButton(menu.playButton, "", {0.f, 0.f}, "../media/menu/start_but.png", {680.f, 300.f});
    initializeButton(menu.shopButton, "", {0.f, 0.f}, "../media/menu/shop_but.png", {680.f, 400.f});
}

void initializeShop(Menu &menu) {
    if (!menu.mainMenuTexture.loadFromFile(MENU_TEXTURE)) {
        exit(1);
    }

    menu.mainMenu.setTexture(menu.mainMenuTexture);
    menu.mainMenu.scale(1.f, 0.91f);
    menu.mainMenu.setPosition(0.f, 0.f);

    initializeButton(menu.empty, "Back", {770.f, 220.f}, "../media/menu/empty_but.png", {680.f, 200.f});
    menu.empty.button.setScale(1.2f, 0.8f);
    initializeButton(menu.buyButton, "Boost = 10c", {740.f, 320.f}, "../media/menu/empty_but.png", {680.f, 300.f});
    menu.buyButton.button.setScale(1.2f, 0.8f);
}

void drawMenu(sf::RenderWindow &window, const Menu &menu) {
    window.draw(menu.mainMenu);
    window.draw(menu.empty.button);
    window.draw(menu.empty.buttonText);
    window.draw(menu.playButton.button);
    window.draw(menu.shopButton.button);
    window.draw(menu.buyButton.button);
    window.draw(menu.buyButton.buttonText);
}