#pragma once

#include "../Player/maddash.h"
#include "../Field/field.h"
#include "../Menu/menu.h"
#include "../Button/button.h"
#include <map>
#include <SFML/Graphics.hpp>

enum class GameState {
    Playing,
    PlayerLose,
    PlayerWon,
    Start,
    Shop,
};

struct GameScene {
    Field field;
    MadDash madDash;
    Menu menu;
    GameState gameState = GameState::Playing;
    unsigned totalCoinsCount;
    unsigned boost;

    Button totalCoins;
    Button boostBut;
};

void initializeGameScene(GameScene &scene);
void updateGameScene(GameScene &scene, float elapsedTime, sf::View &view, sf::RenderWindow &window);
void drawGameScene(sf::RenderWindow &window, GameScene &scene, float elapsedTime);
