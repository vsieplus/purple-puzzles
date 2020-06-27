// class for handling the playback of animations

#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP

#include "utils/animation.hpp"

class Animator {
    public:
        Animator();

        void start();

        void update(float delta);
        void render(int x, int y, SDL_Renderer * renderer);

        void setAnimating(bool animating);
        bool isAnimating() const;

        void setCurrAnimation(std::shared_ptr<Animation> currAnimation);

    private:
        // track the animation progress
        bool animating = false;

        int currFrame = 0;
        
        float msFromStart = 0;

        std::shared_ptr<Animation> currAnimation;

};

#endif // ANIMATOR_HPP