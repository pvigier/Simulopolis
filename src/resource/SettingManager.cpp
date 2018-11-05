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

#include "resource/SettingManager.h"
#include "resource/TextFileManager.h"

SettingManager::SettingManager() : mTextFileManager(nullptr), mPrefixPath("media/"), mFullscreen(false), mMusic(false)
{
    
}

void SettingManager::setTextFileManager(TextFileManager* xmlManager)
{
    mTextFileManager = xmlManager;
}

void SettingManager::setUp()
{
    std::string path = mPrefixPath + "settings.txt";
    std::unordered_map<std::string, std::string> settings = mTextFileManager->loadDictionary(path, "=");
    mFullscreen = std::stoi(settings["fullscreen"]);
    mMusic = std::stoi(settings["music"]);
}

void SettingManager::tearDown()
{
    std::unordered_map<std::string, std::string> settings;
    settings["fullscreen"] = std::to_string(mFullscreen);
    settings["music"] = std::to_string(mMusic);
}

bool SettingManager::isFullscreen() const
{
    return mFullscreen;
}

void SettingManager::setFullscreen(bool fullscreen)
{
    mFullscreen = fullscreen;
}

bool SettingManager::isMusic() const
{
    return mMusic;
}

void SettingManager::setMusic(bool music)
{
    mMusic = music;
}
