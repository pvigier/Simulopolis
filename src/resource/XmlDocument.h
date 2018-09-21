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

#include <vector>
#include <unordered_map>
#include "PropertyList.h"

class XmlDocument
{
public:
    XmlDocument();
    XmlDocument(std::string name, PropertyList attributes, std::vector<XmlDocument> children);

    const std::string& getName() const;
    PropertyList& getAttributes();
    const PropertyList& getAttributes() const;
    const std::vector<XmlDocument>& getChildren() const;
    const std::vector<std::size_t>& getChildrenByName(const std::string& name) const;
    const XmlDocument& getFirstChildByName(const std::string& name) const;
    bool hasChildren(const std::string& name) const;
    void addChild(XmlDocument child);
    void removeChild(std::size_t i);

private:
    std::string mName;
    PropertyList mAttributes;
    std::vector<XmlDocument> mChildren;
    std::unordered_map<std::string, std::vector<std::size_t>> mChildrenByName;
};
