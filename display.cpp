#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <map>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>

#include "display.h"
#include "dungeon.h"
#include "entity.h"

Display::Display():
    display(NULL),
    tileset(NULL),
    tileset_filename("tileset.png"),
    dungeon_draw_width(50),
    dungeon_draw_height(20),
    disp_width(640),
    disp_height(480),
    tile_width(8),
    tile_height(16),
    tileset_width(32),
    tileset_height(9)
{
    init();
}

void Display::init() throw (std::runtime_error)
{
    if(!al_init()) {
        throw std::runtime_error("Failed to initialize allegro.");
    }

    if (!al_init_image_addon()) {
        throw std::runtime_error("Failed to initialize image addon.");
    }

    tileset = al_load_bitmap(tileset_filename.c_str());
    if (!tileset) {
        throw std::runtime_error("Failed to load tileset.");
    }

    display = al_create_display(disp_width, disp_height);
    if(!display) {
        throw std::runtime_error("Failed to create display.");
    }

    al_install_keyboard();

    init_tilemap();
}

void Display::init_tilemap()
{
    uint64_t tileset_char_width = 32;
    uint64_t tileset_char_height = 9;

    // These rows match rows in the current tileset file, with any extraneous
    // spaces being placeholders for special characters.
    std::vector<std::string> char_rows = {
        " !\"#$%&'()*+,-./0123456789:;<=>?",
        "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_",
        "`abcdefghijklmnopqrstuvwxyz{|}~ "
    };
    uint64_t start_row_tileset = 1;

    for (int i = 0; i < char_rows.size(); i++) {
        for (int j = 0; j < tileset_char_width; j++)
        {
            tilemap[char_rows[i][j]] = al_create_sub_bitmap(
                tileset,
                j * tile_width, (start_row_tileset + i) * tile_height,
                tile_width, tile_height
            );
        }
    }

    tilemap[' '] = al_create_sub_bitmap(tileset, 0, 0, tile_width, tile_height);
}

void Display::draw_dungeon(
    int center_x, int center_y,
    const Dungeon* dungeon, const std::vector<const Entity*> entities
) {
    al_clear_to_color(al_map_rgb_f(0, 0, 0));

    // Defer actually "rendering" any drawing we do until this is disabled.
    // Gives a significant performance boost.
    al_hold_bitmap_drawing(true);

    int start_y = center_y - (dungeon_draw_height / 2);
    int start_x = center_x - (dungeon_draw_width / 2);
    int end_x = center_x + (dungeon_draw_width / 2);
    int end_y = center_y + (dungeon_draw_height / 2);

    // Draw the designated "window" into the dungeon
    for (int y = center_y - (dungeon_draw_height / 2); y < end_y; y++)
    {
        for (int x = start_x; x < end_x; x++)
        {
            ALLEGRO_BITMAP* tile;
            if (
                y >= 0 && y < dungeon->get_height() &&
                x >= 0 && x < dungeon->get_width())
            {
                tile = tilemap[dungeon->entity_at_index(x, y).get_symbol()];
            }
            else
            {
                tile = tilemap[' '];
            }

            al_draw_tinted_bitmap_region(
                tile,
                al_map_rgba(255, 255, 255, 255),
                0, 0,
                tile_width, tile_height,
                (x - start_x) * tile_width, (y - start_y) * tile_height,
                0
            );
        }
    }

    // If a given entity shows up within the dungeon display window, draw it
    for (auto it = entities.begin(); it != entities.end(); ++it)
    {
        if (
            (*it)->get_x() >= start_x && (*it)->get_x() <= end_x &&
            (*it)->get_y() >= start_y && (*it)->get_y() <= end_y
        ) {
            al_draw_tinted_bitmap_region(
                tilemap[(*it)->get_symbol()],
                al_map_rgba(255, 255, 255, 255),
                0, 0,
                tile_width, tile_height,
                ((*it)->get_x() - start_x) * tile_width, ((*it)->get_y() - start_y) * tile_height,
                0
            );
        }
    }

    // Re-enable drawing, so that all the work we did above can be batched.
    // We've essentially queued up a bunch of drawing work, to be done all at
    // once.
    al_hold_bitmap_drawing(false);
}

void Display::draw_string(int x, int y, std::string str)
{
    for (int i = 0; i < str.size(); i++)
    {
        al_draw_tinted_bitmap_region(
            tilemap[str[i]],
            al_map_rgba(255, 255, 255, 255),
            0, 0,
            tile_width, tile_height,
            (x + i) * tile_width, y * tile_height,
            0
        );
    }
}

void Display::draw_event_messages(std::vector<std::string> messages)
{
    int start_y = dungeon_draw_height + 1;
    int end_y = disp_height / tile_height - 1;

    int y = end_y;
    auto rit = messages.rbegin();

    al_hold_bitmap_drawing(true);

    for ( ; y >= start_y && rit != messages.rend(); y--, rit++) {
        draw_string(0, y, *rit);
    }

    al_hold_bitmap_drawing(false);
}

void Display::update_screen()
{
    al_flip_display();
}

Display::~Display()
{
    for (auto it = tilemap.begin(); it != tilemap.end(); ++it)
    {
        al_destroy_bitmap(it->second);
    }

    al_destroy_bitmap(tileset);
    al_destroy_display(display);
}
