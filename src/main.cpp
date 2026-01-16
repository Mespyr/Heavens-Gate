#include <SDL3/SDL.h>

#include <cstdint>
#include <iostream>
#include <ostream>

#include "palette.hpp"
#include "util.hpp"
#include "window/window.hpp"

// clang-format off
uint8_t cross[6 * 10] =
	{
		0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0,
		1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1,
		0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0
	};
// clang-format on

int32_t main() {
    std::ostream& log = std::cerr;

    if (!init_lib(log)) exit(1);

    Palette _default(0x0e0e0e, 0x404040, 0x696969, 0x808080, 0xA9A9A9, 0xF0F0F0,
                     0x800000, 0xFF0000, 0xFF8C00, 0xFFFF00, 0xADFF2F, 0x008000,
                     0x008080, 0x00FFFF, 0x0000FF, 0x4B0082, 0x800080, 0xFF1493,
                     0xFF69B4, 0xFA8072);

    Window window(log, "Heaven's Gate", 1280, 720, _default);
    if (!window) exit(1);

    const uint64_t ticks_per_15_fps = (1000 / 15);
    uint64_t       ticks_now, ticks_last_frame = SDL_GetTicks();

    // position of cube
    int32_t x_pos = 20, y_pos = 20;

    do {
        window.handle_events();
        ticks_now = SDL_GetTicks();
        if ((ticks_now - ticks_last_frame) >= ticks_per_15_fps) {
            window.clear(Palette::BLACK);

			for (uint32_t x = 0; x <= 10; x++) {
				window.set(x + x_pos, y_pos, Palette::ORANGE);
				window.set(x + x_pos, y_pos + 10, Palette::ORANGE);
			}
			for (uint32_t y = 0; y <= 10; y++) {
				window.set(x_pos, y_pos + y, Palette::ORANGE);
				window.set(x_pos + 10, y_pos + y, Palette::ORANGE);
			}

            if (y_pos++ >= 180) y_pos = -10;
            if ((x_pos++) >= 320) x_pos = -10;

            window.draw();
            ticks_last_frame = ticks_now;
        }
    } while (window);

    SDL_Quit();
    return 0;
}
