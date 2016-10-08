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

    Creature* load_creature(std::string name);

private:
    nlohmann::json creature_config;

};

#endif
