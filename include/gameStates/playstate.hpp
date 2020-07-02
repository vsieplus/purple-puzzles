// Header for Play State

#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include <string>
#include <memory>

#include "gameStates/gamestate.hpp"
#include "level/level.hpp"

#include "gui/label.hpp"
#include "gui/button.hpp"

// Class for the Play State
class PlayState : public GameState {
    private:
        Level level;

        // for post-level completion menu (popup window)
        Label postGameBoard;

        // buttons
        std::vector<Button> postGameButtons;

        inline const static std::vector<std::string> postGameMenuLabels = {
            "Next Level", "Main Menu", "Level Select"
        };

        inline const static std::string FONT_ID = "mainFont";
        inline const static std::string BUTTON_ID = "menu_menu_btn";
        inline const static std::string POSTGAME_BOARD_ID = "play_postgame_board";

        inline const static std::string POSTGAME_TEXT = "Level Complete!";

        inline const static std::string COMPLETE_SOUND_ID = "complete";

        // enum for postgame menu buttons
        enum PGButton {
            BUTTON_NEXT,
            BUTTON_MAIN,
            BUTTON_LVLS,
        };

        unsigned int currButton = BUTTON_NEXT;

        // track stats for the current play state
        int currNumResets = 0;
        int currMovesUndone = 0;
        int currTilesFlipped = 0;

        // after a level is completed
        bool levelComplete = false;

        void handlePGActivation(MemSwap * game);

    public:
        PlayState(MemSwap * game);

        void enterState(MemSwap * game) override;
        void exitState() override;

        // helper function to load levels
        void loadLevel(MemSwap * game, bool enteringState = false);

        void updateStats(MemSwap * game);

        void handleEvents(MemSwap * game, const SDL_Event & e) override;
        void update(MemSwap * game, float delta) override;
        void render(SDL_Renderer * renderer) const override;

        bool levelIsComplete() const;
};

#endif // PLAYSTATE_HPP