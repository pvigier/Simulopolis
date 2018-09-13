#pragma once

#include "util/IdManager.h"
#include "message/Mailbox.h"
#include "city/Money.h"

class MessageBus;

class Bank
{
public:
    struct Account
    {
        enum Type{PERSON, COMPANY, WORLD};
        Id id;
        Id owner;
        Type type;
        Money balance;
        Money previousBalance;
    };

    struct Event
    {
        enum class Type{CREATE_ACCOUNT, CLOSE_ACCOUNT, ACCOUNT_CREATED, TRANSFER_MONEY};

        struct TransferMoneyEvent
        {
            Id issuer;
            Id receiver;
            Money amount;
        };

        Type type;

        union
        {
            Account::Type accountType;
            TransferMoneyEvent transfer;
            Id account;
        };

        Event();
        Event(Type type);
    };

    Bank();
    ~Bank();

    void setMessageBus(MessageBus* messageBus, bool alreadyAdded = false);

    void update();

    Id getMailboxId() const;

    // Account management
    void createAccount(Id owner, Account::Type type);
    Id createWorldAccount();
    void closeAccount(Id account);
    Money getBalance(Id account) const;

    // Transfer
    void transferMoney(Id issuer, Id receiver, Money amount);

    // Taxes
    void collectTaxes(Id cityAccount, double incomeTax, double corporateTax);

    // Events
    Event createCreateAccountEvent(Account::Type type) const;
    Event createCloseAccountEvent(Id account) const;
    Event createTransferMoneyEvent(Id issuer, Id receiver, Money amount) const;
    Event createAccountCreatedEvent(Id account) const;

private:
    MessageBus* mMessageBus;
    Mailbox mMailbox;
    IdManager<Account> mAccounts;
};

