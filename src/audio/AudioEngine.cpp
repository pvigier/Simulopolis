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
 
#include "AudioEngine.h"

AudioEngine::AudioEngine() : mIndex(0)
{
    //ctor
}

void AudioEngine::update()
{
    if (mMusic.getStatus() == sf::Music::Stopped && !mPlaylist.empty())
    {
        mIndex = (mIndex + 1) % mPlaylist.size();
        playMusic(mPlaylist[mIndex]);
    }
}

void AudioEngine::playMusic(const std::string& path)
{
    mMusic.openFromFile(path);
    mMusic.play();
}

void AudioEngine::addMusic(const std::string& path)
{
    mPlaylist.push_back(path);
}

float AudioEngine::getVolume() const
{
    return mMusic.getVolume();
}

void AudioEngine::setVolume(float volume)
{
    mMusic.setVolume(volume);
}
