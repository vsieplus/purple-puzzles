// Abstract base class for entities

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>

#include "utils/texture.hpp"

class Level;

class Entity {
    protected:
        // Texture for the entity
        Texture texture;

        // x and y position on the screen
        int screenX, screenY;

        // x,y location in the grid
        int gridX, gridY;

    public:
        Entity(int sX, int sY, int gX, int gY, std::string texturePath,
            SDL_Renderer * renderer);

        virtual void handleEvents(const Uint8 * keyStates, Level * level) = 0;
        virtual void update(Level * level) = 0;
        virtual void render(SDL_Renderer * renderer) const = 0;

        bool checkCollision(Level * level, int destGridX, int destGridY);

        int getScreenX() const;
        int getScreenY() const;
        int getGridX() const;
        int getGridY() const;
};

#endif // ENTITY_HPP