// Header for Menu State

#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include "gameStates/gamestate.hpp"

// Singleton class for the Menu State
class MenuState : public GameState {
    public:
        MenuState(MemSwap * game);

        void enterState(MemSwap * game) override;
        void exitState() override;

        void handleEvents(MemSwap * game, const SDL_Event & e) override;
        void handleEvents(MemSwap * game, const Uint8 * keyStates) override;
        void update(MemSwap * game, float delta) override;
        void render(SDL_Renderer * renderer) const override;
};

#endif // MENUSTATE_HPP