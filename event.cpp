#include "creature.h"
#include "display.h"

#include "event.h"

Event::Event(uint64_t ticks, uint64_t speed):
    ticks(ticks),
    speed(speed)
{
}

Event::~Event() {
    for (auto it = subevents.begin(); it != subevents.end(); it++) {
        delete *it;
    }
}

bool Event::is_turn(uint64_t clock_time) {
    if (clock_time % speed == 0) {
        return true;
    }

    return false;
}

bool Event::apply_event() {
    for (auto it = subevents.begin(); it != subevents.end(); it++) {
        (*it)->apply_event();
    }

    if (ticks > 0) {
        ticks--;
    }

    if (ticks == 0) {
        return true;
    }

    return false;
}

void Event::add_poison_effect(Creature* target, uint64_t damage) {
    subevents.push_back(
        new PoisonEvent(target, damage)
    );
}

SubEvent::~SubEvent() {}

PoisonEvent::PoisonEvent(Creature* target, uint64_t damage):
    target(target),
    damage(damage)
{
    target->increment_poison_counter();
}

PoisonEvent::~PoisonEvent() {
    target->decrement_poison_counter();
}

void PoisonEvent::apply_event() {
    if (target->get_poison_counter() > 0) {
        target->take_damage(damage);

        if (target->is_player()) {
            Display::add_event_message("You are hurt by poison.");
        }
    }
}
