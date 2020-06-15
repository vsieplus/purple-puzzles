// Header file for level class

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <SDL.h>
#include <tmxlite/Map.hpp>

#include <string>

#include "level/map.hpp"

class MemSwap;
class Texture;

class Level {
    private:
        // Width/height in tiles
        int gridWidth, gridHeight;

        // Width/height in pixels
        int pixelWidth, pixelHeight;

        // The map representation of the background
        Map map;

        // if level has been completed by the player
        bool completed = false;

    public:
        Level();
        Level(std::string tiledMapPath, SDL_Renderer * renderer, MemSwap * game);

        // game loop
        void handleEvents(const Uint8 * keyStates);
        void update(float delta);
        void render(SDL_Renderer * renderer) const;

        // grid initialization
        void updateSize(const tmx::Map & map, int tileWidth, int tileHeight);

        void flipMapTiles(int movedFromX, int movedFromY, int entityParity);
        void removeGridElement(int x, int y);
        void placeGridElement(std::shared_ptr<Entity> entity, int x, int y);
        void moveGridElement(int startX, int startY, int endX, int endY);

        void placePortals();

        // check for level completion
        void checkComplete();

        bool isCompleted() const;

        int getGridWidth() const;
        int getGridHeight() const;
        int getPixelWidth() const;
        int getPixelHeight() const;

        template <class T>
        std::shared_ptr<T> getGridElement(int x, int y) const {
            return map.getGridElement<T>(x, y);
        }

        bool inBounds(int x, int y) const;
        Parity getTileParity(int x, int y) const;
};

#endif // LEVEL_HPP