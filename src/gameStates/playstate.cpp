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
    // Handle user selecting advance option after completing a level
    if(level.isCompleted()) {

    } else {
        // Check for level reset
        if(keyStates[SDL_SCANCODE_SPACE]) {
            //enterState(game);
            // level.reset();
        } else if(keyStates[SDL_SCANCODE_ESCAPE]) {
            // Check for pause
            paused = true;
        }

        level.handleEvents(keyStates);
    }
}

void PlayState::update(MemSwap * game, float delta) {
    if(paused) {
        game->setNextState(GAME_STATE_PAUSE);
    } else {
        level.update(delta);

        // check if level is succesfully completed
        levelComplete = level.isCompleted();
        if(levelComplete) {
            // prepare to display msg to user

            // check if player ready to advance to next level or return to menu
            if(advanceLevel) {
                
            } else if (goToMenu) {
                game->setNextState(GAME_STATE_MENU);
            }
        }
    }
}

/// Render function for the game state
void PlayState::render(SDL_Renderer * renderer) const {
    // Render background
    bgTexture->render(0, 0, renderer);

    level.render(renderer);

    // render msg over level if level is completed
    if(levelComplete) {

    }
}

void PlayState::setPaused(bool paused) {
    this->paused = paused;
}

bool PlayState::isPaused() const {
    return paused;
}