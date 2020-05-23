// Implementation for level class

#include "level/level.hpp"

Level::Level(std::string tiledMapPath, SDL_Renderer * renderer) {
    loadMap(tiledMapPath, renderer);

    grid.resize(gridWidth * gridHeight);

    grid[0] = std::make_unique<Player>(0, 0, 0, 0, renderer);
}

Level::~Level(){
    grid.clear();
    mapTiles.clear();
}

void Level::update(const Uint8* keyStates) {
    for(int x = 0; x < gridWidth; x++) {
        for(int y = 0; y < gridHeight; y++) {
            int curr_idx = x + y * gridWidth;
        
            if(grid[curr_idx]) {
                grid[curr_idx]->handleEvents(keyStates);
                grid[curr_idx]->update(this);

                // Change pos in grid if needed
                if(p->getGridX() != x || p->getGridY() != y) {
                    grid[curr_idx] = NULL;
                    grid[curr_idx] = p; 
                }
            }
        }
    }
}
void Level::render(SDL_Renderer* renderer) {
    // Render the map
    for(auto& tile: mapTiles) {
        tile->render(renderer);
    } 

    // Render the objs in the grid
    for(auto & entity : grid) {
        if(entity) {
            entity->render(renderer);
        }
    }    
}

void Level::loadMap(std::string tiledMapPath, SDL_Renderer * renderer) {
    tmx::Map map;

    if(map.load(tiledMapPath)) {
        auto map_dimensions = map.getTileCount();

        gridWidth = map_dimensions.x;
        gridHeight = map_dimensions.y;

        auto tilesize = map.getTileSize();
        TILE_WIDTH = tilesize.x;
        TILE_HEIGHT = tilesize.y;

        pixelWidth = TILE_WIDTH * gridWidth;
        pixelHeight = TILE_HEIGHT * gridHeight;

        // Get tilesets        
        std::map<int, SDL_Texture*> tilesets;

        const auto& map_tilesets = map.getTilesets();
        for(const auto& tileset : map_tilesets) {
            SDL_Texture* newTexture = NULL;

            SDL_Surface* surface = IMG_Load(tileset.getImagePath().c_str());

            newTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            tilesets.insert(std::pair<int, SDL_Texture*>(tileset.getFirstGID(), newTexture));
        }

        // Get data from each map layer
        auto& map_layers = map.getLayers();
        for (auto& layer : map_layers) {
            if (layer->getType() != tmx::Layer::Type::Tile) {
                continue;
            }

            auto* tile_layer = dynamic_cast<const tmx::TileLayer*>(layer.get());
            auto& layer_tiles = tile_layer->getTiles();

            for(auto x = 0; x < gridWidth; x++) {
                for(auto y = 0; y < gridHeight; y++) {
                    int tile_idx = x + (y * gridWidth);

                    auto cur_gid = layer_tiles[tile_idx].ID;

                    // Find the tileset it is from
                    auto tileset_gid = -1;
                    for(auto& ts: tilesets) {
                        // Pick the first tileset whose first gid is <= ours
                        if(ts.first <= cur_gid) {
                            tileset_gid = ts.first;
                        } else {
                            break;
                        }
                    }

                    // Normalize gid for the tileset
                    cur_gid -= tileset_gid;

                    // Get tileset width/height
                    auto ts_width = 0;
                    auto ts_height = 0;
                    SDL_QueryTexture(tilesets[tileset_gid], NULL, NULL, &ts_width, &ts_height);

                    auto ts_x = (cur_gid % (ts_width/TILE_WIDTH)) * TILE_WIDTH;
                    auto ts_y = (cur_gid / (ts_width/TILE_HEIGHT)) * TILE_HEIGHT;
//                    auto ts_x = 0;
//                    auto ts_y = 0;
                    auto xpos = x * TILE_WIDTH;
                    auto ypos = y * TILE_HEIGHT;

                    Tile * tile = new Tile(xpos, ypos, ts_x, ts_y, TILE_WIDTH, TILE_HEIGHT, tilesets[tileset_gid]);
                    mapTiles.push_back(tile);
                }
            }
        }
    }
}

int Level::getGridWidth() { 
    return gridWidth;
}

int Level::getGridHeight() { 
    return gridHeight;
}

int Level::getPixelWidth() { 
    return pixelWidth;
}

int Level::getPixelHeight() { 
    return pixelHeight;
}