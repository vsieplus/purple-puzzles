/**
 * @file splashstate.cpp
 * @author vsie
 * @brief implementation for SplashState class - for loading game resources
 * @version 0.1
 * @date 2020-05-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "memswap.hpp"
#include "gameStates/splashstate.hpp"
#include "utils/resmanager.hpp"

SplashState::SplashState() : GameState(GAME_STATE_SPLASH) {}

void SplashState::enterState(MemSwap * game) {
    // load splash textures prematurely
    bgTexture.loadTexture(game->getResManager().getResPath(BG_ID), 
        game->getRenderer());

    auto loadingAnimation = std::make_shared<Animation>(game->getResManager().getResPath(
        LOAD_ANIM_ID), game->getRenderer(), 32, 32, 50, true);

    loadX = game->getScreenWidth() / 2 - loadingAnimation->getFrameWidth() / 2;
    loadY = game->getScreenHeight() / 2 - loadingAnimation->getFrameHeight() / 2;

    splashAnim.setCurrAnimation(loadingAnimation);
    splashAnim.start();
}

void SplashState::exitState() {
    // reset font alpha status
    if(splashFont.get()) {
        splashFont->setAlpha(0xFF);
    }
}

// Events to handle during splash screen
void SplashState::handleEvents(MemSwap * game, const SDL_Event & e) {
    if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
        advance = true;
    }
}

void SplashState::update(MemSwap * game, float delta) {
    // Continue loading resources until finished
    if(loadingRes) {
        game->loadNextResource();
        loadingRes = game->getResManager().loadingResources();

        // retrieve font once it's loaded
        if(game->fontIsLoaded()) {
            if(!splashFont.get()) {
                splashFont = game->getResManager().getFont(FONT_ID);
                splashFont->setFontColor(game->getButtonTextColor());

                // set advText render pos
                advTextX = (game->getScreenWidth() / 2) - 
                    (splashFont->getTextWidth(LOADING_TEXT) / 2);
                advTextY = (game->getScreenHeight() * 3 / 5);

                splashFont->initRenderDynamicText(advTextX, advTextY, LOADING_TEXT, 
                    !TYPED, FLASHING);
            } else {
                splashFont->updateText(delta);
            }
        }

        if(!loadingRes) {
            splashFont->setRenderingDynamic(false);
        }

    } else {
        if (advance) {
            // Otherwise finish the SPLASH state and set next as the MENU state
            game->setNextState(GAME_STATE_MENU);
        }

        // start rendering advance text graphic to signal user
        if(!splashFont->isRenderingDynamic()) {
            advTextX = (game->getScreenWidth() / 2) - 
                (splashFont->getTextWidth(ADV_TEXT) / 2);            
            
            // typed and flashing, with dark green text
            splashFont->initRenderDynamicText(advTextX, advTextY, ADV_TEXT, 
                TYPED, FLASHING);
            splashFont->setAlpha(0xFF);
        } else {
            splashFont->updateText(delta);
        }
    }

    splashAnim.update(delta);
}

/// Render function for the game state
void SplashState::render(SDL_Renderer * renderer) const {
    // Render background
    bgTexture.render(0, 0, renderer);

    splashAnim.render(loadX, loadY, renderer);

    // Render graphic indicating loading is done, or 'loading text'
    if(splashFont.get() && splashFont->isRenderingDynamic()) {
        splashFont->renderText(renderer);
    }
}