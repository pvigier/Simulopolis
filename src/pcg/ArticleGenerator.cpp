/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ArticleGenerator.h"
#include <regex>
#include "util/string.h"
#include "resource/XmlManager.h"
#include "pcg/RandomGenerator.h"
#include "city/Newspaper.h"


XmlManager* ArticleGenerator::sXmlManager = nullptr;

void ArticleGenerator::setXmlManager(XmlManager* xmlManager)
{
    sXmlManager = xmlManager;
}

ArticleGenerator::ArticleGenerator(/*RandomGenerator& generator*/)/* : mGenerator(generator)*/
{
    //ctor
}

void ArticleGenerator::setUp()
{
    loadTemplate(ArticleType::NEW_CITY, "media/articles/new_city.xml");
}

Article ArticleGenerator::generate(ArticleType type, const Context& context, const std::string& author)
{
    const ArticleTemplate& articleTemplate = mTemplates[static_cast<int>(type)];
    std::string title = articleTemplate.title;
    std::string body = articleTemplate.body;
    for (auto& kv : context)
    {
        std::regex re(R"(\{\{ )" + kv.first + R"( \}\})");
        title = std::regex_replace(title, re, kv.second);
        body = std::regex_replace(body, re, kv.second);
    }
    return Article{title, body, author};
}

void ArticleGenerator::loadTemplate(ArticleType type, const std::string& path)
{
    XmlDocument document = sXmlManager->loadDocument(path);
    std::string title = strip(document.getFirstChildByName("title").getText());
    std::string body = strip(document.getFirstChildByName("body").getText());
    // Add tabulation
    std::vector<std::string> paragraphs = split(body, '\n');
    for (std::string& paragraph : paragraphs)
    {
        if (!paragraph.empty())
            paragraph = '\t' + paragraph;
    }
    body = join(paragraphs, '\n');
    mTemplates[static_cast<int>(type)] = ArticleTemplate{std::move(title), std::move(body)};
}
