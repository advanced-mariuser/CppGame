#include "gamescene.h"
#include "utils.h"
#include "iostream"
#include <cassert>
#include <SFML/Graphics.hpp>

using namespace std;

static const sf::Color TRANSPARENT_GRAY = sf::Color(200, 200, 200, 200);

static unsigned getRemainingCoins(const GameScene &scene) {
    return scene.totalCoinsCount + scene.madDash.takenMoney;
}

static void updateGameOverLabel(sf::Text &label, const std::string &text, const sf::Vector2f &vec) {
    label.setString(text);
    utils::centerizeTextOrigin(label, vec);
}

void initializeGameScene(GameScene &scene, const sf::Vector2f &sceneSize) {
    bool succeed = scene.arial.loadFromFile("../media/arialn.ttf");
    if (!succeed) {
        assert(false);
        exit(1);
    }

    initializeField(scene.field);
    initializeMadDash(scene.madDash);

    scene.gameState = GameState::Playing;
    scene.totalCoinsCount = 0;

    scene.gameOverBackground.setFillColor(TRANSPARENT_GRAY);
    scene.gameOverBackground.setSize(sceneSize);

    scene.gameOverLabel.setFont(scene.arial);
    scene.gameOverLabel.setFillColor(sf::Color::Black);
    scene.gameOverLabel.setPosition(0.5f * sceneSize);
    utils::centerizeTextOrigin(scene.gameOverLabel, sceneSize);
}

void destroyGameScene(GameScene &scene) {
    destroyField(scene.field);
}

void updateGameScene(GameScene &scene, float elapsedTime, sf::View &view) {
    if (scene.gameState == GameState::Playing) {
        updateMadDash(scene.madDash, elapsedTime, scene.field, view);

        // Контейнер std::map при обходе с range-based for
        //  последовательно возвращает пары (ключ, значение),
        //  упакованные в контейнер std::pair.
        // pair имеет тип `std::pair<const GhostId, Ghost> &`

        for (size_t i = 0, n = scene.field.width * scene.field.height; i < n; i++) {
            const Cell &cell = scene.field.cells[i];

            if (cell.category == CellCategory::ROAD ||
                cell.category == CellCategory::WALL ||
                cell.category == CellCategory::COINS)
            {
                continue;
            }

            sf::FloatRect newBounds = scene.madDash.shape.getGlobalBounds();
            sf::FloatRect blockBound = cell.bounds;

            if (cell.category == CellCategory::END && newBounds.intersects(blockBound))
            {
                scene.gameOverBackground.setPosition(view.getCenter().x - view.getSize().x/2, view.getCenter().y - view.getSize().y/2);
                updateGameOverLabel(scene.gameOverLabel, "End!", scene.gameOverBackground.getSize());
                scene.gameState = GameState::PlayerWon;
            }
        }

        sf::FloatRect viewRect = sf::FloatRect(view.getCenter().x - view.getSize().x/2, view.getCenter().y - view.getSize().y/2 , view.getSize().x, view.getSize().y);

        if (!viewRect.intersects(scene.madDash.shape.getGlobalBounds()))
        {
            scene.gameOverBackground.setPosition(view.getCenter().x - view.getSize().x/2, view.getCenter().y - view.getSize().y/2);
            updateGameOverLabel(scene.gameOverLabel, "Lose!", scene.gameOverBackground.getSize());
            scene.gameState = GameState::PlayerLose;
        }
    }

    // Проверяем условие поражения - столкновение пакмана и призрака.
}

std::string getGameSceneWindowTitle(const GameScene &scene) {
    std::string title;
    switch (scene.gameState) {
        case GameState::Playing: {
            unsigned coinsLeft = getRemainingCoins(scene);
            title = "MadDash: " + std::to_string(coinsLeft) + " Coins";
        }
            break;
        case GameState::PlayerWon:
            title = "MadDash: Congratulations, You Won!";
            break;
        case GameState::PlayerLose:
            title = "MadDash: Unfortunately, You Lose";
            break;
    }

    return title;
}

void drawGameScene(sf::RenderWindow &window, const GameScene &scene, float elapsedTime) {
    drawField(window, scene.field, elapsedTime);

    // Персонажи рисуются после поля.
    drawMadDash(window, scene.madDash);

    if (scene.gameState == GameState::PlayerLose) {
        window.draw(scene.gameOverBackground);
        window.draw(scene.gameOverLabel);
    }
}
