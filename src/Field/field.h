#pragma once

#include "../Player/maddash.h"
#include <SFML/Graphics.hpp>

enum struct CellCategory
{
    WALL,
    ROAD,
    COINS,
    END,
};

struct Cell
{
    CellCategory category;
    sf::FloatRect bounds;
};

struct Field
{
    size_t width = 0;
    size_t height = 0;
    Cell *cells = nullptr;
};

sf::Vector2f getMadDashStartPosition();

extern const char COIN_TEXTURE[];
extern const char ROAD_TEXTURE[];
extern const char WALL_TEXTURE[];

unsigned eatCoinsInBounds(const Field &field, const sf::FloatRect &bounds);

void initializeField(Field &field);
void drawField(sf::RenderWindow &window, const Field &field, float elapsedTime);

bool checkFieldWallsCollision(const Field &field, const sf::FloatRect &oldBounds, sf::Vector2f &movement, MadDash &maddash);

