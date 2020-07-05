// Header file for memswap.cpp

#ifndef MEMSWAP_HPP
#define MEMSWAP_HPP

#include <unordered_map>
#include <memory>
#include <string>
#include <stdio.h>
#include <array>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "gameStates/gamestate.hpp"

#include "utils/resmanager.hpp"
#include "utils/profile.hpp"

class MemSwap {
    private:
        // for time
        float delta = 0;        // ms passed since last call to update
        Uint64 lastTime = 0;    // last call to update
        Uint64 currTime;        // curr call to update

        // when play state was last started (reset each time enter)
        Uint64 startPlayTime = 0;  

        GameStateID nextState = GAME_STATE_NULL;
        GameStateID currState = GAME_STATE_NULL;

        // Window constants
        int screenWidth = 640;
        int screenHeight = 480;

        // Audio
        const int SOUND_FREQ = 44100;
        const int NUM_CHANNELS = 2;
        const int SAMPLE_SIZE = 2048;

        // current menu screen id
        int currMenuScreen;

        const std::string GAME_TITLE = "Purple Puzzles";
        const std::string RES_PATHS_FILE = "res/res_paths.json";
        const std::string ICON_ID = "window_icon";
        const std::string SAVE_PATH = "res/saves/playerSave.data";

        const std::string CREDITS_STRING = "Purple Puzzles\n\n"
            "Design: vsie\n"
            "Programming: vsie\n"
            "Art: vsie\n"
            "SFX: vsie\n"
            "Music: onemansymphony\n";

        // levelID management
        std::string currLevelID;

        const std::vector<std::string> LVLS_LABELS = {
            "1-1", "1-2", "1-3", "1-4", "1-5", "1-6", "1-7", "1-8", "1-9", "1-X",
            "2-1", "2-2", "2-3", "2-4", "2-5", "2-6", "2-7", "2-8", "2-9", "2-X"
        };

        bool playing = true;
        bool paused = false;

        bool minimized = false;
        bool fullscreen = false; // Press F11 to toggle fullscreen
        
        SDL_Window * window;
        SDL_Renderer * renderer;
        SDL_Event e;

        // colors for GUI

        // outline color (for gui elements) [light purplish]
        const SDL_Color OUTLINE_COLOR = {0x83, 0x86, 0xF5, 0xFF};
        
        // for button text (light greenish)
        const SDL_Color BUTTON_TEXT_COLOR = {0xA0, 0xFF, 0xE3, 0xFF};

        // for title text (same as outline color)
        const SDL_Color TITLE_COLOR = {0x83, 0x86, 0xF5, 0xFF};

        // map for tracking the game states/the current state
        std::unordered_map<GameStateID, std::unique_ptr<GameState>> gameStates;

        // Resource manager for the game
        ResManager resourceManager;

        // Player data/profile
        Profile playerProfile;
        
        // initialize
        SDL_Renderer * init();
        bool initLibs();

        // state management
        void changeState();
        void addGameState(GameStateID gameStateID, std::unique_ptr<GameState> & state);
        void removeGameState(GameStateID gameStateID);

    public:
        /// Constructor
        MemSwap();

        // save/load/reset player profile
        void saveProfile();
        bool loadProfile();
        
        void resetPlayerData();
        void updatePlayTime();
        void updatePlayerStats(int resets, int flipped, int movesUndone, 
            bool completed, bool perfect);

        // Handle events
        void handleEvents();
        void handleWindowEvents();

        /// Update the game state
        void update();

        /// Render the current state of the game
        void render() const;

        // play the specified sound
        void playSound(std::string soundID) const;

        // Manage game states
        void setNextState(GameStateID gameID);

        void quit();

        int indexOfLevelID(std::string ID) const;
        bool levelIsCompleted(std::string levelID) const;

        bool isPlaying() const;
        GameStateID getGameStateID() const;
        SDL_Event getEvent() const;
        SDL_Renderer * getRenderer() const;

        SDL_Color getOutlineColor() const;
        SDL_Color getButtonTextColor() const;
        SDL_Color getTitleColor() const;

        int getScreenWidth() const;
        int getScreenHeight() const;

        int getCurrMenuScreen() const;

        // manage current level in the game
        bool advanceLevel();
        void setCurrLevelID(std::string levelID);

        void setCurrMenuScreen(int screenID);

        std::string getGameTitle() const;
        std::string getCurrLevelID() const;

        std::string getStatsString() const;
        std::string getCreditsString() const;

        std::vector<std::string> getLevelLabels() const;

        void setPaused(bool paused);
        bool isPaused() const;

        void loadNextResource();
        const ResManager & getResManager();
};

#endif // MEMSWAP_HPP