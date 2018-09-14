#pragma once

#include <queue>
#include <set>
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

    static Type getBusinessType(Good::Type goodType);

    Business(const std::string& name, Type type, unsigned int nbStairs, Good::Type goodType, unsigned int maxSizeStock,
        std::size_t nbEmployees, Work::Type employeeType);
    virtual ~Business();

    virtual std::unique_ptr<Tile> clone() const override;
    virtual void update() override;
    virtual void tearDown() override;
    virtual void setOwner(Company* owner) override;

    Good::Type getGoodType() const;
    unsigned int getStock() const;
    bool hasPreparedGoods() const;
    Money getPrice() const;
    std::unique_ptr<Work>& getManager();
    const std::unique_ptr<Work>& getManager() const;
    std::vector<std::unique_ptr<Work>>& getEmployees();
    const std::vector<std::unique_ptr<Work>>& getEmployees() const;

    // Events
    void onNewMonth();

protected:
    Good::Type mGoodType;
    unsigned int mMaxSizeStock;
    unsigned int mStock;
    Money mStockCost;
    double mPreparedGoods;
    Money mPrice;
    std::vector<std::unique_ptr<Work>> mEmployees;
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
        ar & mGoodType & mMaxSizeStock & mStock & mStockCost & mPreparedGoods & mPrice & mEmployees & mWorksInMarket;
    }
};
