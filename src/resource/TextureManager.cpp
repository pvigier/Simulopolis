#include "resource/TextureManager.h"
#include "resource/XmlManager.h"
#include <iostream>

TextureManager::TextureManager() : mXmlManager(nullptr), mPrefixPath("media/")
{
    //ctor
}

TextureManager::~TextureManager()
{
    //dtor
}

void TextureManager::setXmlManager(XmlManager* xmlManager)
{
    mXmlManager = xmlManager;
}

void TextureManager::setUp()
{
    std::string path = mPrefixPath + "textures.xml";
    XmlDocument doc = mXmlManager->loadDocument(path);

    for (const XmlDocument& child : doc.getChildren())
    {
        const std::string& name = child.getAttributes().get("name");
        mTextures[name] = sf::Texture();
        mTextures[name].loadFromFile(mPrefixPath + child.getAttributes().get("path"));
    }
}

void TextureManager::tearDown()
{

}

void TextureManager::addTexture(const std::string& name, sf::Texture texture)
{
    mTextures[name] = texture;
}

const sf::Texture& TextureManager::getTexture(const std::string& name) const
{
    return mTextures.at(name);
}
