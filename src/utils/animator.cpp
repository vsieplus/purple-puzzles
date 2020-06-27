#include "utils/animator.hpp"

Animator::Animator() {}

void Animator::start() {
    msFromStart = 0;
    currFrame = 0;
    setAnimating(true);
}

void Animator::update(float delta) {
    msFromStart += delta;
    currFrame = msFromStart / currAnimation->getMsPerFrame();

    // check if Animator has finished, stop unless looping
    if(currFrame > currAnimation->getNumFrames() - 1) {
        if(currAnimation->isLooping()) {
            currFrame = 0;
        } else {
            setAnimating(false);
        }
    }
}

void Animator::render(int x, int y, SDL_Renderer * renderer) const {
    currAnimation->render(x, y, currFrame, renderer);
}

void Animator::setAnimating(bool animating) {
    this->animating = animating;
}

bool Animator::isAnimating() const {
    return animating;
}

void Animator::setCurrAnimation(std::shared_ptr<Animation> currAnimation) {
    this->currAnimation = currAnimation;
}