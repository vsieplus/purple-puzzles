// Header file for level class

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <utility>

#include "entities/entity.hpp"
#include "level/tile.hpp"

class Level {
    private:
        // Width/height in tiles
        int gridWidth, gridHeight;

        // Width/height in pixels
        int pixelWidth, pixelHeight;

        // A map holding the entities in the grid, key = index, value = u_ptr
        std::unordered_map<int, std::shared_ptr<Entity>> grid;

        // A vector holding the background tiles for the map
        std::vector<Tile> mapTiles;

    public:
        Level();
        Level(std::string tiledMapPath, SDL_Renderer * renderer);

        ~Level();

        void handleEvents(const Uint8 * keyStates);
        void update();
        void render(SDL_Renderer * renderer) const;

        // Load the tiledmap for this level
        void loadMap(std::string tiledMapPath, SDL_Renderer * renderer);

        void addBGTiles(const tmx::TileLayer * tileLayer, 
            const std::map<int, std::shared_ptr<SDL_Texture>> & tilesets);
        void addEntityTiles(const tmx::TileLayer * tileLayer, 
            const std::map<int, std::shared_ptr<SDL_Texture>> & tilesets);

        void initGrid();

        void updateSize(const tmx::Map & map);

        int TILE_HEIGHT;
        int TILE_WIDTH;

        const static std::string BG_LAYER_NAME, ENTITY_LAYER_NAME;

        int getGridWidth() const;
        int getGridHeight() const;
        int getPixelWidth() const;
        int getPixelHeight() const;

        const std::unordered_map<int, std::shared_ptr<Entity>> & getGrid();
        void setGridElement(int startX, int startY, int endX, int endY);

        // functions to convert between x,y indices to map key
        int xyToIndex(int x, int y) const;
        std::pair<int, int> indexToXY(int index) const;
};

#endif // LEVEL_HPP