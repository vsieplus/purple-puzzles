/**
 * @file memswap.cpp
 * @author vsie
 * @brief implementation for game engine
 * @version 0.1
 * @date 2020-05-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

 #include "memswap.hpp"
 
#include "gameStates/splashstate.hpp"
#include "gameStates/menustate.hpp"
#include "gameStates/playstate.hpp"
#include "gameStates/pausestate.hpp"

MemSwap::MemSwap() : currTime(SDL_GetPerformanceCounter()), gameStates(), 
    resourceManager(RES_PATHS_FILE, init()), playerProfile() {
    // Initialize SDL components
    if(!initLibs()) {
        printf("Failed to initialize SDL libraries");
    }

    // set window icon
    SDL_Surface * icon = IMG_Load(resourceManager.getResPath(ICON_ID).c_str());
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    // load player profile, or if none exists, init default level status
    if(!loadProfile()) {
        playerProfile.initLevelsCompleted();
    }

    // Start game in splash state to load res/
    setNextState(GAME_STATE_SPLASH);
    changeState();
}

/**
 * @brief initialize SDL window/renderer
 * 
 * @return pointer to the SDL renderer (null if unsuccessfull)
 */
SDL_Renderer * MemSwap::init () {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
		return nullptr;
	} 

	// Try to create window
	window = SDL_CreateWindow(GAME_TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_RESIZABLE);

	if (!window) {
		printf("SDL_Error: %s\n", SDL_GetError());
		return nullptr;
	}
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

    // for rescaling
    SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, 0);
    SDL_RenderSetLogicalSize(renderer, screenWidth, screenHeight);

	return renderer;
}


/**
 * @brief initialize SDL libraries we're using
 * 
 * @return true if successful
 * @return false if unsuccessful
 */
bool MemSwap::initLibs() {
	// Initialize SDL_image
	int imgFlags = IMG_INIT_PNG;
	if(!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_Image Error: %s\n", IMG_GetError());
		return false;
	}

	// Initialize Mixer
	if(Mix_OpenAudio(SOUND_FREQ, MIX_DEFAULT_FORMAT, NUM_CHANNELS, 
		SAMPLE_SIZE) < 0) {
		printf("SDL_mixer error %s\n", Mix_GetError());
		return false;
	}

	return true;
}

// save player profile
void MemSwap::saveProfile() {
    // Open in write/binary mode
	SDL_RWops * saveFile = SDL_RWFromFile(SAVE_PATH.c_str(), "w+b");
	if(saveFile) {
        // Write to file, the profile's starting address, size, and qty. (1)
        SDL_RWwrite(saveFile, &playerProfile, sizeof(Profile), 1);

		SDL_RWclose(saveFile);
	}
}

// return true if loaded succesfully, false if not (i.e. savefile nonexistent)
bool MemSwap::loadProfile() {
	SDL_RWops * saveFile = SDL_RWFromFile(SAVE_PATH.c_str(), "r+b");
    // if file exists, load data to profile, otherwise return false
	if(saveFile) {
        // Read from file into address of playerProfile, for its size in bytes 1x
        SDL_RWread(saveFile, &playerProfile, sizeof(Profile), 1);

		SDL_RWclose(saveFile);
        return true;
	} else {
		return false;
	}
}

/// Handle game events
void MemSwap::handleEvents() {
    if(nextState != GAME_STATE_EXIT && !minimized && !gameStates.empty()) {
        bool playComplete = currState == GAME_STATE_PLAY &&
            dynamic_cast<PlayState *>(gameStates.at(GAME_STATE_PLAY).get())->levelIsComplete();

        // normal polled events
        while(SDL_PollEvent(&e)) {
            handleWindowEvents();
            
            // no polled events for play state, unless in postgame state
            if(currState != GAME_STATE_PLAY || playComplete) {
                gameStates.at(currState)->handleEvents(this, e);
            }
        }

        // keyState events for play state if not completed
        if(currState == GAME_STATE_PLAY && !playComplete) {
            gameStates.at(currState)->handleEvents(this, e);
        }
    }   
}

