#include "ai/GoalGetBetterWork.h"
#include "city/City.h"
#include "city/Person.h"
#include "city/Market.h"

GoalGetBetterWork::GoalGetBetterWork(Person* owner, unsigned int nbMonthsBeforeFailing) :
    Goal(owner), mNbMonthsBeforeFailing(nbMonthsBeforeFailing), mMarket(nullptr)
{
    //ctor
}

GoalGetBetterWork::~GoalGetBetterWork()
{
    //dtor
}

void GoalGetBetterWork::activate()
{
    mState = State::ACTIVE;
    // Choose the right market
    mMarket = static_cast<Market<Work>*>(mOwner->getCity()->getMarket(VMarket::Type::WORK));
}

Goal::State GoalGetBetterWork::process()
{
    activateIfInactive();

    // Try to obtain a better work
    if (!isCompleted())
    {
        Work::Qualification qualification = mOwner->getQualification();
        const Work* work = mOwner->getWork();
        for (const Market<Work>::Item* item : mMarket->getItems())
        {
            if (static_cast<int>(qualification) >= static_cast<int>(item->good->getQualification()) &&
                (!work || item->good->getSalary() > work->getSalary()))
                mMarket->addBid(item->id, mOwner->getMailboxId(), item->reservePrice);
        }
    }
    else if (mNbMonthsBeforeFailing == 0)
        mState = State::FAILED;
    --mNbMonthsBeforeFailing;

    return mState;
}

void GoalGetBetterWork::terminate()
{

}

bool GoalGetBetterWork::handle(Message message)
{
    if (message.type == MessageType::MARKET)
    {
        const Market<Work>::Event& event = message.getInfo<Market<Work>::Event>();
        if (event.type == Market<Work>::Event::Type::PURCHASE)
        {
            mOwner->quitWork();
            mOwner->setWork(event.good);
            mState = State::COMPLETED;
            return true;
        }
    }
    return false;
}

std::string GoalGetBetterWork::toString() const
{
    return "Get a better work";
}
