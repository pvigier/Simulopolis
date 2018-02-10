#include "ResourceManager.h"

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
}

void ResourceManager::tearDown()
{
    mTextureManager.tearDown();
}

TextureManager& ResourceManager::getTextureManager()
{
    return mTextureManager;
}
