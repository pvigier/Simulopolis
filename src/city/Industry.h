#pragma once

#include "city/Building.h"
#include "city/Good.h"
#include "city/Work.h"

class Work;

class Industry : public Building
{
public:
    Industry(const std::string& name, Type type, unsigned int nbStairs, Good good, unsigned int productivity,
        std::size_t nbEmployees, Work::Type employeeType);
    virtual ~Industry();

    virtual std::unique_ptr<Tile> clone() const override;

    virtual void setOwner(Company* owner) override;

    Work& getManager();
    const Work& getManager() const;
    std::vector<Work>& getEmployees();
    const std::vector<Work>& getEmployees() const;

protected:
    Good mGood;
    unsigned int mProductivity;
    std::vector<Work> mEmployees;
};
