#include "gamescene.h"
#include <string>
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>

using namespace std;

void readParameterValue(std::ifstream &infile, unsigned &value) {
    std::string line;
    if (std::getline(infile, line)) {
        size_t equalSignPos = line.find('=');

        if (equalSignPos != std::string::npos) {
            std::string valueStr = line.substr(equalSignPos + 2);
            try {
                value = std::stoi(valueStr);
            } catch (const std::invalid_argument &e) {
                std::cerr << "Invalid argument: " << e.what() << std::endl;
            } catch (const std::out_of_range &e) {
                std::cerr << "Out of range: " << e.what() << std::endl;
            }
        }
    }
}

void initializeGameScene(GameScene &scene) {
    initializeField(scene.field);
    initializeMadDash(scene.madDash);
    initializeMenu(scene.menu);

    std::ifstream infile("C:/iSpring/C++ Game/CppGame/saves/save.txt");
    if (infile.is_open()) {
        readParameterValue(infile, scene.totalCoinsCount);
        readParameterValue(infile, scene.boost);

        infile.close();
    }

    std::string str = std::to_string(scene.totalCoinsCount);
    std::string str2 = std::to_string(scene.boost);

    initializeButton(scene.totalCoins, str, {1600.f, 60.f}, "../media/menu/coin_but.png", {1400.f, 60.f});
    scene.totalCoins.button.setScale(0.8f, 0.8f);
    initializeButton(scene.boostBut, str2, {1460.f, 160.f}, "../media/menu/boost_but.png", {1450.f, 140.f});
    scene.boostBut.button.setScale(0.8f, 0.8f);
}

