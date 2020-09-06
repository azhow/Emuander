#ifndef SNeanderComputerState_h
#define SNeanderComputerState_h

#include <cstdint>
#include <array>
#include "SRegisters.h"

namespace Neander
{
	typedef struct SNeanderComputerState
	{
		// Counts the number of memory accesses
		uint64_t m_memAccessCounter;

		// Counts the number of instructions
		uint64_t m_instructionCounter;

		// Computer memory
		std::array<uint8_t, 256> m_memory;

		// Computer registers
		SRegisters m_registers;

		// Struct constructor
		SNeanderComputerState() :
			m_registers(0, 0, false, false),
			m_memAccessCounter(0),
			m_instructionCounter(0)
		{ 		
			// Zero initialize memory
			std::fill(m_memory.begin(), m_memory.end(), 0);
		};

		// Struct constructor
		SNeanderComputerState(std::array<uint8_t, 256> memory, SRegisters registers, 
			uint64_t accessCounter, uint64_t instructionCounter) :
			m_memory(memory), m_registers(registers), m_memAccessCounter(0), m_instructionCounter(0) {};
	} SNeanderComputerState;
}

#endif // !SNeanderComputerState_h