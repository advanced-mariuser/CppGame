#pragma once

#include <SFML/Graphics.hpp>

struct Field;

enum struct Direction {
    NONE,
    UP,
    DOWN,
    RIGHT,
    STOP,
    ONLYUP,
    ONLYDOWN,
};

struct MadDash {
    sf::Sprite shape;
    sf::Sprite fire;

    sf::Texture texture;
    sf::Texture firetexture;

    Direction direction;
    float speed;
};

extern const char BARRY_TEXTURE[];

void initializeMadDash(MadDash &maddash);
void updateMadDash(MadDash &maddash, float elapsedTime, const Field &field, sf::View &view, unsigned &boost);
void drawMadDash(sf::RenderWindow &window, MadDash &madDash);
