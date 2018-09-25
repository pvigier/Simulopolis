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

    private:
        // Serialization
        friend class boost::serialization::access;

        template<typename Archive>
        void serialize(Archive& ar, const unsigned int /*version*/)
        {
            ar & id & owner & type & balance & previousBalance;
        }
    };

    struct Event
    {
        enum class Type{CREATE_ACCOUNT, CLOSE_ACCOUNT, ACCOUNT_CREATED, TRANSFER_MONEY};

        struct CreateAccountEvent
        {
            Account::Type accountType;
            Money funds;
        };

        struct TransferMoneyEvent
        {
            Id issuer;
            Id receiver;
            Money amount;
        };

        Type type;

        union
        {
            CreateAccountEvent create;
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
    void createAccount(Id owner, Account::Type type, Money funds);
    Id createWorldAccount();
    void closeAccount(Id account);
    Money getBalance(Id account) const;

    // Transfer
    void transferMoney(Id issuer, Id receiver, Money amount);

    // Taxes
    void collectTaxes(Id cityAccount, double incomeTax, double corporateTax);

    // Events
    Event createCreateAccountEvent(Account::Type type, Money funds) const;
    Event createCloseAccountEvent(Id account) const;
    Event createTransferMoneyEvent(Id issuer, Id receiver, Money amount) const;
    Event createAccountCreatedEvent(Id account) const;

private:
    MessageBus* mMessageBus;
    Mailbox mMailbox;
    IdManager<Account> mAccounts;

    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mMailbox & mAccounts;
    }
};