void MemSwap::handleWindowEvents() {
    if(e.type == SDL_QUIT) {
        setNextState(GAME_STATE_EXIT);
    }

    // Check for resizing/minimization
    if(e.type == SDL_WINDOWEVENT) {
        switch(e.window.event) {
            case SDL_WINDOWEVENT_MINIMIZED:
                minimized = true;
                break;
            case SDL_WINDOWEVENT_RESTORED:
                minimized = false;
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                screenWidth = e.window.data1;
                screenHeight = e.window.data2;
                break;
        }
    }

    // If user presses F11, toggle fullscreen
    if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F11) {
        if(fullscreen) {
            fullscreen = false;
            SDL_SetWindowFullscreen(window, SDL_FALSE);
        } else {
            fullscreen = true;
            SDL_SetWindowFullscreen(window, SDL_TRUE);
        }
    }
}


/// Update the current game state
void MemSwap::update() {
    // update delta
    lastTime = currTime;
    currTime = SDL_GetPerformanceCounter();
    delta = (float) ((currTime - lastTime) * 1000 / (float) SDL_GetPerformanceFrequency());

    if(!gameStates.empty()) {
        gameStates.at(currState)->update(this, delta);
    }
        
    changeState();
}

/// Render the current game state, if not minimized
void MemSwap::render() const {
    if(!minimized) {
        // Clear renderer
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);
        
        // Render stuff for current game state
        gameStates.at(currState)->render(renderer);
        
        // render to screen
        SDL_RenderPresent(renderer);
    }    
}

/// Set next state to change to indicated by the given state ID
void MemSwap::setNextState(GameStateID stateID) {
    nextState = stateID;
}

/// Change states if needed
void MemSwap::changeState() {
    if(nextState != currState) {
        // exit current state/cleanup + fade out
        if(currState != GAME_STATE_NULL) {
            gameStates.at(currState)->exitState();
            gameStates.at(currState)->fade(renderer, this, false); 
        } 

        // If next state set to exit/null, stop playing
        if(nextState == GAME_STATE_EXIT || nextState == GAME_STATE_NULL) {
            playing = false;
            return;
        }

        // check if the next state already exists, if not, create + add to the map
        auto it = gameStates.find(nextState);

        if(it == gameStates.end()) {
            std::unique_ptr<GameState> nextGameState = nullptr;

            switch(nextState) {
                case GAME_STATE_SPLASH:
                    nextGameState = std::make_unique<SplashState>();
                    break;
                case GAME_STATE_MENU:
                    nextGameState = std::make_unique<MenuState>(this);
                    break;
                case GAME_STATE_PLAY:
                    nextGameState = std::make_unique<PlayState>(this);
                    break;
                case GAME_STATE_PAUSE:
                    nextGameState = std::make_unique<PauseState>(this);
                    break;
                default:
                    break;
            }

            if(nextGameState.get()) {
                addGameState(nextState, nextGameState);
            }
        }

        // if we're entering the play state start tracking time
        if(nextState == GAME_STATE_PLAY) {
            startPlayTime = SDL_GetPerformanceCounter();
        }

        // if we're exiting the play state, update play time
        if(currState == GAME_STATE_PLAY) {
            updatePlayTime();
        } else if(currState == GAME_STATE_SPLASH) {
            // Remove splash state if switching off (since it's only used 1x)
            removeGameState(currState);
        }

        // enter new state + fadein        
        gameStates.at(nextState)->enterState(this);
        gameStates.at(nextState)->fade(renderer, this, true);
        currState = nextState;
    }
}

/// Add a game state to the map
void MemSwap::addGameState(GameStateID gameStateID, std::unique_ptr<GameState> & state) {
    gameStates.emplace(gameStateID, std::move(state));
}

/// Remove a game state from the stack (exit a game state)
void MemSwap::removeGameState(GameStateID gameStateID) {
    auto it = gameStates.find(gameStateID);

    if(!gameStates.empty() && it != gameStates.end()) {
        gameStates.at(gameStateID)->exitState();
        gameStates.erase(it);
    }
}

