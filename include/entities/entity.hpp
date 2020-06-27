// Abstract base class for entities (not using ECS for this proj's simplicity)

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SDL.h>

#include <string>
#include <memory>
#include <utility>

#include "utils/sprite.hpp"
#include "utils/animator.hpp"

class Level;
class Map;

enum Direction {
    DIR_NONE,
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
};

enum Parity {
    PARITY_NONE,    // 0 ~ none
    PARITY_GRAY,    // 1 ~ gray
    PARITY_PURPLE   // 2 ~ purple
};

class Entity {
    protected:
        // x,y location in the grid
        int gridX, gridY;

        // parity of the entity
        Parity parity;

        // Texture for the entity
        std::shared_ptr<Sprite> entitySprite;

        // render area on the screen
        SDL_Rect renderArea;

        // animator for this entity
        Animator entityAnimator;

    public:
        // for grid-based entities
        Entity(int screenX, int screenY, int gridX, int gridY, int parity, 
            std::shared_ptr<Sprite> entitySprite);

        // for non grid-based entities (eg tiles)
        Entity(int screenX, int screenY, int parity, 
            std::shared_ptr<Sprite> entitySprite);

        virtual void handleEvents(const Uint8 * keyStates, Level * level) = 0;
        virtual void update(Level * level, float delta);
        virtual void render(SDL_Renderer * renderer) const;

        static bool checkCollision(Level * level, int destGridX, int destGridY);

        void setScreenX(int x);
        void setScreenY(int y);
        void setGridX(int x);
        void setGridY(int y);

        int getScreenX() const;
        int getScreenY() const;
        int getGridX() const;
        int getGridY() const;

        Parity getParity() const;

        std::pair<int, int> getCoords(Direction direction);
};

#endif // ENTITY_HPP