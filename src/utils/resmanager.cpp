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

            int resHashID = resHash(res.first);

            resourcePaths.emplace(resHashID, resPath);

            // add to stack of resources to load if not map or is 0-0 map
            if(jsonObj.first != RES_MAPS_NAME) {
                resourcesToLoad.push_back(resHashID);
            } else if(res.first == BASE_MAP_ID) {
                // add each spritesheet resource from the first base map "0-0"
                tmx::Map map;

                if(map.load(resPath)) {
                    const auto & readTilesets = map.getTilesets();
                    for(const auto & tileset: readTilesets) {
                        // hash the name of the tileset
                        int ssHashID = resHash(tileset.getName());
                        
                        // add map path and hashed ID to resources to load
                        resourcePaths.emplace(ssHashID, resPath);
                        resourcesToLoad.push_back(ssHashID);

                        tilesetNames.emplace(ssHashID, tileset.getName());
                    }
                }
            }
        }
    }
}

// load all resources
void ResManager::loadNextResource() {
    if(!loadingResources()) {
        // clear unused resources
        tilesetNames.clear();

        return;
    }

    int currResID = resourcesToLoad.back();
    resourcesToLoad.pop_back();

    std::string currResFilepath = resourcePaths.at(currResID);

    // get file extension to determine resource type
    std::string resFileExt = getResExt(currResFilepath);

    if(resFileExt == IMAGE_EXT) {
        loadTexture(currResID, currResFilepath);
    } else if (resFileExt == MAP_EXT) {
        loadSpritesheet(currResID, currResFilepath);
    } else if (resFileExt == SOUND_EXT) {
        loadSound(currResID, currResFilepath);
    } else if (resFileExt == MUSIC_EXT) {
        loadMusic(currResID, currResFilepath);
    }
}

// load a standalone texture
void ResManager::loadTexture(int resourceIDHash, std::string resourcePath) {
    std::shared_ptr<Texture> texture (new Texture());
    texture->loadTexture(resourcePath, renderer);

    textures.emplace(resourceIDHash, texture);
}

// load a spritesheet via tutorial tiledmap (from resourcePath)
void ResManager::loadSpritesheet(int resourceIDHash, std::string resourcePath) {
    std::shared_ptr<SpriteSheet> spritesheet(
        new SpriteSheet(resourcePath, tilesetNames[resourceIDHash], renderer));

    spritesheets.emplace(resourceIDHash, spritesheet);
}

void ResManager::loadSound(int resourceIDHash, std::string resourcePath) {
/*    std::shared_ptr<Sound> (resourcePath)
      sounds.emplace(resourceIDHash, resourcePath)
 */
}

void ResManager::loadMusic(int resourceIDHash, std::string resourcePath) {
/*    std::shared_ptr<Music> (resourcePath)
      musics.emplace(resourceIDHash, resourcePath)
 */
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

std::shared_ptr<SpriteSheet> ResManager::getSpriteSheet(std::string id) const {
    return spritesheets.at(resHash(id));
}


std::shared_ptr<Sound> ResManager::getSound(std::string id) const {
    return sounds.at(resHash(id));
}

std::shared_ptr<Music> ResManager::getMusic(std::string id) const {
    return musics.at(resHash(id));
}

// Return the actual path to the file containing the resource with ID id
std::string ResManager::getResPath(std::string id) const {
    return resourcePaths.at(resHash(id));
}