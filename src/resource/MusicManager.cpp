#include "resource/MusicManager.h"
#include "resource/XmlManager.h"
#include <iostream>

MusicManager::MusicManager() : mXmlManager(nullptr), mPrefixPath("media/")
{
    //ctor
}

MusicManager::~MusicManager()
{
    //dtor
}

void MusicManager::setXmlManager(XmlManager* xmlManager)
{
    mXmlManager = xmlManager;
}

void MusicManager::setUp()
{
    std::string path = mPrefixPath + "musics.xml";
    XmlDocument doc = mXmlManager->loadDocument(path);

    for (const XmlDocument& child : doc.getChildren())
    {
        const std::string& name = child.getAttributes().get("name");
        mMusics[name] = mPrefixPath + child.getAttributes().get("path");
    }
}

void MusicManager::tearDown()
{

}

void MusicManager::addMusic(const std::string& name, std::string music)
{
    mMusics[name] = std::move(music);
}

const std::string& MusicManager::getMusic(const std::string& name) const
{
    return mMusics.at(name);
}
