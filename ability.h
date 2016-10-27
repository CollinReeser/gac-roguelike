#ifndef ABILITY_H
#define ABILITY_H

class Ability;

#include <stdint.h>
#include <string>

#include "json/src/json.hpp"

#include "creature.h"
#include "event.h"

class Ability
{
public:
    Ability(std::string name, uint64_t chance, nlohmann::json config);
    ~Ability();

    Event* spawn_event(Creature* target) const;

    const std::string name;
    const uint64_t chance;

private:
    const nlohmann::json config;
};

#endif