void updateGameScene(GameScene &scene, float elapsedTime, sf::View &view, sf::RenderWindow &window) {
    std::ofstream outfile("C:/iSpring/C++ Game/CppGame/saves/save.txt");

    if (outfile.is_open()) {
        if (scene.gameState == GameState::Playing) {
            updateMadDash(scene.madDash, elapsedTime, scene.field, view, scene.boost);

            sf::FloatRect viewRect = sf::FloatRect(view.getCenter().x - view.getSize().x / 2,
                                                   view.getCenter().y - view.getSize().y / 2, view.getSize().x,
                                                   view.getSize().y);

            scene.totalCoinsCount += eatCoinsInBounds(scene.field, scene.madDash.shape.getGlobalBounds());

            scene.totalCoins.button.setPosition(viewRect.left + viewRect.width - 200.f,
                                                viewRect.top + viewRect.height - 698.f);
            scene.totalCoins.buttonText.setPosition(viewRect.left + viewRect.width - 150.f,
                                                    viewRect.top + viewRect.height - 678.f);

            scene.boostBut.button.setPosition(viewRect.left + viewRect.width - 150.f,
                                              viewRect.top + viewRect.height - 618.f);
            scene.boostBut.buttonText.setPosition(viewRect.left + viewRect.width - 140.f,
                                                  viewRect.top + viewRect.height - 598.f);

            std::string str = std::to_string(scene.totalCoinsCount);
            scene.totalCoins.buttonText.setString(str);

            std::string str2 = std::to_string(scene.boost);
            scene.boostBut.buttonText.setString(str2);

            if (!viewRect.intersects(scene.madDash.shape.getGlobalBounds())) {
                outfile << "lastCoins = " << scene.totalCoinsCount << std::endl;
                outfile << "boost = " << scene.boost << std::endl;
                outfile.close();

                scene.gameState = GameState::PlayerLose;
                scene.menu.empty.buttonText.setString("Lose");
            }
            for (size_t i = 0, n = scene.field.width * scene.field.height; i < n; i++) {
                const Cell &cell = scene.field.cells[i];

                if (cell.category == CellCategory::ROAD ||
                    cell.category == CellCategory::WALL ||
                    cell.category == CellCategory::COINS) {
                    continue;
                }

                sf::FloatRect newBounds = scene.madDash.shape.getGlobalBounds();
                sf::FloatRect blockBound = cell.bounds;

                if (cell.category == CellCategory::END && newBounds.intersects(blockBound)) {
                    outfile << "lastCoins = " << scene.totalCoinsCount << std::endl;
                    outfile << "boost = " << scene.boost << std::endl;
                    outfile.close();

                    scene.gameState = GameState::PlayerWon;
                    scene.menu.empty.buttonText.setString("Win");
                }
            }
        } else {

            view.reset(sf::FloatRect(0.f, 0.f, 1600.f, 758.f));

            std::string str = std::to_string(scene.totalCoinsCount);

            scene.totalCoins.buttonText.setString(str);
            scene.totalCoins.buttonText.setPosition(1450.f, 80.f);
            scene.totalCoins.button.setPosition(1400.f, 60.f);

            std::string str2 = std::to_string(scene.boost);

            scene.boostBut.buttonText.setString(str2);
            scene.boostBut.button.setPosition(1450.f, 140.f);
            scene.boostBut.buttonText.setPosition(1460.f, 160.f);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::FloatRect playButtonBounds = scene.menu.playButton.button.getGlobalBounds();
                sf::FloatRect shopButtonBounds = scene.menu.shopButton.button.getGlobalBounds();
                sf::FloatRect buyButtonBounds = scene.menu.buyButton.button.getGlobalBounds();
                sf::FloatRect backButtonBounds = scene.menu.empty.button.getGlobalBounds();

                sf::Vector2i pixelMousePos = sf::Mouse::getPosition(window);
                sf::Vector2f worldMousePos = window.mapPixelToCoords(pixelMousePos);

                if (playButtonBounds.contains(worldMousePos)) {
                    outfile << "lastCoins = " << scene.totalCoinsCount << std::endl;
                    outfile << "boost = " << scene.boost << std::endl;
                    outfile.close();

                    GameScene newScene;
                    newScene.gameState = GameState::Playing;
                    scene = newScene;
                    initializeGameScene(scene);
                }

                if (shopButtonBounds.contains(worldMousePos)) {
                    outfile << "lastCoins = " << scene.totalCoinsCount << std::endl;
                    outfile << "boost = " << scene.boost << std::endl;
                    outfile.close();

                    scene.gameState = GameState::Shop;
                    Menu newMenu;
                    scene.menu = newMenu;
                    initializeShop(scene.menu);
                }

                if (buyButtonBounds.contains(worldMousePos) && scene.gameState == GameState::Shop) {
                    outfile << "lastCoins = " << scene.totalCoinsCount << std::endl;

                    if (scene.totalCoinsCount >= 10) {
                        scene.boost++;
                        scene.totalCoinsCount -= 10;
                        outfile << "boost = " << scene.boost << std::endl;
                        outfile.close();
                    } else {
                        outfile << "boost = " << scene.boost << std::endl;
                        outfile.close();
                    }
                }

                if (backButtonBounds.contains(worldMousePos) && scene.gameState == GameState::Shop) {
                    outfile << "lastCoins = " << scene.totalCoinsCount << std::endl;
                    outfile << "boost = " << scene.boost << std::endl;
                    outfile.close();

                    GameScene newScene;
                    newScene.gameState = GameState::Start;
                    scene = newScene;
                    initializeGameScene(scene);
                }
            }
        }
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }
}

void drawGameScene(sf::RenderWindow &window, GameScene &scene, float elapsedTime) {
    switch (scene.gameState) {
        case GameState::Playing:
            drawField(window, scene.field, elapsedTime);
            drawMadDash(window, scene.madDash);
            window.draw(scene.totalCoins.button);
            window.draw(scene.totalCoins.buttonText);

            window.draw(scene.boostBut.button);
            window.draw(scene.boostBut.buttonText);
            break;
        case GameState::PlayerWon:
        case GameState::PlayerLose:
        case GameState::Start:
            drawMenu(window, scene.menu);
            window.draw(scene.totalCoins.button);
            window.draw(scene.totalCoins.buttonText);

            window.draw(scene.boostBut.button);
            window.draw(scene.boostBut.buttonText);
            break;
        case GameState::Shop:
            drawMenu(window, scene.menu);
            window.draw(scene.totalCoins.button);
            window.draw(scene.totalCoins.buttonText);

            window.draw(scene.boostBut.button);
            window.draw(scene.boostBut.buttonText);
            break;
    }
}
