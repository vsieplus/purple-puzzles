// Header for Score State

#ifndef SCORESTATE_HPP
#define SCORESTATE_HPP

#include "gameStates/gamestate.hpp"

// Singleton class for the Score State
class ScoreState : public GameState {
    public:
        ScoreState();

        ~ScoreState();

        void enterState(MemSwap * game) override;
        void exitState() override;

        void handleEvents(MemSwap * game) override;
        void update(MemSwap * game) override;
        void render(SDL_Renderer * renderer) override;
};

#endif // SCORESTATE_HPP