#include "display.h"
#include "chip8.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <endian.h>
#include <iostream>
#include <stdexcept>

Display::Display(Chip8 &chip8) {

  // uses pattern:
  // number of key in array
  //  1 2  3 12
  //  4 5  6 13
  //  7 8  9 14
  // 10 0 11 15
  configurable_keys = {SDLK_x, SDLK_1, SDLK_2, SDLK_3, SDLK_q, SDLK_w,
                       SDLK_e, SDLK_a, SDLK_s, SDLK_d, SDLK_z, SDLK_c,
                       SDLK_4, SDLK_r, SDLK_f, SDLK_v};

  pixel_data.fill(0); // fill pixel array with black

  // will try to initialize sdl
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cout << "Error: " << SDL_GetError() << std::endl;
    throw std::runtime_error("SDL initialization failed");
  }

  // will try to create a sdl2 window
  window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, chip8.GFX_WIDTH * 10,
                            chip8.GFX_HEIGHT * 10, SDL_WINDOW_OPENGL);
  if (!window) {
    std::cout << "Error: " << SDL_GetError() << std::endl;
    throw std::runtime_error("Could not create a window");
  }

  // will try to create a sdl2 render, necessary to render to the screen
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    std::cout << "Error: " << SDL_GetError() << std::endl;
    throw std::runtime_error("Could not create renderer");
  }

  // will try to create a sdl2 texture where we put our pixel array
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                              SDL_TEXTUREACCESS_STREAMING, chip8.GFX_WIDTH,
                              chip8.GFX_HEIGHT);
  if (SDL_LoadWAV("beep.wav", &wav_spec, &wav_buffer, &wav_length) == NULL) {
  }

  // 2. Abre o dispositivo baseado no próprio arquivo WAV
  audio_device = SDL_OpenAudioDevice(NULL, 0, &wav_spec, NULL, 0);
  if (audio_device == 0) {
    SDL_FreeWAV(wav_buffer);
  }

  // 3. Liga o dispositivo (deixa em prontidão)
  SDL_PauseAudioDevice(audio_device, 0);
}
Display::~Display() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyTexture(texture);
  SDL_DestroyWindow(window);
  SDL_Quit();
  if (audio_device != 0) {
    SDL_CloseAudioDevice(audio_device);
  }
  if (wav_buffer != nullptr) {
    SDL_FreeWAV(wav_buffer);
  }
}

void Display::update_screen(Chip8 &chip8) {
  if (!chip8.update_screen_flag) {
    return;
  }
  // converts my chip8 gfx array to a pixel data in rgba format for sdl2
  for (uint16_t i = 0, size = chip8.gfx.size(); i < size; i++) {
    pixel_data[i] = chip8.gfx[i] ? 0x33FF33FF : 0x0A140AFF;
  }
  // clear the renderer for the next screen
  SDL_RenderClear(renderer);

  // update the texture by using the pixel data of the screen, and cutting it in
  // rows every GFX_WIDTH * sizer o pixel uint32 in formtat 0xFFFFFFF
  SDL_UpdateTexture(texture, NULL, pixel_data.data(),
                    chip8.GFX_WIDTH * sizeof(uint32_t));

  // copy the texture into the render, the null bellow is to use whole texture
  // mapped to whole render
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  // render the preser renderer to the screen
  SDL_RenderPresent(renderer);
  chip8.update_screen_flag = 0;
}

void Display::handle_events(Chip8 &chip8) {
  while (SDL_PollEvent(&event) != 0) {
    switch (event.type) {
    case SDL_QUIT:
      chip8.running_flag = 0;
      break;
    case SDL_KEYDOWN: {
      SDL_Keycode key = event.key.keysym.sym;

      for (uint8_t i = 0; i < 16; i++) {
        if (key == configurable_keys[i]) {

          chip8.keypad[i] = 1;
        }
      }
      break;
    }
    case SDL_KEYUP: {
      SDL_Keycode key = event.key.keysym.sym;
      for (uint8_t i = 0; i < 16; i++) {
        if (key == configurable_keys[i]) {
          chip8.keypad[i] = 0;
          if (chip8.keywaiting == 1) {
            chip8.key_up_hex = i;
          }
        }
      }
      break;
    } break;
    }
  }
}

// Método para chamar no seu loop principal da Main
void Display::update_audio(uint8_t sound_timer) {
  if (audio_device == 0)
    return; // Se o áudio falhou ao abrir, não faz nada

  if (sound_timer > 0) {
    // Se o cronômetro estiver ativo e a fila de som esvaziou, joga o bip de
    // novo
    if (SDL_GetQueuedAudioSize(audio_device) == 0) {
      SDL_QueueAudio(audio_device, wav_buffer, wav_length);
    }
  } else {
    // Se o timer zerou, limpa a fila para cortar o som imediatamente
    SDL_ClearQueuedAudio(audio_device);
  }
}
