#ifndef CNeanderEmulation_h
#define CNeanderEmulation_h

// C++ Includes
#include <array>
#include <filesystem>
#include <set>
// Homemade Includes
#include "CComputer.h"
#include "SNeanderComputerState.h"

namespace Neander
{
	class CNeanderEmulation
	{
	public:
		// Class constructor
		CNeanderEmulation() {};

		// Run one step of the emulation
		// Returns whether it finished or not 
		int runStep();

		// Load memory from a .MEM file
		// Return whether success or error
		int loadMemory(const std::filesystem::path cInputPath);

		// Save memory as a .MEM file
		// Return whether success or error
		int saveMemory(const std::filesystem::path cOutputPath);

		// Get current computer state
		const SNeanderComputerState* getComputerState();
	private:
		// Computer
		CComputer m_computer;
	};
}

#endif // !CNeanderEmulation_h