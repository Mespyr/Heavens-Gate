#include <SDL2/SDL_render.h>
#include <cstdint>
#include <iostream>
#include <SDL2/SDL.h>

#include "include/util.h"
#include "include/engine.h"
#include "include/object.h"
#include "include/vec2.h"

int32_t main() {
	if (init_lib() != 0)
		return 1;

	////////////////// initialize engine and textures ///////////////////
	Engine engine;
	if (engine.init_window() != 0)
		return 1;

	SDL_Texture* player_texture = engine.load_texture("assets/player.png");
	SDL_Texture* crosshair_texture = engine.load_texture("assets/crosshair.png");
	SDL_Texture* bg_texture    = engine.load_texture("assets/tileset.png");
	engine.init_textures(player_texture, crosshair_texture);
	engine.set_background_texture(bg_texture);
	/////////////////////////////////////////////////////////////////////

	engine.player.player_object.position = {100, 40.5}; // temporarily set player position
	
	uint32_t now, last_game_step = SDL_GetTicks();
	float_t delta_time;

	// game loop
	while (engine.running()) {
		now = SDL_GetTicks();
		engine.handle_events();
		if (last_game_step < now) {
			delta_time = (now - last_game_step) / 1000.0f;
			engine.update(delta_time);
			engine.render_scene();
		}
		else SDL_Delay(1);
		last_game_step = now;
	}

	SDL_DestroyTexture(player_texture);
	SDL_DestroyTexture(crosshair_texture);
	SDL_DestroyTexture(bg_texture);
	SDL_Quit();
	return 0;
}
