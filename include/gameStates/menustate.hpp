// Header for Menu State

#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include "gameStates/gamestate.hpp"

// Singleton class for the Menu State
class MenuState : public GameState {
    public:
        MenuState();

        ~MenuState();

        void enterState(MemSwap * game) override;
        void exitState() override;

        void handleEvents(MemSwap * game, const Uint8 * keyStates) override;
        void update(MemSwap * game) override;
        void render(SDL_Renderer * renderer) const override;
};

#endif // MENUSTATE_HPP