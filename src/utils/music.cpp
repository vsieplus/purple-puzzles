// implementation for music resource class

#include "utils/music.hpp"

Music::Music(std::string musicPath) : 
    music(Mix_LoadMUS(musicPath.c_str()), Mix_FreeMusic) {}

// play the track if not currently playing
void Music::play() {
    if(Mix_PlayingMusic() == 0) {
        // -1 for looping,
        Mix_PlayMusic(music.get(), -1);

        Mix_VolumeMusic(MIX_MAX_VOLUME * 2 / 3);
    } 
}

// cut curr. volume in half (e.g. b/c of pause/popup)
void Music::deafen() {
    Mix_VolumeMusic(Mix_VolumeMusic(-1) / 2);
}

// double the curr. volume
void Music::undeafen() {
    Mix_VolumeMusic(Mix_VolumeMusic(-1) * 2);
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