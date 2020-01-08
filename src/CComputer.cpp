#include <algorithm>
#include "CComputer.h"

namespace Neander
{
	CComputer::CComputer() :
		m_registers()
	{
		// Zero initialize memory
		std::fill(m_mainMemory.begin(), m_mainMemory.end(), 0);
	}

	void
		CComputer::runProgram()
	{
		// While with initializer?
		for (EProgramEnd programStatus = EProgramEnd::CONTINUE; (programStatus != EProgramEnd::HALT)
			&& (m_registers.m_programCounter != 255u);)
		{
			// Instruction to be executed
			const uint8_t instruction(fetchInstruction());
			// Operation to be done
			const std::function<CComputer::EProgramEnd()> operation(decodeInstruction(instruction));
			programStatus = executeInstruction(operation);
			updateConditionRegisters();
		}
	}

	void
		CComputer::updateConditionRegisters()
	{
		m_registers.m_negativeCondition = (m_registers.m_accumulator > 127u);
		m_registers.m_zeroCondition = (m_registers.m_accumulator == 0u);
	}

	uint8_t
		CComputer::fetchInstruction() const
	{
		// Return value
		uint8_t retVal;

		retVal = m_mainMemory.at(m_registers.m_programCounter);

		return retVal;
	}

	std::function<CComputer::EProgramEnd()>
		CComputer::decodeInstruction(const uint8_t instruction)
	{
		// Neander operation function
		std::function<CComputer::EProgramEnd()> retVal = [this]() { return haltOperation(); };

		// NOP
		if (instruction < static_cast<uint8_t>(EOperations::STA))
		{
			retVal = [this]() { return nopOperation(); };
		}
		// STA
		else if (instruction < static_cast<uint8_t>(EOperations::LDA))
		{
			// Bind function to parameters
			retVal =
				std::bind([this](uint8_t op) { return storeOperation(op); },
					m_mainMemory.at(m_registers.m_programCounter + 1));
		}
		// LDA
		else if (instruction < static_cast<uint8_t>(EOperations::ADD))
		{
			// Bind function to parameters
			retVal =
				std::bind([this](uint8_t op) { return loadOperation(op); },
					m_mainMemory.at(m_registers.m_programCounter + 1));
		}
		// ADD
		else if (instruction < static_cast<uint8_t>(EOperations::OR))
		{
			// Bind function to parameters
			retVal =
				std::bind([this](uint8_t op) { return addOperation(op); },
					m_mainMemory.at(m_registers.m_programCounter + 1));
		}
		// OR
		else if (instruction < static_cast<uint8_t>(EOperations::AND))
		{
			// Bind function to parameters
			retVal =
				std::bind([this](uint8_t op) { return orOperation(op); },
					m_mainMemory.at(m_registers.m_programCounter + 1));
		}
		// AND
		else if (instruction < static_cast<uint8_t>(EOperations::NOT))
		{
			// Bind function to parameters
			retVal =
				std::bind([this](uint8_t op) { return andOperation(op); },
					m_mainMemory.at(m_registers.m_programCounter + 1));
		}
		// NOT
		else if (instruction < static_cast<uint8_t>(EOperations::JMP))
		{
			retVal = [this]() { return notOperation(); };
		}
		// JMP
		else if (instruction < static_cast<uint8_t>(EOperations::JN))
		{
			// Bind function to parameters
			retVal =
				std::bind([this](uint8_t op) { return jmpOperation(op); },
					m_mainMemory.at(m_registers.m_programCounter + 1));
		}
		// JN
		else if (instruction < static_cast<uint8_t>(EOperations::JZ))
		{
			// Bind function to parameters
			retVal =
				std::bind([this](uint8_t op) { return jnOperation(op); },
					m_mainMemory.at(m_registers.m_programCounter + 1));
		}
		// JZ
		else if (instruction < static_cast<uint8_t>(EOperations::HLT))
		{
			// Bind function to parameters
			retVal =
				std::bind([this](uint8_t op) { return jzOperation(op); },
					m_mainMemory.at(m_registers.m_programCounter + 1));
		}
		// HLT
		else
		{
			retVal = [this]() { return haltOperation(); };
		}

		return retVal;
	}

	CComputer::EProgramEnd
		CComputer::nopOperation()
	{
		m_registers.m_programCounter++;
		return EProgramEnd::CONTINUE;
	}

	CComputer::EProgramEnd
		CComputer::storeOperation(const uint8_t value)
	{
		m_mainMemory[value] = m_registers.m_accumulator;
		m_registers.m_programCounter++;
		m_registers.m_programCounter++;
		return EProgramEnd::CONTINUE;
	}

	CComputer::EProgramEnd
		CComputer::loadOperation(const uint8_t value)
	{
		m_registers.m_accumulator = m_mainMemory[value];
		m_registers.m_programCounter++;
		m_registers.m_programCounter++;
		return EProgramEnd::CONTINUE;
	}

	CComputer::EProgramEnd
		CComputer::addOperation(const uint8_t value)
	{
		m_registers.m_accumulator = static_cast<uint8_t>(m_registers.m_accumulator + m_mainMemory[value]);
		m_registers.m_programCounter++;
		m_registers.m_programCounter++;
		return EProgramEnd::CONTINUE;
	}

	CComputer::EProgramEnd
		CComputer::orOperation(const uint8_t value)
	{
		m_registers.m_accumulator = static_cast<uint8_t>(m_registers.m_accumulator | m_mainMemory[value]);
		m_registers.m_programCounter++;
		m_registers.m_programCounter++;
		return EProgramEnd::CONTINUE;
	}

	CComputer::EProgramEnd
		CComputer::andOperation(const uint8_t value)
	{
		m_registers.m_accumulator = static_cast<uint8_t>(m_registers.m_accumulator & m_mainMemory[value]);
		m_registers.m_programCounter++;
		m_registers.m_programCounter++;
		return EProgramEnd::CONTINUE;
	}

	CComputer::EProgramEnd
		CComputer::notOperation()
	{
		m_registers.m_accumulator = static_cast<uint8_t>(~m_registers.m_accumulator);
		m_registers.m_programCounter++;
		m_registers.m_programCounter++;
		return EProgramEnd::CONTINUE;
	}

	CComputer::EProgramEnd
		CComputer::jmpOperation(const uint8_t value)
	{
		m_registers.m_programCounter = value;
		return EProgramEnd::CONTINUE;
	}

	CComputer::EProgramEnd
		CComputer::jnOperation(const uint8_t value)
	{
		// Contant uint8_t 2
		const uint8_t c_2ui8(static_cast<uint8_t>(2));
		(m_registers.m_accumulator >= 128u) ? 
			m_registers.m_programCounter = value : 
			m_registers.m_programCounter = static_cast<uint8_t>(m_registers.m_programCounter + c_2ui8);

		return EProgramEnd::CONTINUE;
	}

	CComputer::EProgramEnd
		CComputer::jzOperation(const uint8_t value)
	{
		// Contant uint8_t 2
		const uint8_t c_2ui8(static_cast<uint8_t>(2));
		(m_registers.m_accumulator == 0u) ?
			m_registers.m_programCounter = value : 
			m_registers.m_programCounter = static_cast<uint8_t>(m_registers.m_programCounter + c_2ui8);

		return EProgramEnd::CONTINUE;
	}

	CComputer::EProgramEnd
		CComputer::haltOperation()
	{
		m_registers.m_programCounter++;
		return EProgramEnd::HALT;
	};
}