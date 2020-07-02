// Header for Pause State

#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include <string>
#include <vector>

#include "gameStates/gamestate.hpp"

#include "gui/button.hpp"

// enum for tracking buttons
enum ButtonID {
    RESUME_BTN,
    MENU_BTN,
    LVLSELECT_BTN
};

// class for the Pause State
class PauseState : public GameState {
    public:
        PauseState(MemSwap * game);

        void enterState(MemSwap * game) override;
        void exitState() override;

        void handleEvents(MemSwap * game, const SDL_Event & e) override;
        void update(MemSwap * game, float delta) override;
        void render(SDL_Renderer * renderer) const override;

    private:
        // Texture to show in the background
        std::shared_ptr<Texture> bgTexture;

        const std::string BG_ID = "pause_bg";
        const std::string BUTTON_ID = "menu_menu_btn";
        const std::string FONT_ID = "mainFont";

        // Button labels
        const std::vector<std::string> BUTTON_LABELS = {"Resume",
            "Menu", "Level Select"};

        // vector of buttons on the pause screen
        std::vector<Button> buttons;

        // track current button in focus
        int currButton = RESUME_BTN;

        // non-clickable menu
        const bool CLICKABLE = false; 

        void addButtons(MemSwap * game);
        void changeCurrButton(bool left, MemSwap * game);
};

#endif // PAUSESTATE_HPP