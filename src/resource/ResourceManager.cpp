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
}

void ResourceManager::tearDown()
{
    mGuiManager.tearDown();
    mStylesheetManager.tearDown();
    mFontManager.tearDown();
    mTextureManager.tearDown();
    mMusicManager.tearDown();
    mImageManager.tearDown();
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

MusicManager& ResourceManager::getMusicManager()
{
    return mMusicManager;
}

ImageManager& ResourceManager::getImageManager()
{
    return mImageManager;
}
