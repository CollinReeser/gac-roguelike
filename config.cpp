#include <sstream>
#include <fstream>

#include "config.h"

#include "json/src/json.hpp"

using json = nlohmann::json;

Config::Config() {
    std::ifstream in("creature.cfg", std::ifstream::in);

    std::stringstream sstr;
    sstr << in.rdbuf();

    auto j3 = json::parse(sstr.str());
}

Config::~Config() {

}
