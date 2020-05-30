// Resource manager class

#include "utils/resmanager.hpp"


// Construct the resource manager with a path to file containing the
// resource paths (json)
ResManager::ResManager(std::string resourcePathsFile, SDL_Renderer * renderer):
    renderer(renderer) {
    // parse json file + map resource ids -> filenames
    parseJSON(resourcePathsFile);
    
}

void ResManager::parseJSON(std::string resourcePathsFile) {
    // load the json string to the resourcePaths json obj.
    json jsonRes;

    std::ifstream instream(resourcePathsFile);
    instream >> jsonRes;

    std::unordered_map<std::string, json> resPathMaps = jsonRes;

    // convert the json object to a resource path map
    for(auto const & jsonObj: resPathMaps) {
        std::unordered_map<std::string, json> resPathMap = jsonObj.second;

        // loop through all resources in each resource map
        for(auto const & res: resPathMap) {
            std::string resPath = resPathMap[res.first];

            // Add path to folder depending on resource type
            resPath = RES_FOLDER_NAME + PATH_SEP + jsonObj.first +
                PATH_SEP + resPath;

            resourcePaths.emplace(res.first, resPath);

            // add to stack of resources to load if not map or is 0-0 map
            if(jsonObj.first != RES_MAPS_NAME || res.first == "0-0") {
                resourcesToLoad.push_back(res.first);
            }
        }
    }
}

// load all resources
void ResManager::loadNextResource() {
    if(!loadingResources()) return;

    std::string currResID = resourcesToLoad.back();
    resourcesToLoad.pop_back();

    std::string currResFilepath = resourcePaths.at(currResID);

    // get file extension to determine resource type
    std::string resFileExt = getResExt(currResFilepath);

    if(resFileExt == IMAGE_EXT) {
        loadTexture(currResID, currResFilepath);
    } else if (resFileExt == MAP_EXT) {
        loadSpritesheets(currResFilepath);
    } else if (resFileExt == SOUND_EXT) {
        loadSound(currResID, currResFilepath);
    } else if (resFileExt == MUSIC_EXT) {
        loadMusic(currResID, currResFilepath);
    }
}

// load a standalone texture
void ResManager::loadTexture(std::string resourceID, std::string resourcePath) {
    std::shared_ptr<Texture> texture;
    texture->loadTexture(resourcePath, renderer);

    textures.emplace(resHash(resourceID), texture);
}

// load a spritesheet via tutorial tiledmap (from resourcePath)
void ResManager::loadSpritesheets(std::string resourcePath) {
    tmx::Map map;

    if(map.load(resourcePath)) {
        const auto & readTilesets = map.getTilesets();
        for(const auto & tileset: readTilesets) { 
            // load textures for each of the tilesets used in the map
            std::shared_ptr<Texture> texture;
            texture->loadTexture(tileset.getImagePath(), renderer);

            // add the tileset with its first GID as key
            spritesheets.emplace(tileset.getFirstGID(), texture);
            
            // Add tileset clips for each tileset

            // Compute total size of tileset
            auto tilesetWidth = 0;
            auto tilesetHeight = 0;
            SDL_QueryTexture((spritesheets.at(tileset.getFirstGID()).get())->getTexture().get(), 
                NULL, NULL, &tilesetWidth, &tilesetHeight);

            // get vector of (unique) tiles
            const auto & tiles = tileset.getTiles();

            for(auto & tile: tiles) {
                // check/store this tile's parity if properties nonempty (for BG)
                if(tileset.getName() == BG_TILESET_NAME) {
                    checkTileParity(tile, tileset.getFirstGID());
                }
                    
                // Get position/size of tile in the tileset to create the clip
                int tilesetX = tile.imagePosition.x;
                int tilesetY = tile.imagePosition.y;

                int tileWidth = tile.imageSize.x;
                int tileHeight = tile.imageSize.y;

                // Use total GID (relative tile ID + tileset's first GID)
                tilesetClips.emplace(tile.ID + tileset.getFirstGID(), 
                    (struct SDL_Rect) {tilesetX, tilesetY, tileWidth, tileHeight});               
            }
        }
    }
}

// Check the tile to store GIDs for the different tile parities
void ResManager::checkTileParity(const tmx::Tileset::Tile & tile,
    int tilesetFirstGID) {
    auto & tileProperties = tile.properties;

    // return if no properties
    if(tileProperties.empty()) return;
    
    // Find 'parity' property of the tile
    for(unsigned int i = 0; i < tileProperties.size(); i++) {
        if(tileProperties[i].getName() == "parity") {
            tileParities.emplace(tile.ID + tilesetFirstGID,
                tileProperties[i].getIntValue());
            break;
        }
    }
}

void ResManager::loadSound(std::string resourceID, std::string resourcePath) {
//    gBeep = Mix_LoadWAV("beep.wav");
}

void ResManager::loadMusic(std::string resourceID, std::string resourcePath) {
//    gMusic = Mix_LoadMUS("safe.mp3");
}

// return whether done loading resources
bool ResManager::loadingResources() const {
    return !resourcesToLoad.empty();
}

// helper function to get the file extension for a resource
std::string ResManager::getResExt(std::string path) {
    return path.substr(path.length() - EXT_LENGTH);
}

// to retrieve resources, call w/resource id
std::shared_ptr<Texture> ResManager::getTexture(std::string id) const {
    return textures.at(resHash(id));
}

std::map<int, std::shared_ptr<Texture>> * ResManager::getSpritesheets() {
    return &spritesheets;
}

std::unordered_map<int, SDL_Rect> * ResManager::getSpritesheetClips() {
    return &tilesetClips;
}

std::map<int, int> * ResManager::getTileParities() {
    return &tileParities;
}

std::shared_ptr<Mix_Chunk> ResManager::getSound(std::string id) const {
    return sounds.at(resHash(id));
}

std::shared_ptr<Mix_Music> ResManager::getMusic(std::string id) const {
    return musics.at(resHash(id));
}

// Return the actual path to the file containing the resource with ID id
std::string ResManager::getResPath(std::string id) const {
    return resourcePaths.at(id);
}