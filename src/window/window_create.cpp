#include "window.hpp"

bool Window::create_window() {
    window = std::unique_ptr<SDL_Window, SDL_Deleter>(
        SDL_CreateWindow(name.c_str(), WINDOW_WIDTH, WINDOW_HEIGHT, 0),
        SDL_Deleter());
    if (window == nullptr) {
        log_sdl_error(log, "SDL_CreateWindow");
        return false;
    }
    return true;
}

bool Window::create_renderer() {
    renderer = std::unique_ptr<SDL_Renderer, SDL_Deleter>(
        SDL_CreateRenderer(window.get(), NULL), SDL_Deleter());
    if (renderer == nullptr) {
        log_sdl_error(log, "SDL_CreateRenderer");
        return false;
    }
    SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);
    return true;
}

bool Window::create_game_texture() {
    // create the games's texture at native resolution
    // using TEXTUREACCESS_STREAMING to edit individual pixels
    SDL_Texture* game_texture_ptr =
        SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_XRGB8888,
                          SDL_TEXTUREACCESS_STREAMING, GAME_WIDTH, GAME_HEIGHT);
    if (game_texture_ptr == nullptr) {
        log_sdl_error(log, "SDL_CreateTexture");
        return false;
    }
    game_texture = std::unique_ptr<SDL_Texture, SDL_Deleter>(game_texture_ptr,
                                                             SDL_Deleter());
    SDL_SetTextureScaleMode(game_texture.get(), SDL_SCALEMODE_NEAREST);
    SDL_SetTextureBlendMode(game_texture.get(), SDL_BLENDMODE_BLEND);
    return true;
}

bool Window::create_scanline_texture() {
    // create the scanline effect in a texture
    // using TEXTUREACCESS_TARGET to draw using the renderer
    SDL_Texture* scanline_texture_ptr = SDL_CreateTexture(
        renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        SCANLINE_WIDTH, SCANLINE_HEIGHT);
    if (scanline_texture_ptr == nullptr) {
        log_sdl_error(log, "SDL_CreateTexture");
        return false;
    }
    scanline_texture = std::unique_ptr<SDL_Texture, SDL_Deleter>(
        scanline_texture_ptr, SDL_Deleter());
    SDL_SetTextureScaleMode(scanline_texture.get(), SDL_SCALEMODE_NEAREST);
    SDL_SetTextureBlendMode(scanline_texture.get(), SDL_BLENDMODE_BLEND);

    // draw scanlines onto the texture using the renderer
    SDL_SetRenderTarget(renderer.get(), scanline_texture.get());
    SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0xFF, 0xFF, 0x00);
    SDL_RenderFillRect(renderer.get(), NULL);
    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0x20);
    // uint8_t s = 0;
    // uint8_t alpha = 0x20;
    for (uint32_t i = 0; i < SCANLINE_HEIGHT; i += 2) {
        // switch (s++) {
        // case 0:
        //     SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0, 0, alpha);
        //     break;
        // case 1:
        //     SDL_SetRenderDrawColor(renderer.get(), 0, 0xFF, 0, alpha);
        //     break;
        // default:
        //     SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0xFF, alpha);
        //     s = 0;
        //     break;
        // }
        SDL_RenderLine(renderer.get(), 0, i, SCANLINE_WIDTH, i);
    }
    // reset renderer target, now ready to draw frames
    SDL_SetRenderTarget(renderer.get(), NULL);
    return true;
}
