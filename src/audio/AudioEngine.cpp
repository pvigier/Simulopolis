#include "AudioEngine.h"

AudioEngine::AudioEngine() : mIndex(0)
{
    //ctor
}

void AudioEngine::update()
{
    if (mMusic.getStatus() == sf::Music::Stopped && !mPlaylist.empty())
    {
        mIndex = (mIndex + 1) % mPlaylist.size();
        playMusic(mPlaylist[mIndex]);
    }
}

void AudioEngine::playMusic(const std::string& path)
{
    mMusic.openFromFile(path);
    mMusic.play();
}

void AudioEngine::addMusic(const std::string& path)
{
    mPlaylist.push_back(path);
}
