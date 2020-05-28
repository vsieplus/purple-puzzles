// Header file for memswap.cpp

#ifndef MEMSWAP_HPP
#define MEMSWAP_HPP

#include <vector>
#include <memory>
#include <string>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "gameStates/gamestate.hpp"
#include "gameStates/splashstate.hpp"
#include "gameStates/menustate.hpp"
#include "gameStates/playstate.hpp"
#include "gameStates/scorestate.hpp"

class MemSwap {
    private:
        SDL_Window * window;
        SDL_Renderer * renderer;
        SDL_Event e;

        // Stack for tracking the game states
        std::vector<std::unique_ptr<GameState>> gameStates;

        int nextState = GAME_STATE_NULL;
        int currState = GAME_STATE_NULL;

        // Window constants
        int screenWidth = 640;
        int screenHeight = 480;

        // Audio
        const int SOUND_FREQ = 44100;
        const int NUM_CHANNELS = 2;
        const int SAMPLE_SIZE = 2048;

        const std::string GAME_TITLE = "Memory Swap";

        bool playing = true;

        bool minimized = false;
        bool fullscreen = false; // Press F11 to toggle fullscreen

    public:
        /// Constructor
        MemSwap(); 

        // initialize
        bool init ();
        bool initLibs();

        // Handle events
        void handleEvents();
        void handleWindowEvents();

        /// Update the game state
        void update();

        /// Render the current state of the game
        void render();

        // Manage game states
        void changeState();
        void setNextState(int gameID);

        void pushGameState(std::unique_ptr<GameState> & state);
        void popGameState();

        void quit();

        bool isPlaying() const;
        int getGameStateID() const;
        SDL_Event getEvent() const;
        SDL_Renderer * getRenderer() const;

        int getScreenWidth() const;
        int getScreenHeight() const;        
};

#endif // MEMSWAP_HPP