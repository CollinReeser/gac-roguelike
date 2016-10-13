#include <stdint.h>

#include "animation.h"
#include "entity.h"

Animation::Animation(
    AnimationType anim_type,
    const Entity* start, const Entity* end
):
    anim_type(anim_type),
    start(start),
    end(end)
{
}

AnimationType Animation::get_animation_type() const {
    return anim_type;
}

int64_t Animation::get_start_x() const {
    return start->get_x();
}

int64_t Animation::get_start_y() const {
    return start->get_y();
}

int64_t Animation::get_end_x() const {
    return end->get_x();
}

int64_t Animation::get_end_y() const {
    return end->get_y();
}

Animation::~Animation() {

}
