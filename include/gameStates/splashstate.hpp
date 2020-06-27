// Header for Splash State

#ifndef SPLASHSTATE_HPP
#define SPLASHSTATE_HPP

#include <string>

#include <SDL_image.h>

#include "gameStates/gamestate.hpp"
#include "utils/texture.hpp"
#include "utils/bitmapfont.hpp"
#include "utils/animator.hpp"

// class for the Splash State
class SplashState : public GameState {
    public:
        SplashState();
        
        void enterState(MemSwap * game) override;
        void exitState() override;

        void handleEvents(MemSwap * game, const SDL_Event & e) override;
        void update(MemSwap * game, float delta) override;
        void render(SDL_Renderer * renderer) const override;

    private:
        Texture bgTexture;  // BG texture to show while loading resources

        Animator splashAnim;

        // font for splash text rendering
        std::shared_ptr<BitmapFont> splashFont; 
    
        const std::string BG_ID = "splash_bg";
        const std::string LOAD_ANIM_ID = "loading";
        const std::string FONT_ID = "mainFont";
        const std::string ADV_TEXT = "Press Enter to Start...";

        int advTextX, advTextY;

        int loadX, loadY;

        bool loadingRes = true;     // if we're loading resources
        bool advance = false;       // if the user wants to advance

        // typed/flashing advance message
        const bool TYPED = true;
        const bool FLASHING = true;
};

#endif // SPLASHSTATE_HPP