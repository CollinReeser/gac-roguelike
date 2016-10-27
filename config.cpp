#include <allegro5/allegro.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <unordered_map>

#include "config.h"

#include "json/src/json.hpp"

#include "ability.h"
#include "creature.h"
#include "item.h"

Config::Config() {
    std::ifstream creature_in("creature.cfg", std::ifstream::in);

    std::stringstream creature_stream;
    creature_stream << creature_in.rdbuf();

    creature_config = nlohmann::json::parse(creature_stream.str());

    std::ifstream item_in("item.cfg", std::ifstream::in);

    std::stringstream item_stream;
    item_stream << item_in.rdbuf();

    item_config = nlohmann::json::parse(item_stream.str());
}

void Config::dump() {
    std::cout << creature_config.dump(2) << std::endl;
}

Creature* Config::load_creature(std::string name) const {
    nlohmann::json creature = creature_config[name];

    std::string symbol_str = creature["symbol"];
    char symbol = symbol_str.at(0);

    uint64_t max_health = creature["max_health"];

    uint64_t speed = creature["speed"];

    ALLEGRO_COLOR color = al_map_rgb(255, 255, 255);

    if (creature.find("color") != creature.end()) {
        auto color_values = creature["color"];
        color = al_map_rgb(color_values[0], color_values[1], color_values[2]);
    }

    uint64_t strength = 1;
    if (creature.find("strength") != creature.end()) {
        strength = creature["strength"];
    }

    uint64_t dexterity = 1;
    if (creature.find("dexterity") != creature.end()) {
        dexterity = creature["dexterity"];
    }

    bool can_throw = false;
    if (creature.find("can_throw") != creature.end()) {
        can_throw = creature["can_throw"];
    }

    std::unordered_map<char, Item*> items;
    char item_key = 'a';
    if (creature.find("starts_with") != creature.end()) {
        auto starts_with = creature["starts_with"];

        for (auto it = starts_with.begin(); it != starts_with.end(); it++) {
            if (item_config.find(it.key()) != item_config.end()) {
                auto item = load_item(it.key(), it.value());

                items.insert({item_key, item});
            }
        }
    }

    std::vector<Ability*> on_melee_attack_abilities;
    if (creature.find("on_melee_attack") != creature.end()) {
        auto on_melee_attack = creature["on_melee_attack"];

        for (
            auto it = on_melee_attack.begin();
            it != on_melee_attack.end();
            it++
        ) {
            on_melee_attack_abilities.push_back(
                new Ability(
                    it.key(), on_melee_attack[it.key()]["chance"],
                    on_melee_attack[it.key()]["config"]
                )
            );
        }
    }

    auto creature_obj = new Creature(
        symbol, name, color,
        false, false, false,
        speed, max_health,
        strength, dexterity,
        can_throw,
        items,
        on_melee_attack_abilities
    );

    if (creature.find("unarmed_attack_weapon") != creature.end()) {
        creature_obj->set_unarmed_attack_weapon(
            creature["unarmed_attack_weapon"]
        );
    }

    if (creature.find("unarmed_attack_flavor") != creature.end()) {
        creature_obj->set_unarmed_attack_flavor(
            creature["unarmed_attack_flavor"]
        );
    }

    return creature_obj;
}

Creature* Config::load_random_creature() const {
    auto i = 0;
    auto index = rand() % creature_config.size();

    for (auto it = creature_config.begin(); ; it++, i++) {
        if (i == index) {
            return load_creature(it.key());
        }
    }

    return NULL;
}

Item* Config::load_item(std::string name, uint64_t quantity) const {
    nlohmann::json item = item_config[name];

    std::string symbol_str = item["symbol"];
    char symbol = symbol_str.at(0);

    ALLEGRO_COLOR color = al_map_rgb(255, 255, 255);
    if (item.find("color") != item.end()) {
        auto color_values = item["color"];
        color = al_map_rgb(color_values[0], color_values[1], color_values[2]);
    }

    bool can_stack = false;
    if (item.find("can_stack") != item.end()) {
        can_stack = item["can_stack"];
    }

    uint64_t throw_priority = 0;
    if (item.find("throw_priority") != item.end()) {
        throw_priority = item["throw_priority"];
    }

    uint64_t weight = 0;
    if (item.find("weight") != item.end()) {
        weight = item["weight"];
    }

    return new Item(
        symbol, name, color,
        -1, -1,
        can_stack, quantity,
        throw_priority, weight
    );
}

Config::~Config() {

}
