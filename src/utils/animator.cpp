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
/* 
    printf("current frame: %dout of %d\n", currFrame, currAnimation->getNumFrames());
    printf("delta: %.6f\n", delta);
    printf("ms from start %.6f\n", msFromStart); */

    // check if Animator has finished, stop unless looping
    if(currFrame > currAnimation->getNumFrames() - 1) {
        currFrame = 0;
        msFromStart = 0;
        if(!currAnimation->isLooping()) {
            setAnimating(false);
            currAnimation.reset();
        }
    }
}

void Animator::render(int x, int y, SDL_Renderer * renderer, double angle) const {
    currAnimation->render(x, y, currFrame, renderer, angle);
}

void Animator::setAnimating(bool animating) {
    this->animating = animating;
}

bool Animator::isAnimating() const {
    return animating;
}

float Animator::getMsFromStart() const {
    return msFromStart;
}

void Animator::setCurrAnimation(std::shared_ptr<Animation> currAnimation) {
    this->currAnimation = currAnimation;
}