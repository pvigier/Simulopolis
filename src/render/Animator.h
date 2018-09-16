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
