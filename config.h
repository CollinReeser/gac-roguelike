#ifndef CONFIG_H
#define CONFIG_H

#include "json/src/json.hpp"

class Config
{
public:
    Config();
    ~Config();

    void dump();

private:
    nlohmann::json creature_config;

};

#endif
