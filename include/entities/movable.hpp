// abstract class for movable entities

#ifndef MOVABLE_HPP
#define MOVABLE_HPP

#include "entities/entity.hpp"
#include "entities/boost.hpp"
#include "entities/receptor.hpp"
#include "level/tile.hpp"

class Movable : public Entity {
    protected:
        // Movement progress + tracking
        float moveProg = 0.f;
        const float MOVEMENT_BUFFER = 0.85f;

        int startX, startY, endX, endY;   // Start and end positions for movement

        Direction moveDir = DIR_NONE;     // direction of entity's move
        Direction bufferedDir = DIR_NONE; // direction of entity's buffered move
        
        int bufferedX = 0;                // buffered movement position
        int bufferedY = 0;

        int velocity;                     // Pixels moved per sec.

        // for tracking boost status of movable entities
        int boostPower = 0;

        // for storing booster upon activation
        std::shared_ptr<Boost> booster;

        // the receptor to merge with
        std::shared_ptr<Receptor> mReceptor;
        
        // if merging with a receptor/moving
        bool merging = false;
        bool moving = false;

        std::string movableShape;

    public:
        Movable(int screenX, int screenY, int gridX, int gridY, int velocity,
            int parity, std::shared_ptr<Sprite> entitySprite, 
            std::string movableShape);

        virtual void update(Level * level, float delta) override;
        void render(SDL_Renderer* renderer) const override;  

        // initialize movement from a direction
        void initMovement(Direction direction, Level * level);
        void initMovement(int xPosChange, int yPosChange, int xGridChange, 
            int yGridChange, int direction, Level * level);
        
        // update the entity's position
        void move(Level * level, float delta);

        // check for a boost entity
        bool checkBoost(Level * level, Direction direction);
        void checkReceptor(Level * level, Direction direction);

        static std::pair<int,int> lerp(int startX, int startY, int endX,
            int endY, float t);

        void setMoveDir(Direction direction);
        float getMoveProg() const;
        bool isMerging() const;
};

#endif // MOVABLE_HPP