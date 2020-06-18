/**
 * @file menustate.cpp
 * @author vsie
 * @brief implementation for menustate class
 * @version 0.1
 * @date 2020-05-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "memswap.hpp"

#include "gameStates/menustate.hpp"

MenuState::MenuState(MemSwap * game) : GameState(GAME_STATE_MENU) {
    // retrieve resources
    bgTexture = game->getResManager().getTexture(BG_ID);
    menuFont = game->getResManager().getFont(FONT_ID);

    // add buttons/other gui elements for each menu screen
    addMainGUI(game);
    addLvlSelectGUI(game);
    addStatsGUI(game);
    addConfigGUI(game);
    addCreditsGUI(game);

    // set current button to first button on main menu screen, first ptr
    currButton = &(stateButtons.at(currScreen).at(currButtonID));
}


void MenuState::enterState(MemSwap * game) {
    // reset screen to main
}

void MenuState::exitState() {
}

// functions to add buttons for the specified screen
void MenuState::addMainGUI(MemSwap * game) {
    // start placing menu buttons in bottom 2/3 of inner-bg area
    int buttonAreaX = BG_PAD;
    int buttonAreaY = BG_PAD + ((game->getScreenHeight() - (2 * BG_PAD)) / 3);
   
    int buttonAreaXEnd = game->getScreenWidth() - BG_PAD;
    int buttonAreaYEnd = game->getScreenHeight() - BG_PAD;

    auto menuButton = game->getResManager().getTexture(MENU_BUTTON_ID);

    // get buttons spaced evenly
    std::vector<Button> mainButtons = getSpacedButtons(MAIN_LABELS, menuButton,
        menuFont, buttonAreaX, buttonAreaY, buttonAreaXEnd, buttonAreaYEnd,
        game->getOutlineColor(), game->getButtonTextColor(), MenuScreen::MENU_MAIN);

    // set focus of first button in main menu
    mainButtons.front().setFocus(true);

    stateButtons.emplace(MenuScreen::MENU_MAIN, mainButtons);
    
    // add title label
    std::vector<Label> mainLabels;
    addTitleLabel(mainLabels, game->getGameTitle(), true, game);

    stateLabels.emplace(MenuScreen::MENU_MAIN, mainLabels);
}

void MenuState::addLvlSelectGUI(MemSwap * game) {
    // start placing buttons in bottom 4/5 of inner-bg area
    int buttonAreaX = BG_PAD;
    int buttonAreaY = BG_PAD + ((game->getScreenHeight() - (2 * BG_PAD)) / 5);

    int buttonAreaXEnd = game->getScreenWidth() - BG_PAD;
    int buttonAreaYEnd = game->getScreenHeight() - BG_PAD;

    // calc. button spacing in bottom [3 rows, 10 columns] inside border
    auto lvlButton = game->getResManager().getTexture(LVL_BUTTON_ID);

    // get level buttons
    std::vector<Button> levelSelectButtons = getSpacedButtons(LVLS_LABELS, 
        lvlButton, menuFont, buttonAreaX, buttonAreaY, buttonAreaXEnd, buttonAreaYEnd,
        game->getOutlineColor(), game->getTitleColor(), MenuScreen::MENU_LVLS);

    stateButtons.emplace(MenuScreen::MENU_LVLS, levelSelectButtons);

    // labels
    std::vector<Label> lvlsLabels;

    // title label
    addTitleLabel(lvlsLabels, LVL_SELECT_TITLE, true, game);

    stateLabels.emplace(MenuScreen::MENU_LVLS, lvlsLabels);
}

void MenuState::addStatsGUI(MemSwap * game) {
    std::vector<Button> StatsButtons;

    stateButtons.emplace(MenuScreen::MENU_STATS, StatsButtons);


    std::vector<Label> statsLabels;

    // title label
    addTitleLabel(statsLabels, STATS_TITLE, true, game);

    stateLabels.emplace(MenuScreen::MENU_STATS, statsLabels);
}

void MenuState::addConfigGUI(MemSwap * game) {
    std::vector<Button> ConfigButtons;

    stateButtons.emplace(MenuScreen::MENU_CONFIG, ConfigButtons);

    std::vector<Label> configLabels;

    // title label
    addTitleLabel(configLabels, CONFIG_TITLE, false, game);

    stateLabels.emplace(MenuScreen::MENU_CONFIG, configLabels);
}

void MenuState::addCreditsGUI(MemSwap * game) {
    // only need a 'back' button
    std::vector<Button> creditsButtons;

    stateButtons.emplace(MenuScreen::MENU_CREDITS, creditsButtons);

    std::vector<Label> creditsLabels;

    // title label
    addTitleLabel(creditsLabels, CREDITS_TITLE, false, game);

    stateLabels.emplace(MenuScreen::MENU_CREDITS, creditsLabels);
}

void MenuState::handleEvents(MemSwap * game, const SDL_Event & e) {
    currButton->handleEvents(e);

    // handle switching button focus (except for credit screen [only back btn])
    if(currScreen != MenuScreen::MENU_CREDITS &&  e.type == SDL_KEYDOWN) {
        changeCurrButton(e);
    }
}

// general function to change the current button focus (for any screen)
void MenuState::changeCurrButton(const SDL_Event & e) {
    currButton->setFocus(false);
    auto sym = e.key.keysym.sym;

    // depending on dir, change button in current direction, depending on 
    // button layout of the current screen

    // .first = buttons per row, .second = buttons per column
    auto layout = BTN_LAYOUTS.at(currScreen);
    int & rowBtns = layout.second;          // buttons per row = # cols
    int & colBtns = layout.first;           // buttons per col = # rows
    int totalBtns = rowBtns * colBtns;

    int nextID = -1;

    switch(sym) {
        // user wants to move up
        case SDLK_w:
            // compute hypothetical next button
            nextID = currButtonID - rowBtns;

            // check for up move on first row -> same col, last row
            if(nextID < 0) {
                currButtonID = (totalBtns - ((rowBtns - 1) - currButtonID)) - 1;
            } else {
                currButtonID = nextID;
            }
            break;
        
        // similar as above
        case SDLK_a:
            nextID = currButtonID - 1;

            // move left when on first column -> last column, same row
            if(currButtonID % rowBtns == 0) {
                currButtonID = (currButtonID / rowBtns) * colBtns + colBtns - 1;
            } else {
                currButtonID = nextID;
            }
            break;
        case SDLK_s:
            nextID = currButtonID + rowBtns;

            // moving down when on last row [move to first row in same col.]
            if(nextID > totalBtns - 1) {
                currButtonID = currButtonID % rowBtns;
            } else {
                currButtonID = nextID;
            }
            break;
        case SDLK_d:
            nextID = currButtonID + 1;

            // moving right when on last column
            if((currButtonID + 1) % rowBtns == 0) {
                currButtonID = (currButtonID / rowBtns) * colBtns;
            } else {
                currButtonID = nextID;
            }
            break;      
        default:        
            break;                                         
    }

    updateCurrButton();
}

void MenuState::update(MemSwap * game, float delta) {
    currButton->update();

    // check if current button is activated
    if(currButton->isActivated()) {
        currButton->setActivated(false);

        switch(currScreen) {
            case MenuScreen::MENU_MAIN:
                activateMain();
                break;
            case MenuScreen::MENU_LVLS:
                activateLvlSelect();
                break;
            case MenuScreen::MENU_STATS:
                activateStats();
                break;
            case MenuScreen::MENU_CONFIG:
                activateConfig();
                break;
            case MenuScreen::MENU_CREDITS:
                activateCredits();
                break;
        }
    }
}

// handle button activations for each screen
void MenuState::activateMain() {
    currButton->setFocus(false);

    switch(currButtonID) {
        case MainButton::MAIN_LVLS:
            currScreen = MenuScreen::MENU_LVLS;
            break;
        case MainButton::MAIN_STATS:
            currScreen = MenuScreen::MENU_STATS;
            break;
        case MainButton::MAIN_CONFIG:
            currScreen = MenuScreen::MENU_CONFIG;
            break;    
        case MainButton::MAIN_CREDITS:
            currScreen = MenuScreen::MENU_CREDITS;
            break;
    }

    currButtonID = 0;

    updateCurrButton();
}

void MenuState::activateLvlSelect() {
    
}

void MenuState::activateStats() {

}

void MenuState::activateConfig() {

}

void MenuState::activateCredits() {

}

// call this whenever currScreen/currButtonID changes
void MenuState::updateCurrButton() {
    currButton = &(stateButtons.at(currScreen).at(currButtonID));
    currButton->setFocus(true);
}

/// Render function for the game state
void MenuState::render(SDL_Renderer * renderer) const {
    bgTexture->render(0, 0, renderer);

    // render each of the buttons for the current screen
    for(Button button: stateButtons.at(currScreen)) {
        button.render(renderer);
    }

    // render labels for the current screen
    for(Label label: stateLabels.at(currScreen)) {
        label.render(renderer);
    }
}

// util functions to automatically compute space to evenly space 
// buttons/gui elems in a specified area
int MenuState::calculateInterHSpace(int buttonsPerRow, int buttonWidth, 
    int startX, int endX) {

    return ((endX - startX) - (buttonsPerRow * buttonWidth)) / (buttonsPerRow + 1);
}

int MenuState::calculateInterVSpace(int buttonsPerCol, int buttonHeight, 
    int startY, int endY) {

    return ((endY - startY) - (buttonsPerCol * buttonHeight)) / (buttonsPerCol + 1);
}

std::vector<Button> MenuState::getSpacedButtons (
    const std::vector<std::string> & buttonLabels,
    const std::shared_ptr<Texture> & buttonTexture, 
    const std::shared_ptr<BitmapFont> & buttonFont,
    int buttonAreaX, int buttonAreaY, int buttonAreaXEnd, int buttonAreaYEnd, 
    SDL_Color outlineColor, SDL_Color textColor,
    MenuScreen screen) const {

    // compute inter-button spacings
    auto dims = BTN_LAYOUTS.at(screen);
    int rows = dims.first;
    int cols = dims.second;
                            
    int interHSpace = calculateInterHSpace(cols, buttonTexture->getWidth(), 
        buttonAreaX, buttonAreaXEnd);
    int interVSpace = calculateInterVSpace(rows, buttonTexture->getHeight(),
        buttonAreaY, buttonAreaYEnd);

    std::vector<Button> buttons;

    int buttonWidth = buttonTexture->getWidth();
    int buttonHeight = buttonTexture->getHeight();

    // add each of the four buttons
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            buttons.emplace_back(buttonAreaX + (j * buttonWidth) + ((j + 1) *
                interHSpace), buttonAreaY + (i * buttonHeight) + ((i + 1) *
                interVSpace), CLICKABLE, buttonTexture, buttonFont,
                outlineColor, textColor, buttonLabels.at((i * cols) + j));
        }
    }

    return buttons;
}

// general function to add a title label to the passed vector (modifies it)
void MenuState::addTitleLabel(std::vector<Label> & labels, std::string label,
    bool longLabel, MemSwap * game) const {
        
    // get correct label texture
    auto labelID = longLabel ? MENU_LABEL_LONG_ID : MENU_LABEL_SHORT_ID;
    auto labelSprite = game->getResManager().getTexture(labelID);

    // compute coords (center title label)
    int titleX = game->getScreenWidth() / 2 - labelSprite->getWidth() / 2;
    int titleY = BG_PAD + (game->getScreenHeight() - (2 * BG_PAD)) / 6 -
        labelSprite->getHeight() / 2;

    labels.emplace_back(titleX, titleY, labelSprite, menuFont, label,
        game->getTitleColor());
}