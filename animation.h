#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdint.h>

enum AnimationType {
    DRAGON_FIRE,
    PROJECTILE
};

class Animation
{
public:
    Animation(
        AnimationType anim_type,
        int64_t src_x, int64_t src_y, int64_t target_x, int64_t target_y
    );
    ~Animation();

    AnimationType get_animation_type() const;

private:
    AnimationType anim_type;

    int64_t src_x;
    int64_t src_y;
    int64_t target_x;
    int64_t target_y;
};

#endif
