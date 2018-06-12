#include "resource/ImageManager.h"
#include "resource/XmlManager.h"
#include <iostream>

ImageManager::ImageManager() : mXmlManager(nullptr), mPrefixPath("media/")
{
    //ctor
}

ImageManager::~ImageManager()
{
    //dtor
}

void ImageManager::setXmlManager(XmlManager* xmlManager)
{
    mXmlManager = xmlManager;
}

void ImageManager::setUp()
{
    std::string path = mPrefixPath + "images.xml";
    XmlDocument doc = mXmlManager->loadDocument(path);

    for (const XmlDocument& child : doc.getChildren())
    {
        const std::string& name = child.getAttributes().get("name");
        mImages[name] = sf::Image();
        mImages[name].loadFromFile(mPrefixPath + child.getAttributes().get("path"));
    }
}

void ImageManager::tearDown()
{

}

void ImageManager::addImage(const std::string& name, sf::Image image)
{
    mImages[name] = std::move(image);
}

const sf::Image& ImageManager::getImage(const std::string& name) const
{
    return mImages.at(name);
}
