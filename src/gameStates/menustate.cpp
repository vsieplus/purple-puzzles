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

MenuState::MenuState(MemSwap * game) : GameState(GAME_STATE_MENU),
    bgTexture(game->getResManager().getTexture(BG_ID)),
    menuFont(game->getResManager().getFont(FONT_ID)),
    menuMusic(game->getResManager().getMusic(MENU_MUSIC_ID)) {

    // add buttons/other gui elements for each menu screen
    addMainGUI(game);
    addLvlSelectGUI(game);
    addStatsGUI(game);
    addHTPGUI(game);
    addCreditsGUI(game);

    // set current button to first button on main menu screen, first ptr
    currButton = &(stateButtons.at(currScreen).at(currButtonID));
}


void MenuState::enterState(MemSwap * game) {
    // get current menu screen (or if first time entering, do nothing)
    if(returning) {
        currScreen = (MenuScreen) game->getCurrMenuScreen();
        currButtonID = 0;
        updateCurrButton();

        // update level select buttons based on player progress
        updateLevelSelectButtons(game);
    } else {
        returning = true;
    }

    menuMusic->play();
}

void MenuState::exitState() {
    Music::stop();
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
    std::vector<Button> levelSelectButtons = getSpacedButtons(game->getLevelLabels(), 
        lvlButton, menuFont, buttonAreaX, buttonAreaY, buttonAreaXEnd, buttonAreaYEnd,
        game->getOutlineColor(), game->getButtonTextColor(), MenuScreen::MENU_LVLS);

    addBackButton(levelSelectButtons, game);
    stateButtons.emplace(MenuScreen::MENU_LVLS, levelSelectButtons);

    // set locked graphic for locked levels
    updateLevelSelectButtons(game);

    // labels
    std::vector<Label> lvlsLabels;

    // title label
    addTitleLabel(lvlsLabels, LVL_SELECT_TITLE, true, game);

    stateLabels.emplace(MenuScreen::MENU_LVLS, lvlsLabels);
}


// set levels not yet unlocked to display the 'locked' graphic temporarily
void MenuState::updateLevelSelectButtons(MemSwap * game) {
    auto & lsButtons = stateButtons.at(MenuScreen::MENU_LVLS);

    auto lockedGraphic = game->getResManager().getTexture(LVL_LOCKED_ID);

    for(auto & levelID: game->getLevelLabels()) {
        unsigned int IDIndex = game->indexOfLevelID(levelID);

        // (no need to check the last two buttons (last level/back button))
        if(IDIndex >= lsButtons.size() - 2) continue;

        // if current is completed, set the next one unlocked
        if(game->levelIsCompleted(levelID)) {
            lsButtons.at(IDIndex + 1).removeGraphic();
        } else {
            // otherwise add a 'locked' graphic to the button
            lsButtons.at(IDIndex + 1).setGraphic(lockedGraphic);
        }
    }
}

void MenuState::addStatsGUI(MemSwap * game) {
    std::vector<Label> statsLabels;

    // title label
    addTitleLabel(statsLabels, STATS_TITLE, true, game);

    // stats board as a label
    addTextBoard(statsLabels, game->getStatsString(), game, 
        Label::TextAlignment::ALIGN_TOP);

    stateLabels.emplace(MenuScreen::MENU_STATS, statsLabels);

    // Buttons: reset data, back
    
    // start placing buttons in the bottom 1/2, right 1/2 of the stats board
    int buttonAreaX = statsLabels.back().getScreenX() + 
        statsLabels.back().getWidth() * 2 / 5;
    int buttonAreaY = statsLabels.back().getScreenY() + 
        statsLabels.back().getHeight() / 2;
   
    int buttonAreaXEnd = statsLabels.back().getScreenX() + statsLabels.back().getWidth();
    int buttonAreaYEnd = statsLabels.back().getScreenY() + statsLabels.back().getHeight();

    auto menuButton = game->getResManager().getTexture(MENU_BUTTON_ID);

    // get buttons spaced evenly
    std::vector<Button> statsButtons = getSpacedButtons(STATS_LABELS, menuButton,
        menuFont, buttonAreaX, buttonAreaY, buttonAreaXEnd, buttonAreaYEnd,
        game->getOutlineColor(), game->getButtonTextColor(), MenuScreen::MENU_STATS);
    

    addBackButton(statsButtons, game);
    stateButtons.emplace(MenuScreen::MENU_STATS, statsButtons);
}

void MenuState::addHTPGUI(MemSwap * game) {
    std::vector<Button> htpButtons;

    addBackButton(htpButtons, game);
    stateButtons.emplace(MenuScreen::MENU_HTP, htpButtons);

    std::vector<Label> htpLabels;

    // title label
    addTitleLabel(htpLabels, HTP_TITLE, false, game);

    // How to play board label
    auto htpBoard = game->getResManager().getTexture(MENU_HTP_BOARD_ID);
    int boardX = game->getScreenWidth() / 2 - htpBoard->getWidth() / 2;
    int boardY = htpLabels.back().getScreenY() + htpLabels.back().getHeight() + BG_PAD / 4;
    htpLabels.emplace_back(boardX, boardY, htpBoard);

    stateLabels.emplace(MenuScreen::MENU_HTP, htpLabels);
}

