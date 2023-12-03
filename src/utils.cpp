#include "utils.h"
#include "iostream"

void utils::centerizeTextOrigin(sf::Text &text, const sf::Vector2f &vec)
{
    const sf::FloatRect labelBounds = text.getLocalBounds();
    text.setPosition(0.5f * vec.x, 0.4f * vec.y);
    text.setOrigin(0.5f * labelBounds.height,
                   0.5f * labelBounds.width);
}
