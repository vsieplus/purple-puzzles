#include "utils/animation.hpp"

Animation::Animation(std::string animationPath, SDL_Renderer * renderer, 
    int frameWidth, int frameHeight, bool looping) : 
    animationSpritesheet(animationPath, renderer, frameWidth, frameHeight),
    numFrames(animationSpritesheet.getNumSprites()),
    frameWidth(frameWidth), frameHeight(frameHeight),
    looping(looping) {}

void Animation::render(int x, int y, int frameNum, SDL_Renderer * renderer) {
    animationSpritesheet.getSprite(frameNum)->render(renderer, 
        (struct SDL_Rect) {x, y, frameWidth, frameHeight});
}

int Animation::getMsPerFrame() const {
    return msPerFrame;
}

int Animation::getNumFrames() const {
    return numFrames;
}

int Animation::getFrameHeight() const {
    return frameHeight;
}

int Animation::getFrameWidth() const {
    return frameWidth;
}

bool Animation::isLooping() const {
    return looping;
}

void Animation::setLooping(bool looping) {
    this->looping = looping;
}