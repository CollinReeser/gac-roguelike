#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>
#include <vector>

#include "entity.h"
#include "creature.h"

struct SubEvent {
    virtual ~SubEvent();

    virtual void apply_event() = 0;
};

struct PoisonEvent : SubEvent {
    PoisonEvent(Creature* target, uint64_t damage);
    ~PoisonEvent();

    void apply_event();

    Creature* target;
    uint64_t damage;
};

class Event {
public:
    Event(uint64_t ticks, uint64_t speed);
    ~Event();

    bool is_turn(uint64_t clock_time);
    // Return true iff the event is exhausted and should be destroyed
    bool apply_event();

    void add_poison_effect(Creature* target, uint64_t damage);

private:
    uint64_t ticks;
    uint64_t speed;

    std::vector<SubEvent*> subevents;
};

#endif
