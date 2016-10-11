#ifndef DISPLAY_H
#define DISPLAY_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <unordered_map>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>

#include "animation.h"
#include "creature.h"
#include "dungeon.h"
#include "entity.h"

class Display
{
public:
    Display();
    ~Display();

    void draw_dungeon(
        int center_x, int center_y,
        const Dungeon* dungeon,
        std::vector<Creature*>::const_iterator creatures_it_begin,
        std::vector<Creature*>::const_iterator creatures_it_end,
        std::vector<Animation*> animations
    );
    void draw_borders();
    void draw_event_messages(std::vector<std::string> messages);
    void draw_game_over();
    void update_screen();

    ALLEGRO_DISPLAY* display;

private:
    ALLEGRO_BITMAP* tileset;

    std::unordered_map<char, ALLEGRO_BITMAP*> tilemap;

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

    void draw_animation(
        int start_x, int start_y, int end_x, int end_y,
        const Animation* animation
    );

    void clear_dungeon();

    void draw_creatures(
        int start_x, int start_y, int end_x, int end_y,
        std::vector<Creature*>::const_iterator creatures_it_begin,
        std::vector<Creature*>::const_iterator creatures_it_end
    );

    void draw_dungeon_floor(
        int start_x, int start_y, int end_x, int end_y,
        const Dungeon* dungeon
    );
};

#endif
