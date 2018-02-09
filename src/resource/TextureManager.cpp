#include "TextureManager.h"

TextureManager::TextureManager()
{

}

void TextureManager::loadTexture(const std::string& name, const std::string& filename)
{
    // Load the texture
    sf::Texture texture;
    texture.loadFromFile(filename);

    // Add it to the map of textures
    mTextures[name] = texture;
}

sf::Texture& TextureManager::getRef(const std::string& name)
{
    return mTextures.at(name);
}
