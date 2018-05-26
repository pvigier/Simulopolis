#pragma once

#include <SFML/Audio/Music.hpp>

class AudioEngine
{
public:
    AudioEngine();

    void update();

    void playMusic(const std::string& path);
    void addMusic(const std::string& path);

private:
    sf::Music mMusic;
    std::size_t mIndex;
    std::vector<std::string> mPlaylist;
};
