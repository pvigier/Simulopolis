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
    Industry(const std::string& name, Type type, unsigned int nbStairs, Good::Type goodType, double employeeProductivity,
        std::size_t nbEmployees, Work::Type employeeType);
    virtual ~Industry();

    virtual std::unique_ptr<Tile> clone() const override;
    virtual void update() override;
    virtual void tearDown() override;
    virtual void setOwner(Company* owner) override;

    Good::Type getGoodType() const;
    std::unique_ptr<Work>& getManager();
    const std::unique_ptr<Work>& getManager() const;
    std::vector<std::unique_ptr<Work>>& getEmployees();
    const std::vector<std::unique_ptr<Work>>& getEmployees() const;

    // Events
    void onNewMonth();

protected:
    struct Batch
    {
        double quantity;
        Money cost;

        Money getCostPerUnit();

    private:
        // Serialization
        friend class boost::serialization::access;

        template <typename Archive>
        void serialize(Archive &ar, const unsigned int version)
        {
            ar & quantity & cost;
        }
    };

    Good::Type mGoodType;
    std::deque<Batch> mStock;
    std::set<Id> mGoodsInMarket;
    double mEmployeeProductivity;
    std::vector<std::unique_ptr<Work>> mEmployees;
    std::set<Id> mWorksInMarket;

    void updateStock();
    void sellGoods();
    const Market<const Building>* getMarket();

private:
    // Serialization
    friend class boost::serialization::access;

    Industry() = default;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Building>(*this);
        ar & mGoodType & mStock & mGoodsInMarket & mEmployeeProductivity & mEmployees & mWorksInMarket;
    }
};
