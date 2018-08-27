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
