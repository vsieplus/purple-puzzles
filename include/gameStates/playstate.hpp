// Header for Play State

#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include <memory>

#include "gameStates/gamestate.hpp"
#include "level/level.hpp"
#include "utils/timer.hpp"

// Singleton class for the Play State
class PlayState : public GameState {
    private:
        Timer timer;
        Level level;

        // Texture to show in the background
        std::shared_ptr<Texture> bgTexture;

        const std::string BG_ID = "play_bg";

    public:
        PlayState();

        void enterState(MemSwap * game) override;
        void exitState() override;

        void handleEvents(MemSwap * game, const Uint8 * keyStates) override;
        void update(MemSwap * game) override;
        void render(SDL_Renderer * renderer) const override;

        void pause();
};

#endif // PLAYSTATE_HPP