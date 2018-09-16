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

#include <stack>
#include <map>
#include <string>
#include "util/NonCopyable.h"
#include "util/NonMovable.h"
#include "message/MessageBus.h"
#include "render/RenderEngine.h"
#include "input/InputEngine.h"
#include "audio/AudioEngine.h"
#include "resource/ResourceManager.h"

class GameState;

class Game : public NonCopyable, public NonMovable
{
public:
    Game();
    ~Game();

    void pushState(std::unique_ptr<GameState> state);
    void popState();
    void changeState(std::unique_ptr<GameState> state);
    GameState* peekState();
    void clearStates();

    void run();

private:
    MessageBus mMessageBus;
    Mailbox mMailbox;
    RenderEngine mRenderEngine;
    InputEngine mInputEngine;
    AudioEngine mAudioEngine;
    ResourceManager mResourceManager;
    std::stack<std::unique_ptr<GameState>> mStates;

    void handleMessages();
};
