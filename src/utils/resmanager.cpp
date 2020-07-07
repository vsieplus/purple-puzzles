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

    // convert the json object to a resource path map, 1 val for each type of res.
    for(auto const & jsonObj: resPathMaps) {
        std::unordered_map<std::string, json> resPathMap = jsonObj.second;

        // loop through all resources in each resource map
        for(auto const & res: resPathMap) {
            std::string resPath = resPathMap[res.first].get<std::string>();

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
    int currResID = resourcesToLoad.back();
    resourcesToLoad.pop_back();

    std::string currResFilepath = resourcePaths.at(currResID);

    // get file extension to determine resource type
    std::string resFileExt = getResExt(currResFilepath);

    if(resFileExt == ANIMATION_EXT) {
        loadAnimation(currResID, currResFilepath);
    } else if(resFileExt == IMAGE_EXT) {
        loadTexture(currResID, currResFilepath);
    } else if (resFileExt == MAP_EXT) {
        loadSpritesheet(currResID, currResFilepath);
    } else if (resFileExt == SOUND_EXT) {
        loadSound(currResID, currResFilepath);
    } else if (resFileExt == MUSIC_EXT) {
        loadMusic(currResID, currResFilepath);
    } else if (resFileExt == FONT_EXT) {
        loadFont(currResID, currResFilepath);
    }

    // check if finished
    if(!loadingResources()) {
        // construct animation maps
        constructAnimationMaps();

        // clear unused resources
        animations.clear();
        tilesetNames.clear();
    }
}

// load a standalone texture
void ResManager::loadTexture(int resourceIDHash, std::string resourcePath) {
    auto texture = std::make_shared<Texture>();
    texture->loadTexture(resourcePath, renderer);

    textures.emplace(resourceIDHash, texture);
}

// load a spritesheet via tutorial tiledmap (from resourcePath)
void ResManager::loadSpritesheet(int resourceIDHash, std::string resourcePath) {    
    spritesheets.emplace(resourceIDHash, 
        std::make_shared<SpriteSheet>(resourcePath, tilesetNames[resourceIDHash], renderer));
}

void ResManager::loadSound(int resourceIDHash, std::string resourcePath) {
    sounds.emplace(resourceIDHash, std::make_shared<Sound>(resourcePath));
}

void ResManager::loadMusic(int resourceIDHash, std::string resourcePath) {
    musics.emplace(resourceIDHash, std::make_shared<Music>(resourcePath));
}

void ResManager::loadFont(int resourceIDHash, std::string resourcePath) {
    // pass path to font json (config) file
    fonts.emplace(resourceIDHash, std::make_shared<BitmapFont>(resourcePath, renderer));

    fontLoaded = true;
}

void ResManager::loadAnimation(int resourceIDHash, std::string resourcePath) {
    animations.emplace(resourceIDHash, 
        std::make_shared<Animation>(resourcePath, renderer, ANIM_FRAMEWIDTH, ANIM_FRAMEHEIGHT));
}

void ResManager::constructAnimationMaps() {
    tileAnimations = {{0, getAnimation(TILE_FLIP_ID)}};

    boostAnimations = {{0, getAnimation(BOOST_VANISH1_ID)}, 
        {1, getAnimation(BOOST_VANISH2_ID)}};

    playerAnimations = {{0, getAnimation(PLAYER_MERGE_ID)},
        {1, getAnimation(PLAYER_MFUP_ID)},
        {2, getAnimation(PLAYER_MFDOWN_ID)},
        {3, getAnimation(PLAYER_MFLEFT_ID)},
        {4, getAnimation(PLAYER_MFRIGHT_ID)},
        {5, getAnimation(PLAYER_TELEPORT_ID)}};

    diamondAnimations = {{0, getAnimation(DIAMOND_MERGE_ID)}};

    portalAnimations = {{0, getAnimation(PORTAL_MERGE_ID)}};
}

// return whether done loading resources
bool ResManager::loadingResources() const {
    return !resourcesToLoad.empty();
}

// helper function to get the resource extension from a path
// which should have the form res/EXT.../...
std::string ResManager::getResExt(std::string path) {
    size_t pos = 0;
    size_t firstSlash = path.find(PATH_SEP, pos);
    size_t secondSlash = path.find(PATH_SEP, firstSlash + 1);

    return path.substr(firstSlash + 1, secondSlash - firstSlash - 1);
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

std::shared_ptr<BitmapFont> ResManager::getFont(std::string id) const {
    return fonts.at(resHash(id));
}

std::shared_ptr<Animation> ResManager::getAnimation(std::string id) const {
    return animations.at(resHash(id));
}

// Return the actual path to the file containing the resource with ID id
std::string ResManager::getResPath(std::string id) const {
    return resourcePaths.at(resHash(id));
}

const std::unordered_map<int, std::shared_ptr<Animation>> & ResManager::getTileAnimations() const {
    return tileAnimations;
}

const std::unordered_map<int, std::shared_ptr<Animation>> & ResManager::getPlayerAnimations() const {
    return playerAnimations;
}

const std::unordered_map<int, std::shared_ptr<Animation>> & ResManager::getDiamondAnimations() const {
    return diamondAnimations;
}

const std::unordered_map<int, std::shared_ptr<Animation>> & ResManager::getBoostAnimations() const {
    return boostAnimations;
}

const std::unordered_map<int, std::shared_ptr<Animation>> & ResManager::getPortalAnimations() const {
    return portalAnimations;
}

const std::unordered_map<int, std::shared_ptr<Animation>> & ResManager::getReceptorAnimations() const {
    return receptorAnimations;
}

bool ResManager::fontIsLoaded() const {
    return fontLoaded;
}