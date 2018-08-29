#pragma once

#include <deque>
#include <set>
#include "city/Building.h"
#include "city/Good.h"
#include "city/Work.h"

class Work;
template<typename T> class Market;

class Industry : public Building
{
public:
    Industry(const std::string& name, Type type, unsigned int nbStairs, Good good, double employeeProductivity,
        std::size_t nbEmployees, Work::Type employeeType);
    virtual ~Industry();

    virtual std::unique_ptr<Tile> clone() const override;
    virtual void update() override;
    virtual void tearDown() override;
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
    std::set<Id> mGoodsInMarket;
    double mEmployeeProductivity;
    std::vector<Work> mEmployees;
    std::set<Id> mWorksInMarket;

    void updateStock();
    void sellGoods();
    const Market<const Building>* getMarket();
};
