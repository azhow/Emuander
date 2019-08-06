#ifndef CNeanderComputer_h
#define CNeanderComputer_h

#include <array>

namespace Neander
{
	class CNeanderComputer
	{
	public:
		// Size of byte in bits
		static inline const std::size_t m_cByteSize = 8;

		// Memory size
		static inline const std::size_t m_cMemorySize = sizeof(unsigned char) * m_cByteSize;

		CNeanderComputer();

		~CNeanderComputer();

		// Run a given program in the computer
		std::array<unsigned char, m_cMemorySize> runProgram(std::array<unsigned char, m_cMemorySize> program);
	private:
		// Accumulator
		unsigned char m_accumulator;

		// Program counter
		unsigned char m_programCounter;

		// Zero?
		bool m_zeroCondition;

		// Negative?
		bool m_negativeCondition;

		// Main memory
		std::array<unsigned char, m_cMemorySize> m_mainMemory;
	};
}

#endif // !CNeanderComputer_h