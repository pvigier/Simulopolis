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

#include "game/GameState.h"
#include "message/MessageBus.h"

MessageBus* GameState::sMessageBus = nullptr;
Id GameState::sGameId = UNDEFINED;
RenderEngine* GameState::sRenderEngine = nullptr;
InputEngine* GameState::sInputEngine = nullptr;
AudioEngine* GameState::sAudioEngine = nullptr;
TextureManager* GameState::sTextureManager = nullptr;
StylesheetManager* GameState::sStylesheetManager = nullptr;
GuiManager* GameState::sGuiManager = nullptr;
MusicManager* GameState::sMusicManager = nullptr;
SaveManager* GameState::sSaveManager = nullptr;

GameState::Event::Event(Type type) : type(type)
{

}

GameState::GameState()
{
    // Register the mailbox
    sMessageBus->addMailbox(mMailbox);
}

GameState::~GameState()
{
    sMessageBus->removeMailbox(mMailbox);
}

void GameState::setMessageBus(MessageBus* messageBus)
{
    sMessageBus = messageBus;
}

void GameState::setGameId(Id gameId)
{
    sGameId = gameId;
}

void GameState::setRenderEngine(RenderEngine* renderEngine)
{
    sRenderEngine = renderEngine;
}

void GameState::setInputEngine(InputEngine* inputEngine)
{
    sInputEngine = inputEngine;
}

void GameState::setAudioEngine(AudioEngine* audioEngine)
{
    sAudioEngine = audioEngine;
}

void GameState::setTextureManager(TextureManager* textureManager)
{
    sTextureManager = textureManager;
}

void GameState::setStylesheetManager(StylesheetManager* stylesheetManager)
{
    sStylesheetManager = stylesheetManager;
}

void GameState::setGuiManager(GuiManager* guiManager)
{
    sGuiManager = guiManager;
}

void GameState::setMusicManager(MusicManager* musicManager)
{
    sMusicManager = musicManager;
}

void GameState::setSaveManager(SaveManager* saveManager)
{
    sSaveManager = saveManager;
}
