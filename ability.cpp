#include <sstream>
#include <stdint.h>
#include <string>
#include <unordered_map>

#include "ability.h"

#include "json/src/json.hpp"

#include "creature.h"
#include "event.h"

Ability::Ability(std::string name, uint64_t chance, nlohmann::json config):
    name(name),
    chance(chance),
    config(config)
{

}

Event* Ability::spawn_event(Creature* target) const {
    auto event = new Event(config["ticks"], config["speed"]);

    if (name.compare("poison") == 0) {
        event->add_poison_effect(target, config["damage"]);
    }

    return event;
}

Ability::~Ability() {

}
