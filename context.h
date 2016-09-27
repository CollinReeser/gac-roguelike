#ifndef CONTEXT_H
#define CONTEXT_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <stdexcept>
#include <string>

class Context
{
public:
    Context();
    ~Context();

private:
    ALLEGRO_DISPLAY* display;
    ALLEGRO_BITMAP* tileset;

    const std::string tileset_filename;

    const int DISP_WIDTH;
    const int DISP_HEIGHT;

    void init() throw (std::runtime_error);
};

#endif
