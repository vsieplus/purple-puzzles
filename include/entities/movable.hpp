// abstract class for movable entities

#ifndef MOVABLE_HPP
#define MOVABLE_HPP

#include <stack>

#include "entities/entity.hpp"
#include "entities/boost.hpp"
#include "entities/receptor.hpp"
#include "level/level.hpp"
#include "level/tile.hpp"

class Movable : public Entity {
    public:
        Movable(int screenX, int screenY, int gridX, int gridY, int velocity,
            int parity, std::shared_ptr<Sprite> entitySprite, 
            std::string movableShape,
            const std::unordered_map<int, std::shared_ptr<Animation>> & entityAnimations);

        virtual void update(Level * level, float delta) override;
        void render(SDL_Renderer* renderer) const override;  

        static std::pair<int,int> lerp(int startX, int startY, int endX,
            int endY, float t);

        Direction currCheckDir() const;

        template <class T>
        std::shared_ptr<T> getEntity(Level * level, Direction direction) {
            std::pair<int, int> coords = getCoords(direction);
            return level->getGridElement<T>(coords.first, coords.second);
        }

        void setMoveDir(Direction direction);
        float getMoveProg() const;
        bool isMerging() const;
        bool isMoving() const;

        // enum of actions to store play history for undo functionality
        enum MovableAction {
            MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN,
            BOOST_LEFT, BOOST_RIGHT, BOOST_UP, BOOST_DOWN, BOOST_NONE, // dummy boost-inplace
            MERGE,
            // (player exclusive actions)
            PUSH, TELEPORT
        };

        virtual void undoAction(Level * level);

    protected:
        // Movement progress + tracking
        float moveProg = 0.f;
        const float MOVEMENT_BUFFER = 0.85f;

        int startX, startY, endX, endY;   // Start and end positions for movement

        Direction moveDir = DIR_NONE;     // direction of entity's move
        Direction bufferedDir = DIR_NONE; // direction of entity's buffered move

        int velocity;                     // Pixels moved per sec.

        // for tracking boost status of movable entities
        int boostPower = 0;

        // stack of actions
        std::stack<MovableAction> actionHistory;

        // for storing boosters upon activation
        std::stack<std::shared_ptr<Boost>> boosters;

        // the receptor to merge with
        std::shared_ptr<Receptor> mReceptor;

        // the last portal the entity used
        std::shared_ptr<Portal> lastPortal;
        
        // if merging with a receptor/moving
        bool merging = false;
        bool moving = false;

        std::string movableShape;
        
        inline const static std::string MERGE_SOUND_ID = "merge";

        // initialize movement from a direction
        void initMovement(Direction direction, Level * level);
        void initMovement(int xPosChange, int yPosChange, int xGridChange, 
            int yGridChange, Direction direction, Level * level);
        
        // update the entity's position
        void move(Level * level, float delta);

        // check for a boost entity
        bool checkBoost(Level * level, Direction direction);
        void checkReceptor(Level * level);

        void addMoveToHistory(Direction direction);

        void undoMovement(Direction direction, Level * level);
        void undoBoost(Direction direction, Level * level, MovableAction lastAction);
        void undoMerge(Level * level);
};

#endif // MOVABLE_HPP