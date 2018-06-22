#pragma once

#include <vector>
#include <unordered_map>
#include "PropertyList.h"

class XmlDocument
{
public:
    XmlDocument(std::string name, PropertyList attributes, std::vector<XmlDocument> children);

    const std::string& getName() const;
    const PropertyList& getAttributes() const;
    const std::vector<XmlDocument>& getChildren() const;
    const std::vector<std::size_t>& getChildrenByName(const std::string& name) const;
    const XmlDocument& getFirstChildByName(const std::string& name) const;
    bool hasChildren(const std::string& name) const;

private:
    std::string mName;
    PropertyList mAttributes;
    std::vector<XmlDocument> mChildren;
    std::unordered_map<std::string, std::vector<std::size_t>> mChildrenByName;
};
