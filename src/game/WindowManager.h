#pragma once

#include <vector>

class VWindowManager
{
public:
    VWindowManager(Id id, const std::string& prefix) : mId(id), mPrefix(prefix), mCounter(0)
    {

    }

    virtual ~VWindowManager()
    {

    }

    std::string getNewName()
    {
        return mPrefix + std::to_string(mCounter++);
    }

    virtual bool removeWindow(GuiWidget* window) = 0;

protected:
    Id mId;

private:
    std::string mPrefix;
    unsigned int mCounter;
};

template<typename T>
class WindowManager : public VWindowManager
{
public:
    using VWindowManager::VWindowManager;

    virtual ~WindowManager()
    {
        for (std::unique_ptr<T>& window : mWindows)
            window->getWindow()->unsubscribe(mId);
    }

    void addWindow(T* window)
    {
        window->getWindow()->subscribe(mId);
        mWindows.emplace_back(window);
    }

    virtual bool removeWindow(GuiWidget* window)
    {
        for (std::size_t i = 0; i < mWindows.size(); ++i)
        {
            if (mWindows[i]->getWindow() == window)
            {
                std::swap(mWindows[i], mWindows.back());
                mWindows.pop_back();
                return true;
            }
        }
        return false;
    }

    std::vector<std::unique_ptr<T>>& getWindows()
    {
        return mWindows;
    }

private:
    std::vector<std::unique_ptr<T>> mWindows;
};
