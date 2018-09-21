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

#include "XmlDocument.h"
#include "util/debug.h"

XmlDocument::XmlDocument()
{

}

XmlDocument::XmlDocument(std::string name, PropertyList attributes, std::vector<XmlDocument> children) :
    mName(std::move(name)), mAttributes(std::move(attributes)), mChildren(std::move(children))
{
    for (std::size_t i = 0; i < mChildren.size(); ++i)
        mChildrenByName[mChildren[i].getName()].push_back(i);
}

const std::string& XmlDocument::getName() const
{
    return mName;
}

PropertyList& XmlDocument::getAttributes()
{
    return mAttributes;
}

const PropertyList& XmlDocument::getAttributes() const
{
    return mAttributes;
}

const std::vector<XmlDocument>& XmlDocument::getChildren() const
{
    return mChildren;
}

const std::vector<std::size_t>& XmlDocument::getChildrenByName(const std::string& name) const
{
    DEBUG_IF(!hasChildren(name), name << " is not a child of this xml document (" << mName << ").\n");
    return mChildrenByName.at(name);
}

const XmlDocument& XmlDocument::getFirstChildByName(const std::string& name) const
{
    return mChildren.at(getChildrenByName(name).front());
}

bool XmlDocument::hasChildren(const std::string& name) const
{
    return mChildrenByName.find(name) != mChildrenByName.end();
}

void XmlDocument::addChild(XmlDocument child)
{
    mChildren.emplace_back(std::move(child));
}

void XmlDocument::removeChild(std::size_t i)
{
    mChildren.erase(mChildren.begin() + i);
}
