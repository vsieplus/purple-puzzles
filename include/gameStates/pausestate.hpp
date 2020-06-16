// Header for Pause State

#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include <string>
#include <vector>

#include "gameStates/gamestate.hpp"

#include "gui/button.hpp"

// enum for tracking buttons
enum ButtonID {
    RESUME_ID,
    MENU_ID,
    LVLSELECT_ID
};

// class for the Pause State
class PauseState : public GameState {
    public:
        PauseState(MemSwap * game);

        void addButtons(MemSwap * game);

        void enterState(MemSwap * game) override;
        void exitState() override;

        void handleEvents(MemSwap * game, const SDL_Event & e) override;
        void handleEvents(MemSwap * game, const Uint8 * keyStates) override;
        void update(MemSwap * game, float delta) override;
        void render(SDL_Renderer * renderer) const override;

        void changeCurrButton(bool left);

    private:
        // Texture to show in the background
        std::shared_ptr<Texture> bgTexture;

        const std::string BG_ID = "pause_bg";

        const std::vector<std::string> BUTTON_IDS = {"pause_resume_btn",
            "pause_menu_btn", "pause_lvlselect_btn"};

        // vector of buttons on the pause screen
        std::vector<Button> buttons;

        // track current button in focus
        int currButton = RESUME_ID;

        const int BUTTON_WIDTH = 128;
};

#endif // PAUSESTATE_HPP