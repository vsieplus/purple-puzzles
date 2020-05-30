// Header for Splash State

#ifndef SPLASHSTATE_HPP
#define SPLASHSTATE_HPP

#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include "gameStates/gamestate.hpp"
#include "utils/texture.hpp"

// class for the Splash State
class SplashState : public GameState {
    public:
        SplashState();

        ~SplashState();

        void enterState(MemSwap * game) override;
        void exitState() override;

        void handleEvents(MemSwap * game, const Uint8 * keyStates) override;
        void update(MemSwap * game) override;
        void render(SDL_Renderer * renderer) const override;

    private:
        Texture bgTexture;  // BG texture to show while loading resources
        Texture advTexture; // Texture to prompt player to advance

        const std::string BG_ID = "splash_bg";
        const std::string ADV_ID = "splash_adv";

        int advTextX, advTextY;

        bool loadingRes = true;     // if we're loading resources
        bool advance = false;       // if the user wants to advance
};

#endif // SPLASHSTATE_HPP