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

ScoreState::ScoreState() {
    gameStateID = GAME_STATE_SCORE;
}

ScoreState::~ScoreState() {
    exitState();
}

void ScoreState::enterState() {

}

void ScoreState::exitState() {

}

void ScoreState::handleEvents(MemSwap * game) {
    
}

void ScoreState::update(MemSwap * game) {
    printf("score state");
}

/// Render function for the game state
void ScoreState::render(SDL_Window * window, SDL_Renderer * renderer) {

}