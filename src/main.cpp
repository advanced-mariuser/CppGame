#include "GameScene/gamescene.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>

void handleEvents(sf::RenderWindow &window, GameScene &scene) {

    sf::Event event{};
    while (window.pollEvent(event)) {
        switch (event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
        }
    }
}

void update(float elapsedTime, GameScene& scene, sf::View &view, sf::RenderWindow &window) {
    updateGameScene(scene, elapsedTime, view, window);
}

void render(sf::RenderWindow &window, GameScene& scene, sf::View &view, float elapsedTime) {
    window.clear();
    drawGameScene(window, scene, elapsedTime);
    window.setView(view);
    window.display();
}

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Mad Dash", sf::Style::Default, settings);

    sf::Image icon;
    if (!icon.loadFromFile("../media/icon.png")) {
        exit(1);
    }

    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    sf::Clock clock;
    sf::View view(sf::FloatRect(0.f, 0.f, 1600.f, 758.f));
    window.setView(view);
    window.setTitle("Mad-Dash");

    GameScene scene;
    scene.gameState = GameState::Start;
    initializeGameScene(scene);

    while (window.isOpen()) {
        handleEvents(window, scene);
        const float elapsedTime = clock.getElapsedTime().asSeconds();
        clock.restart();
        update(elapsedTime, scene, view, window);

        render(window, scene, view, elapsedTime);
    }

    std::ofstream outfile("C:/iSpring/C++ Game/CppGame/saves/save.txt");
    if (outfile.is_open()) {
        outfile << "lastCoins = " << scene.totalCoinsCount << std::endl;
        outfile << "boost = " << scene.boost << std::endl;
        outfile.close();
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }

    return 0;
}
