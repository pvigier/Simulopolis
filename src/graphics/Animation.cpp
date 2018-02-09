#include "Animation.h"

Animation::Animation(unsigned int startFrame, unsigned int endFrame, float duration) :
    startFrame(startFrame), endFrame(endFrame), duration(duration)
{

}

unsigned int Animation::getLength() const
{
    return endFrame - startFrame + 1;
}
