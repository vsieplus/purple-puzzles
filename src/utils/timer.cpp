/**
 * @file timer.cpp
 * @author vsie
 * @brief implementation for timer class
 * @version 0.1
 * @date 2020-05-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */

 #include "utils/timer.hpp"

// Start the timer when called
void Timer::start() {
    startTick = SDL_GetTicks();
    started = true;
}

// Pause the current timer, if running and not currently paused
void Timer::pause() {
    if(started && !paused) {
        pausedTick = SDL_GetTicks() - startTick;
        paused = true;   
    }        
}

// Unpause the timer if currently paused
void Timer::unpause() {
    if(started && paused) {
        // Set start time to difference from curr. ticks to when we paused.
        // This resumes the timer at the ticks originally passed when paused.
        startTick = SDL_GetTicks() - pausedTick;

        paused = false;
        pausedTick = 0;
    }
}

// Reset the timer
void Timer::reset() {
    started = false;
    paused = false;

    startTick = 0;
    pausedTick = 0;
}

// Get the current ticks of the timer, depending if paused or not
Uint32 Timer::getCurrentTick() {
    if(started) {
        if(paused) {
            return pausedTick;
        } else {
            return SDL_GetTicks() - startTick;
        }
    }

    return 0;
}

bool Timer::isStarted() { 
    return started; 
}

bool Timer::isPaused() {
    return started && paused;
}