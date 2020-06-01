// resource manager header file

#ifndef RESMANAGER_HPP
#define RESMANAGER_HPP

#include <vector>
#include <unordered_map>
#include <map>
#include <memory>
#include <utility>
#include <string>
#include <functional>
#include <fstream>

#include <SDL.h>

#include <nlohmann/json.hpp>
#include <tmxlite/Map.hpp>

#include "utils/texture.hpp"
#include "utils/spritesheet.hpp"
#include "utils/sound.hpp"
#include "utils/music.hpp"

using json = nlohmann::json;

// constants for resource file extensions used in this project
const int EXT_LENGTH = 3;

const std::string RES_FOLDER_NAME = "res";
const std::string RES_MAPS_NAME = "maps";

const char PATH_SEP = '/';

const std::string IMAGE_EXT = "png";
const std::string MAP_EXT = "tmx";
const std::string SOUND_EXT = "wav";
const std::string MUSIC_EXT = "ogg";

const std::string BG_TILESET_NAME = "bgTiles";

const std::string BASE_MAP_ID = "0-0";

class ResManager {
    private:
        SDL_Renderer * renderer;

        // map. holding data that maps hashes of resourceType IDs -> paths
        std::unordered_map<int, std::string> resourcePaths;

        // stack of hashes of resourceIDs to be loaded
        std::vector<int> resourcesToLoad;

        // hashmaps for game resources; 
        //   key: hash of resource id (as specified in json file),
        //   val: shared ptr to resource
        
        // standalone textures
        std::unordered_map<int, std::shared_ptr<Texture>> textures;

        // spritesheets
        std::unordered_map<int, std::shared_ptr<SpriteSheet>> spritesheets;

        // sound fx/music
        std::unordered_map<int, std::shared_ptr<Sound>> sounds;
        std::unordered_map<int, std::shared_ptr<Music>> musics;

        // tileset names
        std::unordered_map<int, std::string> tilesetNames;

        // for hashing
        std::hash<std::string> resHash;

    public:
        // Construct the resource manager with a path to file containing the
        // resource paths (json)
        ResManager(std::string resourcePathsFile, SDL_Renderer * renderer);

        // parse the json file
        void parseJSON(std::string resourcePathsFile);

        // load the next resource
        void loadNextResource();

        void loadTexture(int resourceIDHash, std::string resourcePath);        
        void loadSound(int resourceIDHash, std::string resourcePath);
        void loadMusic(int resourceIDHash, std::string resourcePath);
        void loadSpritesheet(int resourceIDHash, std::string resourcePath);

        bool loadingResources() const;

        std::string getResExt(std::string path);

        // to retrieve resources, call w/resource id
        std::shared_ptr<Texture> getTexture(std::string id) const;
        std::shared_ptr<SpriteSheet> getSpriteSheet(std::string id) const;
        std::shared_ptr<Sound> getSound(std::string id) const;
        std::shared_ptr<Music> getMusic(std::string id) const;
        
        std::string getResPath(std::string id) const;
};

#endif // RESMANAGER_HPP