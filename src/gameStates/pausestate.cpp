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
    auto emptyButton = game->getResManager().getTexture(BUTTON_ID);

    // load buttons
    int buttonY = game->getScreenHeight() / 2 - emptyButton->getHeight() / 2;

    // calc horiz. button spacing
    int interButtonSpace = (game->getScreenWidth() - (BUTTON_LABELS.size() * 
        emptyButton->getWidth())) / (BUTTON_LABELS.size() + 1);

    // resume, menu, + lvl select
    for(unsigned int i = 0; i < BUTTON_LABELS.size() ; i++) {
        buttons.emplace_back((i + 1) * interButtonSpace + i * emptyButton->getWidth(), 
        buttonY, false, emptyButton, game->getOutlineColor(), BUTTON_LABELS.at(i),
        game->getResManager().getFont(FONT_ID));
        
    }
    
    buttons.at(RESUME_BTN).setFocus(true);
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
        if(e.key.keysym.sym == SDLK_a) {
            changeCurrButton(true);
        } else if(e.key.keysym.sym == SDLK_d) {
            changeCurrButton(false);
        }
    }
}

void PauseState::changeCurrButton(bool left) {
    buttons.at(currButton).setFocus(false);
    if(left) {
        currButton = currButton == RESUME_BTN ? LVLSELECT_BTN : currButton - 1;
    } else {
        currButton = currButton == LVLSELECT_BTN ? RESUME_BTN : currButton + 1;
    }
    buttons.at(currButton).setFocus(true);
}

void PauseState::update(MemSwap * game, float delta) {
    buttons.at(currButton).update();

    // check if the current button has been activated
    if(buttons.at(currButton).isActivated()) {
        switch(currButton) {
            case RESUME_BTN:
                game->setNextState(GAME_STATE_PLAY);
                break;
            case MENU_BTN:
                game->setNextState(GAME_STATE_MENU);
                break;
            case LVLSELECT_BTN:
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