#include "TextureManager.h"
#include <iostream>

using namespace tinyxml2;

TextureManager::TextureManager() : mPrefixPath("media/")
{
    //ctor
}

TextureManager::~TextureManager()
{
    //dtor
}

void TextureManager::setUp()
{
    XMLDocument doc;
    std::string path = mPrefixPath + "textures.xml";
    doc.LoadFile(path.c_str());

    XMLNode* root = doc.FirstChild();

    if (root == nullptr)
    {
        std::cout << mPrefixPath + "textures.xml" << " has not been loaded correctly." << std::endl;
        return;
    }

    // Textures
    for (XMLElement* node = root->FirstChildElement("texture"); node != nullptr; node = node->NextSiblingElement("texture"))
        loadTexture(node);
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

void TextureManager::loadTexture(XMLElement* node)
{
    // Parameters
    std::string name = node->Attribute("name");
    // Path
    std::string path = node->Attribute("path");
    mTextures[name] = sf::Texture();
    mTextures[name].loadFromFile(mPrefixPath + path);
}
