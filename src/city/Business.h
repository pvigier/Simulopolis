#pragma once

#include <queue>
#include <set>
#include <boost/serialization/set.hpp>
#include "city/Building.h"
#include "city/Good.h"
#include "city/Work.h"

template<typename T> class Market;

class Business : public Building
{
public:
    struct Event
    {
        enum class Type{RESERVATION, RESERVATION_ACCEPTED, RESERVATION_REFUSED};

        Type type;
        Id accountId;
        Money price;
    };

    static Type getBusinessType(Good good);

    Business(const std::string& name, Type type, unsigned int nbStairs, Good good, unsigned int maxSizeStock,
        std::size_t nbEmployees, Work::Type employeeType);
    virtual ~Business();

    virtual std::unique_ptr<Tile> clone() const override;
    virtual void update() override;
    virtual void tearDown() override;
    virtual void setOwner(Company* owner) override;

    Good getGood() const;
    unsigned int getStock() const;
    bool hasPreparedGoods() const;
    Money getPrice() const;
    Work& getManager();
    const Work& getManager() const;
    std::vector<Work>& getEmployees();
    const std::vector<Work>& getEmployees() const;

    // Events
    void onNewMonth();

protected:
    Good mGood;
    unsigned int mMaxSizeStock;
    unsigned int mStock;
    Money mStockCost;
    double mPreparedGoods;
    Money mPrice;
    std::vector<Work> mEmployees;
    std::set<Id> mWorksInMarket;

    void prepareGoods();
    void buyGoods();
    const Market<const Building>* getMarket();
    void updatePrice();
    void updateDesiredQuantity();

private:
    // Serialization
    friend class boost::serialization::access;

    Business() = default;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Building>(*this);
        ar & mGood & mMaxSizeStock & mStock & mStockCost & mPreparedGoods & mPrice /*& mEmployees*/ & mWorksInMarket;
    }
};
