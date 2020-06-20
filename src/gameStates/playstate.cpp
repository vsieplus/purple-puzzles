// implementation for Play state

#include "memswap.hpp"
#include "gameStates/menustate.hpp"
#include "gameStates/playstate.hpp"

PlayState::PlayState() : GameState(GAME_STATE_PLAY) {}

void PlayState::enterState(MemSwap * game) {
    // retrieve bg texture
    bgTexture = game->getResManager().getTexture(BG_ID);

    // load current level (if entering from non-paused state)
    if(!game->isPaused()) {
        loadLevel(game);
    } else {
        game->setPaused(false);
    }
}

// load the curr. level
void PlayState::loadLevel(MemSwap * game) {
    std::string levelPath = game->getResManager().getResPath(game->getCurrLevelID()); 
    level = Level(levelPath, game->getRenderer(), game);
}

void PlayState::exitState() {
}

void PlayState::handleEvents(MemSwap * game, const SDL_Event & e) {
    const Uint8 * keyStates = SDL_GetKeyboardState(NULL);
    // Handle user selecting advance option after completing a level
    if(level.isCompleted()) {

    } else {
        // Check for level reset (cannot reset on default position)
        if(level.getTilesFlipped() != 0 && keyStates[SDL_SCANCODE_R]) {
            // store stats
            currTilesFlipped += level.getTilesFlipped();
            currNumResets++;

            level.reset(game);
        } else if(keyStates[SDL_SCANCODE_ESCAPE]) {
            // Check for pause
            game->setPaused(true);
        }

        level.handleEvents(keyStates);
    }
}

void PlayState::updateStats(MemSwap * game) {
    // update profile data after each level complete
    if(currNumResets != 0 || currTilesFlipped + level.getTilesFlipped() != 0) {
        game->updatePlayerStats(currNumResets, currTilesFlipped + 
            level.getTilesFlipped(), levelComplete, level.isPerfect());
        currNumResets = 0;
        currTilesFlipped = 0;
        level.setTilesFlipped(0);
    }
}
 
void PlayState::update(MemSwap * game, float delta) {
    // if paused update stats and set pause state
    if(game->isPaused()) {
        updateStats(game);
        game->setNextState(GAME_STATE_PAUSE);
    } else {
        level.update(delta);

        // check if level is succesfully completed
        levelComplete = level.isCompleted();
        if(levelComplete) {
            updateStats(game);

            // check if player ready to advance to next level or return to menu
            if(advanceLevel) {
                // try to advance level (or go to special screen if unable to)
                if(game->advanceLevel()) {
                    loadLevel(game);
                } else {
                    // do something special
                }
            } else if (goToMenu) {
                game->setNextState(GAME_STATE_MENU);
                game->setCurrMenuScreen(MenuState::MenuScreen::MENU_LVLS);
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