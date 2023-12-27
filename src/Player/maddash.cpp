#include "maddash.h"
#include "../Field/field.h"
#include <SFML/Graphics.hpp>
#include <cassert>

static const float MADDASH_SPEED = 200.1f; // pixels per second.

float GRAVITY = 60.f;
float timeBoost = 2.f;

int checkBoost = 0;

static const sf::IntRect FRAME_BARRY_LEFT(0, 0, 27, 38);
static const sf::IntRect FRAME_BARRY_CENTER(27, 0, 27, 38);
static const sf::IntRect FRAME_BARRY_RIGHT(54, 0, 27, 38);
static const sf::IntRect FRAME_BARRY_RIGHTER(81, 0, 27, 38);

static const sf::IntRect FRAME_FIRE_LEFTER(0, 0, 11, 23);
static const sf::IntRect FRAME_FIRE_LEFT(11, 0, 11, 23);
static const sf::IntRect FRAME_FIRE_CENTER_LEFT(22, 0, 11, 23);
static const sf::IntRect FRAME_FIRE_CENTER_RIGHT(33, 0, 11, 23);
static const sf::IntRect FRAME_FIRE_RIGHT(44, 0, 11, 23);
static const sf::IntRect FRAME_FIRE_RIGHTER(55, 0, 11, 23);
static const sf::IntRect FRAME_FIRE_NONE(66, 0, 11, 23);

const char BARRY_TEXTURE[] = "../media/player/barry.png";
const char FIRE_TEXTURE[] = "../media/player/exhaust.png";

float animationMovementMad = 0.0f;
float animationFire = 0.0f;

static void updateMadDashDirection(MadDash &maddash) {
    if (maddash.direction != Direction::STOP && maddash.direction != Direction::ONLYUP && maddash.direction != Direction::ONLYDOWN) {
        maddash.direction = Direction::NONE;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && maddash.direction != Direction::ONLYDOWN) {
        maddash.direction = Direction::UP;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        maddash.direction = Direction::DOWN;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && maddash.direction == Direction::ONLYDOWN) {
        maddash.direction = Direction::ONLYUP;
    }

    checkBoost = 0;
}

void initializeMadDash(MadDash &maddash) {
    if (!maddash.texture.loadFromFile(BARRY_TEXTURE)) {
        exit(1);
    }

    if (!maddash.firetexture.loadFromFile(FIRE_TEXTURE)) {
        exit(1);
    }

    maddash.direction = Direction::NONE;
    maddash.shape.scale(2.2f, 1.5f);
    maddash.shape.setTexture(maddash.texture);
    maddash.shape.setTextureRect(FRAME_BARRY_LEFT);
    maddash.shape.setPosition(getMadDashStartPosition());


    maddash.fire.scale(1.25f, 1.25f);
    maddash.fire.setTexture(maddash.firetexture);
    maddash.fire.setTextureRect(FRAME_FIRE_LEFTER);
    maddash.fire.setPosition(getMadDashStartPosition().x + 7.f, getMadDashStartPosition().y + 49.f);

    maddash.speed = 1.f;
}

