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
        void render(SDL_Renderer * renderer) override;

    private:
        // BG texture to show while loading resources
        Texture bgTexture;

        std::string SPLASH_BG_PATH = "res/images/splash/bg.png";

        bool loadingRes = false;     // if we're loading resources
        bool advance = false;       // if the user wants to advance

        // Load all game resources from res/
        void loadResources();
};

#endif // SPLASHSTATE_HPP