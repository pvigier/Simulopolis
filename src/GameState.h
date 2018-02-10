#pragma once

#include "util/Id.h"

enum class GameStateName{START, EDITOR};

class MessageBus;
class RenderEngine;
class TextureManager;
class StylesheetManager;

class GameState
{
public:
    GameState();
    virtual ~GameState();

    static void setMessageBus(MessageBus* messageBus);
    static void setGameId(Id gameId);
    static void setRenderEngine(RenderEngine* renderEngine);
    static void setTextureManager(TextureManager* textureManager);
    static void setStylesheetManager(StylesheetManager* stylesheetManager);

    virtual void draw(const float dt) = 0;
    virtual void update(const float dt) = 0;
    virtual void handleInput() = 0;

protected:
    static MessageBus* sMessageBus;
    static Id sGameId;
    static RenderEngine* sRenderEngine;
    static TextureManager* sTextureManager;
    static StylesheetManager* sStylesheetManager;
};
