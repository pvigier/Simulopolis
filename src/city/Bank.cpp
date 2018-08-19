#include "city/Bank.h"
#include "message/MessageBus.h"

MessageBus* Bank::sMessageBus = nullptr;

void Bank::setMessageBus(MessageBus* messageBus)
{
    sMessageBus = messageBus;
}

Bank::Bank()
{
    sMessageBus->addMailbox(mMailbox);
}

Bank::~Bank()
{
    sMessageBus->removeMailbox(mMailbox);
}

void Bank::update()
{
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type == MessageType::BANK)
        {
            const Event& event = message.getInfo<Event>();
            switch (event.type)
            {
                case Event::Type::CREATE_ACCOUNT:
                    createAccount(message.sender);
                    break;
                case Event::Type::CLOSE_ACCOUNT:
                    closeAccount(event.account);
                    break;
                case Event::Type::TRANSFER_MONEY:
                    transferMoney(event.transfer.issuer, event.transfer.receiver, event.transfer.amount);
                default:
                    break;
            }
        }
    }
}

Id Bank::getMailboxId() const
{
    return mMailbox.getId();
}

void Bank::createAccount(Id owner)
{
    Id account = mAccounts.add(Account{owner, Money(0.0)});
    sMessageBus->send(Message::create(owner, MessageType::BANK, Event{Event::Type::ACCOUNT_CREATED, account}));
}

void Bank::closeAccount(Id account)
{
    mAccounts.erase(account);
}

Money Bank::getBalance(Id account) const
{
    return mAccounts.get(account).balance;
}

bool Bank::transferMoney(Id issuer, Id receiver, Money amount)
{
    mAccounts.get(issuer).balance -= amount;
    mAccounts.get(receiver).balance += amount;
}
