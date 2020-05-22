/**
 * @file splashstate.cpp
 * @author vsie
 * @brief implementation for SplashState class - for loading game resources
 * @version 0.1
 * @date 2020-05-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "memswap.hpp"
#include "gameStates/splashstate.hpp"

SplashState::SplashState() {
    gameStateID = GAME_STATE_SPLASH;
}

SplashState::~SplashState() {
    exitState();
}

void SplashState::enterState() {    
}

void SplashState::exitState() {

}

// Events to handle during splash screen
void SplashState::handleEvents(MemSwap * game) {
    e = game->getEvent();
    while(SDL_PollEvent(&e)) {
        // Check if user pressed Enter to advance from splash
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
            advance = true;
        }
    } 
}

void SplashState::update(MemSwap * game) {
    // Continue loading resources until finished
    if(loadingRes) {

    } else if (advance) {
            // Otherwise finish the SPLASH state the MENU state when
            game->setNextState(GAME_STATE_MENU);
    }     
}

/// Render function for the game state
void SplashState::render(SDL_Window * window, SDL_Renderer * renderer) {

}

void SplashState::loadResources() {

}