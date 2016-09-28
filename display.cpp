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
    tileset_filename("tileset.png")
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

    display = al_create_display(DISP_WIDTH, DISP_HEIGHT);
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
    uint64_t tile_width = 8;
    uint64_t tile_height = 16;

    std::vector<std::string> char_rows = {
        " !\"#$%&'()*+,-./0123456789:;<=>?",
        "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_",
        "`abcdefghijklmnopqrstuvwxyz{|}~ "
    };
    uint64_t start_row_tileset = 1;

    for (int i = 0; i < char_rows.size(); i++) {
        for (int j = 0; j < tileset_char_width; j++)
        {
            printf("i: %d, j: %d, c: %c\n", i, j, char_rows[i][j]);

            tilemap[char_rows[i][j]] = al_create_sub_bitmap(
                tileset,
                j * tile_width, (start_row_tileset + i) * tile_height,
                8, 16
            );
        }
    }

    tilemap[' '] = al_create_sub_bitmap(tileset, 0, 0, 8, 16);
}

void Display::draw_dungeon(
    int width, int height, int center_x, int center_y,
    const Dungeon* dungeon, const std::vector<const Entity*> entities
) {
    al_clear_to_color(al_map_rgb_f(0, 0, 0));

    al_hold_bitmap_drawing(true);

    int start_y = center_y - (height / 2);
    int start_x = center_x - (width / 2);
    int end_x = center_x + (width / 2);
    int end_y = center_y + (height / 2);

    for (int y = center_y - (height / 2); y < end_y; y++)
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
                8, 16,
                (x - start_x) * 8, (y - start_y) * 16,
                0
            );
        }
    }

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
                8, 16,
                ((*it)->get_x() - start_x) * 8, ((*it)->get_y() - start_y) * 16,
                0
            );
        }
    }

    al_hold_bitmap_drawing(false);

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
