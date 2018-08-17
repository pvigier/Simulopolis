#pragma once

#define STRONG_TYPEDEF(Type, Handle)       \
class Handle                               \
{                                          \
public:                                    \
    Handle()                               \
    {                                      \
                                           \
    }                                      \
                                           \
    explicit Handle(const Type& t) : mT(t) \
    {                                      \
                                           \
    }                                      \
                                           \
    Handle& operator=(const Type& t)       \
    {                                      \
        mT = t;                            \
        return *this;                      \
    }                                      \
                                           \
    operator const Type&() const           \
    {                                      \
        return mT;                         \
    }                                      \
                                           \
    operator Type&()                       \
    {                                      \
        return mT;                         \
    }                                      \
                                           \
    bool operator<(const Handle& h) const  \
    {                                      \
        return mT < h.mT;                  \
    }                                      \
                                           \
private:                                   \
    Type mT;                               \
};
