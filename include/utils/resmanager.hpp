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
#include "utils/bitmapfont.hpp"
#include "utils/animation.hpp"

using json = nlohmann::json;

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

        // fonts
        std::unordered_map<int, std::shared_ptr<BitmapFont>> fonts;

        // animations
        std::unordered_map<int, std::shared_ptr<Animation>> animations;
        std::unordered_map<int, std::shared_ptr<Animation>> tileAnimations;
        std::unordered_map<int, std::shared_ptr<Animation>> playerAnimations;
        std::unordered_map<int, std::shared_ptr<Animation>> boostAnimations;
        std::unordered_map<int, std::shared_ptr<Animation>> diamondAnimations;
        std::unordered_map<int, std::shared_ptr<Animation>> portalAnimations;
        std::unordered_map<int, std::shared_ptr<Animation>> receptorAnimations;

        // tileset names
        std::unordered_map<int, std::string> tilesetNames;

        // for hashing
        std::hash<std::string> resHash;

        const int ANIM_FRAMEWIDTH = 32;
        const int ANIM_FRAMEHEIGHT = 32;
                
        // constants for resource file extensions used in this project
        inline const static std::string RES_FOLDER_NAME = "res";
        inline const static std::string RES_MAPS_NAME = "maps";

        inline const static char PATH_SEP = '/';

        inline const static std::string ANIMATION_EXT = "animations";
        inline const static std::string IMAGE_EXT = "images";
        inline const static std::string MAP_EXT = "maps";
        inline const static std::string SOUND_EXT = "sounds";
        inline const static std::string MUSIC_EXT = "music";
        inline const static std::string FONT_EXT = "fonts";

        inline const static std::string BG_TILESET_NAME = "bgTiles";

        inline const static std::string BASE_MAP_ID = "testing";

        inline static const std::string TILE_FLIP_ID = "tileFlip";
        inline const static std::string BOOST_VANISH1_ID = "boostVanish1";
        inline const static std::string BOOST_VANISH2_ID = "boostVanish2";
        inline const static std::string DIAMOND_MERGE_ID = "diamondMerge";
        inline const static std::string PORTAL_MERGE_ID = "portalMerge";

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
        void loadFont(int resourceIDHash, std::string resourcePath);
        void loadAnimation(int resourceIDHash, std::string resourcePath);

        void constructAnimationMaps();

        bool loadingResources() const;

        std::string getResExt(std::string path);

        // to retrieve resources, call w/resource id
        std::shared_ptr<Texture> getTexture(std::string id) const;
        std::shared_ptr<SpriteSheet> getSpriteSheet(std::string id) const;
        std::shared_ptr<Sound> getSound(std::string id) const;
        std::shared_ptr<Music> getMusic(std::string id) const;
        std::shared_ptr<BitmapFont> getFont(std::string id) const;
        std::shared_ptr<Animation> getAnimation(std::string id) const;
        
        std::string getResPath(std::string id) const;

        const std::unordered_map<int, std::shared_ptr<Animation>> & getTileAnimations() const;
        const std::unordered_map<int, std::shared_ptr<Animation>> & getPlayerAnimations() const;
        const std::unordered_map<int, std::shared_ptr<Animation>> & getDiamondAnimations() const;
        const std::unordered_map<int, std::shared_ptr<Animation>> & getBoostAnimations() const;
        const std::unordered_map<int, std::shared_ptr<Animation>> & getPortalAnimations() const;
        const std::unordered_map<int, std::shared_ptr<Animation>> & getReceptorAnimations() const;
};

#endif // RESMANAGER_HPP