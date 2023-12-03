#include "field.h"
#include "madDash.h"
#include "iostream"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <limits>
#include <cassert>

struct FieldGraphics {
    sf::RectangleShape roadShape;
    sf::RectangleShape endShape;
    sf::RectangleShape wallShape;
    sf::CircleShape coinShape;
    sf::Texture coinTexture;
    sf::Texture roadTexture;
    sf::Texture endTexture;
};

FieldGraphics graphics;

static const float BLOCK_SIZE = 30.f;
static const sf::Vector2i START_POINT = {100, 100};
static const float COIN_RADIUS = 12.5f;

static const float MIN_COIN_OVERLAP_AREA = 400.f;

static const float EPSILON = std::numeric_limits<float>::epsilon();
static const float MAX_SHIFT = 0.5f * BLOCK_SIZE;

static const size_t FIELD_WIDTH = 67;
static const size_t FIELD_HEIGHT = 23;

static const char FIELD_MAZE[] = "###################################################################"
                                 "#                                                                 E"
                                 "#                                                                 E"
                                 "#                                                                 E"
                                 "#@                                                                E"
                                 "#                                                                 E"
                                 "#                                            $                    E"
                                 "#                                                                 E"
                                 "#                       #####                                     E"
                                 "#                       #                                         E"
                                 "#                       #                                         E"
                                 "#               $                                                 E"
                                 "#                                                                 E"
                                 "#                                                                 E"
                                 "#                                                                 E"
                                 "#       $                                                         E"
                                 "#                                     ######     $                E"
                                 "#       #                                                         E"
                                 "#       #                                      #                  E"
                                 "#       #                                      #                  E"
                                 "#       #               #                                         E"
                                 "#       #               #                                         E"
                                 "###################################################################";

static const sf::Color WALL_COLOR = sf::Color(166, 191, 172);
static const sf::Color ROAD_COLOR = sf::Color(230, 232, 210);

static const sf::IntRect FRAME_COINS_LEFT(0, 0, 12, 12);
static const sf::IntRect FRAME_COINS_CENTER(12, 0, 12, 12);
static const sf::IntRect FRAME_COINS_RIGHT(24, 0, 12, 12);
static const sf::IntRect FRAME_COINS_RIGHTER(35, 0, 12, 12);

static const sf::IntRect FRAME_ROAD_LEFT(0, 0, 27, 38);
static const sf::IntRect FRAME_ROAD_CENTER(27, 0, 27, 38);
static const sf::IntRect FRAME_ROAD_RIGHT(54, 0, 27, 38);
static const sf::IntRect FRAME_ROAD_RIGHTER(81, 0, 27, 38);

// Размещаем переменные, указанные в заголовке как `extern`.
const char COIN_TEXTURE[] = "../media/coin.png";
const char ROAD_TEXTURE[] = "../media/background.png";

float animationMovement = 0.0f;

void initFieldGraphics(FieldGraphics &graphics) {
    if (!graphics.coinTexture.loadFromFile(COIN_TEXTURE)) {
        exit(1);
    }

    if (!graphics.endTexture.loadFromFile(ROAD_TEXTURE)) {
        exit(1);
    }

    graphics.wallShape.setFillColor(WALL_COLOR);
    graphics.wallShape.setSize({BLOCK_SIZE, BLOCK_SIZE});
    graphics.roadShape.setFillColor(ROAD_COLOR);
    graphics.roadShape.setSize({BLOCK_SIZE, BLOCK_SIZE});
    graphics.coinShape.setRadius(COIN_RADIUS);
    graphics.coinShape.setTexture(&graphics.coinTexture);
    graphics.coinShape.setTextureRect(FRAME_COINS_CENTER);
    graphics.endShape.setSize({BLOCK_SIZE, BLOCK_SIZE});
    graphics.endShape.setTexture(&graphics.endTexture);
}

static sf::FloatRect moveRect(const sf::FloatRect &rect, sf::Vector2f &offset) {
    return {rect.left + offset.x, rect.top + offset.y, rect.width, rect.height};
}

static float getArea(const sf::FloatRect &rect) {
    return rect.width * rect.height;
}

static float getBottom(const sf::FloatRect &rect) {
    return rect.top + rect.height;
}

static float getRight(const sf::FloatRect &rect) {
    return rect.left + rect.width;
}

static bool isBetween(float value, float minValue, float maxValue) {
    return (value >= minValue) && (value <= maxValue);
}

static Direction selectShiftDirection(float rightShift,
                                      float topShift, float bottomShift,
                                      float minShift, float maxShift, size_t i) {
    Direction result = Direction::NONE;
    float bestShift = FIELD_WIDTH * BLOCK_SIZE;
    if (isBetween(rightShift, minShift, maxShift) && (rightShift < bestShift)) {
        result = Direction::STOP;
        bestShift = rightShift;
    }
    if (isBetween(topShift, minShift, maxShift) && (topShift < bestShift)) {
        result = Direction::UP;
        bestShift = topShift;
    }
    if (isBetween(bottomShift, minShift, maxShift) && bottomShift < bestShift) {
        result = Direction::DOWN;
        bestShift = bottomShift;
    }

    return result;
}


// Находит символ '@' в исходной карте лабиринта.
sf::Vector2f getMadDashStartPosition() {
    for (size_t y = 0; y < FIELD_HEIGHT; y++) {
        for (size_t x = 0; x < FIELD_WIDTH; x++) {
            const size_t offset = x + y * FIELD_WIDTH;
            if (FIELD_MAZE[offset] == '@') {
                return {x * BLOCK_SIZE + START_POINT.x, y * BLOCK_SIZE + START_POINT.y};
            }
        }
    }
    return {0, 0};
}

