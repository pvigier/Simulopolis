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
        Id owner;
        Money balance;
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
            Id account;
            TransferMoneyEvent transfer;
        };
    };

    static void setMessageBus(MessageBus* messageBus);

    Bank();
    ~Bank();

    void update();

    Id getMailboxId() const;

    // Account management
    void createAccount(Id owner);
    Id createAccount();
    void closeAccount(Id account);
    Money getBalance(Id account) const;

    // Transfer
    bool transferMoney(Id issuer, Id receiver, Money amount);

private:
    static MessageBus* sMessageBus;

    Mailbox mMailbox;
    IdManager<Account> mAccounts;
};

