#ifndef DISPLAY_H
#define DISPLAY_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <unordered_map>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>

#include "dungeon.h"
#include "entity.h"

class Display
{
public:
    Display();
    ~Display();

    void draw_dungeon(
        int center_x, int center_y,
        const Dungeon* dungeon, const std::vector<const Entity*> entities
    );
    void draw_event_messages(std::vector<std::string> messages);

    ALLEGRO_DISPLAY* display;

private:
    ALLEGRO_BITMAP* tileset;

    std::unordered_map<char, ALLEGRO_BITMAP*> tilemap;

    const std::string tileset_filename;

    uint64_t disp_width;
    uint64_t disp_height;

    int dungeon_draw_height;
    int dungeon_draw_width;

    int tile_width;
    int tile_height;
    int tileset_width;
    int tileset_height;

    void init() throw (std::runtime_error);
    void init_tilemap();

    void draw_string(int x, int y, std::string str);
};

#endif
