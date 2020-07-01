// wrapper class for animation spritesheet

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "utils/spritesheet.hpp"

class Animation {
    public:
        Animation(std::string animationPath, SDL_Renderer * renderer, 
            int frameWidth, int frameHeight, int msPerFrame = 40,
            bool looping = false);

        void render(int x, int y, int frameNum, SDL_Renderer * renderer,
            double angle = 0.0) const;

        int getMsPerFrame() const;
        int getNumFrames() const;
        int getFrameHeight() const;
        int getFrameWidth() const;
        bool isLooping() const;

        void setLooping(bool looping);

    private:
        SpriteSheet animationSpritesheet;

        // amt. of ms to show each frame of the animation for
        int msPerFrame;

        int numFrames;

        // size of animation frames
        int frameWidth;
        int frameHeight;

        bool looping;
        
};

#endif // ANIMATION_HPP