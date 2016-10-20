#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#include "json/src/json.hpp"

#include "creature.h"

class Config
{
public:
    Config();
    ~Config();

    void dump();

    Creature* load_creature(std::string name) const;
    Creature* load_random_creature() const;

    Item* load_item(std::string name, uint64_t quantity = 1) const;

private:
    nlohmann::json creature_config;
    nlohmann::json item_config;

};

#endif
