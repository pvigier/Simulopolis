#pragma once

// City
#include "city/Bank.h"
#include "city/Business.h"
#include "city/City.h"
#include "city/Market.h"
#include "city/Person.h"

// Events

template <typename Archive>
void serialize(Archive &ar, Bank::Event& event, const unsigned int version)
{
    ar & event.type;
    switch (event.type)
    {
        case Bank::Event::Type::CREATE_ACCOUNT:
            ar & event.accountType;
            break;
        case Bank::Event::Type::CLOSE_ACCOUNT:
        case Bank::Event::Type::ACCOUNT_CREATED:
            ar & event.account;
            break;
        case Bank::Event::Type::TRANSFER_MONEY:
            ar & event.transfer.issuer & event.transfer.receiver & event.transfer.amount;
            break;
    }
}

template <typename Archive>
void serialize(Archive &ar, Business::Event& event, const unsigned int version)
{
    ar & event.type & event.accountId & event.price;
}

template <typename Archive>
void serialize(Archive &ar, City::Event& event, const unsigned int version)
{
    ar & event.type;
    switch (event.type)
    {
        case City::Event::Type::NEW_MONTH:
            ar & event.month;
            break;
        case City::Event::Type::NEW_YEAR:
            ar & event.year;
            break;
        case City::Event::Type::NEW_IMMIGRANT:
        case City::Event::Type::IMMIGRANT_EJECTED:
        case City::Event::Type::NEW_CITIZEN:
        case City::Event::Type::CITIZEN_LEFT:
        case City::Event::Type::REMOVE_CITIZEN:
            //ar & event.person;
            break;
        case City::Event::Type::NEW_MINIMUM_WAGE:
            ar & event.minimumWage;
            break;
        case City::Event::Type::BUILDING_DESTROYED:
            //ar & event.building;
            break;
    }
}

template <typename Archive>
void serialize(Archive &ar, Person::Event& event, const unsigned int version)
{
    ar & event.type;
    switch (event.type)
    {
        case Person::Event::Type::LEAVE_HOUSING:
            //ar & event.lease;
            break;
        case Person::Event::Type::QUIT_WORK:
            //ar & event.work;
            break;
        case Person::Event::Type::FIRED:
        case Person::Event::Type::EXPELLED:
            break;
    }
}

// Message

BOOST_SERIALIZATION_SPLIT_FREE(Message)

template <typename Archive>
void save(Archive& ar, const Message& message, const unsigned int version)
{
    ar & message.sender & message.receiver & message.type;
    switch (message.type)
    {
        case MessageType::BANK:
            ar & std::static_pointer_cast<Bank::Event>(message.info);
            break;
        case MessageType::BUSINESS:
            ar & std::static_pointer_cast<Business::Event>(message.info);
            break;
        case MessageType::CITY:
            ar & std::static_pointer_cast<City::Event>(message.info);
            break;
        case MessageType::MARKET:
            ar & std::static_pointer_cast<VMarket::EventBase>(message.info);
            break;
        case MessageType::PERSON:
            ar & std::static_pointer_cast<Person::Event>(message.info);
            break;
        default:
            DEBUG("Message with info of type " << static_cast<int>(message.type) << " is not serializable.");
            break;
    }
}

template <typename Archive>
void load(Archive& ar, Message& message, const unsigned int version)
{
    ar & message.sender & message.receiver & message.type;
    switch (message.type)
    {
        case MessageType::BANK:
        {
            std::shared_ptr<Bank::Event> info;
            ar & info;
            message.info = info;
            break;
        }
        case MessageType::BUSINESS:
        {
            std::shared_ptr<Business::Event> info;
            ar & info;
            message.info = info;
            break;
        }
        case MessageType::CITY:
        {
            std::shared_ptr<City::Event> info;
            ar & info;
            message.info = info;
            break;
        }
        case MessageType::MARKET:
        {
            std::shared_ptr<VMarket::EventBase> info;
            ar & info;
            message.info = info;
            break;
        }
        case MessageType::PERSON:
        {
            std::shared_ptr<Person::Event> info;
            ar & info;
            message.info = info;
            break;
        }
        default:
            break;
    }
}
