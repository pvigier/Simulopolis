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
    Work::Qualification qualification = mOwner->getQualification();
    VMarket::Type marketType;
    if (qualification == Work::Qualification::NON_QUALIFIED)
        marketType = VMarket::Type::NON_QUALIFIED_JOB;
    else if (qualification == Work::Qualification::QUALIFIED)
        marketType = VMarket::Type::QUALIFIED_JOB;
    else
        marketType = VMarket::Type::HIGHLY_QUALIFIED_JOB;
    mMarket = static_cast<Market<Work>*>(mOwner->getCity()->getMarket(marketType));
}

Goal::State GoalGetBetterWork::process()
{
    activateIfInactive();

    const Work* work = mOwner->getWork();
    for (const Market<Work>::Item* item : mMarket->getItems())
    {
        if (item->good->getSalary() > work->getSalary())
            mMarket->addBid(item->id, mOwner->getMailboxId(), item->reservePrice);
    }

    --mNbMonthsBeforeFailing;
    if (mNbMonthsBeforeFailing == 0)
        mState = State::FAILED;

    return mState;
}

void GoalGetBetterWork::terminate()
{

}

bool GoalGetBetterWork::handle(Message message)
{
    // To do
}

std::string GoalGetBetterWork::toString() const
{
    return "Get a better work";
}
