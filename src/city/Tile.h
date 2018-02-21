#pragma once

#include <map>
#include <vector>
#include <SFML/Graphics.hpp>
#include "render/Animator.h"

class Tile
{
public:
    enum class Type{VOID, GRASS, FOREST, WATER, RESIDENTIAL, COMMERCIAL, INDUSTRIAL, ROAD};
    enum class State{DESELECTED, SELECTED, INVALID};
    static constexpr unsigned int SIZE = 64;

    Tile();
    Tile(unsigned int height, const sf::Texture& texture,
        std::vector<Animation> animations, Type type, unsigned int cost);

    void draw(sf::RenderWindow& window, float dt);
    void update();

    sf::Sprite& getSprite();
    Type& getType();
    Type getType() const;
    unsigned int& getVariant();
    unsigned int getCost() const;

private:
    Animator mAnimator;
    sf::Sprite mSprite;
    Type mType;
    unsigned int mVariant;
    unsigned int mCost;
};

using TileAtlas = std::map<std::string, Tile>;

std::string tileTypeToStr(Tile::Type type);
