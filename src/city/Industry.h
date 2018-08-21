#pragma once

#include <deque>
#include "city/Building.h"
#include "city/Good.h"
#include "city/Work.h"
#include "city/Market.h"

class Work;

class Industry : public Building
{
public:
    Industry(const std::string& name, Type type, unsigned int nbStairs, Good good, double employeeProductivity,
        std::size_t nbEmployees, Work::Type employeeType);
    virtual ~Industry();

    virtual std::unique_ptr<Tile> clone() const override;

    virtual void setOwner(Company* owner) override;

    Good getGood() const;
    Work& getManager();
    const Work& getManager() const;
    std::vector<Work>& getEmployees();
    const std::vector<Work>& getEmployees() const;

    // Events
    void onNewMonth();

protected:
    struct Batch
    {
        double quantity;
        Money cost;

        Money getCostPerUnit();
    };

    Good mGood;
    std::deque<Batch> mStock;
    double mEmployeeProductivity;
    std::vector<Work> mEmployees;

    void sellGoods();
    const Market<const Building>* getMarket();
};
