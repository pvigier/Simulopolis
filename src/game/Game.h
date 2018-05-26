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

    void pushState(GameState* state);
    void popState();
    void changeState(GameState* state);
    GameState* peekState();

    void run();

private:
    std::stack<GameState*> mStates;
    MessageBus mMessageBus;
    Mailbox mMailbox;
    RenderEngine mRenderEngine;
    InputEngine mInputEngine;
    AudioEngine mAudioEngine;
    ResourceManager mResourceManager;

    void handleMessages();
};
