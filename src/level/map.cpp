// Implementation for map class

#include "memswap.hpp"

#include "entities/player.hpp"
#include "entities/diamond.hpp"
#include "entities/receptor.hpp"

#include "level/level.hpp"
#include "level/map.hpp"


const std::string Map::BG_LAYER_NAME = "background";
const std::string Map::ENTITY_LAYER_NAME = "entities";

const std::string Map::PARITY_PROP = "parity";
const std::string Map::NAME_PROP = "name";

const std::string Map::PLAYER_ENAME = "player";
const std::string Map::RECEPTOR_ENAME = "receptor";
const std::string Map::BOOST_ENAME = "boost";
const std::string Map::EXIT_ENAME = "exit";
const std::string Map::DIAMOND_ENAME = "diamond";
const std::string Map::PORTAL_ENAME = "portal";

// constructors
Map::Map() {}
Map::Map(std::string tiledMapPath, SDL_Renderer * renderer, Level * level,
    MemSwap * game) {
    loadMap(tiledMapPath, renderer, level, game);
}

// handle events
void Map::handleEvents(Level * level, const Uint8 * keyStates) {
    for(int y = 0; y < mapHeight; y++) {
        for(int x = 0; x < mapWidth; x++) {
            int currIdx = xyToIndex(x,y);
            if(entityGrid.at(currIdx)) {
                entityGrid.at(currIdx)->handleEvents(keyStates, level);
            }
        }
    }
}

// Update each tile in the map
void Map::update(Level * level, float delta) {
    for(Tile tile: mapTiles) {
        tile.update(level);
    }
    
    // Update the entities
    for(int y = 0; y < mapHeight; y++) {
        for(int x = 0; x < mapWidth; x++) {
            int currIdx = xyToIndex(x,y);
            if(entityGrid.at(currIdx)) {
                entityGrid.at(currIdx)->update(level, delta);
            }
        }
    }
}

void Map::render(SDL_Renderer * renderer) const {
    // Render the background tiles
    for(Tile tile: mapTiles) {
        tile.render(renderer);
    }
    
    // Render the entities in the grid
    for(int y = 0; y < mapHeight; y++) {
        for(int x = 0; x < mapWidth; x++) {
            int currIdx = xyToIndex(x,y);
            if(entityGrid.at(currIdx)) {
                entityGrid.at(currIdx)->render(renderer);
            }
        }
    }    
}

// Load the map for the given level
void Map::loadMap(std::string tiledMapPath, SDL_Renderer * renderer,
    Level * level, MemSwap * game) {
    tmx::Map map;

    if(map.load(tiledMapPath)) {
        // update size variables
        auto tilesize = map.getTileSize();
        tileWidth = tilesize.x;
        tileHeight = tilesize.y;
        
        level->updateSize(map, tileWidth, tileHeight);
        mapWidth = level->getGridWidth();
        mapHeight = level->getGridHeight();

        // retrieve pre-loaded spritesheets for the tilesets used in this map
        for(auto & tileset: map.getTilesets()) {
            std::string tilesetName = tileset.getName();
            auto ssPtr = game->getResManager().getSpriteSheet(tilesetName);

            mapSpritesheets.emplace(tileset.getFirstGID(), ssPtr);
        }

        // Get data from each map layer, create necessary tiles/objects
        auto & mapLayers = map.getLayers();
        for (auto & layer : mapLayers) {
            if (layer->getType() != tmx::Layer::Type::Tile) {
                continue; // Do stuff with non-tile layers
            }

            auto * tileLayer = dynamic_cast<const tmx::TileLayer*>(layer.get());
            
            // process tiles differently depending on the layer we're on
            addTiles(tileLayer, level, game, layer->getName());
        }
    }
}

// Add tiles to the map
void Map::addTiles(const tmx::TileLayer * tileLayer, Level * level, 
    MemSwap * game, std::string layerName) {
    
    auto & layerTiles = tileLayer->getTiles();

    // Compute where to start placing tiles, given map size (center the map)
    renderX = (game->getScreenWidth() / 2) - (mapWidth * tileWidth / 2);
    renderY = (game->getScreenHeight() / 2) - (mapHeight * tileHeight / 2);            
            
    // Iterate through each tile in this layer (top left corner -> down right)
    for(int y = 0; y < mapHeight; y++) {
        for(int x = 0; x < mapWidth; x++) {
            int tileIndex = xyToIndex(x,y);

            // Get the GID for the current tile in this layer
            int tileGID = layerTiles[tileIndex].ID;

            // get tileset's firstGID (greatest ID <= ours)
            int tilesetFirstGID = -1;
            for(auto & spritesheet: mapSpritesheets) {
                int currFirstGID = spritesheet.first;
                if(currFirstGID <= tileGID &&  currFirstGID > tilesetFirstGID) {
                    tilesetFirstGID = currFirstGID;
                }
            }

            // no tileset found
            if(tilesetFirstGID == -1) continue;

            // normalize ID to the BG spritesheet
            int tileID = tileGID - tilesetFirstGID;

            // Get position of tile in the map, centered for the given map size 
            auto screenX = renderX + x * tileWidth;
            auto screenY = renderY + y * tileHeight;

            // Get the spritesheet of the tile
            auto & tileSpritesheet = mapSpritesheets.at(tilesetFirstGID);

            // Depending on the type of layer we're loading, process differently
            if(layerName == BG_LAYER_NAME) {
                addBGTile(screenX, screenY, tileID, tileSpritesheet);
            } else if(layerName == ENTITY_LAYER_NAME) {
                // initialize grid before loading first entity
                if(entityGrid.size() == 0) initGrid();

                addEntity(screenX, screenY, x, y, tileID, tileSpritesheet);
            }
        }
    }
}

