#include "utils/profile.hpp"
#include "memswap.hpp"

// default constructor to create a new profile
Profile::Profile(const std::vector<std::string> & levelIDs) {
    initLevelsCompleted(levelIDs);
}

void Profile::initLevelsCompleted(const std::vector<std::string> & levelIDs) {
    // initialize levelsCompleted map all false, except the first
    for(unsigned int i = 0; i < levelIDs.size(); i++) {
        levelsCompleted[levelIDs.at(i)] = i == 0;
    }
}

// only call these periodically (when level complete/stats activated/exit/etc.)
void Profile::addPlayTime(int seconds) {
    playTime += seconds;
}

void Profile::addTilesFlipped(int tiles) {
    tilesFlipped += tiles;
}

/// check if a given level has been completed
bool Profile::levelIsComplete(std::string levelID) const {
    return levelsCompleted.at(levelID);
}

// call at the end of each level if applicable
void Profile::setLevelComplete(std::string levelID) {
    if(!levelsCompleted.at(levelID)) {
        levelsCompleted[levelID] = true;
        numLevelsCompleted++;
    }
}

void Profile::addPerfectPlay() {
    perfectPlays++;
}

void Profile::addLevelReset() {
    levelResets++;
}

void Profile::resetProfile(const std::vector<std::string> & levelIDs) {
    playTime = 0;
    perfectPlays = 0;
    tilesFlipped = 0;
    levelResets = 0;
    numLevelsCompleted = 0;

    initLevelsCompleted(levelIDs);
}

// construct stats string 你好
std::string Profile::getStatsString() const {
    std::string stats;

    // get current player data ...
    stats += "Play Time: " + std::to_string(playTime) + NEWLINE_CHAR;
    stats += "Tiles Flipped: " + std::to_string(tilesFlipped) + NEWLINE_CHAR;
    stats += "Perfect Plays (no reset): " + std::to_string(perfectPlays) + NEWLINE_CHAR;
    stats += "Level Resets: " + std::to_string(levelResets) + NEWLINE_CHAR;
    stats += "Levels Completed: " + std::to_string(numLevelsCompleted) + 
        "/30" + NEWLINE_CHAR;

    return stats;
}