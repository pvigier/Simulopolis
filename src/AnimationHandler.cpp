#include "AnimationHandler.h"

AnimationHandler::AnimationHandler() : mElapsedTime(0.0f), mCurAnimation(0)
{

}

AnimationHandler::AnimationHandler(sf::IntRect frameSize) :
    mElapsedTime(0.0f), mCurAnimation(-1), mFrameSize(frameSize)
{

}

void AnimationHandler::addAnimation(Animation& animation)
{
    mAnimations.push_back(animation);
}

void AnimationHandler::update(const float dt)
{
    if (mCurAnimation >= mAnimations.size())
        return;

    float duration = mAnimations[mCurAnimation].duration;
    unsigned int length = mAnimations[mCurAnimation].getLength();

    // Update the elapsed time
    mElapsedTime = mElapsedTime + dt;
    while (mElapsedTime >= length * duration)
        mElapsedTime -= length * duration;

    // Compute the new frame
    unsigned int newFrame = mElapsedTime / duration;

    // Update the displayed rect
    if (newFrame != mCurFrame)
    {
        mCurFrame = newFrame;
        updateDisplayedRect();
    }
}

void AnimationHandler::changeAnimation(std::size_t iAnimation)
{
    if(mCurAnimation == iAnimation || iAnimation >= mAnimations.size())
        return;

    mCurAnimation = iAnimation;
    mElapsedTime = 0.0f;
    mCurFrame = 0;
    updateDisplayedRect();
}

void AnimationHandler::updateDisplayedRect()
{
    mDisplayedRect = sf::IntRect(mFrameSize.width * mCurFrame, mFrameSize.height * mCurAnimation,
        mFrameSize.width, mFrameSize.height);
}
