 #pragma once

 #include <deque>
 #include "message/Message.h"

class Person;

class Goal
{
public:
    enum class State{ACTIVE, INACTIVE, COMPLETED, FAILED};

    Goal(Person* owner);
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

    void pushFront(Goal* goal);
    void pushBack(Goal* goal);
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
};
