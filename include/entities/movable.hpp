// abstract class for movable entities

#ifndef MOVABLE_HPP
#define MOVABLE_HPP

#include "entities/entity.hpp"
#include "level/tile.hpp"

class Movable : public Entity {
    protected:
        // Movement progress + tracking
        float moveProg = 0.f;

        bool moving = false;

        int startX, startY, endX, endY;   // Start and end positions for movement

        Direction moveDir = DIR_NONE;     // direction of entity's move
        Direction bufferedDir = DIR_NONE; // direction of entity's buffered move
        int bufferedX = 0;                // buffered movement position
        int bufferedY = 0;

        int velocity;                     // Pixels moved per sec.

        const float MOVEMENT_BUFFER = 0.85f;

        // parity of the movable entity
        int movableParity;

    public:
        Movable(int screenX, int screenY, int gridX, int gridY, int velocity,
            std::shared_ptr<Sprite> entitySprite, int movableParity);

        virtual void update(Level * level, float delta) override;    

        // initialize movement from a direction
        void initMovement(int direction, Level * level);
        void initMovement(int xPosChange, int yPosChange, int xGridChange, 
            int yGridChange, int direction, Level * level);
        
        // update the entity's position
        void move(Level * level, float delta);

        static std::pair<int,int> lerp(int startX, int startY, int endX,
            int endY, float t);

        void setMoveDir(Direction direction);
        float getMoveProg() const;
};

#endif // MOVABLE_HPP