#pragma once
#include <array>
#define CHIP8

#include <cstdint>

class Chip8 {
public:
  Chip8();
  ~Chip8();
  void initialize();
  uint8_t load_rom(char *filename);
  void execute_cycle();
  void decrease_timers();

  inline static constexpr uint8_t GFX_WIDTH = 64;
  inline static constexpr uint8_t GFX_HEIGHT = 32;
  std::array<uint8_t, GFX_WIDTH * GFX_HEIGHT> gfx; // display array
  uint8_t sound_timer;
  uint8_t update_screen_flag; // flag to update screen
  uint16_t instructions_per_second;
  uint8_t running_flag;
  uint8_t basic_emulator_flag;
  uint8_t amiga_quirck;
  std::array<uint8_t, 16> keypad;
  uint8_t keywaiting;
  uint16_t key_up_hex;
  uint8_t debug_mode;
  uint16_t step;

private:
  std::array<uint8_t, 4096> memory;
  uint16_t pc;
  uint16_t i;
  std::array<uint16_t, 16> stack;
  uint8_t stack_pointer;
  std::array<uint8_t, 16> v_register;
  uint8_t delay_timer;

  // opcode pointers for deconding
  uint16_t opcode;
  uint8_t type;
  uint8_t X;
  uint8_t Y;
  uint8_t N;
  uint8_t NN;
  uint16_t NNN;

  inline static std::array<uint8_t, 80> FONT_SET = {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C 0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                                    // 0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };
};
