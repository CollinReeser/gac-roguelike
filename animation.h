#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdint.h>

#include "entity.h"

enum AnimationType {
    DRAGON_FIRE,
    PROJECTILE
};

class Animation
{
public:
    Animation(
        AnimationType anim_type,
        const Entity* start, const Entity* end
    );
    ~Animation();

    AnimationType get_animation_type() const;
    int64_t get_start_x() const;
    int64_t get_start_y() const;
    int64_t get_end_x() const;
    int64_t get_end_y() const;

private:
    AnimationType anim_type;

    const Entity* start;
    const Entity* end;
};

#endif
