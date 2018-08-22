#pragma once

#include <queue>
#include "message/Mailbox.h"
#include "city/Building.h"
#include "city/Good.h"
#include "city/Work.h"

template<typename T> class Market;

class Business : public Building
{
public:
    struct Event
    {
        enum class Type{RESERVATION, RESERVATION_ACCEPTED, RESERVATION_REFUSED, GIVE_GOOD};

        Type type;
    };

    Business(const std::string& name, Type type, unsigned int nbStairs, Good good, std::size_t maxSizeStock,
        std::size_t nbEmployees, Work::Type employeeType);
    virtual ~Business();

    virtual std::unique_ptr<Tile> clone() const override;

    virtual void setOwner(Company* owner) override;

    void update();

    Good getGood() const;
    bool hasPreparedGoods() const;
    Money getPrice() const;
    Work& getManager();
    const Work& getManager() const;
    std::vector<Work>& getEmployees();
    const std::vector<Work>& getEmployees() const;

    // Events
    void onNewMonth();

protected:
    Mailbox mMailbox;
    Good mGood;
    unsigned mMaxSizeStock;
    std::queue<Money> mStock;
    double mPreparedGoods;
    Money mPrice;
    std::vector<Work> mEmployees;

    void prepareGoods();
    void buyGoods();
    const Market<const Building>* getMarket();
};
