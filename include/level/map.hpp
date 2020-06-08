// Header file for map class

#ifndef MAP_HPP
#define MAP_HPP

#include <utility>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <list>

#include <SDL.h>
#include <SDL_image.h>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>

#include "entities/entity.hpp"
#include "level/tile.hpp"
#include "utils/spritesheet.hpp"
#include "utils/sprite.hpp"

class MemSwap;

class Level;

class Map {
    private:
        int mapWidth, mapHeight;         // size of map in tiles
        int tileWidth, tileHeight;       // size of tiles in pixels

        int renderX, renderY;            // x,y on the screen to render map

        // A vector holding the background tiles for the map
        std::vector<Tile> mapTiles;

        // A map holding the entities in the grid, key = index, value = ptr
        std::unordered_map<int, std::shared_ptr<Entity>> entityGrid;

        // store spritesheets used by this map  (key: ss first GID, val: ptr->ss)
        std::unordered_map<int, std::shared_ptr<SpriteSheet>> mapSpritesheets;

        // store tile sprites for parity tiles (key: parity, val: sprite ptr)
        std::unordered_map<int, std::shared_ptr<Sprite>> parityTileSprites;

    public:
        // strings used to interface with tiledmap properties/labels
        const static std::string BG_LAYER_NAME, ENTITY_LAYER_NAME;
        const static std::string PARITY_PROP, NAME_PROP;
        const static std::string PLAYER_ENAME, RECEPTOR_ENAME;

        Map();
        Map(std::string tiledMapPath, SDL_Renderer * renderer, Level * level, 
            MemSwap * game);

        void handleEvents(Level * level, const Uint8 * keyStates);
        void update(Level * level, float delta);
        void render(SDL_Renderer * renderer) const;

        // Load map for the level
        void loadMap(std::string tiledMapPath, SDL_Renderer * renderer, 
            Level * level, MemSwap * game);

        // add background tiles to the map from the given tileLayer
        void addTiles(const tmx::TileLayer * tileLayer, Level * level,
            MemSwap * game, std::string layerName);

        void addBGTile(int screenX, int screenY, int tileID, 
            const std::shared_ptr<SpriteSheet> & tileSpritesheet);
        void addEntity(int screenX, int screenY, int gridX, int gridY, int tileID, 
            const std::shared_ptr<SpriteSheet> & tileSpritesheet);   

        void initGrid();

        // Check if a tile is in bounds
        bool inBounds(int x, int y) const;

        // Update bg tiles when the specified movement occurs
        void flipTile(int tileX, int tileY, int entityParity, Level * level);

        // Get parity of tile at the specified grid location
        int getTileParity(int x, int y) const;

        const std::unordered_map<int, std::shared_ptr<Entity>> & getGrid() const;
        void setGridElement(int startX, int startY, int endX, int endY);

        int getRenderX() const;
        int getRenderY() const;

        // functions to convert between x,y indices to map key
        int xyToIndex(int x, int y) const;
        std::pair<int, int> indexToXY(int index) const;
};

#endif // MAP_HPP
