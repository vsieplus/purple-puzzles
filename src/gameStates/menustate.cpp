/**
 * @file menustate.cpp
 * @author vsie
 * @brief implementation for menustate class
 * @version 0.1
 * @date 2020-05-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "gameStates/menustate.hpp"

MenuState::MenuState(MemSwap * game) : GameState(GAME_STATE_MENU) {}

void MenuState::enterState(MemSwap * game) {
    // Load menu resources
}

void MenuState::exitState() {

}

void MenuState::handleEvents(MemSwap * game, const Uint8 * keyStates) {
    
}

void MenuState::handleEvents(MemSwap * game, const SDL_Event & e) {
    
}

void MenuState::update(MemSwap * game, float delta) {
}

/// Render function for the game state
void MenuState::render(SDL_Renderer * renderer) const {

}