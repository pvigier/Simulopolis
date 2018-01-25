#pragma once

class Animation
{
public:
    unsigned int startFrame;
    unsigned int endFrame;
    float duration; // duration of a frame

    Animation(unsigned int startFrame, unsigned int endFrame, float duration);

    unsigned int getLength() const;
};

