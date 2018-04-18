#pragma once

// STL
#include <string>
#include <unordered_map>
#include <memory>

class XmlManager;
class XmlDocument;
class FontManager;

/**
 * \brief Manager that manages the stylesheets
 *
 * \author Pierre Vigier
 */
class StylesheetManager
{
public:
    /**
     * \brief Default constructor
     */
    StylesheetManager();

    /**
     * \brief Destructor
     */
    ~StylesheetManager();

    /**
     * \brief Set xml manager
     *
     * \param xmlManager XmlManager to use
     */
    void setXmlManager(XmlManager* xmlManager);

    /**
     * \brief Set up the manager
     *
     * Open the "stylesheets.xml" file in the folder mPrefixPath and load the
     * stylesheets specified in this file.
     */
    void setUp();

    /**
     * \brief Tear down the manager
     */
    void tearDown();

    /**
     * \brief Add a stylesheet
     *
     * \param name Name of the stylesheet
     * \param stylesheet Stylesheet to add
     */
    void addStylesheet(const std::string& name, XmlDocument stylesheet);

    /**
     * \brief Get a stylesheet
     *
     * \param name Name of the stylesheet to retrieve
     *
     * \return Stylesheet that corresponds to name
     */
    const XmlDocument* getStylesheet(const std::string& name) const;

private:
    XmlManager* mXmlManager;
    std::string mPrefixPath; /**< Path of the folder in which is located "stylesheets.xml" */
    std::unordered_map<std::string, std::unique_ptr<XmlDocument>> mStylesheets; /**< Hash map that contains the stylesheets */
};
