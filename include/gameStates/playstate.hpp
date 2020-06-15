// Header for Play State

#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include <string>
#include <memory>

#include "gameStates/gamestate.hpp"
#include "level/level.hpp"
#include "utils/timer.hpp"

// Class for the Play State
class PlayState : public GameState {
    private:
        Timer timer;
        Level level;

        // Texture to show in the background
        std::shared_ptr<Texture> bgTexture;

        const std::string BG_ID = "play_bg";

        // after a level is completed, handle user's choice
        bool levelComplete = false;
        bool advanceLevel = false;
        bool goToMenu = true;

        // check if paused
        bool paused = false;

    public:
        PlayState();

        void enterState(MemSwap * game) override;
        void exitState() override;

        void handleEvents(MemSwap * game, const Uint8 * keyStates) override;
        void update(MemSwap * game, float delta) override;
        void render(SDL_Renderer * renderer) const override;

        void setPaused(bool paused);
        bool isPaused() const;
};

#endif // PLAYSTATE_HPP