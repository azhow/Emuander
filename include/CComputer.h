#ifndef CComputer_h
#define CComputer_h

// C++ Includes
#include <array>
#include <utility>
#include <functional>
// Homemade Includes
#include "SRegisters.h"

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
		enum class EProgramEnd
		{
			HALT = 0, // Program ended
			CONTINUE = 1 // Program continue execution
		};

		// Memory size in bytes
		static inline const std::size_t ms_cMemorySize = 256;

		// Default constructor
		CComputer();

		// Set main memory (load a program) returns true upon success
		inline void setMemory(const std::array<uint8_t, ms_cMemorySize>& program) { m_mainMemory = program; };

		// Get the contents of the memory, returns the contents of the memory
		inline std::array<uint8_t, ms_cMemorySize> getMemory() const { return m_mainMemory; };

		// Set the registers contents
		inline void setRegisters(SRegisters newRegisters)
		{
			// Reset counters
			m_memAccessCounter = 0;
			m_instructionCounter = 0;
			m_registers = newRegisters;
			updateConditionRegisters();
		};

		// Get the registers contents
		inline SRegisters getRegisters() const { return m_registers; };

		// Get memory accesses counter
		inline uint64_t getMemoryAccesses() const { return m_memAccessCounter; };

		// Get instructions executed counter
		inline uint64_t getInstructionsExecuted() const { return m_instructionCounter; };

		// Run the contents of the memory in a while Fetch->Decode->Read->Execute
		void runProgram();

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

		// Computer registers
		SRegisters m_registers;

		// Main memory is the program running or nothing at all
		std::array<uint8_t, ms_cMemorySize> m_mainMemory;

		// Counts the number of memory accesses
		uint64_t m_memAccessCounter;
		
		// Counts the number of instructions
		uint64_t m_instructionCounter;
	};
}

#endif // !CComputer_h