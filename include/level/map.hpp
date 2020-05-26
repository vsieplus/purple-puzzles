// Header file for map class

#ifndef MAP_HPP
#define MAP_HPP

#include <utility>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <list>

#include <SDL.h>
#include <SDL_image.h>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>

#include "level/tile.hpp"

class Level;

class Map {
    private:
        int mapWidth, mapHeight;         // size of map in tiles
        int tileWidth, tileHeight;       // size of tiles in pixels

        std::string mapPath;             // path to map (tmx file)

        // A vector holding the background tiles for the map
        std::vector<Tile> mapTiles;
        
        // map of pointers to textures of tilesets used in the map
        // key: first tileset GID, val: pointer to ts texture 
        std::map<int, std::shared_ptr<SDL_Texture>> mapTilesets;

        // map of tile parities for BG Tiles (Should only contain 2 elems)
        // key: tile GID, val: Tile parity (int indicating parity) 
        std::map<int, int> tileParities;

        // map of tileset clips for BG tiles
        // key: tile GID, val: SDL_Rect
        std::unordered_map<int, SDL_Rect> bgTilesetClips;

    public:
        // strings used to interface with tiledmap properties/labels
        const static std::string BG_LAYER_NAME, ENTITY_LAYER_NAME,
            BG_TILESET_NAME;

        Map();
        Map(std::string tiledMapPath);
        ~Map();

        void update(Level * level);
        void render(SDL_Renderer * renderer) const;

        // Load map for the level
        void loadMap(SDL_Renderer * renderer, Level * level);
        void loadTilesets(const tmx::Map & map, SDL_Renderer * renderer);

        void checkTileParity(const tmx::Tileset::Tile & tile, int tilesetFirstGID);

        // add background tiles to the map from the given tileLayer
        void addBGTiles(const tmx::TileLayer * tileLayer, Level * level);

        // Check if a tile is in bounds
        bool inBounds(int x, int y) const;

        // Update bg tiles when the specified movement occurs
        void flipTiles(int tileX, int tileY, int moveDir, Level * level);

        // Get parity of tile at the specified grid location
        int getTileParity(int x, int y) const;
};

#endif // MAP_HPP
