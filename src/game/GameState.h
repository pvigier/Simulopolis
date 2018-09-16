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

#include "util/NonCopyable.h"
#include "util/NonMovable.h"
#include "message/Mailbox.h"

class MessageBus;
class RenderEngine;
class InputEngine;
class AudioEngine;
class TextureManager;
class StylesheetManager;
class GuiManager;
class MusicManager;

class GameState : public NonCopyable, public NonMovable
{
public:
    struct Event
    {
        enum class Type{OPEN_MENU, OPEN_NEW_CITY_SCREEN, OPEN_CITY_LOADING_SCREEN, OPEN_SETTINGS,
            NEW_GAME, LOAD_GAME, PAUSE_GAME, RESUME_GAME};

        Type type;

        union
        {
            uint64_t seed;
        };

        Event(Type type);
        Event(uint64_t seed);
    };

    GameState();
    virtual ~GameState();

    static void setMessageBus(MessageBus* messageBus);
    static void setGameId(Id gameId);
    static void setRenderEngine(RenderEngine* renderEngine);
    static void setInputEngine(InputEngine* inputEngine);
    static void setAudioEngine(AudioEngine* audioEngine);
    static void setTextureManager(TextureManager* textureManager);
    static void setStylesheetManager(StylesheetManager* stylesheetManager);
    static void setGuiManager(GuiManager* guiManager);
    static void setMusicManager(MusicManager* musicManager);

    virtual void enter() = 0;
    virtual void handleMessages() = 0;
    virtual void update(float dt) = 0;
    virtual void draw() = 0;
    virtual void exit() = 0;

protected:
    static MessageBus* sMessageBus;
    static Id sGameId;
    static RenderEngine* sRenderEngine;
    static InputEngine* sInputEngine;
    static AudioEngine* sAudioEngine;
    static TextureManager* sTextureManager;
    static StylesheetManager* sStylesheetManager;
    static GuiManager* sGuiManager;
    static MusicManager* sMusicManager;

    Mailbox mMailbox;
};
