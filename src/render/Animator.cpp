/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include "render/Animator.h"

Animator::Animator() : mElapsedTime(0.0f), mCurAnimation(0), mCurFrame(0)
{

}

Animator::Animator(std::vector<Animation> animations) :
    mAnimations(std::move(animations)), mElapsedTime(0.0f), mCurAnimation(0), mCurFrame(0)
{

}

void Animator::addAnimation(Animation& animation)
{
    mAnimations.push_back(animation);
}

void Animator::update(float dt)
{
    // Update the elapsed time
    mElapsedTime += dt;

    // Update the current frame
    const Animation& animation = mAnimations[mCurAnimation];
    while (mCurFrame < animation.getNbFrames() && mElapsedTime >= animation.getFrame(mCurFrame).duration)
    {
        mElapsedTime -= animation.getFrame(mCurFrame).duration;
        mCurFrame = (mCurFrame + 1) % animation.getNbFrames();
    }
}

void Animator::changeAnimation(std::size_t iAnimation)
{
    if(mCurAnimation == iAnimation || iAnimation >= mAnimations.size())
        return;

    mElapsedTime = 0.0f;
    mCurAnimation = iAnimation;
    mCurFrame = 0;
}

sf::IntRect Animator::getDisplayedRect() const
{
    return mAnimations[mCurAnimation].getFrame(mCurFrame).rect;
}
