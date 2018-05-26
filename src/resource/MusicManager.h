#pragma once

// STL
#include <string>
#include <unordered_map>

class XmlManager;

/**
 * \brief Manager that manages the musics
 *
 * \author Pierre Vigier
 */
class MusicManager
{
public:
    /**
     * \brief Default constructor
     */
    MusicManager();

    /**
     * \brief Destructor
     */
    ~MusicManager();

    /**
     * \brief Set xml manager
     *
     * \param xmlManager XmlManager to use
     */
    void setXmlManager(XmlManager* xmlManager);

    /**
     * \brief Set up the manager
     *
     * Open the "musics.xml" file in the folder mPrefixPath and load the
     * musics specified in this file.
     */
    void setUp();

    /**
     * \brief Tear down the manager
     */
    void tearDown();

    /**
     * \brief Add a music
     *
     * \param name Name of the music
     * \param music Music to add
     */
    void addMusic(const std::string& name, std::string music);

    /**
     * \brief Get a music
     *
     * \param name Name of the music to retrieve
     *
     * \return Music that corresponds to name
     */
    const std::string& getMusic(const std::string& name) const;

private:
    XmlManager* mXmlManager;
    std::string mPrefixPath; /**< Path of the folder in which is located "musics.xml" */
    std::unordered_map<std::string, std::string> mMusics; /**< Hash map that contains the musics */
};
