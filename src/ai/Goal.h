 #pragma once

 #include <deque>
 #include "message/Message.h"

class Person;

class Goal
{
public:
    enum class State{ACTIVE, INACTIVE, COMPLETED, FAILED};

    Goal();
    virtual ~Goal();

    virtual void activate() = 0;
    virtual State process() = 0;
    virtual void terminate() = 0;

    virtual void handle(Message message) = 0;

    State getState() const;
    bool isActive() const;
    bool isInactive() const;
    bool isCompleted() const;
    bool hasFailed() const;

    void pushFront(Goal* goal);
    void pushBack(Goal* goal);
    void clearSubgoals();

protected:
    Person* mOwner;
    State mState;
    std::deque<std::unique_ptr<Goal>> mSubgoals;

    void activateIfInactive();
    void reactivateIfFailed();

    State processSubgoals();
};
