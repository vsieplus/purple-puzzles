// Header file for Player class

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <utility>
#include <SDL.h>

#include "utils/texture.hpp"
#include "entities/movable.hpp"


class Player : public Movable {
    private:

    public:
        Player(int screenX, int screenY, int gridX, int gridY, 
            std::shared_ptr<Sprite> entitySprite);

        const int PLAYER_VELOCITY = 6;

        // game loop stuff
        void handleEvents(const Uint8 * keyStates, Level * level) override;
        void update(Level * level, float delta) override;
        void render(SDL_Renderer* renderer) const override;

        // check if player has input movement
        void checkMovement(const Uint8* keyStates, Level * level);
};

#endif // PLAYER_HPP