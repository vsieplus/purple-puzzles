// Header file for Player class

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <utility>
#include <SDL.h>

#include "utlis/texture.hpp"

enum Directions {
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
        bool bufferedMove = false;      // Allow buffered movement

        int startX, startY, endX, endY; // Start and end positions for movement
        
        int bufferedX = 0;              // buffered movement position
        int bufferedY = 0;              
        int bufferedDir = DIR_NONE;     // direction of buffered movement

        Uint32 moveStartTime = 0;       // time since movement started


    public:
        Player(int screenX, int screenY, int gridX, int gridY, 
            SDL_Renderer * renderer);

        static const int PLAYER_VELOCITY = 10;

        void handleEvents(const Uint8* keyStates, Level * level) override;
        void update(Level * level) override;
        void render(SDL_Renderer* renderer) override;

        void checkMovement(const Uint8* keyStates, Level * level);
        void initMovement();
        void move();

        static std::pair<int,int> lerp(int startX, int startY, int endX,
            int endY, float t);

        float getMoveProg();

};

#endif // PLAYER_HPP