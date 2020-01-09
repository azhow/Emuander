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

			TEST_METHOD(STA_Instruction)
			{
				// Neander computer instance
				CComputer neanderComputer;

				// Input memory
				std::array<uint8_t, CComputer::ms_cMemorySize> inputMemory;

				// Zero initializes memory
				std::fill(inputMemory.begin(), inputMemory.end(), 0);

				// STA Instruction 1 stores the ACC in position 128
				inputMemory[0] = static_cast<uint8_t>(CComputer::EOperations::STA);
				inputMemory[1] = 128;
				// STA Instruction 2 stores the ACC in position 10
				inputMemory[2] = 31; // Shows that it works by the bit activated instead of the number itself
				inputMemory[3] = 10;
				// HLT Instruction
				inputMemory[4] = static_cast<uint8_t>(CComputer::EOperations::HLT);

				neanderComputer.setMemory(inputMemory);

				// ACC initial and final value
				const uint8_t c_accValue(45);

				// Input registers
				const SRegisters c_inputRegs(c_accValue, 0, false, false);

				neanderComputer.setRegisters(c_inputRegs);

				neanderComputer.runProgram();

				// Final registers values
				const SRegisters c_finalRegs(neanderComputer.getRegisters());
				// Final memory values
				const std::array<uint8_t, CComputer::ms_cMemorySize> c_outputMemory(neanderComputer.getMemory());

				// Assert ACC value
				Assert::AreEqual(c_finalRegs.m_accumulator, c_accValue, L"ACC has wrong value.");

				// Assert PC value
				Assert::AreEqual(static_cast<int>(c_finalRegs.m_programCounter), 5, L"PC has wrong value.");

				// Assert NEG flag
				Assert::AreEqual(c_finalRegs.m_negativeCondition, false, L"NEG has wrong value.");

				// Assert Zero flag
				Assert::AreEqual(c_finalRegs.m_zeroCondition, false, L"ZERO has wrong value.");

				// Check memory
				Assert::AreEqual(c_outputMemory[10], c_accValue, L"Value on memory position 10 is invalid.");
				Assert::AreEqual(c_outputMemory[128], c_accValue, L"Value on memory position 128 is invalid.");
			}

			TEST_METHOD(LDA_Instruction)
			{
				// Neander computer instance
				CComputer neanderComputer;

				// Input memory
				std::array<uint8_t, CComputer::ms_cMemorySize> inputMemory;

				// Zero initializes memory
				std::fill(inputMemory.begin(), inputMemory.end(), 0);

				// Value Memory address
				const uint8_t c_valueMemoryAdd(64);

				// LDA Instruction 1 load the ACC from position 64
				inputMemory[0] = static_cast<uint8_t>(CComputer::EOperations::LDA);
				inputMemory[1] = c_valueMemoryAdd;
				// HLT Instruction
				inputMemory[2] = static_cast<uint8_t>(CComputer::EOperations::HLT);

				// ACC Desired value 1
				const uint8_t c_accFinalValue(255);

				// Test value final value
				inputMemory[c_valueMemoryAdd] = c_accFinalValue;

				neanderComputer.setMemory(inputMemory);

				neanderComputer.runProgram();

				// Final registers values
				const SRegisters c_finalRegs(neanderComputer.getRegisters());
				// Final memory values
				const std::array<uint8_t, CComputer::ms_cMemorySize> c_outputMemory(neanderComputer.getMemory());

				// Assert ACC value
				Assert::AreEqual(c_finalRegs.m_accumulator, c_accFinalValue, L"ACC has wrong value.");

				// Assert PC value
				Assert::AreEqual(static_cast<int>(c_finalRegs.m_programCounter), 3, L"PC has wrong value.");

				// Assert NEG flag
				Assert::AreEqual(c_finalRegs.m_negativeCondition, true, L"NEG has wrong value.");

				// Assert Zero flag
				Assert::AreEqual(c_finalRegs.m_zeroCondition, false, L"ZERO has wrong value.");

				// Check memory
				// Memory cannot change when executing LDA
				for (int idx = 0; idx < c_outputMemory.size(); idx++)
				{
					Assert::AreEqual(c_outputMemory[idx], inputMemory[idx], L"Input and output memory are different.");
				}
			}

			TEST_METHOD(ADD_Instruction_1)
			{
				// Neander computer instance
				CComputer neanderComputer;

				// Input memory
				std::array<uint8_t, CComputer::ms_cMemorySize> inputMemory;

				// Zero initializes memory
				std::fill(inputMemory.begin(), inputMemory.end(), 0);

				// Value Memory address
				const uint8_t c_valueMemoryAdd(64);

				// ADD Instruction, sums ACC (55) with 20
				inputMemory[0] = static_cast<uint8_t>(CComputer::EOperations::ADD);
				inputMemory[1] = c_valueMemoryAdd;
				// HLT Instruction
				inputMemory[2] = static_cast<uint8_t>(CComputer::EOperations::HLT);

				// ACC Desired value
				const uint8_t c_accFinalValue(75);

				// Test value final value
				inputMemory[c_valueMemoryAdd] = 20;

				neanderComputer.setRegisters(SRegisters(55, 0, false, false));

				neanderComputer.setMemory(inputMemory);

				neanderComputer.runProgram();

				// Final registers values
				const SRegisters c_finalRegs(neanderComputer.getRegisters());

				// Final memory values
				const std::array<uint8_t, CComputer::ms_cMemorySize> c_outputMemory(neanderComputer.getMemory());

				// Assert ACC value
				Assert::AreEqual(c_finalRegs.m_accumulator, c_accFinalValue, L"ACC has wrong value.");

				// Assert PC value
				Assert::AreEqual(static_cast<int>(c_finalRegs.m_programCounter), 3, L"PC has wrong value.");

				// Assert NEG flag
				Assert::AreEqual(c_finalRegs.m_negativeCondition, false, L"NEG has wrong value.");

				// Assert Zero flag
				Assert::AreEqual(c_finalRegs.m_zeroCondition, false, L"ZERO has wrong value.");

				// Check memory
				// Memory cannot change when executing LDA
				for (int idx = 0; idx < c_outputMemory.size(); idx++)
				{
					Assert::AreEqual(c_outputMemory[idx], inputMemory[idx], L"Input and output memory are different.");
				}
			}

			TEST_METHOD(ADD_Instruction_2)
			{
				// Neander computer instance
				CComputer neanderComputer;

				// Input memory
				std::array<uint8_t, CComputer::ms_cMemorySize> inputMemory;

				// Zero initializes memory
				std::fill(inputMemory.begin(), inputMemory.end(), 0);

				// Value Memory address
				const uint8_t c_valueMemoryAdd(64);

				// ADD Instruction, sums ACC (55) with 100
				inputMemory[0] = static_cast<uint8_t>(CComputer::EOperations::ADD);
				inputMemory[1] = c_valueMemoryAdd;
				// HLT Instruction
				inputMemory[2] = static_cast<uint8_t>(CComputer::EOperations::HLT);

				// ACC Desired value
				const uint8_t c_accFinalValue(155);

				// Test value final value
				inputMemory[c_valueMemoryAdd] = 100;

				neanderComputer.setRegisters(SRegisters(55, 0, false, false));

				neanderComputer.setMemory(inputMemory);

				neanderComputer.runProgram();

				// Final registers values
				const SRegisters c_finalRegs(neanderComputer.getRegisters());

				// Final memory values
				const std::array<uint8_t, CComputer::ms_cMemorySize> c_outputMemory(neanderComputer.getMemory());

				// Assert ACC value
				Assert::AreEqual(c_finalRegs.m_accumulator, c_accFinalValue, L"ACC has wrong value.");

				// Assert PC value
				Assert::AreEqual(static_cast<int>(c_finalRegs.m_programCounter), 3, L"PC has wrong value.");

				// Assert NEG flag
				Assert::AreEqual(c_finalRegs.m_negativeCondition, true, L"NEG has wrong value.");

				// Assert Zero flag
				Assert::AreEqual(c_finalRegs.m_zeroCondition, false, L"ZERO has wrong value.");

				// Check memory
				// Memory cannot change when executing LDA
				for (int idx = 0; idx < c_outputMemory.size(); idx++)
				{
					Assert::AreEqual(c_outputMemory[idx], inputMemory[idx], L"Input and output memory are different.");
				}
			}

			TEST_METHOD(ADD_Instruction_3)
			{
				// Neander computer instance
				CComputer neanderComputer;

				// Input memory
				std::array<uint8_t, CComputer::ms_cMemorySize> inputMemory;

				// Zero initializes memory
				std::fill(inputMemory.begin(), inputMemory.end(), 0);

				// Value Memory address
				const uint8_t c_valueMemoryAdd(64);

				// ADD Instruction, sums ACC (128) with 128
				inputMemory[0] = static_cast<uint8_t>(CComputer::EOperations::ADD);
				inputMemory[1] = c_valueMemoryAdd;
				// HLT Instruction
				inputMemory[2] = static_cast<uint8_t>(CComputer::EOperations::HLT);

				// ACC Desired value
				const uint8_t c_accFinalValue(0);

				// Test value final value
				inputMemory[c_valueMemoryAdd] = 128;

				neanderComputer.setRegisters(SRegisters(128, 0, false, false));

				neanderComputer.setMemory(inputMemory);

				neanderComputer.runProgram();

				// Final registers values
				const SRegisters c_finalRegs(neanderComputer.getRegisters());

				// Final memory values
				const std::array<uint8_t, CComputer::ms_cMemorySize> c_outputMemory(neanderComputer.getMemory());

				// Assert ACC value
				Assert::AreEqual(c_finalRegs.m_accumulator, c_accFinalValue, L"ACC has wrong value.");

				// Assert PC value
				Assert::AreEqual(static_cast<int>(c_finalRegs.m_programCounter), 3, L"PC has wrong value.");

				// Assert NEG flag
				Assert::AreEqual(c_finalRegs.m_negativeCondition, false, L"NEG has wrong value.");

				// Assert Zero flag
				Assert::AreEqual(c_finalRegs.m_zeroCondition, true, L"ZERO has wrong value.");

				// Check memory
				// Memory cannot change when executing LDA
				for (int idx = 0; idx < c_outputMemory.size(); idx++)
				{
					Assert::AreEqual(c_outputMemory[idx], inputMemory[idx], L"Input and output memory are different.");
				}
			}
		};
	}
}