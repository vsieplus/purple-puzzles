// Header file for Player class

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <utility>
#include <SDL.h>

#include "entities/movable.hpp"

class Portal;

class Player : public Movable {
    private:
        static const int PLAYER_VELOCITY = 6;

        // track when a player is undoing a buffer/set to BUFFER_CAP each time
        static const int UNDO_BUFFER_CAP = 10;
        int undoBuffer = 0;

        int movesUndone = 0;

        bool teleporting = false;
        bool stuck = false;

        // maintain stack of pointers to objects pushed by player for undo purposes
        std::stack<std::shared_ptr<Movable>> pushedObjects;

        inline const static std::string PLAYER_SHAPE = "player";
        inline const static std::string BONK_SOUND_ID = "bonk";
        
        // check if player has input movement
        void checkMovement(const Uint8 * keyStates, Level * level);

        // try to push a diamond
        void pushDiamond(Level * level);

        // check for portal
        void checkPortal(Level * level);

        void undoAction(Level * level) override;
    public:
        enum PlayerAnimation {PLAYER_MERGE, PLAYER_MOVEFAIL_UP, PLAYER_MOVEFAIL_DOWN,
            PLAYER_MOVEFAIL_LEFT, PLAYER_MOVEFAIL_RIGHT, PLAYER_TELEPORT};

        Player(int screenX, int screenY, int gridX, int gridY, int parity,
            std::shared_ptr<Sprite> entitySprite,
            const std::unordered_map<int, std::shared_ptr<Animation>> & entityAnimations);

        // game loop stuff
        void handleEvents(const Uint8 * keyStates, Level * level) override;
        void update(Level * level, float delta) override;
        void render(SDL_Renderer* renderer) const override;

        bool isTeleporting() const;
        void setTeleporting(bool teleporting);

        int getMovesUndone() const;

        void pushAction(MovableAction action);

        void setLastPortal(std::shared_ptr<Portal> lastPortal);
};

#endif // PLAYER_HPP