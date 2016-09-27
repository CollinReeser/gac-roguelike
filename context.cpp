#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "context.h"

Context::Context():
    display(NULL),
    tileset(NULL),
    DISP_WIDTH(640),
    DISP_HEIGHT(480),
    tileset_filename("tileset.png")
{
    init();
}

void Context::init() throw (std::runtime_error)
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
}

Context::~Context() {
    al_destroy_bitmap(tileset);
    al_destroy_display(display);
}
