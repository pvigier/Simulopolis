#pragma once

#include <SFML/Graphics.hpp>

class Tile
{
public:
    enum class Type : int {VOID, GRASS, FOREST, WATER, RESIDENTIAL, COMMERCIAL, INDUSTRIAL, ROAD};
    enum class State{DESELECTED, SELECTED, INVALID};
    static constexpr unsigned int SIZE = 64;

    Tile(const sf::Texture& texture, sf::IntRect rect, Type type, unsigned int height);
    virtual ~Tile();

    virtual std::unique_ptr<Tile> clone() const;

    virtual bool updateVariant(Tile* neighbors[3][3]);

    virtual bool isBuilding() const;
    virtual bool hasSidewalk() const;
    sf::Sprite& getSprite();
    Type getType() const;

protected:
    sf::Sprite mSprite;
    Type mType;
};

std::string tileTypeToStr(Tile::Type type);
