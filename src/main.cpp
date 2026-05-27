#include "chip8.h"
#include "display.h"
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <sys/types.h>
#include <thread>

uint8_t exit_program();

int main(int argc, char *argv[]) {
  try {
    // initialize the interpreter, load the rom, and tries to initialize the
    // sdl2 display
    Chip8 chip8;
    chip8.initialize();
    if (argc >= 3) {
      if (std::strcmp(argv[2], "--debug") == 0) {
        std::cout << "Debug mode enabled!" << std::endl;
        chip8.debug_mode = 1;
      }
    }
    chip8.load_rom(argv[1]);
    Display display(chip8);

    // starting time at system clock, this will get increaser every 1/60th of a
    // second, the rate of the chip8 timers. Bellow in the code chrono will
    // sleep until it completes 1/60 of a second and goes on
    auto start_time = std::chrono::system_clock::now();
    while (true) {
      if (chip8.running_flag != 1) {
        return 0;
      }
      start_time += std::chrono::microseconds(16660);

      display.handle_events(chip8);
      // executes deafult = 12 instructions per loop, 12 * 60 (loops
      // per seconds), results in ~700 instructions per second, pretty close to
      // default chip8
      for (uint8_t i = 0, target = chip8.instructions_per_second / 60;
           i < target; i++) {
        chip8.execute_cycle();
        display.update_screen(chip8);
        // checks if the screen have any changes if so updates it
      }

      chip8.decrease_timers();
      display.update_audio(chip8.sound_timer);

      std::this_thread::sleep_until(start_time);
    }
  } catch (std::runtime_error e) {
    std::cout << e.what() << std::endl;
  }
}

uint8_t exit_program() {
  std::cout << "Exiting program!" << std::endl;
  return 1;
}
