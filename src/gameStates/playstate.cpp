// implementation for Play state
#include "memswap.hpp"
#include "gameStates/menustate.hpp"
#include "gameStates/playstate.hpp"

PlayState::PlayState(MemSwap * game) : GameState(GAME_STATE_PLAY),
    postGameBoard(game->getScreenWidth() / 2 - 
        (game->getResManager().getTexture(POSTGAME_BOARD_ID)->getWidth() / 2),
        game->getScreenHeight() / 2 - 
        (game->getResManager().getTexture(POSTGAME_BOARD_ID)->getHeight() / 2),
        game->getResManager().getTexture(POSTGAME_BOARD_ID),
        game->getResManager().getFont(FONT_ID), POSTGAME_TEXT,
        game->getButtonTextColor(), Label::TextAlignment::ALIGN_CENTER,
        Label::TextAlignment::ALIGN_TOP) {
            
    postGameButtons = MenuState::getSpacedButtons(postGameMenuLabels,
        game->getResManager().getTexture(BUTTON_ID), game->getResManager().getFont(FONT_ID),
        postGameBoard.getScreenX(), postGameBoard.getScreenY() + postGameBoard.getHeight() / 3 , 
        postGameBoard.getScreenX() + postGameBoard.getWidth(),
        postGameBoard.getScreenY() + postGameBoard.getHeight(),
        game->getOutlineColor(), game->getButtonTextColor(), 
        MenuState::MenuScreen::PLAY_POSTGAME);
}

void PlayState::enterState(MemSwap * game) {
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
    levelComplete = false;
}

void PlayState::exitState() {
}

void PlayState::handleEvents(MemSwap * game, const SDL_Event & e) {
    if(!levelComplete) {
        const Uint8 * keyStates = SDL_GetKeyboardState(NULL);

        // Check for level reset (cannot reset on default position)
        if(level.getTilesFlipped() != 0 && keyStates[SDL_SCANCODE_R]) {
            // store stats before resetting
            currTilesFlipped += level.getTilesFlipped();
            currMovesUndone += level.getMovesUndone();
            currNumResets++;

            level.reset(game);
        } else if(keyStates[SDL_SCANCODE_ESCAPE]) {
            // Check for pause
            game->setPaused(true);
        }

        level.handleEvents(keyStates);
    } else {
        // Handle user selecting advance option after completing a level
        postGameButtons.at(currButton).handleEvents(e);

        // check change of button focus
        if(e.type == SDL_KEYDOWN) {
            postGameButtons.at(currButton).setFocus(false);
            switch(e.key.keysym.sym) {
                case SDLK_a:
                    currButton = currButton == 0 ? postGameButtons.size() - 1 : currButton - 1;
                    break;
                case SDLK_d:
                    currButton = currButton == postGameButtons.size() - 1 ? 0 : currButton + 1;
                    break;
            }
            postGameButtons.at(currButton).setFocus(true);
        }
    }
}

void PlayState::updateStats(MemSwap * game) {
    // update profile data after each level complete
    currTilesFlipped += level.getTilesFlipped();
    level.setTilesFlipped(0);

    currMovesUndone += level.getMovesUndone();

    if(currNumResets > 0 || currTilesFlipped > 0 || currMovesUndone > 0) {
        game->updatePlayerStats(currNumResets, currTilesFlipped,
            currMovesUndone, levelComplete, level.isPerfect());
        currNumResets = 0;
        currTilesFlipped = 0;
        currMovesUndone = 0;
    }
}
 
void PlayState::update(MemSwap * game, float delta) {
    // if paused update stats and set pause state
    if(game->isPaused()) {
        updateStats(game);
        game->setNextState(GAME_STATE_PAUSE);
    } else if(levelComplete) {
        // update curr button/check if it has been activated
        postGameButtons.at(currButton).update();
        if(postGameButtons.at(currButton).isActivated()) {
            postGameButtons.at(currButton).setActivated(false);
            handlePGActivation(game);
        }
    } else {
        level.update(delta);

        // check if level is succesfully completed and animation has finished
        levelComplete = level.isCompleted();

        // update stats 1x
        if(levelComplete) {
            postGameButtons.front().setFocus(true);
            updateStats(game);
        }
    }
}

/// handle postgame button activations
void PlayState::handlePGActivation(MemSwap * game) {
    switch(currButton) {
        case PGButton::BUTTON_NEXT:
            // try to advance level (or go to special screen if unable to)
            if(game->advanceLevel()) {
                loadLevel(game);
            } else {
                // do something special
            }
            break;
        case PGButton::BUTTON_MAIN:
            game->setNextState(GAME_STATE_MENU);
            game->setCurrMenuScreen(MenuState::MenuScreen::MENU_MAIN);
            break;
        case PGButton::BUTTON_LVLS:
            game->setNextState(GAME_STATE_MENU);
            game->setCurrMenuScreen(MenuState::MenuScreen::MENU_LVLS);
            break;
    }
}

/// Render function for the game state
void PlayState::render(SDL_Renderer * renderer) const {
    level.render(renderer);

    // render postgame board over level if level is completed
    if(levelComplete) {
        postGameBoard.render(renderer);

        for(auto & button: postGameButtons) {
            button.render(renderer);
        }
    }
}

bool PlayState::levelIsComplete() const {
    return levelComplete;
}