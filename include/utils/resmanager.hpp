// resource manager header file

#ifndef RESMANAGER_HPP
#define RESMANAGER_HPP

#include <unordered_map>
#include <memory>
#include <utility>
#include <string>
#include <functional>
#include <fstream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <nlohmann/json.hpp>

#include "utils/texture.hpp"

using json = nlohmann::json;

class ResManager {
    private:
        // json obj. holding data that maps resourceIDs -> filepaths
        json resourcePaths;

        // hashmaps for game resources; 
        //   key: hash of resource id (as specified in json file),
        //   val: shared ptr to resource
        
        // standalone textures
        std::unordered_map<int, std::shared_ptr<Texture>> textures;

        // spritesheets
        std::unordered_map<int, std::shared_ptr<Texture>> spritesheets;

        // sound fx/music
        // Deleters: Mix_FreeChunk/Mix_FreeMusic
        std::unordered_map<int, std::shared_ptr<Mix_Chunk>> sounds;
        std::unordered_map<int, std::shared_ptr<Mix_Music>> musics;

        // for hashing
        std::hash<std::string> resHash;

    public:
        // Construct the resource manager with a path to file containing the
        // resource paths (json)
        ResManager(std::string resourcePathsFile);

        // parse the json file
        void parseJSON(std::string resourcePathsFile);

        // load all resources
        void loadResourceMaps();

        void loadTextures();
        void loadSpritesheets();
        void loadSounds();
        void loadMusic();

        // to retrieve resources, call w/resource id
        std::shared_ptr<Texture> getTexture(std::string id) const;
        std::shared_ptr<Texture> getSpritesheet(std::string id) const;
        std::shared_ptr<Mix_Chunk> getSound(std::string id) const;
        std::shared_ptr<Mix_Music> getMusic(std::string id) const;
        std::string getMapPath(std::string id) const;
};

#endif // RESMANAGER_HPP