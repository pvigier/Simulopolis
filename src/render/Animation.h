#pragma once

#include <SFML/Graphics.hpp>

class Animation
{
public:
    struct Frame
    {
        sf::IntRect rect;
        float duration;
    };

    Animation();
    Animation(std::vector<Frame> frames);

    void addFrame(Frame frame);
    const Frame& getFrame(std::size_t iFrame) const;

    std::size_t getNbFrames() const;
    float getTotalDuration() const;

private:
    std::vector<Frame> mFrames;
    float mTotalDuration;
};

