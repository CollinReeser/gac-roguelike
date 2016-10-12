#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <map>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>

#include "animation.h"
#include "creature.h"
#include "display.h"
#include "dungeon.h"

Display::Display():
    display(NULL),
    tileset(NULL),
    tileset_filename("tileset.png"),
    disp_width(640),
    disp_height(480),
    dungeon_draw_width(50),
    dungeon_draw_height(20),
    tile_width(8),
    tile_height(16),
    tileset_height(8),
    tileset_width(32),
    disp_tile_width(disp_width / tile_width),
    disp_tile_height(disp_height / tile_height)
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
    for (int i = 0; i < tileset_height; i++) {
        for (int j = 0; j < tileset_width; j++)
        {
            tilemap[i * tileset_width + j] = al_create_sub_bitmap(
                tileset,
                j * tile_width, i * tile_height,
                tile_width, tile_height
            );
        }
    }

    tilemap[' '] = al_create_sub_bitmap(tileset, 0, 0, tile_width, tile_height);
}

void Display::draw_creatures(
    int start_x, int start_y, int end_x, int end_y,
    std::vector<Creature*>::const_iterator creatures_it_begin,
    std::vector<Creature*>::const_iterator creatures_it_end
) {
    // If a given entity shows up within the dungeon display window, draw it
    for (; creatures_it_begin != creatures_it_end; ++creatures_it_begin)
    {
        if (
            (*creatures_it_begin)->get_x() >= start_x &&
            (*creatures_it_begin)->get_x() < end_x &&
            (*creatures_it_begin)->get_y() >= start_y &&
            (*creatures_it_begin)->get_y() < end_y
        ) {
            al_draw_tinted_bitmap_region(
                tilemap[(*creatures_it_begin)->get_symbol()],
                (*creatures_it_begin)->get_color(),
                0, 0,
                tile_width, tile_height,
                ((*creatures_it_begin)->get_x() - start_x) * tile_width,
                ((*creatures_it_begin)->get_y() - start_y) * tile_height,
                0
            );
        }
    }
}

void Display::draw_dungeon_floor(
    int start_x, int start_y, int end_x, int end_y,
    const Dungeon* dungeon
) {
    // Draw the designated "window" into the dungeon
    for (int y = start_y; y < end_y; y++)
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
}

void Display::draw_animation(
    int start_x, int start_y, int end_x, int end_y,
    const Animation* animation
) {
    switch (animation->get_animation_type()) {
    case DRAGON_FIRE:
        break;
    case PROJECTILE:
        break;
    default:
        break;
    }
}

void Display::draw_dungeon(
    int center_x, int center_y,
    const Dungeon* dungeon,
    std::vector<Creature*>::const_iterator creatures_it_begin,
    std::vector<Creature*>::const_iterator creatures_it_end,
    std::vector<Animation*> animations
) {
    // Defer actually "rendering" any drawing we do until this is disabled.
    // Gives a significant performance boost.
    al_hold_bitmap_drawing(true);

    int start_x = center_x - (dungeon_draw_width / 2);
    int start_y = center_y - (dungeon_draw_height / 2);
    int end_x = center_x + (dungeon_draw_width / 2);
    int end_y = center_y + (dungeon_draw_height / 2);

    draw_dungeon_floor(start_x, start_y, end_x, end_y, dungeon);

    draw_creatures(
        start_x, start_y, end_x, end_y,
        creatures_it_begin, creatures_it_end
    );

    for (auto it = animations.cbegin(); it != animations.cend(); it++) {
        draw_animation(start_x, start_y, end_x, end_y, *it);

        clear_dungeon();

        // Draw the buffer we've been building to the user-visible display
        al_flip_display();

        draw_dungeon_floor(start_x, start_y, end_x, end_y, dungeon);

        draw_creatures(
            start_x, start_y, end_x, end_y,
            creatures_it_begin, creatures_it_end
        );
    }

    // Re-enable drawing, so that all the work we did above can be batched.
    // We've essentially queued up a bunch of drawing work, to be done all at
    // once.
    al_hold_bitmap_drawing(false);
}

void Display::clear_dungeon()
{
    al_draw_scaled_bitmap(
        tilemap[' '],
        0, 0,
        tile_width, tile_height,
        0, 0,
        dungeon_draw_width * tile_width, dungeon_draw_height * tile_height,
        0
    );
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

void Display::draw_borders()
{
    // Try drawing a border
    al_hold_bitmap_drawing(true);

    // Bottom line
    for (int i = 0; i < dungeon_draw_width; i++) {
        al_draw_tinted_bitmap_region(
            tilemap[196], // '─'
            al_map_rgba(255, 255, 255, 255),
            0, 0,
            tile_width, tile_height,
            i * tile_width, dungeon_draw_height * tile_height,
            0
        );
    }

    // Right side line
    for (int i = 0; i < dungeon_draw_height; i++) {
        al_draw_tinted_bitmap_region(
            tilemap[179], // '│'
            al_map_rgba(255, 255, 255, 255),
            0, 0,
            tile_width, tile_height,
            dungeon_draw_width * tile_width, i * tile_height,
            0
        );
    }

    // Draw corner piece
    al_draw_tinted_bitmap_region(
        tilemap[217], // '┘'
        al_map_rgba(255, 255, 255, 255),
        0, 0,
        tile_width, tile_height,
        dungeon_draw_width * tile_width, dungeon_draw_height * tile_height,
        0
    );


    al_hold_bitmap_drawing(false);

}

void Display::update_screen()
{
    // Draw the buffer we've been building to the user-visible display
    al_flip_display();

    // Clear the buffer, to start from scratch on the next draw sequence
    al_clear_to_color(al_map_rgb(0,0,0));
}

void Display::draw_game_over() {
    al_clear_to_color(al_map_rgb(0,0,0));

    std::string game_over_str("YOU DIED");

    draw_string(
        disp_tile_width / 2 - game_over_str.size() / 2,
        disp_tile_height / 2 - 1,
        game_over_str
    );

    al_flip_display();

    al_rest(3);
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
