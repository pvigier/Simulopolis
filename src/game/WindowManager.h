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
