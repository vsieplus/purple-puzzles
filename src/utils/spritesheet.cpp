// implementation for spritesheet class

#include "utils/spritesheet.hpp"

SpriteSheet::SpriteSheet(std::string mapPath, std::string tilesetName, 
    SDL_Renderer * renderer) : spritesheetTexture(new Texture()) {
    
    loadSpritesheet(mapPath, tilesetName, renderer);
}
    

void SpriteSheet::loadSpritesheet(std::string mapPath, std::string tilesetName, 
    SDL_Renderer * renderer) {

    tmx::Map map;

    if(map.load(mapPath)) {
        const auto & readTilesets = map.getTilesets();

        for(const auto & tileset: readTilesets) {
            // check matching tileset names
            if(tileset.getName() == tilesetName) {
                // load the texture
                spritesheetTexture->loadTexture(tileset.getImagePath(), renderer);

                // store first GID
                firstGID = tileset.getFirstGID();

                // get vector of (unique) tiles
                const auto & tiles = tileset.getTiles();

                // construct sprites for each tile in the spritesheet
                for(auto & tile: tiles) {
                    loadTileProperties(tile);

                    // Get position/size of tile in the tileset to create the sprite/clip
                    int tilesetX = tile.imagePosition.x;
                    int tilesetY = tile.imagePosition.y;

                    int tileWidth = tile.imageSize.x;
                    int tileHeight = tile.imageSize.y;

                    std::shared_ptr<Sprite> tileSprite = 
                        std::make_shared<Sprite>(spritesheetTexture,
                        (struct SDL_Rect) {tilesetX, tilesetY, tileWidth, tileHeight});

                    sprites.emplace(tile.ID, tileSprite);               
                }
                break;
            }
        }
    }    
}

void SpriteSheet::loadTileProperties(const tmx::Tileset::Tile & tile) {
    // check/store this tile's properties if nonempty
    auto & properties = tile.properties;

    if(!properties.empty()) {
        std::unordered_map<std::string, std::any> propertyMap;

        for(auto & property: properties) {
            std::string propName = property.getName();
            tmx::Property::Type type = property.getType();

            switch(type) {
                case tmx::Property::Type::Boolean:
                    propertyMap.emplace(propName,
                        std::any_cast<bool>(property.getBoolValue()));
                    break;
                case tmx::Property::Type::Float:
                    propertyMap.emplace(propName, 
                        std::any_cast<float>(property.getFloatValue()));
                    break;
                case tmx::Property::Type::Int:
                    propertyMap.emplace(propName, 
                        std::any_cast<int>(property.getIntValue()));
                    break;
                case tmx::Property::Type::String:
                    propertyMap.emplace(propName,
                        std::any_cast<std::string>(property.getStringValue()));
                    break;
                case tmx::Property::Type::Colour:
                    propertyMap.emplace(propName,
                        std::any_cast<tmx::Colour>(property.getColourValue()));
                    break;
                case tmx::Property::Type::File:
                    propertyMap.emplace(propName, 
                        std::any_cast<std::string>(property.getFileValue()));
                    break;
                case tmx::Property::Type::Undef:
                    break;                    
            } 
        }
        tileProperties.emplace(tile.ID, propertyMap);
    }
}

// get the clip in this spritesheet for the given tile (as SDL_Rect) 
std::shared_ptr<Sprite> SpriteSheet::getSprite(int tileID) {
    return sprites.at(tileID);
}

int SpriteSheet::getFirstGID() const {
    return firstGID;
}

SDL_Texture * SpriteSheet::getTexture() const {
    return spritesheetTexture->getTexture().get();
}