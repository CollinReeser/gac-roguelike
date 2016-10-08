#include <fstream>
#include <iostream>
#include <sstream>

#include "config.h"

#include "json/src/json.hpp"

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

Config::~Config() {

}
