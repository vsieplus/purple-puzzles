// Implementation for map class

#include "level/level.hpp"
#include "level/map.hpp"

const std::string Map::BG_LAYER_NAME = "background";
const std::string Map::ENTITY_LAYER_NAME = "entities";
const std::string Map::BG_TILESET_NAME = "bgTiles";

Map::Map() {}

// store map path
Map::Map(std::string tiledMapPath) : mapPath(tiledMapPath) {}

Map::~Map() {
    mapTilesets.clear();
}

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
        tile.render(renderer, mapTilesets.at(tile.getTilesetFirstGID()).get(),
            bgTilesetClips.at(tile.getTilesetGID()));
    }
}

// Load the map for the given level
void Map::loadMap(SDL_Renderer * renderer, Level * level) {
    tmx::Map map;

    if(map.load(mapPath)) {
        // update size variables
        auto tilesize = map.getTileSize();
        tileWidth = tilesize.x;
        tileHeight = tilesize.y;
        
        level->updateSize(map, tileWidth, tileHeight);
        mapWidth = level->getGridWidth();
        mapHeight = level->getGridHeight();

        loadTilesets(map, renderer);

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
                addBGTiles(tileLayer, level);
            } else if (layer->getName() == ENTITY_LAYER_NAME) {
                // load entities into the level
                level->addEntityTiles(tileLayer, mapTilesets);
            }
        }
    }
}

// Load the tilesets used in the map
void Map::loadTilesets(const tmx::Map & map, SDL_Renderer * renderer) {
    const auto & readTilesets = map.getTilesets();
    for(const auto & tileset: readTilesets) { 
        // load textures for each of the tilesets used in the map
        SDL_Surface * surface = IMG_Load(tileset.getImagePath().c_str());
        
        auto newTexture = std::shared_ptr<SDL_Texture> 
            (SDL_CreateTextureFromSurface(renderer, surface), SDL_DestroyTexture);
        
        SDL_FreeSurface(surface);

        // Insert the texture into the map
        mapTilesets.insert(std::pair<int, std::shared_ptr<SDL_Texture>>
            (tileset.getFirstGID(), std::move(newTexture)));
 
        // Get tileset clips for the background tiles
        if(tileset.getName() == BG_TILESET_NAME) {
            // Compute total size of tileset
            auto tilesetWidth = 0;
            auto tilesetHeight = 0;
            SDL_QueryTexture(mapTilesets.at(tileset.getFirstGID()).get(), NULL,
                NULL, &tilesetWidth, &tilesetHeight);

            // get vector of (unique) tiles
            const auto & tiles = tileset.getTiles();

            for(auto & tile: tiles) {
                // check/store this tile's parity
                checkTileParity(tile);

                // Get position of tile in the tileset to create the clip
                int tilesetX = tile.imagePosition.x;
                int tilesetY = tile.imagePosition.y;

                // Use total GID (relative tile ID + tileset's first GID)
                bgTilesetClips.emplace(tile.ID + tileset.getFirstGID(), 
                    (struct SDL_Rect) {tilesetX, tilesetY, tileWidth, tileHeight});               
            }
        }
        
    }
}

// Check the tile to store GIDs for the different tile parities
void Map::checkTileParity(const tmx::Tileset::Tile & tile) {
    auto & tileProperties = tile.properties;

    // return if no properties
    if(tileProperties.empty()) return;
    
    // Find 'parity' property of the tile
    unsigned int i = 0;
    while(i < tileProperties.size()) {
        if(tileProperties[i].getName() == "parity") {
            tileParities.emplace(tile.ID, tileProperties[i].getIntValue());
            break;
        }
        i++;
    }
}

// Add background tiles to the map
void Map::addBGTiles(const tmx::TileLayer * tileLayer, Level * level) {
    auto & layerTiles = tileLayer->getTiles();
            
    // Iterate through each tile in this layer
    for(auto x = 0; x < mapWidth; x++) {
        for(auto y = 0; y < mapHeight; y++) {
            int tileIndex = level->xyToIndex(x,y);

            // Get the GID for the current tile in this layer
            int tileGID = layerTiles[tileIndex].ID;

            // Find the first GID for the tileset this tile belongs to
            int tilesetFirstGID = -1;
            for(auto & tileset: mapTilesets) {
                // find the greatest GID which is <= ours
                if(tileset.first > tileGID) break;

                tilesetFirstGID = tileset.first;
            }

            // Get position of tile in the map
            auto mapX = x * tileWidth;
            auto mapY = y * tileHeight;

            // Get parity of the BG Tile (0:gray, 1:purple)
            int tileParity = tileParities[tileGID]; 

            // Add new tile to mapTiles
            mapTiles.emplace_back(mapX, mapY, tileWidth, tileHeight, 
                tilesetFirstGID, tileGID, tileParity);            
        }
    }
}