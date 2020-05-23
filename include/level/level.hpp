// Header file for level class

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <memory>

#include "tmxlite/Map.hpp"
#include "tmxlite/Layer.hpp"
#include "tmxlite/TileLayer.hpp"
#include "entities/player.hpp"
#include "level/tile.hpp"

class Level {
    private:
        // Width/height in tiles
        int gridWidth, gridHeight;

        // Width/height in pixels
        int pixelWidth, pixelHeight;

        // A vector holding the entities in the grid
        std::vector<std::unique_ptr<Entity>> grid;

        // A vector holding the background tiles for the map
        std::vector<std::unique_ptr<Tile>> mapTiles;

    public:
        Level(std::string tiledMapPath, SDL_Renderer * renderer);
        ~Level();

        void update(const Uint8* keyStates);
        void render(SDL_Renderer* renderer);

        // Load the tiledmap for this level
        void loadMap(std::string tiledMapPath, SDL_Renderer * renderer);

        const int TILE_HEIGHT;
        const int TILE_WIDTH;

        int getGridWidth();
        int getGridHeight();
        int getPixelWidth();
        int getPixelHeight();    
};

#endif // LEVEL_HPP