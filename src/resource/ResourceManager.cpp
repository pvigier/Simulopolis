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

#include "resource/ResourceManager.h"
#include "resource/PropertyList.h"

ResourceManager::ResourceManager()
{
    //ctor
}

void ResourceManager::setUp()
{
    mXmlManager.setUp();
    mTextFileManager.setUp();
    mTextureManager.setXmlManager(&mXmlManager);
    mTextureManager.setUp();
    mFontManager.setXmlManager(&mXmlManager);
    mFontManager.setUp();
    mStylesheetManager.setXmlManager(&mXmlManager);
    mStylesheetManager.setUp();
    PropertyList::setTextureManager(&mTextureManager);
    PropertyList::setFontManager(&mFontManager);
    PropertyList::setStylesheetManager(&mStylesheetManager);
    mGuiManager.setXmlManager(&mXmlManager);
    mGuiManager.setUp();
    mMusicManager.setUp();
    mImageManager.setXmlManager(&mXmlManager);
    mImageManager.setUp();
    mSaveManager.setXmlManager(&mXmlManager);
    mSaveManager.setTextureManager(&mTextureManager);
    mSaveManager.setUp();
    mSettingManager.setTextFileManager(&mTextFileManager);
    mSettingManager.setUp();
}

void ResourceManager::tearDown()
{
    mSettingManager.tearDown();
    mSaveManager.tearDown();
    mImageManager.tearDown();
    mMusicManager.tearDown();
    mGuiManager.tearDown();
    mStylesheetManager.tearDown();
    mFontManager.tearDown();
    mTextureManager.tearDown();
    mTextFileManager.tearDown();
    mXmlManager.tearDown();
}

XmlManager& ResourceManager::getXmlManager()
{
    return mXmlManager;
}

TextFileManager& ResourceManager::getTextFileManager()
{
    return mTextFileManager;
}

TextureManager& ResourceManager::getTextureManager()
{
    return mTextureManager;
}

FontManager& ResourceManager::getFontManager()
{
    return mFontManager;
}

StylesheetManager& ResourceManager::getStylesheetManager()
{
    return mStylesheetManager;
}

GuiManager& ResourceManager::getGuiManager()
{
    return mGuiManager;
}

MusicManager& ResourceManager::getMusicManager()
{
    return mMusicManager;
}

ImageManager& ResourceManager::getImageManager()
{
    return mImageManager;
}

SaveManager& ResourceManager::getSaveManager()
{
    return mSaveManager;
}

SettingManager& ResourceManager::getSettingManager()
{
    return mSettingManager;
}
