
#include <kit.h>
#include "boot.h"

int main(int argc, char* argv[])
{
    Queue* msgqueue = queue_create(20);

    random_init(23);

    kit_init(msgqueue);

    // Open the display
    u8 dispidx = display_new(0, 240, 160, DISPLAY_FIXEDWINDOW);

    // Enable bitmap mode
    display_mode_tilemap(dispidx, 4, 8);

    // Run the game engine
    kit_execute(&bootstage);

    // Close the display
    display_close(dispidx);

    kit_shutdown();

    return 0;
}

