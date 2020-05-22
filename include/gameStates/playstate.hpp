// Header for Play State

#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include <memory>

#include "gameStates/gamestate.hpp"
#include "utils/timer.hpp"

// Singleton class for the Play State
class PlayState : public GameState {
    private:
        std::unique_ptr<Timer> timer;

    public:
        PlayState();

        ~PlayState();

        void enterState(MemSwap * game) override;
        void exitState() override;

        void handleEvents(MemSwap * game, const Uint8 * keyStates) override;
        void update(MemSwap * game) override;
        void render(SDL_Renderer * renderer) override;

        void pause();
};

#endif // PLAYSTATE_HPP