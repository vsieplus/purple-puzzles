#include "utils/profile.hpp"
#include "memswap.hpp"

// default constructor to create a new default profile
Profile::Profile() {
    initLevelsCompleted();
}

void Profile::initLevelsCompleted() {
    levelsCompleted.fill(false);
}

// only call these periodically (when level complete/stats activated/exit/etc.)
void Profile::addPlayTime(int seconds) {
    playTime += seconds;
}

void Profile::addTilesFlipped(int tiles) {
    tilesFlipped += tiles;
}

/// check if a given level has been completed
bool Profile::levelIsComplete(int levelNum) const {
    return levelsCompleted.at(levelNum);
}

// call at the end of each level if applicable
void Profile::setLevelComplete(int levelNum) {
    if(!levelsCompleted.at(levelNum)) {
        levelsCompleted[levelNum] = true;
        numLevelsCompleted++;
    }
}

void Profile::addPerfectPlay() {
    perfectPlays++;
}

void Profile::addLevelReset() {
    levelResets++;
}

void Profile::resetProfile() {
    playTime = 0;
    perfectPlays = 0;
    tilesFlipped = 0;
    levelResets = 0;
    numLevelsCompleted = 0;

    initLevelsCompleted();
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