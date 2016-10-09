#include <stdint.h>

#include "animation.h"

Animation::Animation(
    AnimationType anim_type,
    int64_t src_x, int64_t src_y, int64_t target_x, int64_t target_y
):
    anim_type(anim_type),
    src_x(src_x),
    src_y(src_y),
    target_x(target_x),
    target_y(target_y)
{
}

AnimationType Animation::get_animation_type() const {
    return anim_type;
}

Animation::~Animation() {

}
