// Abstract base class for entities

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SDL.h>

#include <string>
#include <memory>
#include <utility>

#include "utils/sprite.hpp"

class Level;
class Map;

enum Direction {
    DIR_NONE,
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
};

class Entity {
    protected:
        // x and y position on the screen
        int screenX, screenY;

        // x,y location in the grid
        int gridX, gridY;

        // Texture for the entity
        std::shared_ptr<Sprite> entitySprite;

        SDL_Rect renderArea;

    public:
        Entity(int sX, int sY, int gX, int gY, std::shared_ptr<Sprite> entitySprite);

        virtual void handleEvents(const Uint8 * keyStates, Level * level) = 0;
        virtual void update(Level * level, float delta) = 0;
        virtual void render(SDL_Renderer * renderer) const;

        static bool checkCollision(const Map & map, int destGridX, int destGridY);

        int getScreenX() const;
        int getScreenY() const;
        int getGridX() const;
        int getGridY() const;

        std::pair<int, int> getCoords(Direction direction);
};

#endif // ENTITY_HPP