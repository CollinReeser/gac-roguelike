#ifndef DISPLAY_H
#define DISPLAY_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <unordered_map>
#include <stdexcept>
#include <stdint.h>
#include <list>
#include <string>
#include <vector>

#include "animation.h"
#include "creature.h"
#include "dungeon.h"
#include "entity.h"

class Display
{
public:
    static void add_event_message(std::string msg);

    Display();
    ~Display();

    void draw_animation(
        int center_x, int center_y,
        const Dungeon* dungeon,
        std::list<Creature*>::const_iterator creatures_it_begin,
        std::list<Creature*>::const_iterator creatures_it_end,
        Animation* animation
    );

    void draw_dungeon(
        int center_x, int center_y,
        const Dungeon* dungeon,
        std::list<Creature*>::const_iterator creatures_it_begin,
        std::list<Creature*>::const_iterator creatures_it_end
    );
    void draw_borders();
    void draw_event_messages();
    void draw_game_over();

    void draw_basic_screen(
        int center_x, int center_y,
        const Dungeon* dungeon,
        std::list<Creature*>::const_iterator creatures_it_begin,
        std::list<Creature*>::const_iterator creatures_it_end
    );

    void update_screen();
    void clear_screen();

    ALLEGRO_DISPLAY* display;

private:
    static std::vector<std::string> event_messages;

    ALLEGRO_BITMAP* tileset;

    std::unordered_map<uint8_t, ALLEGRO_BITMAP*> tilemap;

    const std::string tileset_filename;

    uint64_t disp_width;
    uint64_t disp_height;

    uint64_t dungeon_draw_width;
    uint64_t dungeon_draw_height;

    uint64_t tile_width;
    uint64_t tile_height;
    uint64_t tileset_height;
    uint64_t tileset_width;

    uint64_t disp_tile_width;
    uint64_t disp_tile_height;

    void init() throw (std::runtime_error);
    void init_tilemap();

    void draw_string(int x, int y, std::string str);

    void clear_dungeon();

    void draw_creatures(
        int start_x, int start_y, int end_x, int end_y,
        std::list<Creature*>::const_iterator creatures_it_begin,
        std::list<Creature*>::const_iterator creatures_it_end
    );

    void draw_dungeon_floor(
        int start_x, int start_y, int end_x, int end_y,
        const Dungeon* dungeon
    );

    void draw_projectile_animation(
        int center_x, int center_y,
        const Dungeon* dungeon,
        std::list<Creature*>::const_iterator creatures_it_begin,
        std::list<Creature*>::const_iterator creatures_it_end,
        Animation* animation
    );
};

#endif
