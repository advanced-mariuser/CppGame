#include "madDash.h"
#include "field.h"
#include "iostream"
#include <SFML/Graphics.hpp>

static const float MADDASH_SPEED = 300.1f; // pixels per second.
static const float MADDASH_RADIUS = 30.f; // pixels

float GRAVITY = 120.f;
static const float MOVEMENT_SPEED = 1.f;

static const sf::IntRect FRAME_BARRY_LEFT(0, 0, 27, 38);
static const sf::IntRect FRAME_BARRY_CENTER(27, 0, 27, 38);
static const sf::IntRect FRAME_BARRY_RIGHT(54, 0, 27, 38);
static const sf::IntRect FRAME_BARRY_RIGHTER(81, 0, 27, 38);

const char BARRY_TEXTURE[] = "../media/barry.png";

float animationMovementMad = 0.0f;

static void updateMadDashDirection(MadDash &maddash) {
    if (maddash.direction != Direction::RIGHT && maddash.direction != Direction::STOP) {
        maddash.direction = Direction::NONE;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        maddash.direction = Direction::UP;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        maddash.direction = Direction::DOWN;
    }
}

void initializeMadDash(MadDash &maddash) {
    if (!maddash.texture.loadFromFile(BARRY_TEXTURE)) {
        exit(1);
    }

    maddash.direction = Direction::NONE;
    maddash.shape.setRadius(MADDASH_RADIUS);
    maddash.shape.setTexture(&maddash.texture);
    maddash.shape.setTextureRect(FRAME_BARRY_LEFT);
    maddash.shape.setPosition(getMadDashStartPosition());
    maddash.takenMoney = 0;
}

void updateMadDash(MadDash &maddash, float elapsedTime, const Field &field, sf::View &view) {
    const float step = MADDASH_SPEED * elapsedTime;
    const float gravity = GRAVITY * elapsedTime;
    const float animation = 8.f * elapsedTime;
    animationMovementMad += 4.f * animation;
    float viewmove = MOVEMENT_SPEED;

    updateMadDashDirection(maddash);

    sf::Vector2f movement(0.f, 0.f);

    switch (maddash.direction) {
        case Direction::UP:
            movement.y -= step;
            movement.x += viewmove;
            GRAVITY = 120.f;
            maddash.shape.setTextureRect(FRAME_BARRY_RIGHTER);
            break;
        case Direction::DOWN:
            movement.y += step + gravity;
            movement.x += viewmove;
            maddash.shape.setTextureRect(FRAME_BARRY_RIGHTER);
            break;
        case Direction::RIGHT:
            movement.x += viewmove;
            if (animationMovementMad <= 9.1f) {
                if (animationMovementMad <= 3.f) {
                    maddash.shape.setTextureRect(FRAME_BARRY_LEFT);
                };
                if (animationMovementMad <= 6.0f && animationMovementMad >= 3.f) {
                    maddash.shape.setTextureRect(FRAME_BARRY_CENTER);
                };
                if (animationMovementMad <= 9.5f && animationMovementMad >= 6.f) {
                    maddash.shape.setTextureRect(FRAME_BARRY_RIGHT);
                }
            } else {
                animationMovementMad = 0.0f;
            }
            break;
        case Direction::NONE:
            GRAVITY += 0.4f;
            movement.y += gravity;
            movement.x += viewmove;
            maddash.shape.setTextureRect(FRAME_BARRY_RIGHTER);
            break;
        case Direction::STOP:
            viewmove = 0;
            break;
    }
    const sf::FloatRect maddashBounds = maddash.shape.getGlobalBounds();
    if (checkFieldWallsCollision(field, maddashBounds, movement, maddash)) {
        if (maddash.direction != Direction::UP && maddash.direction != Direction::STOP &&
            maddash.direction != Direction::NONE) {
            maddash.direction = Direction::RIGHT;
        }
        GRAVITY = 120.f;
    }

    maddash.takenMoney += eatAllCoinsInBounds(field, maddash.shape.getGlobalBounds());
    maddash.shape.move(movement);

    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();

    float rightEdge = viewCenter.x + viewSize.x / 2;
    if (rightEdge <= (field.width * 30.f)) {
        view.move({viewmove, 0.f});
    }
}

void drawMadDash(sf::RenderWindow &window, const MadDash &madDash) {
    window.draw(madDash.shape);
}

sf::FloatRect getMadDashBounds(const MadDash &maddash) {
    return maddash.shape.getGlobalBounds();
}

