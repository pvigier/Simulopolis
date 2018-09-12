#include "city/Bank.h"
#include "util/debug.h"
#include "message/MessageBus.h"

Bank::Event::Event()
{

}

Bank::Event::Event(Event::Type type) : type(type)
{

}

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
                    createAccount(message.sender, event.accountType);
                    break;
                case Event::Type::CLOSE_ACCOUNT:
                    closeAccount(event.account);
                    break;
                case Event::Type::TRANSFER_MONEY:
                    transferMoney(event.transfer.issuer, event.transfer.receiver, event.transfer.amount);
                    break;
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

void Bank::createAccount(Id owner, Account::Type type)
{
    Id account = mAccounts.add(Account{UNDEFINED, owner, type, Money(0.0), Money(0.0)});
    mAccounts.get(account).id = account;
    sMessageBus->send(Message::create(owner, MessageType::BANK, createAccountCreatedEvent(account)));
}

Id Bank::createWorldAccount()
{
    Id account = mAccounts.add(Account{UNDEFINED, UNDEFINED, Account::Type::WORLD, Money(0.0), Money(0.0)});
    mAccounts.get(account).id = account;
    return account;
}

void Bank::closeAccount(Id account)
{
    mAccounts.erase(account);
}

Money Bank::getBalance(Id account) const
{
    return mAccounts.get(account).balance;
}

void Bank::transferMoney(Id issuer, Id receiver, Money amount)
{
    if (mAccounts.has(issuer) && mAccounts.has(receiver))
    {
        mAccounts.get(issuer).balance -= amount;
        mAccounts.get(receiver).balance += amount;
    }
    else
    {
        DEBUG_IF(!mAccounts.has(issuer), "Transfer money: invalid issuer (" << issuer << ")\n");
        DEBUG_IF(!mAccounts.has(receiver), "Transfer money: invalid receiver (" << receiver << ")\n");
    }
}

void Bank::collectTaxes(Id cityAccount, double incomeTax, double corporateTax)
{
    for (Account& account : mAccounts.getObjects())
    {
        double rate = 0.0;
        switch (account.type)
        {
            case Account::Type::PERSON:
                rate = incomeTax;
                break;
            case Account::Type::COMPANY:
                rate = corporateTax;
                break;
            default:
                break;
        }
        Money income(account.previousBalance - account.balance); // Maybe change to really tax the income
        if (income > 0.0)
            transferMoney(account.id, cityAccount, Money(rate * income));
        // Update previousBalance
        account.previousBalance = account.balance;
    }
}

Bank::Event Bank::createCreateAccountEvent(Account::Type type) const
{
    Event event(Event::Type::CREATE_ACCOUNT);
    event.accountType = type;
    return event;
}

Bank::Event Bank::createCloseAccountEvent(Id account) const
{
    Event event(Event::Type::CLOSE_ACCOUNT);
    event.account = account;
    return event;
}

Bank::Event Bank::createTransferMoneyEvent(Id issuer, Id receiver, Money amount) const
{
    Event event(Event::Type::TRANSFER_MONEY);
    event.transfer = Event::TransferMoneyEvent{issuer, receiver, amount};
    return event;
}

Bank::Event Bank::createAccountCreatedEvent(Id account) const
{
    Event event(Event::Type::ACCOUNT_CREATED);
    event.account = account;
    return event;
}
