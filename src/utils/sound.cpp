// sound implementation class

#include "utils/sound.hpp"

Sound::Sound(std::string soundPath) :
    sound(Mix_LoadWAV(soundPath.c_str()), Mix_FreeChunk) {
}

void Sound::play() {
    // Play sound on next available channel, repeated 0 times
    Mix_PlayChannel(-1, sound.get(), 0);
}