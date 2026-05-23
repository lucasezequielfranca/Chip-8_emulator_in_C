#include "chip8.h"
#include "display.h"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <thread>

uint8_t exit_program();

int main(int argc, char *argv[]) {
  try {
    Chip8 cpu;
    cpu.initialize();
    cpu.load_rom(argv[1]);
    Display display;

    while (true) {
      cpu.execute_cycle();
      display.update_screen(cpu);
      std::this_thread::sleep_for(std::chrono::milliseconds(17));
    }

  } catch (std::runtime_error e) {
    std::cout << e.what() << std::endl;
  }
}

uint8_t exit_program() {
  std::cout << "Exiting program!" << std::endl;
  return 1;
}
