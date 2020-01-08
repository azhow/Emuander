#include "CppUnitTest.h"
#include <array>
#include <algorithm>

#include "CComputer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Neander
{
	namespace Tests
	{
		TEST_CLASS(ComputerTests)
		{
		public:
			TEST_METHOD(NOP_Instruction)
			{
				// Neander computer instance
				CComputer neanderComputer;

				// Input memory
				std::array<uint8_t, CComputer::ms_cMemorySize> inputMemory;

				// Zero initializes memory
				std::fill(inputMemory.begin(), inputMemory.end(), 0);

				neanderComputer.setMemory(inputMemory);

				neanderComputer.runProgram();

				// Final registers values
				const SRegisters c_finalRegs(neanderComputer.getRegisters());
				// Final memory values
				const std::array<uint8_t, CComputer::ms_cMemorySize> c_outputMemory(neanderComputer.getMemory());

				// Assert ACC value
				Assert::AreEqual(static_cast<int>(c_finalRegs.m_accumulator), 0, L"ACC has wrong value.");

				// Assert PC value
				Assert::AreEqual(static_cast<int>(c_finalRegs.m_programCounter), 255, L"PC has wrong value.");

				// Assert NEG flag
				Assert::AreEqual(c_finalRegs.m_negativeCondition, false, L"NEG has wrong value.");

				// Assert Zero flag
				Assert::AreEqual(c_finalRegs.m_zeroCondition, true, L"ZERO has wrong value.");

				// Check memory
				for (const auto memVal : c_outputMemory)
				{
					Assert::AreEqual(static_cast<int>(memVal), 0, L"Memory has wrong value.");
				}
			}
		};
	}
}