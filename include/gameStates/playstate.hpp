// Header for Play State

#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include <string>
#include <memory>

#include "gameStates/gamestate.hpp"
#include "level/level.hpp"

// Class for the Play State
class PlayState : public GameState {
    private:
        Level level;

        // Texture to show in the background
        std::shared_ptr<Texture> bgTexture;

        const std::string BG_ID = "play_menu_bg";

        // track resets for the play state
        int currNumResets = 0;
        int currTilesFlipped = 0;

        // after a level is completed, handle user's choice
        bool levelComplete = false;
        bool advanceLevel = false;
        bool goToMenu = false;

    public:
        PlayState();

        void enterState(MemSwap * game) override;
        void exitState() override;

        // helper function to load levels
        void loadLevel(MemSwap * game);

        void updateStats(MemSwap * game);

        void handleEvents(MemSwap * game, const SDL_Event & e) override;
        void update(MemSwap * game, float delta) override;
        void render(SDL_Renderer * renderer) const override;
};

#endif // PLAYSTATE_HPP