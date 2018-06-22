#include "XmlDocument.h"

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
    return mChildrenByName.at(name);
}

const XmlDocument& XmlDocument::getFirstChildByName(const std::string& name) const
{
    return mChildren.at(mChildrenByName.at(name).front());
}

bool XmlDocument::hasChildren(const std::string& name) const
{
    return mChildrenByName.find(name) != mChildrenByName.end();
}
