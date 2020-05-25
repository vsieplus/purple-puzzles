// Header file for map class

#ifndef MAP_HPP
#define MAP_HPP

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>

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

        // map of tileset clips for BG tiles
        // key: tile GID, val: SDL_Rect
        std::unordered_map<int, SDL_Rect> bgTilesetClips;

        // map of tile parities for BG Tiles
        // key: tile GID, val: Tile parity (int indicating parity) 
        std::unordered_map<int, int> tileParities;

    public:
        // strings used to interface with tiledmap properties/labels
        const static std::string BG_LAYER_NAME, ENTITY_LAYER_NAME,
            BG_TILESET_NAME;

        Map();
        Map(std::string tiledMapPath);
        ~Map();

        void render(SDL_Renderer * renderer) const;

        // Load map for the level
        void loadMap(SDL_Renderer * renderer, Level * level);
        void loadTilesets(const tmx::Map & map, SDL_Renderer * renderer);

        void checkTileParity(const tmx::Tileset::Tile & tile);

        // add background tiles to the map from the given tileLayer
        void addBGTiles(const tmx::TileLayer * tileLayer, Level * level);
};

#endif // MAP_HPP
