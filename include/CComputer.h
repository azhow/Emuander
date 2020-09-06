#ifndef CComputer_h
#define CComputer_h

// C++ Includes
#include <array>
#include <utility>
#include <functional>
// Homemade Includes
#include "SNeanderComputerState.h"

namespace Neander
{
	class CComputer
	{
	public:
		// Neander computer supported operations
		enum class EOperations : uint8_t
		{
			NOP = 0u,   // No operation
			STA = 16u,  // Store to address
			LDA = 32u,  // Load from address
			ADD = 48u,  // Add address content with ACC
			OR = 64u,  // Binary Or address content with ACC
			AND = 80u,  // Binary And address content with ACC
			NOT = 96u,  // Inverts all bits of the ACC
			JMP = 128u, // Set PC to address
			JN = 144u, // Set PC if Negative register is on
			JZ = 160u, // Set PC if Zero register is on
			HLT = 240u  // Halts execution
		};

		// If the program has halted
		enum EProgramEnd
		{
			HALT = 0, // Program ended
			CONTINUE = 1 // Program continue execution
		};

		// Memory size in bytes
		static inline const std::size_t ms_cMemorySize = 256;

		// Default constructor
		CComputer();

		// Set main memory (load a program) returns true upon success
		inline void setMemory(const std::array<uint8_t, ms_cMemorySize>& program) { m_computerState.m_memory = program; };

		// Get the contents of the memory, returns the contents of the memory
		inline const SNeanderComputerState& getComputerState() const { return m_computerState; };

		// Execute one instruction of the while loop
		EProgramEnd runStep();
	private:
		// Update condition registers
		void updateConditionRegisters();

		// Fetch instruction (read instruction from memory) must return an instruction
		uint8_t fetchInstruction() const;

		// Decode instruction (decodes what the fetched instruction does)
		// return the function it should operate with operators bound
		std::function<EProgramEnd()> decodeInstruction(const uint8_t instruction);

		// Execute instruction (realize the operation), update memory and registers according to the operation
		template <typename F>
		EProgramEnd executeInstruction(F function)
		{
			EProgramEnd retVal(EProgramEnd::HALT);

			retVal = function();
			
			return retVal;
		}

		// NOP Operation
		EProgramEnd nopOperation();

		// Store operation
		EProgramEnd storeOperation(const uint8_t value);

		// Load operation
		EProgramEnd loadOperation(const uint8_t value);

		// Add operation
		EProgramEnd addOperation(const uint8_t value);

		// OR operation
		EProgramEnd orOperation(const uint8_t value);

		// And operation
		EProgramEnd andOperation(const uint8_t value);

		// Not operation
		EProgramEnd notOperation();

		// jmp operation
		EProgramEnd jmpOperation(const uint8_t value);

		// jn operation
		EProgramEnd jnOperation(const uint8_t value);

		// jz operation
		EProgramEnd jzOperation(const uint8_t value);

		// Halt operation
		EProgramEnd haltOperation();

		// Computer state
		SNeanderComputerState m_computerState;
	};
}

#endif // !CComputer_h