#include <fstream>
#include <iostream>
#include <sstream>
#include <stdint.h>

#include "config.h"

#include "json/src/json.hpp"

#include "creature.h"

using json = nlohmann::json;

Config::Config() {
    std::ifstream in("creature.cfg", std::ifstream::in);

    std::stringstream sstr;
    sstr << in.rdbuf();

    creature_config = json::parse(sstr.str());
}

void Config::dump() {
    std::cout << creature_config.dump(2) << std::endl;
}

Creature* Config::load_creature(std::string name) {
    json cfg = creature_config[name];

    std::string symbol_str = cfg["symbol"];
    char symbol = symbol_str.at(0);

    uint64_t max_health = cfg["max_health"];

    uint64_t speed = cfg["speed"];

    return new Creature(
        symbol, name, false, false, false, speed, max_health
    );
}

Creature* Config::load_random_creature() {
    auto i = 0;
    auto index = rand() % creature_config.size();

    for (auto it = creature_config.begin(); ; it++, i++) {
        if (i == index) {
            return load_creature(it.key());
        }
    }

    return NULL;
}

Config::~Config() {

}
