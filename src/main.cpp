#include "gamescene.h"
#include "windows.h"
#include <SFML/Graphics.hpp>

void handleEvents(sf::RenderWindow &window) {
    sf::Event event{};
    while (window.pollEvent(event)) {
        // Кнопка закрытия окна
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void update(float elapsedTime, GameScene& scene, sf::View &view) {
    updateGameScene(scene, elapsedTime, view);
}

void render(sf::RenderWindow &window, GameScene& scene, sf::View &view, float elapsedTime) {
    window.clear();
    drawGameScene(window, scene, elapsedTime);
    window.setView(view);
    window.display();
}

int main(int, char *[]) {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Mad Dash", sf::Style::Default, settings);

    sf::Image icon;
    if (!icon.loadFromFile("../media/icon.png")) {
        // Handle error
    }

    // Set the window's icon
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    sf::Clock clock;
    sf::View view(sf::FloatRect(0.f, 0.f, 1600.f, 758.f));
    window.setView(view);

    GameScene scene;
    initializeGameScene(scene, sf::Vector2f(window.getSize()));

    while (window.isOpen()) {
        handleEvents(window);
        const float elapsedTime = clock.getElapsedTime().asSeconds();
        clock.restart();
        update(elapsedTime, scene, view);
        window.setTitle(getGameSceneWindowTitle(scene));
        render(window, scene, view, elapsedTime);
    }

    return 0;
}
