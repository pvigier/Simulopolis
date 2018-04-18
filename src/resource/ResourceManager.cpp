#include "resource/ResourceManager.h"
#include "resource/PropertyList.h"

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
    mXmlManager.setUp();
    mTextureManager.setUp();
    mFontManager.setUp();
    mStylesheetManager.setFontManager(&mFontManager);
    mStylesheetManager.setUp();
    PropertyList::setTextureManager(&mTextureManager);
    PropertyList::setFontManager(&mFontManager);
    PropertyList::setStylesheetManager(&mStylesheetManager);
    mGuiManager.setTextureManager(&mTextureManager);
    mGuiManager.setFontManager(&mFontManager);
    mGuiManager.setStylesheetManager(&mStylesheetManager);
    mGuiManager.setUp();
}

void ResourceManager::tearDown()
{
    mGuiManager.tearDown();
    mStylesheetManager.tearDown();
    mFontManager.tearDown();
    mTextureManager.tearDown();
}

XmlManager& ResourceManager::getXmlManager()
{
    return mXmlManager;
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
