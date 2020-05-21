// Header for Pause State

#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "gameStates/gamestate.hpp"

// class for the Pause State
class PauseState : public GameState {
    public:
        PauseState();

        ~PauseState();

        void enterState() override;
        void exitState() override;

        void handleEvents(MemSwap * game) override;
        void update(MemSwap * game) override;
        void render(SDL_Window * window, SDL_Renderer * renderer) override;
};

#endif // PAUSESTATE_HPP