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
#include "utils/resmanager.hpp"

SplashState::SplashState() : GameState(GAME_STATE_SPLASH) {}

SplashState::~SplashState() {
    exitState();
}

void SplashState::enterState(MemSwap * game) {
    // load splash textures prematurely
    bgTexture.loadTexture(game->getResManager().getResPath(BG_ID), 
        game->getRenderer());
    advTexture.loadTexture(game->getResManager().getResPath(ADV_ID), 
        game->getRenderer());

    // set advText render pos
    advTextX = (game->getScreenWidth() / 2) - (advTexture.getWidth() / 2);
    advTextY = (game->getScreenHeight() / 2) - (advTexture.getHeight() / 2);
}

void SplashState::exitState() {

}

// Events to handle during splash screen
void SplashState::handleEvents(MemSwap * game, const Uint8 * keyStates) {
    // Check if user pressed Enter to advance from splash
    if(keyStates[SDL_SCANCODE_RETURN]) {
        advance = true;
    } 
}

void SplashState::update(MemSwap * game) {
    // Continue loading resources until finished
    if(loadingRes) {
        game->getResManager().loadNextResource();
        loadingRes = game->getResManager().loadingResources();
    } else {
        if (advance) {
            // Otherwise finish the SPLASH state and set next as the MENU state
            game->setNextState(GAME_STATE_PLAY);
        }

        // Flash advance text graphic to signal user
        advTexture.updateAlpha();
    }     
}

/// Render function for the game state
void SplashState::render(SDL_Renderer * renderer) const {
    // Render background
    bgTexture.render(0, 0, renderer);

    // Render progress

    // Render graphic indicating loading is done
    if(!loadingRes) {
        advTexture.render(advTextX, advTextY, renderer);
    }
}