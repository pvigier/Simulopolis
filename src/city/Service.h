#pragma once

#include "city/Building.h"
#include "city/Work.h"

class Service : public Building
{
public:
    Service(const std::string& name, Type type, unsigned int nbStairs, std::size_t nbEmployees,
        Work::Type employeeType);
    virtual ~Service();

    virtual std::unique_ptr<Tile> clone() const override;

    virtual void setOwner(Company* owner) override;

    std::vector<Work>& getEmployees();

protected:
    std::vector<Work> mEmployees;
};
