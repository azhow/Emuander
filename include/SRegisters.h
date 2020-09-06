#ifndef SRegisters_h
#define SRegisters_h

#include <cstdint>

namespace Neander
{
	typedef struct SRegisters
	{
		// Accumulator
		uint8_t m_accumulator;

		// Program counter
		uint8_t m_programCounter;

		// Zero?
		bool m_zeroCondition;

		// Negative?
		bool m_negativeCondition;

		// Struct constructor
		SRegisters(uint8_t acc, uint8_t pc, bool zero, bool negative) :
			m_accumulator(acc), m_programCounter(pc), m_zeroCondition(zero), m_negativeCondition(negative) {};
	} SRegisters;
}

#endif // !SRegisters_h