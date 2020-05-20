// Header file for memswap.cpp

#ifndef MEMSWAP_HPP
#define MEMSWAP_HPP

#include <vector>
#include <memory>

#include <SDL.h>

#include "gameStates/gamestate.hpp"
#include "gameStates/splashstate.hpp"
#include "gameStates/menustate.hpp"
#include "utils/timer.hpp"

class MemSwap {
    private:
        SDL_Window * window;
        SDL_Renderer * renderer;

        std::unique_ptr<Timer> timer;

        // Stack for tracking the game states
        std::vector<std::unique_ptr<GameState>> gameStates;

        bool playing = true;

    public:
        /// Constructor
        MemSwap(SDL_Window * window, SDL_Renderer * renderer); 

        // Handle events
        void handleEvents();

        /// Update the game state
        void update();

        /// Render the current state of the game
        void render(SDL_Window * window, SDL_Renderer * renderer);

        // Manage game states
        void pushGameState(std::unique_ptr<GameState> & state);
        void popGameState();

        void quit();

        bool isPlaying();
        int getGameStateID();
};

#endif // MEMSWAP_HPP