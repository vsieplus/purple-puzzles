// Resource manager class

#include "utils/resmanager.hpp"

// Construct the resource manager with a path to file containing the
// resource paths (json)
ResManager::ResManager(std::string resourcePathsFile) {
    // parse json file + map resource ids -> filenames
    parseJSON(resourcePathsFile);
    
}

void ResManager::parseJSON(std::string resourcePathsFile) {
    // load the json string to the resourcePaths json obj.
    std::ifstream instream(resourcePathsFile);
    instream >> resourcePaths;

    // count total resources (excluding maps)
    totalResources = 100;
}

// load all resources
void ResManager::loadNextResource() {
    if(!loadingResources()) return;



    resourcesLoaded++;
}


void ResManager::loadTextures() {
}

void ResManager::loadSpritesheets() {

}

void ResManager::loadSounds() {
//    gBeep = Mix_LoadWAV("beep.wav");
}

void ResManager::loadMusic() {
//    gMusic = Mix_LoadMUS("safe.mp3");
}

// return whether done loading resources
bool ResManager::loadingResources() const {
    return resourcesLoaded < totalResources;
}

// to retrieve resources, call w/resource id
std::shared_ptr<Texture> ResManager::getTexture(std::string id) const {
    return textures.at(resHash(id));
}

std::shared_ptr<Texture> ResManager::getSpritesheet(std::string id) const {
    return spritesheets.at(resHash(id));
}

std::shared_ptr<Mix_Chunk> ResManager::getSound(std::string id) const {
    return sounds.at(resHash(id));
}

std::shared_ptr<Mix_Music> ResManager::getMusic(std::string id) const {
    return musics.at(resHash(id));
}

// Return the actual path to the file containing the resource with ID id
std::string ResManager::getResPath(std::string id) const {
    return "foo.txt";
//    return resourcePaths[id];
}