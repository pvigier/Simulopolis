#include "resource/ResourceManager.h"

ResourceManager::ResourceManager()
{
    //ctor
}

ResourceManager::~ResourceManager()
{
    //dtor
}

void ResourceManager::setUp()
{
    mTextureManager.setUp();
    mFontManager.setUp();
    mStylesheetManager.setFontManager(&mFontManager);
    mStylesheetManager.setUp();
    mGuiManager.setTextureManager(&mTextureManager);
    mGuiManager.setFontManager(&mFontManager);
    mGuiManager.setStylesheetManager(&mStylesheetManager);
    mGuiManager.setUp();
}

void ResourceManager::tearDown()
{
    mTextureManager.tearDown();
    mFontManager.tearDown();
    mStylesheetManager.tearDown();
    mGuiManager.tearDown();
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
