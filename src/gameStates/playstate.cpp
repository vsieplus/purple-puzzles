// implementation for Play state

#include "memswap.hpp"
#include "gameStates/playstate.hpp"

PlayState::PlayState() : GameState(GAME_STATE_PLAY) {}

void PlayState::enterState(MemSwap * game) {
    level = Level("res/maps/0-0.tmx", game->getRenderer(), game);
}

void PlayState::exitState() {

}

void PlayState::handleEvents(MemSwap * game, const Uint8 * keyStates) {
    // Check for level reset
    if(keyStates[SDL_SCANCODE_SPACE]) {
        enterState(game);
    }

    level.handleEvents(keyStates);
}

void PlayState::update(MemSwap * game) {
    level.update();
}

/// Render function for the game state
void PlayState::render(SDL_Renderer * renderer) {
    level.render(renderer);
}