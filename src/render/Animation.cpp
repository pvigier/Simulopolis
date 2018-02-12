#include "render/Animation.h"

Animation::Animation() : mTotalDuration(0.0f)
{

}

Animation::Animation(std::vector<Frame> frames) : mFrames(std::move(frames)), mTotalDuration(0.0f)
{
    for (const Frame& frame : mFrames)
        mTotalDuration += frame.duration;
}

void Animation::addFrame(Frame frame)
{
    mTotalDuration += frame.duration;
    mFrames.push_back(std::move(frame));
}

const Animation::Frame& Animation::getFrame(std::size_t iFrame) const
{
    return mFrames[iFrame];
}

std::size_t Animation::getNbFrames() const
{
    return mFrames.size();
}

float Animation::getTotalDuration() const
{
    return mTotalDuration;
}
