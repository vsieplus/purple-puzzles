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
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <nlohmann/json.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>

#include "utils/texture.hpp"

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

class ResManager {
    private:
        SDL_Renderer * renderer;

        // map. holding data that maps hashes of resourceType IDs -> paths
        std::unordered_map<std::string, std::string> resourcePaths;

        // stack of resourceIDs to be loaded
        std::vector<std::string> resourcesToLoad;

        // hashmaps for game resources; 
        //   key: hash of resource id (as specified in json file),
        //   val: shared ptr to resource
        
        // standalone textures
        std::unordered_map<int, std::shared_ptr<Texture>> textures;

        // sound fx/music
        // Deleters: Mix_FreeChunk/Mix_FreeMusic
        std::unordered_map<int, std::shared_ptr<Mix_Chunk>> sounds;
        std::unordered_map<int, std::shared_ptr<Mix_Music>> musics;

        // spritesheet map; key: first GID, val: ptr to texture
        std::map<int, std::shared_ptr<Texture>> spritesheets;

        // map of tileset clips for tiles; key: tile GID, val: SDL_Rect
        std::unordered_map<int, SDL_Rect> tilesetClips;

        // map of tile parities for BG Tiles (Should only contain 2 elems)
        // key: tile GID, val: Tile parity (int indicating parity) 
        std::map<int, int> tileParities;

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

        void loadTexture(std::string resourceID, std::string resourcePath);        
        void loadSound(std::string resourceID, std::string resourcePath);
        void loadMusic(std::string resourceID, std::string resourcePath);

        void loadSpritesheets(std::string resourcePath);
        void checkTileParity(const tmx::Tileset::Tile & tile, int tilesetFirstGID);

        bool loadingResources() const;

        std::string getResExt(std::string path);

        // to retrieve resources, call w/resource id
        std::shared_ptr<Texture> getTexture(std::string id) const;
        std::shared_ptr<Mix_Chunk> getSound(std::string id) const;
        std::shared_ptr<Mix_Music> getMusic(std::string id) const;

        std::map<int, std::shared_ptr<Texture>> * getSpritesheets();
        std::unordered_map<int, SDL_Rect> * getSpritesheetClips();
        std::map<int, int> * getTileParities();
        
        std::string getResPath(std::string id) const;
};

#endif // RESMANAGER_HPP