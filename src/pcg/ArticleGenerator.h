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

#pragma once

#include <string>
#include <unordered_map>

class XmlManager;
class RandomGenerator;
class Article;

class ArticleGenerator
{
public:
    struct ArticleTemplate
    {
        std::string title;
        std::string body;
    };

    enum class ArticleType : int {NEW_CITY, COUNT};
    using Context = std::unordered_map<std::string, std::string>;

    static void setXmlManager(XmlManager* xmlManager);

    ArticleGenerator(RandomGenerator& generator);

    void setUp();

    Article generate(ArticleType type, const Context& context, const std::string& date);

private:
    static XmlManager* sXmlManager;
    RandomGenerator& mGenerator;
    std::array<ArticleTemplate, static_cast<int>(ArticleType::COUNT)> mTemplates;

    void loadTemplate(ArticleType type, const std::string& path);
};
