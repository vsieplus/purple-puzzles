// Header for Menu State

#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

#include "gameStates/gamestate.hpp"
#include "utils/bitmapfont.hpp"
#include "utils/texture.hpp"
#include "gui/button.hpp"
#include "gui/label.hpp"

// class for the Menu State
class MenuState : public GameState {
    public:
        MenuState(MemSwap * game);

        void enterState(MemSwap * game) override;
        void exitState() override;

        void handleEvents(MemSwap * game, const SDL_Event & e) override;
        void update(MemSwap * game, float delta) override;
        void render(SDL_Renderer * renderer) const override;

        // enums for menu screens/their buttons
        enum MenuScreen {
            MENU_MAIN,        // main menu
            MENU_LVLS,        // level select
            MENU_STATS,       // player stats/data
            MENU_HTP,         // how to play
            MENU_CREDITS,     // credits
            PLAY_POSTGAME     // postgame menu during play state
        };
        

        // construct and return vector of spaced buttons
        static std::vector<Button> getSpacedButtons(
            const std::vector<std::string> & buttonLabels,
            std::shared_ptr<Texture> buttonTexture,
            std::shared_ptr<BitmapFont> buttonFont,
            int buttonAreaX, int buttonAreaY, int buttonAreaXEnd, int buttonAreaYEnd, 
            SDL_Color outlineColor, SDL_Color textColor,
            MenuScreen screen);
    
    private:
        enum MainButton {
            MAIN_LVLS,
            MAIN_STATS,
            MAIN_HTP,
            MAIN_CREDITS,
        };

        // track curr screen and button focus
        MenuScreen currScreen = MENU_MAIN;
        Button * currButton = nullptr;
        
        // id for current button focus
        int currButtonID = MAIN_LVLS;
        
        // store last main menu screen to restore focus on "back"
        int lastMainScreen = MAIN_LVLS;

        const static int BG_PAD = 40;

        // if returning to the menu state, or first time entering
        bool returning = false;

        // menu type
        inline const static bool CLICKABLE = false;

        // ID's for gui resources
        
        inline const static std::string FONT_ID = "mainFont";
        inline const static std::string BG_ID = "play_menu_bg";
        inline const static std::string MENU_BUTTON_ID = "menu_menu_btn";
        inline const static std::string LVL_BUTTON_ID = "menu_lvl_btn";
        inline const static std::string LVL_LOCKED_ID = "menu_lvl_locked";
        inline const static std::string BACK_BUTTON_ID = "menu_back_btn";
        inline const static std::string MENU_LABEL_LONG_ID = "menu_label_long";
        inline const static std::string MENU_LABEL_SHORT_ID = "menu_label_short";
        inline const static std::string MENU_TEXT_BOARD_ID = "menu_text_board";
        inline const static std::string MENU_HTP_BOARD_ID = "menu_htp_board";
        

        // button labels
        const std::vector<std::string> MAIN_LABELS = {
            "Play",         // -> level select
            "Stats",        // -> user stats/data
            "How To Play",  // -> how to play
            "Credits",      // -> credits
        };

        const std::vector<std::string> STATS_LABELS = {"Reset Data"};

        // titles/labels for menu screens
        inline const static std::string LVL_SELECT_TITLE = "Level Select";
        inline const static std::string STATS_TITLE = "Player Stats";
        inline const static std::string HTP_TITLE = "How To Play";
        inline const static std::string CREDITS_TITLE = "Credits";

        // bg texture
        std::shared_ptr<Texture> bgTexture;

        // font for text rendering
        std::shared_ptr<BitmapFont> menuFont;

        // maps of menu labels + button vectors, for each menu screen
        // (eventually generalize -> dynamic/static GUI elems)
        std::unordered_map<MenuScreen, std::vector<Button>> stateButtons;
        std::unordered_map<MenuScreen, std::vector<Label>> stateLabels;

        // store button layouts, for general button focus changing [not including back-button]
        // key: screen enum value, val: Pair of (rows, columns) [of buttons]
        const static std::unordered_map<MenuScreen, std::pair<int, int>> BTN_LAYOUTS;

        // load buttons for each menu screen 
        void addMainGUI(MemSwap * game);
        void addLvlSelectGUI(MemSwap * game);
        void addStatsGUI(MemSwap * game);
        void addHTPGUI(MemSwap * game);
        void addCreditsGUI(MemSwap * game);

        void updateLevelSelectButtons(MemSwap * game);

        void addBackButton(std::vector<Button> & buttons, MemSwap * game);

         // add a label to the given label vector
        void addTitleLabel(std::vector<Label> & labels, std::string label,
            bool longLabel, MemSwap * game) const;

        void addTextBoard(std::vector<Label> & labels, std::string label, 
            MemSwap * game, Label::TextAlignment vAlignment);

        void changeCurrButton(const SDL_Event & e);
        void updateCurrButton();

        // handle button activations for each screen
        void activateMain(MemSwap * game);
        void activateLvlSelect(MemSwap * game);
        void activateStats(MemSwap * game);

        bool checkOnBackButton() const;

        // automatically compute space to evenly space buttons/gui elems in a specified area
        static int calculateInterHSpace(int buttonsPerRow, int buttonWidth,
            int startX, int endX);
        static int calculateInterVSpace(int buttonsPerCol, int buttonHeight,
            int startY, int endY);
};

#endif // MENUSTATE_HPP