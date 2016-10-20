#ifndef ENTITY_H
#define ENTITY_H

#include <allegro5/allegro.h>

#include <stdint.h>
#include <string>
#include <vector>

class Entity
{
public:
    Entity(
        char symbol, std::string name,
        int64_t pos_x, int64_t pos_y,
        bool passable
    );
    Entity(
        char symbol, std::string name, ALLEGRO_COLOR color,
        int64_t pos_x, int64_t pos_y,
        bool passable
    );
    ~Entity();

    ALLEGRO_COLOR get_color() const;
    std::string get_name() const;
    char get_symbol() const;
    bool is_passable() const;
    int64_t get_x() const;
    int64_t get_y() const;
    void set_position(int64_t x, int64_t y);
    void set_x(int64_t x);
    void set_y(int64_t y);

private:
    char symbol;
    std::string name;
    ALLEGRO_COLOR color;

    int64_t pos_x;
    int64_t pos_y;

    bool passable;

};

#endif
