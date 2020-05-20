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

    // Start game in menu state
    std::unique_ptr<GameState> menuState = std::make_unique<MenuState>();
    pushGameState(menuState);
}

/// Update the current game state
void MemSwap::update() {
    gameStates.back()->update();
}

/// Render the current game state
void MemSwap::render(SDL_Window * window, SDL_Renderer * renderer) {
    gameStates.back()->render(window, renderer);
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

bool MemSwap::isPlaying() { return playing; }
int MemSwap::getGameStateID() { return gameStates.back()->getGameStateID(); }