void updateMadDash(MadDash &maddash, float elapsedTime, const Field &field, sf::View &view, unsigned &boost) {
    if(timeBoost < 1.5f) {
        timeBoost += elapsedTime;
    } else {
        timeBoost = 2.f;
        maddash.speed = 1.f;
    }

    const float step = MADDASH_SPEED * elapsedTime;
    const float gravity = GRAVITY * elapsedTime;
    const float animation = 8.f * elapsedTime;
    animationMovementMad += 4.f * animation;
    animationFire += 4.f * animation;

    float viewmove = maddash.speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::B) && boost != 0 && checkBoost == 0) {
        maddash.speed = maddash.speed + 1.f;
        timeBoost = 0.f;
        boost--;
        checkBoost++;
    }

    updateMadDashDirection(maddash);

    sf::Vector2f movement(0.f, 0.f);

    switch (maddash.direction) {
        case Direction::UP:
            movement.y -= step;
            movement.x += viewmove;
            GRAVITY = 120.f;
            maddash.shape.setTextureRect(FRAME_BARRY_RIGHTER);
            if (animationFire <= 18.1f) {
                if (animationFire <= 3.f) {
                    maddash.fire.setTextureRect(FRAME_FIRE_LEFTER);
                };
                if (animationFire <= 6.0f && animationFire >= 3.f) {
                    maddash.fire.setTextureRect(FRAME_FIRE_LEFT);
                };
                if (animationFire <= 9.0f && animationFire >= 6.f) {
                    maddash.fire.setTextureRect(FRAME_FIRE_CENTER_LEFT);
                }
                if (animationFire <= 12.0f && animationFire >= 9.f) {
                    maddash.fire.setTextureRect(FRAME_FIRE_CENTER_RIGHT);
                }
                if (animationFire <= 15.0f && animationFire >= 12.f) {
                    maddash.fire.setTextureRect(FRAME_FIRE_RIGHT);
                };
                if (animationFire <= 18.0f && animationFire >= 15.f) {
                    maddash.fire.setTextureRect(FRAME_FIRE_RIGHTER);
                }
            } else {
                animationFire = 0.0f;
            }
            break;
        case Direction::ONLYUP:
            movement.y -= step;
            movement.x = 0;
            maddash.shape.setTextureRect(FRAME_BARRY_RIGHTER);
            if (animationFire <= 18.1f) {
                if (animationFire <= 3.f) {
                    maddash.fire.setTextureRect(FRAME_FIRE_LEFTER);
                };
                if (animationFire <= 6.0f && animationFire >= 3.f) {
                    maddash.fire.setTextureRect(FRAME_FIRE_LEFT);
                };
                if (animationFire <= 9.0f && animationFire >= 6.f) {
                    maddash.fire.setTextureRect(FRAME_FIRE_CENTER_LEFT);
                }
                if (animationFire <= 12.0f && animationFire >= 9.f) {
                    maddash.fire.setTextureRect(FRAME_FIRE_CENTER_RIGHT);
                }
                if (animationFire <= 15.0f && animationFire >= 12.f) {
                    maddash.fire.setTextureRect(FRAME_FIRE_RIGHT);
                };
                if (animationFire <= 18.0f && animationFire >= 15.f) {
                    maddash.fire.setTextureRect(FRAME_FIRE_RIGHTER);
                }
            } else {
                animationFire = 0.0f;
            }
            break;
        case Direction::ONLYDOWN:
            GRAVITY += 0.4f;
            movement.y += step + gravity;
            movement.x = 0;
            maddash.shape.setTextureRect(FRAME_BARRY_RIGHTER);
            maddash.fire.setTextureRect(FRAME_FIRE_NONE);
            break;
        case Direction::DOWN:
            movement.y += step + gravity;
            movement.x += viewmove;
            maddash.shape.setTextureRect(FRAME_BARRY_RIGHTER);
            maddash.fire.setTextureRect(FRAME_FIRE_NONE);
            break;
        case Direction::RIGHT:
            movement.x += viewmove;
            break;
        case Direction::NONE:
            GRAVITY += 0.4f;
            movement.y += gravity;
            movement.x += viewmove;
            maddash.shape.setTextureRect(FRAME_BARRY_RIGHTER);
            maddash.fire.setTextureRect(FRAME_FIRE_NONE);
            break;
        case Direction::STOP:
            viewmove = 0;
            GRAVITY = 120.f;
            break;
    }
    const sf::FloatRect maddashBounds = maddash.shape.getGlobalBounds();
    if (checkFieldWallsCollision(field, maddashBounds, movement, maddash)) {
        if(maddash.direction == Direction::RIGHT)
        {
            if (animationMovementMad <= 9.1f) {
                if (animationMovementMad <= 3.f) {
                    maddash.shape.setTextureRect(FRAME_BARRY_LEFT);
                };
                if (animationMovementMad <= 6.0f && animationMovementMad >= 3.f) {
                    maddash.shape.setTextureRect(FRAME_BARRY_CENTER);
                };
                if (animationMovementMad <= 9.0f && animationMovementMad >= 6.f) {
                    maddash.shape.setTextureRect(FRAME_BARRY_RIGHT);
                }
            } else {
                animationMovementMad = 0.0f;
            }
        }

        GRAVITY = 120.f;
    }

    maddash.shape.move(movement);
    maddash.fire.move(movement);

    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();

    float rightEdge = viewCenter.x + viewSize.x / 2;
    if (rightEdge <= (field.width * 30.f)) {
        view.move({viewmove, 0.f});
    }
}

void drawMadDash(sf::RenderWindow &window, MadDash &madDash) {
    window.draw(madDash.shape);
    window.draw(madDash.fire);
}

