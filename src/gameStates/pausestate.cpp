/**
 * @file pausestate.cpp
 * @author vsie
 * @brief implementation for PauseState class
 * @version 0.1
 * @date 2020-05-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "memswap.hpp"
#include "gameStates/pausestate.hpp"

PauseState::PauseState(MemSwap * game) : GameState(GAME_STATE_PAUSE) {
    // get bg texture
    bgTexture = game->getResManager().getTexture(BG_ID);

    addButtons(game);
}

void PauseState::addButtons(MemSwap * game) {
    // load buttons
    int buttonY = game->getScreenHeight() * 2 / 5;

    // calc horiz. button spacing
    int interButtonSpace = (game->getScreenWidth() - 
        (BUTTON_IDS.size() * BUTTON_WIDTH)) / (BUTTON_IDS.size() + 1);

    int i = 1;

    // resume, menu, + lvl select
    for(std::string buttonID: BUTTON_IDS) {
        buttons.emplace_back(i * interButtonSpace + (i - 1) * BUTTON_WIDTH, buttonY, false,
            game->getResManager().getTexture(buttonID), game->getOutlineColor());
        i++;
    }
    
    buttons.at(RESUME_ID).setFocus(true);
}


void PauseState::enterState(MemSwap * game) {

}

void PauseState::exitState() {

}

void PauseState::handleEvents(MemSwap * game, const SDL_Event & e) {
    // handle events for curr button
    buttons.at(currButton).handleEvents(e);

    // check if user switch between buttons (press A/D key to cycle through)
    if(e.type == SDL_KEYDOWN) {
        switch(e.key.keysym.sym) {
            case SDLK_a:
                changeCurrButton(true);
                break;
            case SDLK_d:
                changeCurrButton(false);
                break;
            default:
                break;
        }
    }
}

void PauseState::changeCurrButton(bool left) {
    buttons.at(currButton).setFocus(false);
    if(left) {
        currButton = currButton == RESUME_ID ? LVLSELECT_ID : currButton - 1;
    } else {
        currButton = currButton == LVLSELECT_ID ? RESUME_ID : currButton + 1;
    }
    buttons.at(currButton).setFocus(true);
}

void PauseState::update(MemSwap * game, float delta) {
    // check if the current button has been activated
    if(buttons.at(currButton).isActivated()) {
        switch(currButton) {
            case RESUME_ID:
                game->setNextState(GAME_STATE_PLAY);
                break;
            case MENU_ID:
                game->setNextState(GAME_STATE_MENU);
                break;
            case LVLSELECT_ID:
                game->setNextState(GAME_STATE_MENU);
                // setMenuPos(... lvl select)
                break;
        }
    }
}

/// Render function for the game state
void PauseState::render(SDL_Renderer * renderer) const {
    bgTexture->render(0, 0, renderer);

    // render each of the buttons
    for(Button button: buttons) {
        button.render(renderer);
    }
}