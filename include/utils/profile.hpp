// Class for a user profile

#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <string>
#include <array>

#include <SDL.h>

class Profile {
    public:
        // default constructor to create a new profile
        Profile();

        void initLevelsCompleted();

        // only call these periodically (when level complete/stats activated/exit/etc.)
        void addPlayTime(int seconds);
        void addTilesFlipped(int tiles);

        // call/within at the end of each level if applicable
        void setLevelComplete(int levelNum);
        void addPerfectPlay();
        void addLevelReset();

        // reset all data -> default
        void resetProfile();

        bool levelIsComplete(int levelNum) const;
        std::string getStatsString() const;

    private:
        const static char NEWLINE_CHAR = '\n';

        const static int NUM_LEVELS = 30;

        int playTime = 0;             // in seconds (time in play state)
        int perfectPlays = 0;         // levels completed with no resets
        int tilesFlipped = 0;         // total tiles flipped during play
        int levelResets = 0;          // total level resets
        int numLevelsCompleted = 0;

        // bool array indicating level completeness
        // in order of level number (1 -> 30)
        std::array<bool, NUM_LEVELS> levelsCompleted;
};

#endif // PROFILE_HPP