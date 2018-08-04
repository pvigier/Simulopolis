#pragma once

#include "city/Building.h"
#include "city/Good.h"
#include "city/Work.h"

class Business : public Building
{
public:
    Business(const std::string& name, Type type, unsigned int nbStairs, Good good, std::size_t nbEmployees,
        Work::Type employeeType);
    virtual ~Business();

    virtual std::unique_ptr<Tile> clone() const override;

    virtual void setOwner(Company* owner) override;

    Good getGood() const;
    float getPrice() const;
    const Work& getManager() const;
    const std::vector<Work>& getEmployees() const;

protected:
    Good mGood;
    unsigned int mQuantity;
    float mPrice;
    Work mManager;
    std::vector<Work> mEmployees;
};
