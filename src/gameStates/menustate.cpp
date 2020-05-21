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

MenuState::MenuState() {
    gameStateID = GAME_STATE_MENU;
}

MenuState::~MenuState() {
    exitState();
}

void MenuState::enterState() {
    // Load menu resources
}

void MenuState::exitState() {

}

void MenuState::handleEvents(MemSwap * game) {
    
}

void MenuState::update(MemSwap * game) {
    printf("menu state");
}

/// Render function for the game state
void MenuState::render(SDL_Window * window, SDL_Renderer * renderer) {

}