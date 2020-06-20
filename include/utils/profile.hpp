// Class for a user profile

#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include <SDL.h>

class Profile {
    public:
        // default constructor to create a new profile
        Profile(const std::vector<std::string> & levelIDs);

        void initLevelsCompleted(const std::vector<std::string> & levelIDs);

        // only call these periodically (when level complete/stats activated/exit/etc.)
        void addPlayTime(int seconds);
        void addTilesFlipped(int tiles);

        // call/within at the end of each level if applicable
        void setLevelComplete(std::string levelID);
        void addPerfectPlay();
        void addLevelReset();

        // reset all data -> default
        void resetProfile(const std::vector<std::string> & levelIDs);

        bool levelIsComplete(std::string levelID) const;
        std::string getStatsString() const;

    private:
        const static char NEWLINE_CHAR = '\n';

        const static int NUM_LEVELS = 30;

        int playTime = 0;             // in seconds (time in play state)
        int perfectPlays = 0;         // levels completed with no resets
        int tilesFlipped = 0;         // total tiles flipped during play
        int levelResets = 0;          // total level resets
        int numLevelsCompleted = 0;

        // map of booleans indicating level completeness
        // key: levelID, val: true == level is completed
        std::unordered_map<std::string, bool> levelsCompleted;

};

#endif // PROFILE_HPP