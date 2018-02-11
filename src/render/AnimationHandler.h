#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "render/Animation.h"

class AnimationHandler
{
public:
    AnimationHandler();
    AnimationHandler(std::vector<Animation> animations, sf::IntRect frameSize);

    void addAnimation(Animation& animation);
    void update(float dt);
    void changeAnimation(std::size_t iAnimation);

    sf::IntRect getDisplayedRect();

private:
    std::vector<Animation> mAnimations;
    float mElapsedTime;
    std::size_t mCurAnimation;
    unsigned int mCurFrame;
    sf::IntRect mDisplayedRect;
    sf::IntRect mFrameSize;

    void updateDisplayedRect();
};
