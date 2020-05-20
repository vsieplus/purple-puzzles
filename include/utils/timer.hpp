// Header for utility timer class

#ifndef TIMER_HPP
#define TIMER_HPP

#include <SDL.h>

class Timer {
    private:
        Uint32 startTick = 0;
        Uint32 pausedTick = 0;

        bool started = false;
        bool paused = false;

    public:
        void start();
        void pause();
        void unpause();
        void reset();

        Uint32 getCurrentTick();

        bool isStarted();
        bool isPaused();
};

#endif // TIMER_HPP