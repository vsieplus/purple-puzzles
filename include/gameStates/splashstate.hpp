// Header for Splash State

#ifndef SPLASHSTATE_HPP
#define SPLASHSTATE_HPP

#include <SDL.h>
#include <SDL_image.h>

#include "gameStates/gamestate.hpp"

// Singleton class for the Splash State
class SplashState : public GameState {
    public:
        SplashState();

        ~SplashState();

        void enterState() override;
        void exitState() override;

        void handleEvents() override;
        void update() override;
        void render(SDL_Window * window, SDL_Renderer * renderer) override;

    private:
        bool loadingRes = true;

        // Load all game resources from res/
        void loadResources();
};

#endif // SPLASHSTATE_HPP