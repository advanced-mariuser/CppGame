#pragma once
#include "madDash.h"
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

// `extern` означает, что переменная/константа есть, но размещение в памяти
//   и начальное значение указаны в другом участке кода.
extern const char COIN_TEXTURE[];
extern const char ROAD_TEXTURE[];

// Возвращает количество поглощённых монет.
unsigned eatAllCoinsInBounds(const Field &field, const sf::FloatRect &bounds);

void initializeField(Field &field);
void drawField(sf::RenderWindow &window, const Field &field, float elapsedTime);

bool checkFieldWallsCollision(const Field &field, const sf::FloatRect &oldBounds, sf::Vector2f &movement, MadDash &maddash);

void destroyField(Field &field);
