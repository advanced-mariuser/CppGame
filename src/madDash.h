#pragma once

#include <SFML/Graphics.hpp>

struct Field;

enum struct Direction
{
    NONE,
    UP,
    DOWN,
    RIGHT,
    STOP,
};

struct MadDash
{
    sf::CircleShape shape;
    Direction direction;
    unsigned takenMoney;
    sf::Texture texture;
};

extern const char BARRY_TEXTURE[];

void initializeMadDash(MadDash & maddash);
void updateMadDash(MadDash & maddash, float elapsedTime, const Field &field, sf::View &view);
void drawMadDash(sf::RenderWindow &window, const MadDash &madDash);
sf::FloatRect getMadDashBounds(const MadDash &maddash);
