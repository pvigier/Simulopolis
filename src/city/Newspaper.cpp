#include "Newspaper.h"

Newspaper::Newspaper()
{

}

Newspaper::~Newspaper()
{

}

const std::string& Newspaper::getName() const
{
    return mName;
}

void Newspaper::setName(std::string name)
{
    mName = std::move(name);
}