// update player profile data
void MemSwap::updatePlayTime() {
    // update playtime
    Uint64 stopPlayTime = SDL_GetPerformanceCounter();
    playerProfile.addPlayTime((stopPlayTime - startPlayTime) 
        / SDL_GetPerformanceFrequency());
}

// update player game-related stats - call at the end of each level from playstate
// or when exit play state->pause/menu
void MemSwap::updatePlayerStats(int resets, int flipped, int movesUndone, 
    bool completed, bool perfect) {
    if(completed) {
        // set level complete (get idx of ID in LVLS_LABELS)
        playerProfile.setLevelComplete(indexOfLevelID(currLevelID));

        // add perfect play
        if(perfect) {
            playerProfile.addPerfectPlay();
        }
    }

    // add resets/tiles flipped
    playerProfile.addLevelResets(resets);
    playerProfile.addMovesUndone(movesUndone);
    playerProfile.addTilesFlipped(flipped);

    // save
    saveProfile();
}

void MemSwap::quit() {
    // if exiting on play state, update time/player stats
    if(currState == GAME_STATE_PLAY) {
        updatePlayTime();
        auto playState = dynamic_cast<PlayState *>(gameStates.at(GAME_STATE_PLAY).get());
        playState->updateStats(this);
    }

    // save player data on exit
    saveProfile();

    // sdl cleanup
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

int MemSwap::indexOfLevelID(std::string ID) const {
    auto it = std::find(LVLS_LABELS.begin(), LVLS_LABELS.end(), ID);
    return std::distance(LVLS_LABELS.begin(), it);
}

bool MemSwap::levelIsCompleted(std::string levelID) const {
    return playerProfile.levelIsComplete(indexOfLevelID(levelID));
}

bool MemSwap::isPlaying() const { 
    return playing; 
}

GameStateID MemSwap::getGameStateID() const { 
    return currState; 
}

SDL_Event MemSwap::getEvent() const {
    return e;
}

SDL_Renderer * MemSwap::getRenderer() const {
    return renderer;
}

int MemSwap::getScreenWidth() const {
    return screenWidth;
}

int MemSwap::getScreenHeight() const {
    return screenHeight;
}

int MemSwap::getCurrMenuScreen() const {
    return currMenuScreen;
}

std::string MemSwap::getGameTitle() const {
    return GAME_TITLE;
}

std::string MemSwap::getCurrLevelID() const {
    return currLevelID;
}

std::string MemSwap::getStatsString() const {
    return playerProfile.getStatsString();
}

std::string MemSwap::getCreditsString() const {
    std::string credits;

    // get current player data ...
    credits += "Purple Puzzles\n\n";
    credits += "Design: vsie\n";
    credits += "Programming: vsie\n";
    credits += "Art: vsie\n";
    credits += "SFX/Music: vsie\n";

    return credits;
}

std::vector<std::string> MemSwap::getLevelLabels() const {
    return LVLS_LABELS;
}

// tries to advance to next level, returns false if unable to
bool MemSwap::advanceLevel() {
    auto currIndex = (unsigned int) indexOfLevelID(currLevelID);
    
    if(currIndex < LVLS_LABELS.size() - 1) {
        currLevelID = LVLS_LABELS.at(currIndex + 1);
        return true;
    }

    return false;
}

void MemSwap::setCurrLevelID(std::string levelID) {
    currLevelID = levelID;
}

void MemSwap::setCurrMenuScreen(int screenID) {
    currMenuScreen = screenID;
}

void MemSwap::loadNextResource () {
    resourceManager.loadNextResource();
}

const ResManager & MemSwap::getResManager() {
    return resourceManager;
}

SDL_Color MemSwap::getOutlineColor() const {
    return OUTLINE_COLOR;
}

SDL_Color MemSwap::getButtonTextColor() const {
    return BUTTON_TEXT_COLOR;
}

SDL_Color MemSwap::getTitleColor() const {
    return TITLE_COLOR;
}

void MemSwap::setPaused(bool paused) {
    this->paused = paused;
}

bool MemSwap::isPaused() const {
    return paused;
}

void MemSwap::resetPlayerData() {
    playerProfile.resetProfile();
}