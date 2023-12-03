#pragma once

#include "madDash.h"
#include "field.h"
#include <map>
#include <SFML/Graphics.hpp>

enum class GameState
{
    Playing,
    PlayerLose,
    PlayerWon,
};

struct GameScene
{
    Field field;
    MadDash madDash;
    GameState gameState = GameState::Playing;
    unsigned totalCoinsCount;

    sf::Font arial;
    sf::RectangleShape gameOverBackground;
    sf::Text gameOverLabel;
};

void initializeGameScene(GameScene &scene, const sf::Vector2f &sceneSize);
void updateGameScene(GameScene &scene, float elapsedTime, sf::View &view);
static void updateGameOverLabel(sf::Text &label, const std::string &text, const sf::Vector2f &vec);
std::string getGameSceneWindowTitle(const GameScene &scene);
void drawGameScene(sf::RenderWindow &window, const GameScene &scene, float elapsedTime);
