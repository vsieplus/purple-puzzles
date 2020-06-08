// implementation for Play state

#include "memswap.hpp"
#include "gameStates/playstate.hpp"

PlayState::PlayState() : GameState(GAME_STATE_PLAY) {}

void PlayState::enterState(MemSwap * game) {
    // retrieve bg texture
    bgTexture = game->getResManager().getTexture(BG_ID);

    // load level
    std::string levelPath = game->getResManager().getResPath("testing"); 
    level = Level(levelPath, game->getRenderer(), game);
}

void PlayState::exitState() {

}

void PlayState::handleEvents(MemSwap * game, const Uint8 * keyStates) {
    // Check for level reset
    if(keyStates[SDL_SCANCODE_SPACE]) {
        //enterState(game);
        // level.reset();
    }

    level.handleEvents(keyStates);
}

void PlayState::update(MemSwap * game, float delta) {
    level.update(delta);
}

/// Render function for the game state
void PlayState::render(SDL_Renderer * renderer) const {
    // Render background
    bgTexture->render(0, 0, renderer);

    level.render(renderer);
}