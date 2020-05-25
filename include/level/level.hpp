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
#include "level/map.hpp"

class Level {
    private:
        // Width/height in tiles
        int gridWidth, gridHeight;

        // Width/height in pixels
        int pixelWidth, pixelHeight;

        // A map holding the entities in the grid, key = index, value = u_ptr
        std::unordered_map<int, std::shared_ptr<Entity>> grid;

        // The map representation of the background
        Map map;

    public:
        Level();
        Level(std::string tiledMapPath, SDL_Renderer * renderer);

        ~Level();

        void handleEvents(const Uint8 * keyStates);
        void update();
        void render(SDL_Renderer * renderer) const;

        void addEntityTiles(const tmx::TileLayer * tileLayer, 
            const std::map<int, std::shared_ptr<SDL_Texture>> & tilesetTextures);

        void initGrid();

        void updateSize(const tmx::Map & map, int tileWidth, int tileHeight);

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