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

        // update upon switching on/off play state
        void addPlayTime(int seconds);

        // call at the end of each level if applicable
        void setLevelComplete(int levelNum);
        void addLevelResets(int resets);
        void addMovesUndone(int movesUndone);
        void addTilesFlipped(int tiles);
        void addPerfectPlay();

        // reset all data -> default
        void resetProfile();

        bool levelIsComplete(int levelNum) const;
        std::string getStatsString() const;

    private:
        const static char NEWLINE_CHAR = '\n';

        const static int NUM_LEVELS = 30;
        const static int SEC_PER_HOUR = 3600;
        const static int SEC_PER_MIN = 60;

        int playTime = 0;             // in seconds (time in play state)
        int perfectPlays = 0;         // levels completed with no resets
        int tilesFlipped = 0;         // total tiles flipped during play
        int levelResets = 0;          // total level resets
        int movesUndone = 0;          // moves undone
        int numLevelsCompleted = 0;

        // bool array indicating level completeness
        // in order of level number (1 -> 30)
        std::array<bool, NUM_LEVELS> levelsCompleted;
};

#endif // PROFILE_HPP