// Implementation for map class

#include "memswap.hpp"

#include "entities/player.hpp"
#include "entities/diamond.hpp"
#include "entities/receptor.hpp"
#include "entities/boost.hpp"
#include "entities/portal.hpp"

#include "level/level.hpp"
#include "level/map.hpp"


const std::string Map::BG_LAYER_NAME = "background";
const std::string Map::ENTITY_LAYER_NAME = "entities";

const std::string Map::PARITY_PROP = "parity";
const std::string Map::NAME_PROP = "name";
const std::string Map::DIR_PROP = "direction";
const std::string Map::POWER_PROP = "power";
const std::string Map::SHAPE_PROP = "shape";

const std::string Map::PLAYER_ENAME = "player";
const std::string Map::RECEPTOR_ENAME = "receptor";
const std::string Map::BOOST_ENAME = "boost";
const std::string Map::DIAMOND_ENAME = "diamond";
const std::string Map::PORTAL_ENAME = "portal";

// constructors
Map::Map() {}
Map::Map(std::string tiledMapPath, SDL_Renderer * renderer, Level * level,
    MemSwap * game) {
    loadMap(tiledMapPath, renderer, level, game);
}

// handle events (top left -> down right)
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
        tile.update(level, delta);
    }

    // if level has portals and they're removed, update manually
    if(!mapPortals.empty() && mapPortals.back()->isRemoved()) {
        for(auto portal: mapPortals) {
            portal->update(level, delta);
        }
    }
    
    // Update the entities
    for(int y = 0; y < mapHeight; y++) {
        for(int x = 0; x < mapWidth; x++) {
            int currIdx = xyToIndex(x,y);
            if(entityGrid.at(currIdx)) {
                entityGrid.at(currIdx)->update(level, delta);
            }

            // if level has been compl
        }
    }
}

void Map::render(SDL_Renderer * renderer) const {
    // Render the background tiles
    for(Tile tile: mapTiles) {
        tile.render(renderer);
    }

    // render portals manually if tmp. removed
    if(!mapPortals.empty() && mapPortals.back()->isRemoved()) {
        for(auto portal: mapPortals) {
            portal->render(renderer);
        }
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
    const std::shared_ptr<SpriteSheet> & spritesheet) {
    
    // Get parity of the BG Tile from spritesheet properties
    int tileParity = spritesheet->getPropertyValue<int>(tileID, PARITY_PROP);
    
    // add to parityTileIDs if not yet complete
    if(parityTileSprites.size() < 2) {
        parityTileSprites.emplace(tileParity, spritesheet->getSprite(tileID));
    }
    
    // Add new tile to mapTiles
    mapTiles.emplace_back(screenX, screenY, tileParity, spritesheet->getSprite(tileID));
}

void Map::addEntity(int screenX, int screenY, int gridX, int gridY, int tileID, 
    const std::shared_ptr<SpriteSheet> & spritesheet) {

    auto entitySprite = spritesheet->getSprite(tileID);

    // Get name/parity of entity to determine what entity to create
    auto entityName = spritesheet->getPropertyValue<std::string>(tileID, NAME_PROP);
    int parity = spritesheet->getPropertyValue<int>(tileID, PARITY_PROP);

    std::shared_ptr<Entity> newEntity;

    if(entityName == PLAYER_ENAME) {
        newEntity = std::make_shared<Player>(screenX, screenY, gridX, gridY, 
            parity, entitySprite);
    } else if(entityName == DIAMOND_ENAME) {
        newEntity = std::make_shared<Diamond>(screenX, screenY, gridX, gridY, 
            parity, entitySprite);
    } else if(entityName == RECEPTOR_ENAME) {
        auto shape = spritesheet->getPropertyValue<std::string>(tileID, SHAPE_PROP);

        newEntity = std::make_shared<Receptor>(screenX, screenY, gridX, gridY, 
            parity, entitySprite, shape);
    } else if(entityName == BOOST_ENAME) {
        // get direction/power properties for boost
        int power = spritesheet->getPropertyValue<int>(tileID, POWER_PROP);
        int direction = spritesheet->getPropertyValue<int>(tileID, DIR_PROP);

        newEntity = std::make_shared<Boost>(screenX, screenY, gridX, gridY,
            parity, entitySprite, power, direction);
    } else if(entityName == PORTAL_ENAME && mapPortals.size() < 2) {
        usesPortals = true;

        newEntity = std::make_shared<Portal>(screenX, screenY, gridX, gridY, 
            parity, entitySprite);

        std::shared_ptr<Portal> newPortal = std::dynamic_pointer_cast<Portal>(newEntity);

        if(mapPortals.size() == 1) {
            mapPortals.back()->setOtherPortal(newPortal);
            newPortal->setOtherPortal(mapPortals.back());
        }

        mapPortals.push_back(newPortal);
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
        int idx = xyToIndex(tileX, tileY);

        // skip if a portal is on this tile/is curr. removed
        if(!mapPortals.empty() && mapPortals.back()->isRemoved()) {
            for(auto portal: mapPortals) {
                if(idx == xyToIndex(portal->getGridX(), portal->getGridY())) {
                    return;
                }
            }
        }

        Tile & currTile = mapTiles.at(xyToIndex(tileX, tileY));

        // skip if tile is parity-neutral/has already been flipped
        if(currTile.getParity() == PARITY_NONE || currTile.isFlipped()) return;

        // Flip if parity differs from player's
        if(entityParity != currTile.getParity()) {
            currTile.flip(parityTileSprites.at(entityParity));
        }
    }
}

void Map::moveGridElement(int startX, int startY, int endX, int endY) {
    if(inBounds(startX, startY) && inBounds(endX, endY)) {
        auto eIdx = xyToIndex(endX, endY);
        entityGrid[eIdx] = std::move(entityGrid[xyToIndex(startX, startY)]);

        entityGrid[eIdx]->setGridX(endX);
        entityGrid[eIdx]->setGridY(endY);
    }
}

void Map::placeGridElement(std::shared_ptr<Entity> entity, int x, int y) {
    if(inBounds(x,y)) {
        entityGrid[xyToIndex(x,y)] = entity;
    }
}

void Map::removeGridElement(int x, int y) {
    if(inBounds(x,y)) {
        entityGrid.at(xyToIndex(x,y)).reset();
    }
}

// place portals in grid
void Map::placePortals() {
    for(auto portal: mapPortals) {
        placeGridElement(portal, portal->getGridX(), portal->getGridY());
    }
}

Parity Map::getTileParity(int x, int y) const {
    if(inBounds(x, y)) {
        return mapTiles.at(x + y * mapWidth).getParity();
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

bool Map::hasPortals() const {
    return usesPortals;
}

int Map::getRenderX() const {
    return renderX;
}

int Map::getRenderY() const {
    return renderY;
}