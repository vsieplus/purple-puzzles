// wrapper class for a music resource (mix_music)

#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <string>
#include <memory>

#include <SDL_mixer.h>

class Music {
    private:
        std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)> music;

    public:
        Music(std::string musicPath);

        void play();
        void pause();
        void resume();
        void stop();
};

#endif // MUSIC_HPP