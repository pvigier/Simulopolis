#pragma once

#include <SFML/Graphics.hpp>

class Tile : public sf::Drawable
{
public:
    enum class Type : int {VOID, GRASS, FOREST, WATER, RESIDENTIAL, COMMERCIAL, INDUSTRIAL, ROAD};
    enum class State{DESELECTED, SELECTED, INVALID};
    static constexpr unsigned int SIZE = 64;

    Tile(const sf::Texture& texture, Type type, unsigned int height);
    virtual ~Tile();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual std::unique_ptr<Tile> clone() const;

    virtual bool updateVariant(Tile* neighbors[3][3]);

    virtual bool isBuilding() const;
    virtual bool hasSidewalk() const;
    sf::Sprite& getSprite();
    void setPosition(sf::Vector2f position);
    Type getType() const;
    State getState() const;
    void setState(State state);

protected:
    sf::Sprite mSprite;
    Type mType;
    State mState;
};

std::string tileTypeToStr(Tile::Type type);
