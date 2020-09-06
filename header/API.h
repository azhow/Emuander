#ifndef API_h
#define API_h

#ifdef EMUANDER_EXPORTS
#define EMUANDER_API __declspec(dllexport)
#else
#define EMUANDER_API __declspec(dllimport)
#endif

#include "CNeanderEmulation.h"

// Emulation
Neander::CNeanderEmulation g_emulation;

// Load memory file
// Returns 0 when successful
extern "C" EMUANDER_API int load_memory_file(const char* input_path);

// Save memory file
// Returns 0 when successful
extern "C" EMUANDER_API int save_memory_file(const char* output_path);

// Execute one step of the program
// Returns 0 when the program has halted or executed nop to the end of memory
extern "C" EMUANDER_API int run_step();

// Get Neander computer state
extern "C" EMUANDER_API const Neander::SNeanderComputerState* get_computer_state();

#endif // !API_H