/**
 * @file memswap.cpp
 * @author vsie
 * @brief implementation for game instance
 * @version 0.1
 * @date 2020-05-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

 #include "memswap.hpp"

MemSwap::MemSwap(SDL_Window * window, SDL_Renderer * renderer) : 
    window(window), renderer(renderer) {

    // Start game in splash state to load res/
    setNextState(GAME_STATE_SPLASH);
    changeState();
}

/// Handle game events
void MemSwap::handleEvents() {
    gameStates.back()->handleEvents(this);
}


/// Update the current game state
void MemSwap::update() {
    gameStates.back()->update(this);

    changeState();
}

/// Render the current game state
void MemSwap::render(SDL_Window * window, SDL_Renderer * renderer) {
    gameStates.back()->render(window, renderer);
}

/// Set next state to change to indicated by the given state ID
void MemSwap::setNextState(int stateID) {
    nextState = stateID;
}

/// Change states if needed
void MemSwap::changeState() {
    int currState = getGameStateID();

    if(nextState != currState) {
        // If next state set to exit, stop playing
        if(nextState == GAME_STATE_EXIT) {
            playing = false;
            return;
        }

        // Pop off current state, unless it's a PLAY state
        if(currState != GAME_STATE_NULL && currState != GAME_STATE_PLAY) {
            popGameState();
        }

        std::unique_ptr<GameState> nextGameState = NULL;

        switch(nextState) {
            case GAME_STATE_SPLASH:
                // Load game resources from splash state
                nextGameState = std::make_unique<SplashState>();
                break;
            case GAME_STATE_MENU:

                break;
            case GAME_STATE_PLAY:

                break;
            case GAME_STATE_SCORE:

                break;
        }

        // Push the next game state
        pushGameState(nextGameState);
    }
}

/// Add a game state to the stack (enter a game state)
void MemSwap::pushGameState(std::unique_ptr<GameState> & state) {
    state->enterState();
    gameStates.push_back(std::move(state));
}

/// Remove a game state from the stack (exit a game state)
void MemSwap::popGameState() {
    gameStates.pop_back();
}

void MemSwap::quit() {
    gameStates.clear();
}

bool MemSwap::isPlaying() { 
    return playing; 
}

int MemSwap::getGameStateID() { 
    if(gameStates.empty()) {
        return GAME_STATE_NULL;
    }

    return gameStates.back()->getGameStateID(); 
}