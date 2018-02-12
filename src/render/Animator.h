#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "render/Animation.h"

class Animator
{
public:
    Animator();
    Animator(std::vector<Animation> animations);

    void addAnimation(Animation& animation);
    void update(float dt);
    void changeAnimation(std::size_t iAnimation);

    sf::IntRect getDisplayedRect() const;

private:
    std::vector<Animation> mAnimations;
    float mElapsedTime;
    std::size_t mCurAnimation;
    std::size_t mCurFrame;
};