void MenuState::addCreditsGUI(MemSwap * game) {
    // only need a 'back' button
    std::vector<Button> creditsButtons;
    
    addBackButton(creditsButtons, game);
    stateButtons.emplace(MenuScreen::MENU_CREDITS, creditsButtons);

    std::vector<Label> creditsLabels;

    // title label + text board for credits
    addTitleLabel(creditsLabels, CREDITS_TITLE, false, game);
    addTextBoard(creditsLabels, game->getCreditsString(), game,
        Label::TextAlignment::ALIGN_TOP);

    stateLabels.emplace(MenuScreen::MENU_CREDITS, creditsLabels);
}


// add a 'back' button as the last button to the given vector 
void MenuState::addBackButton(std::vector<Button> & buttons, MemSwap * game) {
    auto backButton = game->getResManager().getTexture(BACK_BUTTON_ID);
    
    int lastX, lastY;

    // (bottom right of previous buttons for lvlSelect, or bottom right if none)
    if(buttons.size() == game->getLevelLabels().size()) {
        lastX = buttons.back().getScreenX() + buttons.back().getWidth() * 8 / 7;
        lastY = buttons.back().getScreenY() + buttons.back().getHeight() * 8 / 7;
    } else {
        lastX = game->getScreenWidth() * 5 / 6 - backButton->getWidth() / 2;
        lastY = game->getScreenHeight() * 5 / 6 - backButton->getHeight() / 2;
    }

    buttons.emplace_back(lastX, lastY, CLICKABLE, backButton, game->getOutlineColor());
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

// add a text board to the label vector (default alignment = center)
void MenuState::addTextBoard(std::vector<Label> & labels, std::string label, 
    MemSwap * game, Label::TextAlignment vAlignment) {
    
    auto textBoard = game->getResManager().getTexture(MENU_TEXT_BOARD_ID);
    int boardX = game->getScreenWidth() / 2 - textBoard->getWidth() / 2;
    int boardY = labels.back().getScreenY() + labels.back().getHeight() + BG_PAD / 4;
    labels.emplace_back(boardX, boardY, textBoard, menuFont, label,
        game->getButtonTextColor(), Label::TextAlignment::ALIGN_LEFT,
        vAlignment);
}

void MenuState::handleEvents(MemSwap * game, const SDL_Event & e) {
    currButton->handleEvents(e);

    // handle switching button focus (except for credit screen [only back btn])
    if(currScreen != MenuScreen::MENU_CREDITS && e.type == SDL_KEYDOWN) {
        changeCurrButton(e, game);
    }
}

// general function to change the current button focus (for any screen)
void MenuState::changeCurrButton(const SDL_Event & e, MemSwap * game) {
    auto sym = e.key.keysym.sym;

    // depending on dir, change button in current direction, depending on 
    // button layout of the current screen

    // .first = buttons per row, .second = buttons per column
    auto layout = BTN_LAYOUTS.at(currScreen);
    int & rowBtns = layout.second;          // buttons per row = # cols
    int & colBtns = layout.first;           // buttons per col = # rows
    int totalBtns = rowBtns * colBtns;

    // cannot change for 0-button screens (e.g. credits)
    if(totalBtns == 0) return;

    int nextID = -1;

    auto & currButtons = stateButtons.at(currScreen);

    bool hasBackButton = currScreen != MenuScreen::MENU_MAIN;
    bool onLastButton = (unsigned int) currButtonID == currButtons.size() - 2;
    bool onBackButton = checkOnBackButton();

    switch(sym) {
        // user wants to move up
        case SDLK_w:
            // compute hypothetical next button (special case moving from back btn)
            nextID = currButtonID - rowBtns;

            // check for up move on first row -> same col, last row
            if(nextID < 0) {
                currButtonID = (totalBtns - ((rowBtns - 1) - currButtonID)) - 1;
            } else {
                currButtonID = onBackButton ? currButtons.size() - 2 : nextID;
            }
            break;
        
        // left
        case SDLK_a:
            nextID = currButtonID - 1;

            // move left when on first column -> last column, same row
            if(!onBackButton && currButtonID % rowBtns == 0) {
                currButtonID += (rowBtns - 1);
            } else {
                currButtonID = onBackButton ? currButtons.size() - 2 : nextID;
            }
            break;

        // down
        case SDLK_s:
            // do nothing of on back button currently
            if(onBackButton) break;

            nextID = currButtonID + rowBtns;

            // moving down when on last row [move to first row in same col.]
            if(nextID > totalBtns - 1) {
                // if on (second to last) button, go to back button (for non-main screens)
                if(hasBackButton && onLastButton) {
                    currButtonID = currButtons.size() - 1;
                } else {
                    currButtonID = currButtonID % rowBtns;
                }
            } else {
                currButtonID = nextID;
            }
            break;

        // right
        case SDLK_d:
            if(onBackButton) break;

            nextID = currButtonID + 1;

            // moving right when on last column
            if((currButtonID + 1) % rowBtns == 0) {
                // check for back button like above
                if(hasBackButton && onLastButton) {                    
                    currButtonID = currButtons.size() - 1;
                } else {
                    currButtonID -= (rowBtns - 1);
                }
            } else {
                currButtonID = nextID;
            }
            break;      
        default:
            // if some other key was pressed, don't bother changing focus 
            return;                                         
    }

    game->playSound(SWITCH_SOUND_ID);

    updateCurrButton();
}

// update the current button and check for activations
void MenuState::update(MemSwap * game, float delta) {
    currButton->update();

    // check if current button is activated
    if(currButton->isActivated()) {
        currButton->setActivated(false);

        // play activate button
        game->playSound(ACTIVATE_SOUND_ID);

        // if on the back button (for non-main screens), reset to main
        if(checkOnBackButton()) {
            currScreen = MenuScreen::MENU_MAIN;
            currButtonID = lastMainScreen;
            updateCurrButton();
        } else {
            // otherwise look for other corresponding activations
            switch(currScreen) {
                case MenuScreen::MENU_MAIN:
                    activateMain(game);
                    break;
                case MenuScreen::MENU_LVLS:
                    activateLvlSelect(game);
                    break;
                case MenuScreen::MENU_STATS:
                    activateStats(game);
                    break;
                // no activations for HTP/credits screen
                default:
                    break;
            }
        }
    }

    if(currScrollX < game->getScreenWidth() - bgTexture->getWidth()) {
        currScrollX = 0;
    } else {
        currScrollX -= SCROLL_VELOCITY * (delta / 1000.f);
    }
}

// handle button activations for each screen
void MenuState::activateMain(MemSwap * game) {
    switch(currButtonID) {
        case MainButton::MAIN_LVLS:
            currScreen = MenuScreen::MENU_LVLS;
            break;
        case MainButton::MAIN_STATS:
            currScreen = MenuScreen::MENU_STATS;
            // retrieve updated stats
            stateLabels.at(MenuScreen::MENU_STATS).back().setText(
                game->getStatsString());
            break;
        case MainButton::MAIN_HTP:
            currScreen = MenuScreen::MENU_HTP;
            break;    
        case MainButton::MAIN_CREDITS:
            currScreen = MenuScreen::MENU_CREDITS;
            break;
    }

    lastMainScreen = currButtonID;
    currButtonID = 0;

    updateCurrButton();
}

void MenuState::activateLvlSelect(MemSwap * game) {
    auto levelID = currButton->getText();

    // the level will be unlocked if its 'locked' graphic doesn't exist
    if(!currButton->checkHasGraphic()) {
        // move to play state + set correct level id
        game->setCurrLevelID(levelID);
        game->setNextState(GAME_STATE_PLAY);
    }
}

void MenuState::activateStats(MemSwap * game) {
    // reset player data
    game->resetPlayerData();

    // update stats string
    stateLabels.at(MenuScreen::MENU_STATS).back().setText(game->getStatsString());
}

// call this whenever currScreen/currButtonID changes
void MenuState::updateCurrButton() {
    currButton->setFocus(false);
    currButton = &(stateButtons.at(currScreen).at(currButtonID));
    currButton->setFocus(true);
}

/// Render function for the game state
void MenuState::render(SDL_Renderer * renderer) const {
    // scrolling background texture
    bgTexture->render((int)currScrollX, 0, renderer);

    // render labels for the current screen
    for(Label label: stateLabels.at(currScreen)) {
        label.render(renderer);
    }

    // render each of the buttons for the current screen
    for(Button button: stateButtons.at(currScreen)) {
        button.render(renderer);
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

// get evenly spaced buttons and return a vector containing them
std::vector<Button> MenuState::getSpacedButtons (
    const std::vector<std::string> & buttonLabels,
    std::shared_ptr<Texture> buttonTexture,
    std::shared_ptr<BitmapFont> buttonFont,
    int buttonAreaX, int buttonAreaY, int buttonAreaXEnd, int buttonAreaYEnd, 
    SDL_Color outlineColor, SDL_Color textColor,
    MenuScreen screen) {

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

    // add each of the buttons with the appropriate spacing
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

// check if currently on a 'back' button
bool MenuState::checkOnBackButton() const {
    auto & currButtons = stateButtons.at(currScreen);

    // bools for back button handling
    bool hasBackBtn = currScreen != MenuScreen::MENU_MAIN;
    bool onBackBtn = (unsigned int) currButtonID == currButtons.size() - 1;

    return hasBackBtn && onBackBtn;
}