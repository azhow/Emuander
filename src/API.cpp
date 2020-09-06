// Homemade Headers
#include "API.h"

int 
load_memory_file(const char* input_path)
{
	return g_emulation.loadMemory(input_path);
}

int 
save_memory_file(const char* output_path)
{
	return g_emulation.saveMemory(output_path);
}

int 
run_step()
{
	return g_emulation.runStep();
}

const Neander::SNeanderComputerState*
get_computer_state()
{
	return g_emulation.getComputerState();
}