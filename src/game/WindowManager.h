#pragma once

#include <vector>
#include "gui/GuiWindow.h"

class WindowManager
{
public:
    WindowManager(Id id) : mId(id)
    {

    }

    virtual ~WindowManager()
    {
        for (GuiWindow* window : mWindows)
            window->unsubscribe(mId);
    }

    void addWindow(GuiWindow* window)
    {
        window->subscribe(mId);
        mWindows.emplace_back(window);
    }

    void removeWindow(std::size_t i)
    {
        mWindows[i] = mWindows.back();
        mWindows.pop_back();
    }

    bool removeWindow(GuiWidget* window)
    {
        for (std::size_t i = 0; i < mWindows.size(); ++i)
        {
            if (mWindows[i] == window)
            {
                removeWindow(i);
                return true;
            }
        }
        return false;
    }

    std::vector<GuiWindow*>& getWindows()
    {
        return mWindows;
    }


private:
    Id mId;
    std::vector<GuiWindow*> mWindows;
};
