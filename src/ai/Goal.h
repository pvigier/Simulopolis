 #pragma once

 #include <deque>
 #include <boost/serialization/access.hpp>
 #include "message/Message.h"

class Person;

class Goal
{
public:
    enum class State{ACTIVE, INACTIVE, COMPLETED, FAILED};

    Goal(Person* owner = nullptr);
    virtual ~Goal();

    virtual void activate() = 0;
    virtual State process() = 0;
    virtual void terminate() = 0;

    virtual bool handle(Message message);

    State getState() const;
    bool isActive() const;
    bool isInactive() const;
    bool isCompleted() const;
    bool hasFailed() const;

    void pushFront(std::unique_ptr<Goal> goal);
    void pushBack(std::unique_ptr<Goal> goal);
    void clearSubgoals();

    virtual std::string toString() const;

protected:
    Person* mOwner;
    State mState;
    std::deque<std::unique_ptr<Goal>> mSubgoals;

    void activateIfInactive();
    void reactivateIfFailed();

    State processSubgoals();
    bool forward(Message message);

private:
    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mOwner & mState & mSubgoals;
    }
};
