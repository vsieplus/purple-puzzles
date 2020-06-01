// Header for sound resource class (mix_chunk wrapper)

#ifndef SOUND_HPP
#define SOUND_HPP

#include <string>
#include <memory>

#include <SDL_mixer.h>

class Sound {
    private:
        std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> sound;

    public:
        Sound(std::string soundPath);

        void play();
};

#endif //SOUND_HPP