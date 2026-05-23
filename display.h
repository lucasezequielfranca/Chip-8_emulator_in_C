#pragma once

#include "chip8.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <array>
#include <cstdint>

class Display {
public:
  Display();
  ~Display();
  void update_screen(Chip8 &chip8);

private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  inline static uint16_t SCREEN_WIDTH = 640;
  inline static int16_t SCREEN_HEIGHT = 320;
  std::array<uint32_t, 64 * 32> pixel_data;
};
