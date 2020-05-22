/**
 * @file playstate.cpp
 * @author vsie
 * @brief implementation for PlayState class
 * @version 0.1
 * @date 2020-05-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "gameStates/playstate.hpp"

PlayState::PlayState() : GameState(GAME_STATE_PLAY) {}

PlayState::~PlayState() {
    exitState();
}

void PlayState::enterState(MemSwap * game) {

}

void PlayState::exitState() {

}

void PlayState::handleEvents(MemSwap * game) {
    
}

void PlayState::update(MemSwap * game) {
    printf("play state");
}

/// Render function for the game state
void PlayState::render(SDL_Renderer * renderer) {

}