// add a given bg tile
void Map::addBGTile(int screenX, int screenY, int tileID, 
    const std::shared_ptr<SpriteSheet> & tileSpritesheet) {
    
    // Get parity of the BG Tile from spritesheet properties
    int tileParity = tileSpritesheet->getPropertyValue<int>(tileID, PARITY_PROP);
    
    // add to parityTileIDs if not yet complete
    if(parityTileSprites.size() < 2) {
        parityTileSprites.emplace(tileParity, tileSpritesheet->getSprite(tileID));
    }
    
    // Add new tile to mapTiles
    mapTiles.emplace_back(screenX, screenY, tileWidth, tileHeight, tileParity,
        tileSpritesheet->getSprite(tileID));
}

void Map::addEntity(int screenX, int screenY, int gridX, int gridY, int tileID, 
    const std::shared_ptr<SpriteSheet> & tileSpritesheet) {

    auto entitySprite = tileSpritesheet->getSprite(tileID);

    // Get name of entity to determine what entity to create
    auto entityName = tileSpritesheet->getPropertyValue<std::string>(tileID, NAME_PROP);
    
    std::shared_ptr<Entity> newEntity;

    if(entityName == PLAYER_ENAME) {
        newEntity = std::make_shared<Player>(screenX, screenY, gridX, gridY, 
            entitySprite);
    } else if(entityName == DIAMOND_ENAME) {
        newEntity = std::make_shared<Diamond>(screenX, screenY, gridX, gridY, 
            entitySprite, tileSpritesheet->getPropertyValue<int>(tileID, PARITY_PROP));
    } else if(entityName == RECEPTOR_ENAME) {
        newEntity = std::make_shared<Receptor>(screenX, screenY, gridX, gridY, 
            entitySprite);
    } else if(entityName == BOOST_ENAME) {

    } else if(entityName == PORTAL_ENAME) {

    } else if(entityName == EXIT_ENAME) {

    }

    if(newEntity.get()) {
        entityGrid[xyToIndex(gridX, gridY)] = newEntity;
    }
}

// init entity grid
void Map::initGrid() {
    for(int y = 0; y < mapHeight; y++) {
        for(int x = 0; x < mapWidth; x++) {
            int currIdx = xyToIndex(x,y);
            entityGrid[currIdx] = nullptr;
        }
    }
}

// Check if a tile at the given index is inbounds
bool Map::inBounds(int x, int y) const {
    return (x >= 0 && x <= mapWidth - 1) && (y >= 0 && y <= mapHeight - 1);
}


// Update bg tile when the specified movement occurs at the specified pos. by
// an entity with the given parity
void Map::flipTile(int tileX, int tileY, int entityParity, Level * level) {
    // Check if in bounds
    if(inBounds(tileX, tileY)) {
        Tile & currTile = mapTiles.at(xyToIndex(tileX, tileY));

        // skip if tile is parity-neutral/has already been flipped
        if(currTile.getTileParity() == PARITY_NONE || currTile.isFlipped()) return;

        // Flip if parity differs from player's
        if(entityParity != currTile.getTileParity()) {
            currTile.flip(parityTileSprites.at(entityParity));
        }
    }
}

void Map::moveGridElement(int startX, int startY, int endX, int endY) {
    if(inBounds(startX, startY) && inBounds(endX, endY)) {
        entityGrid[xyToIndex(endX, endY)] = 
            std::move(entityGrid[xyToIndex(startX, startY)]);
    }
}

void Map::removeGridElement(int x, int y) {
    if(inBounds(x,y)) {
        entityGrid.at(xyToIndex(x,y)).reset();
    }
}

int Map::getTileParity(int x, int y) const {
    if(inBounds(x, y)) {
        return mapTiles.at(x + y * mapWidth).getTileParity();
    }
    
    return PARITY_NONE;
}

int Map::xyToIndex(int x, int y) const {
    return x + y * mapWidth;
}

std::pair<int, int> Map::indexToXY(int index) const {
    int x = index % mapWidth;
    int y = index / mapWidth;
    return std::make_pair(x,y);
}


int Map::getRenderX() const {
    return renderX;
}

int Map::getRenderY() const {
    return renderY;
}