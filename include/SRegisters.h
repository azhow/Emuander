#ifndef SRegisters_h
#define SRegisters_h

namespace Neander
{
	typedef struct SRegisters
	{
		// Accumulator
		uint8_t m_accumulator = 0;

		// Program counter
		uint8_t m_programCounter = 0;

		// Zero?
		bool m_zeroCondition = false;

		// Negative?
		bool m_negativeCondition = false;
	} SRegisters;
}

#endif // !SRegisters_h