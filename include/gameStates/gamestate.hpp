// Header for gamestate.cpp

#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <memory>

#include <SDL.h>

#include "utils/texture.hpp"

enum GameStateID {
    GAME_STATE_NULL,    // NULL placeholder
    GAME_STATE_SPLASH,  // load resources/splash screen
    GAME_STATE_MENU,    // menu
    GAME_STATE_PLAY,    // play state
    GAME_STATE_PAUSE,   // pause state
    GAME_STATE_EXIT,    // when user wants to exit the game
};

class MemSwap;

class GameState {
    protected:
        int gameStateID;
        SDL_Event e;

    public:
        GameState(int stateID) : gameStateID(stateID) {};

        // Entering/exiting this specific state
        virtual void enterState(MemSwap * game) = 0;
        virtual void exitState() = 0;

        virtual ~GameState() {};

        /// Event handling method for the game state, event polling
        virtual void handleEvents(MemSwap * game, const SDL_Event & e) = 0;

        /// Update method for the particular game state 
        virtual void update(MemSwap * game, float delta) = 0;

        /// Render function for the game state
        virtual void render(SDL_Renderer * renderer) const = 0;

        int getGameStateID() {
            return gameStateID;
        }
};



#endif // GAMESTATE_HPP