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
