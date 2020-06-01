// implementation for music resource class

#include "utils/music.hpp"

Music::Music(std::string musicPath) : 
    music(Mix_LoadMUS(musicPath.c_str()), Mix_FreeMusic) {}

// play the track if not currently playing
void Music::play() {
    if(Mix_PlayingMusic() == 0) {
            Mix_PlayMusic(music.get(), -1); // -1 for next available channel
    } 
}

void Music::pause() {
    Mix_PauseMusic();
}

void Music::resume() {
    Mix_ResumeMusic();
}

void Music::stop() {
    Mix_HaltMusic();
}