void initializeField(Field &field) {
    initFieldGraphics(graphics);
    field.width = FIELD_WIDTH;
    field.height = FIELD_HEIGHT;
    field.cells = new Cell[field.width * field.height];
    for (size_t y = 0; y < field.height; y++) {
        for (size_t x = 0; x < field.width; x++) {
            const size_t offset = x + y * field.width;
            CellCategory category;
            sf::Color color;
            switch (FIELD_MAZE[offset]) {
                case ' ':
                    category = CellCategory::ROAD;
                    break;
                case '#':
                    category = CellCategory::WALL;
                    break;
                case '$':
                    category = CellCategory::COINS;
                    break;
                case 'E':
                    category = CellCategory::END;
                    break;
            }
            Cell &cell = field.cells[offset];
            cell.category = category;
            cell.bounds.left = x * BLOCK_SIZE;
            cell.bounds.top = y * BLOCK_SIZE;
            cell.bounds.width = BLOCK_SIZE;
            cell.bounds.height = BLOCK_SIZE;
        }
    }
}

unsigned eatAllCoinsInBounds(const Field &field, const sf::FloatRect &bounds) {
    unsigned coinsCount = 0;
    for (size_t i = 0, n = field.width * field.height; i < n; i++) {
        Cell &cell = field.cells[i];
        if (cell.category != CellCategory::COINS) {
            continue;
        }
        sf::FloatRect intersection;
        // Нужно не просто пересекаться с печеньем, но и иметь
        // достаточную площадь пересечения.
        if (cell.bounds.intersects(bounds, intersection)
            && (getArea(intersection) >= MIN_COIN_OVERLAP_AREA)) {
            ++coinsCount;
            cell.category = CellCategory::ROAD;
        }
    }
    return coinsCount;
}

void drawField(sf::RenderWindow &window, const Field &field, float elapsedTime) {
    const float animation = 8.f * elapsedTime;
    animationMovement += animation;

    for (size_t i = 0; i < field.width * field.height; i++) {
        const Cell &cell = field.cells[i];
        const sf::Vector2f position = {cell.bounds.left, cell.bounds.top};
        const sf::Vector2f center = position
                                    + sf::Vector2f(0.5f * cell.bounds.width, 0.5f * cell.bounds.height);
        switch (cell.category) {
            case CellCategory::WALL:
                graphics.wallShape.setPosition(position);
                window.draw(graphics.wallShape);
                break;
            case CellCategory::ROAD:
                graphics.roadShape.setPosition(position);
                window.draw(graphics.roadShape);
                break;
            case CellCategory::COINS:
                graphics.roadShape.setPosition(position);
                graphics.coinShape.setPosition(center.x - COIN_RADIUS, center.y - COIN_RADIUS);
                if (animationMovement <= 10.1f)
                {
                    if (animationMovement <= 2.5f) {
                        graphics.coinShape.setTextureRect(FRAME_COINS_LEFT);
                    };
                    if (animationMovement <= 5.0f && animationMovement >= 2.5f) {
                        graphics.coinShape.setTextureRect(FRAME_COINS_CENTER);
                    };
                    if (animationMovement <= 7.5f && animationMovement >= 5.0f) {
                        graphics.coinShape.setTextureRect(FRAME_COINS_RIGHT);
                    };
                    if (animationMovement <= 1.0f && animationMovement >= 7.5f) {
                        graphics.coinShape.setTextureRect(FRAME_COINS_RIGHTER);
                    };
                }
                else
                {
                    animationMovement = 0.0f;
                }

                window.draw(graphics.roadShape);
                window.draw(graphics.coinShape);
                break;
            case CellCategory::END:
                graphics.endShape.setPosition(position);
                window.draw(graphics.endShape);
                break;
        }
    }
}

// Модифицирует вектор перемещения, избегая столкновения
// прямоугольника `rect` со стенами лабиринта в поле `field`.
// Возвращает `true`, если вектор перемещения изменён.
bool
checkFieldWallsCollision(const Field &field, const sf::FloatRect &oldBounds, sf::Vector2f &movement, MadDash &maddash) {
    sf::FloatRect newBounds = moveRect(oldBounds, movement);
    bool changed = false;
    for (size_t i = 0, n = field.width * field.height; i < n; i++) {
        const Cell &cell = field.cells[i];

        if (cell.category != CellCategory::WALL) {
            continue;
        }

        sf::FloatRect blockBound = cell.bounds;

        if (newBounds.intersects(blockBound)) {
            const float bottomShift = getBottom(blockBound) - newBounds.top;
            const float topShift = getBottom(newBounds) - blockBound.top;
            const float rightShift = getRight(newBounds) - blockBound.left;
            const float movementShift = std::max(std::abs(movement.x), std::abs(movement.y));

            Direction direction = selectShiftDirection(rightShift,
                                                       topShift, bottomShift,
                                                       movementShift + EPSILON, MAX_SHIFT, i);
            if (direction == Direction::NONE) {
                direction = selectShiftDirection(rightShift,
                                                 topShift, bottomShift,
                                                 0, MAX_SHIFT, i);
            }
            switch (direction) {
                case Direction::UP:
                    movement.y -= topShift;
                    break;
                case Direction::DOWN:
                    movement.y += bottomShift;
                    maddash.direction = Direction::NONE;
                    break;
                case Direction::STOP:
                    movement.x -= rightShift + 3;
                    maddash.direction = Direction::STOP;
                    break;
            }
            changed = true;
            newBounds = moveRect(oldBounds, movement);
        }
    }
    return changed;
}

void destroyField(Field &field) {
    delete[] field.cells;
}
