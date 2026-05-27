# CHIP-8++

A CHIP-8 interpreter written in C++ and SDL2 for graphics. Made to study emulation, low-level concepts, and how a CPU cycle works. The interpreter was tested with Timendius chip8 test suite, which is included in roms folder.

The interpreter runs with default CHIP-8 resolution but has the super-chip quirks enabled as it is required for most games.

---

* **Instruction Cycle (Fetch-Decode-Execute):** Basic cycle implemented, running instructions straight from memory.
* **Clock Sync:** The Cycle runs freely inside a batch of instructions per frame (~700 Hz), while the Timers and the screen renderer run locked at 60 Hz using `std::chrono`.
* **Log debugger:** Fully functional log debugger, with an option to toggle step by step (input won't work in this mode).
* **Settings**: Configurable colors and keyboard key inside display.cpp and instructions per frame inside chip8.cpp.
* **Keypad**: Default keypad works using the following map:
```text
original chip-8 keypad        mine interpreter keypad
      1 2 3 C                       1 2 3 4
      4 5 6 D          ---->        q w e r
      7 8 9 E                       a s d f
      A 0 B F                       z x c v
```

---

## 🕹️ Test Status

* **Pong:** Fully functional and playable. 
* **Timendius Test Suit:** Passing all checks on the modern behavior screen (with Shift, Memory, and Jump flags enabled).
<table>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/c6e5db39-3efa-485e-946d-9a08db335aa8" alt="Chip-8 Interpreter Running" width="100%"></td>
    <td><img src="https://github.com/user-attachments/assets/4614bf16-36f2-47c4-8ea8-c67f72e05df5" alt="Chip-8 Interpreter Running" width="100%"></td>
  </tr>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/cc5cb125-99c8-44c2-889f-48cd4e03a5f4" alt="Chip-8 Interpreter Running" width="100%"></td>
    <td><img width="100%" alt="Pong running" src="https://github.com/user-attachments/assets/575cdc9e-9996-4544-b365-a1eecadbff42" /></td>
  </tr>
</table>

---

* **Usage:**
```bash
./chip8 <path-to-rom> [--debug]
```

---

## 🔧 How to Compile and Run
### Compiling
Without using a Makefile you will need to use the follow command at root folder:
```bash
# Using g++
g++ src/*.cpp -o chip8 -std=c++17 -lSDL2

# Using clang++
clang++ src/*.cpp -o chip8 -std=c++17 -lSDL2
```

### Dependencies
You need a working compiler with C++17 support and the **SDL2** library installed on your system.
I've only tested it on Arch Linux, so windows support may not be working
