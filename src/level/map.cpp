// Implementation for map class

#include "memswap.hpp"
#include "entities/player.hpp"
#include "level/level.hpp"
#include "level/map.hpp"

const std::string Map::BG_LAYER_NAME = "background";
const std::string Map::ENTITY_LAYER_NAME = "entities";

Map::Map() {}

// store map path
Map::Map(std::string tiledMapPath) : mapPath(tiledMapPath) {}

// Update each tile in the map
void Map::update(Level * level) {
    for(Tile tile: mapTiles) {
        tile.update(level);
    }
}

void Map::render(SDL_Renderer * renderer) const {
    // Render the background tiles
    for(Tile tile: mapTiles) {
        // Render tile with correct tileset + clip
        tile.render(renderer, 
            (mapTilesets->at(tile.getTilesetFirstGID()).get())->getTexture().get(),
             tilesetClips->at(tile.getTilesetGID()));
    }
}

// Load the map for the given level
void Map::loadMap(SDL_Renderer * renderer, Level * level, MemSwap * game) {
    tmx::Map map;

    if(map.load(mapPath)) {
        // update size variables
        auto tilesize = map.getTileSize();
        tileWidth = tilesize.x;
        tileHeight = tilesize.y;
        
        level->updateSize(map, tileWidth, tileHeight);
        mapWidth = level->getGridWidth();
        mapHeight = level->getGridHeight();

        // assign pointers to pre-loaded tilesets
        mapTilesets = game->getResManager().getSpritesheets();
        tilesetClips = game->getResManager().getSpritesheetClips();
        tileParities = game->getResManager().getTileParities();

        // Get data from each map layer, create necessary tiles/objects
        auto & mapLayers = map.getLayers();
        for (auto & layer : mapLayers) {
            if (layer->getType() != tmx::Layer::Type::Tile) {
                continue; // Do stuff with non-tile layers
            }

            auto * tileLayer = dynamic_cast<const tmx::TileLayer*>(layer.get());
            
            // process tiles differently depending on the layer we're on
            if(layer->getName() == BG_LAYER_NAME) {
                // Load background tiles
                addBGTiles(tileLayer, level, game);
            } else if (layer->getName() == ENTITY_LAYER_NAME) {
                // load entities into the level
                level->addEntityTiles(tileLayer, *mapTilesets);
            }
        }
    }
}

// Add background tiles to the map
void Map::addBGTiles(const tmx::TileLayer * tileLayer, Level * level, 
    MemSwap * game) {
    
    auto & layerTiles = tileLayer->getTiles();

    // Compute where to start placing tiles, given map size (center the map)
    renderX = (game->getScreenWidth() / 2) - (mapWidth * tileWidth / 2);
    renderY = (game->getScreenHeight() / 2) - (mapHeight * tileHeight / 2);            
            
    // Iterate through each tile in this layer (top left corner -> down right)
    for(int y = 0; y < mapHeight; y++) {
        for(int x = 0; x < mapWidth; x++) {
            int tileIndex = level->xyToIndex(x,y);

            // Get the GID for the current tile in this layer
            int tileGID = layerTiles[tileIndex].ID;

            // Find the first GID for the tileset this tile belongs to
            int tilesetFirstGID = -1;
            for(auto & tileset: *mapTilesets) {
                // find the greatest GID which is <= ours
                if(tileset.first > tileGID) break;

                tilesetFirstGID = tileset.first;
            }

            // Get position of tile in the map, centered for the given map size 
            auto mapX = renderX + x * tileWidth;
            auto mapY = renderY + y * tileHeight;

            // Get parity of the BG Tile if available (0:gray, 1:purple)
            auto tileParity = tileParities->find(tileGID);
            int tp = tileParity == tileParities->end() ? PARITY_NONE : tileParity->second;

            // Add new tile to mapTiles
            mapTiles.emplace_back(mapX, mapY, tileWidth, tileHeight, 
                tilesetFirstGID, tileGID, tp);
        }
    }
}

// Check if a tile at the given index is inbounds
bool Map::inBounds(int x, int y) const {
    return (x >= 0 && x <= mapWidth - 1) && (y >= 0 && y <= mapHeight - 1);
}


// Update bg tiles when the specified movement occurs
void Map::flipTiles(int tileX, int tileY, int moveDir, Level * level) {
    // Get indices for all tiles on map surrounding the old pos
    std::list<std::pair<int, int>> tileIndices;

    // none, Up, down, left, right (order matches Direction enum)
    tileIndices.push_back(std::make_pair(tileX, tileY));
/*     tileIndices.push_back(std::make_pair(tileX, tileY - 1));
    tileIndices.push_back(std::make_pair(tileX, tileY + 1));
    tileIndices.push_back(std::make_pair(tileX - 1, tileY));
    tileIndices.push_back(std::make_pair(tileX + 1, tileY));

    // Remove the pair for the tile we moved onto
    auto it = tileIndices.begin();
    advance(it, moveDir);
    tileIndices.erase(it); */

    // For each tile except the one we moved onto, try to call flip if inbounds
    for(auto indices: tileIndices) {
        // Check if in bounds
        if(inBounds(indices.first, indices.second)) {
            Tile & currTile = mapTiles.at(level->xyToIndex(indices.first, indices.second));

            // skip if tile is parity-neutral
            if(currTile.getTileParity() == PARITY_NONE) break;

            // Get new tileset GID (gray <-> purple)
            int tileGID = currTile.getTilesetGID();
            for(auto & tp: *tileParities) {
                // skip tiles already flipped once
                if(currTile.isFlipped()) continue;

                // Flip upon finding the first non-matching GID tile parity
                if(tp.first != tileGID) {
                    currTile.flip(tp.first);
                    break;
                }
            }

        }
    }
}

int Map::getTileParity(int x, int y) const {
    return mapTiles.at(x + y * mapWidth).getTileParity();
}

int Map::getRenderX() const {
    return renderX;
}

int Map::getRenderY() const {
    return renderY;
}