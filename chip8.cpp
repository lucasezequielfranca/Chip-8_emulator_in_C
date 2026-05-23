#include "chip8.h"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <ostream>
#include <sys/types.h>

Chip8::Chip8() {
  memory.fill(0);
  pc = 0;
  i = 0;
  stack.fill(0);
  v_register.fill(0);
  delay_timer = 0;
  sound_timer = 0;
  gfx.fill(0);
  update_screen_flag = 0;
}
Chip8::~Chip8() {}

void Chip8::initialize() {
  pc = 0x200;
  std::copy(std::begin(FONT_SET), std::end(FONT_SET), &memory[0x50]);

  std::cout << "Interpreter initialized with sucess" << std::endl;
}

uint8_t Chip8::load_rom(char *filename) {
  std::ifstream rom(filename, std::ios::in | std::ios::binary | std::ios::ate);
  if (!rom.is_open()) {
    std::cout << "Could not open rom" << filename << std::endl;
    return 1;
  }
  uint16_t size = rom.tellg();
  rom.seekg(std::ios::beg);

  rom.read(reinterpret_cast<char *>(&memory[pc]), size);

  rom.close();
  std::cout << "Rom loaded with sucess" << std::endl;
  return 0;
}

void Chip8::execute_cycle() {
  uint16_t opcode = (memory[pc] << 8) | memory[pc + 1];
  std::cout << "Decoding opcode: " << std::hex << opcode << std::endl;

  pc += 2;

  uint8_t type = (opcode & 0xF000) >> 12;

  uint8_t X = (opcode & 0x0F00) >> 8;
  uint8_t Y = (opcode & 0x00F0) >> 4;
  uint8_t N = opcode & 0x000F;
  uint8_t NN = opcode & 0x00FF;
  uint16_t NNN = opcode & 0x0FFF;

  switch (type) {
  case 0x0:
    switch (NN) {
    default:
      std::cout << "Could Not decode opcode: " << static_cast<int>(opcode)
                << std::endl;
      break;
    case 0xE0:
      gfx.fill(0);
      break;
    }
    break;
  case 0x1:
    pc = NNN;
    break;
  case 0x6:
    v_register[X] = NN;
    break;
  case 0x7:
    v_register[X] += NN;
    break;
  case 0xA:
    i = NNN;
    break;
  case 0xD:
    uint8_t base_coord_x = v_register[X] & 63;
    uint8_t base_coord_y = v_register[Y] & 31;
    v_register[0xF] = 0;

    for (uint8_t row = 0; row < N; row++) {
      uint8_t sprite_data = memory[i + row];

      if ((base_coord_y + row) >= 32) {
        break;
      }
      for (int collum = 0; collum < 8; collum++) {
        if ((base_coord_x + collum) >= 64) {
          continue;
        }
        uint8_t bit = (0x80 >> collum) & sprite_data;

        uint16_t display_index =
            (base_coord_x + collum) + (((base_coord_y + row) * 64));
        if (gfx[display_index] && bit) {
          v_register[0xF] = 1;
        }
        gfx[display_index] ^= bit;
        continue;
      }
      update_screen_flag = 1;
    }
    break;
  }
}
