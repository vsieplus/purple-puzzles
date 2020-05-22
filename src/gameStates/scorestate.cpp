/**
 * @file scorestate.cpp
 * @author vsie
 * @brief implementation for ScoreState class
 * @version 0.1
 * @date 2020-05-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "gameStates/scorestate.hpp"

ScoreState::ScoreState() : GameState(GAME_STATE_SCORE) {}

ScoreState::~ScoreState() {
    exitState();
}

void ScoreState::enterState(MemSwap * game) {

}

void ScoreState::exitState() {

}

void ScoreState::handleEvents(MemSwap * game, const Uint8 * keyStates) {
    
}

void ScoreState::update(MemSwap * game) {
    printf("score state");
}

/// Render function for the game state
void ScoreState::render(SDL_Renderer * renderer) {

}