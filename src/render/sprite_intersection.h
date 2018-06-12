#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Image.hpp>

bool sprite_intersect(const sf::Sprite& sprite, const sf::Image& mask, const sf::Vector2f& position);
