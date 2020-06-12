// Header file for Player class

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <utility>
#include <SDL.h>

#include "entities/movable.hpp"

class Player : public Movable {
    private:
        static const int PLAYER_VELOCITY = 7;

        Direction pushDir = DIR_NONE;

    public:
        const static std::string PLAYER_SHAPE;

        Player(int screenX, int screenY, int gridX, int gridY, int parity,
            std::shared_ptr<Sprite> entitySprite);

        // game loop stuff
        void handleEvents(const Uint8 * keyStates, Level * level) override;
        void update(Level * level, float delta) override;
        void render(SDL_Renderer* renderer) const override;

        // check if player has input movement
        void checkMovement(const Uint8 * keyStates, Level * level);

        // try to push a diamond
        void pushDiamond(Level * level);

        // check for exit/determine if level is complete
        void checkExit(Level * level, Direction direction);
};

#endif // PLAYER_HPP