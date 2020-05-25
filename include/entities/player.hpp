// Header file for Player class

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <utility>
#include <SDL.h>

#include "utils/texture.hpp"
#include "entities/entity.hpp"
#include "level/tile.hpp"

enum Direction {
    DIR_NONE,
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
};

class Player : public Entity {
    private:
        // Movement progress + tracking
        float moveProg = 0.f;

        bool moving = false;
        bool bufferedMove = false;        // Allow buffered movement

        int startX, startY, endX, endY;   // Start and end positions for movement

        // parity of tile player is currently on
        TileParity tileParity = PARITY_GRAY;

        Direction moveDir = DIR_NONE;     // direction of player's move
        Direction bufferedDir = DIR_NONE; // direction of player's buffered move
        int bufferedX = 0;                // buffered movement position
        int bufferedY = 0;              

        Uint32 moveStartTime = 0;         // time since movement started

    public:
        Player(int screenX, int screenY, int gridX, int gridY, 
            SDL_Renderer * renderer);

        const int PLAYER_VELOCITY = 6;         // Pixels moved per sec.
        const float MOVEMENT_BUFFER = 0.85f;   // How soon through the curr 
                                               // movement player can buffer

        // game loop stuff
        void handleEvents(const Uint8* keyStates, Level * level) override;
        void update(Level * level) override;
        void render(SDL_Renderer* renderer) const override;

        // check if player has input movement
        void checkMovement(const Uint8* keyStates, Level * level);

        // initialize movement from a direction
        void initMovement(int direction, Level * level);
        void initMovement(int xPosChange, int yPosChange, int xGridChange, 
            int yGridChange, Level * level);
        
        // update the player's position
        void move(Level * level);

        static std::pair<int,int> lerp(int startX, int startY, int endX,
            int endY, float t);

        float getMoveProg() const;
};

#endif // PLAYER_HPP