/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "city/Industry.h"
#include "city/City.h"
#include "city/Company.h"
#include "city/Market.h"

Industry::Industry(const std::string& name, Type type, unsigned int nbStairs, Good::Type goodType,
    double employeeProductivity, std::size_t nbEmployees, Work::Type employeeType) :
    Building(name, type, nbStairs), mGood(std::make_unique<Good>(goodType, this)),
    mEmployeeProductivity(employeeProductivity)
{
    mEmployees.emplace_back(std::make_unique<Work>(Work::Type::MANAGER, this));
    for (std::size_t i = 0; i < nbEmployees; ++i)
        mEmployees.emplace_back(std::make_unique<Work>(employeeType, this));
}

Industry::~Industry()
{
    //dtor
}

std::unique_ptr<Tile> Industry::clone() const
{
    return std::make_unique<Industry>(mTextureName, mType, mNbStairs, mGood->getType(), mEmployeeProductivity, mEmployees.size() - 1, mEmployees.back()->getType());
}

void Industry::update()
{
    // Read messages
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type == MessageType::MARKET)
        {
            const MarketBase::EventBase& eventBase = message.getInfo<MarketBase::EventBase>();
            if (eventBase.marketType == MarketBase::Type::WORK)
            {
                const Market<Work>::Event& event = static_cast<const Market<Work>::Event&>(eventBase);
                switch (event.type)
                {
                    case Market<Work>::Event::Type::ITEM_ADDED:
                        mWorksInMarket.insert(event.itemId);
                        break;
                    case Market<Work>::Event::Type::SALE:
                        mWorksInMarket.erase(event.sale.itemId);
                        break;
                    default:
                        break;
                }
            }
            else
            {
                const Market<Good>::Event& event = static_cast<const Market<Good>::Event&>(eventBase);
                switch (event.type)
                {
                    case Market<Good>::Event::Type::ITEM_ADDED:
                        mGoodsInMarket.insert(event.itemId);
                        break;
                    case Market<Good>::Event::Type::SALE:
                        mGoodsInMarket.erase(event.sale.itemId);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

void Industry::tearDown()
{
    update();
    // Remove everything from markets
    const Market<Work>* laborMarket = static_cast<const Market<Work>*>(mOwner->getCity()->getMarket(MarketBase::Type::WORK));
    for (Id id : mWorksInMarket)
        mOwner->getMessageBus()->send(Message::create(laborMarket->getMailboxId(), MessageType::MARKET, laborMarket->createRemoveItemEvent(id)));
    const Market<Good>* goodsMarket = getMarket();
    for (Id id : mGoodsInMarket)
        mOwner->getMessageBus()->send(Message::create(goodsMarket->getMailboxId(), MessageType::MARKET, goodsMarket->createRemoveItemEvent(id)));
}

void Industry::setOwner(Company* owner)
{
    Building::setOwner(owner);
    for (std::unique_ptr<Work>& employee : mEmployees)
        employee->setEmployer(mOwner);
}

Good::Type Industry::getGoodType() const
{
    return mGood->getType();
}

std::unique_ptr<Work>& Industry::getManager()
{
    return mEmployees[0];
}

const std::unique_ptr<Work>& Industry::getManager() const
{
    return mEmployees[0];
}

std::vector<std::unique_ptr<Work>>& Industry::getEmployees()
{
    return mEmployees;
}

const std::vector<std::unique_ptr<Work>>& Industry::getEmployees() const
{
    return mEmployees;
}

void Industry::onNewMonth()
{
    updateStock();
    sellGoods();
}

Money Industry::Batch::getCostPerUnit()
{
    return Money(cost / quantity);
}

void Industry::updateStock()
{
    Batch batch{0.0, Money(0.0)};
    double dmonth = mOwner->getCity()->getWeeklyStandardWorkingHours() / City::NB_HOURS_PER_MONTH;
    for (std::size_t i = 1; i < mEmployees.size(); ++i)
    {
        if (mEmployees[i]->hasWorkedThisMonth())
        {
            batch.quantity += mEmployeeProductivity * mEmployees[i]->getEmployee()->getProductivity() * dmonth;
            batch.cost += mEmployees[i]->getSalary();
        }
    }
    if (getManager()->hasWorkedThisMonth())
    {
        batch.quantity += batch.quantity * getManager()->getEmployee()->getProductivity() * dmonth;
        batch.cost += getManager()->getSalary();
    }
    if (batch.quantity > 0.0)
        mStock.push_back(batch);
}

void Industry::sellGoods()
{
    const Market<Good>* market = getMarket();
    while (!mStock.empty() && (mStock.size() > 1 || mStock.front().quantity >= 1.0))
    {
        // Sell goods
        if (mStock.front().quantity >= 1.0)
        {
            Money costPerUnit = mStock.front().getCostPerUnit();
            Money price(costPerUnit * (1.0 + mOwner->getWholesaleMargin(mGood->getType())));
            mOwner->getMessageBus()->send(Message::create(mMailbox.getId(), market->getMailboxId(), MessageType::MARKET, market->createAddItemEvent(mOwner->getAccount(), mGood.get(), price)));
            mStock.front().quantity -= 1.0;
            mStock.front().cost -= costPerUnit;
            if (mStock.front().quantity <= 0.0)
                mStock.pop_front();
        }
        // Merge two batches
        else
        {
            double needQuantity = 1.0 - mStock.front().quantity;
            if (mStock[1].quantity >= needQuantity)
            {
                Money costPerUnit = mStock[1].getCostPerUnit();
                mStock[1].quantity -= needQuantity;
                mStock[1].cost -= needQuantity * costPerUnit;
                mStock.front().quantity = 1.0;
                mStock.front().cost += needQuantity * costPerUnit;
            }
            else
            {
                mStock[1].quantity += mStock.front().quantity;
                mStock[1].cost += mStock.front().cost;
                mStock.pop_front();
            }
        }
    }
}

const Market<Good>* Industry::getMarket()
{
    switch (mGood->getType())
    {
        case Good::Type::NECESSARY:
            return static_cast<const Market<Good>*>(mOwner->getCity()->getMarket(MarketBase::Type::NECESSARY_GOOD));
        case Good::Type::NORMAL:
            return static_cast<const Market<Good>*>(mOwner->getCity()->getMarket(MarketBase::Type::NORMAL_GOOD));
        case Good::Type::LUXURY:
            return static_cast<const Market<Good>*>(mOwner->getCity()->getMarket(MarketBase::Type::LUXURY_GOOD));
        default:
            return nullptr;
    };
}
