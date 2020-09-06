#include "CNeanderEmulation.h"
#include "CIOHelper.h"

namespace Neander
{
	int 
		CNeanderEmulation::runStep()
	{
		// Program status
		return static_cast<int>(m_computer.runStep());
	}

	int 
		CNeanderEmulation::loadMemory(const std::filesystem::path cInputPath)
	{
		// Input memory
		std::array<uint8_t, CComputer::ms_cMemorySize> inputMemory;

		// Return value
		CIOHelper::EIOCode retVal{ CIOHelper::LoadProgramFromFile(cInputPath, inputMemory) };

		if (retVal == CIOHelper::SUCCESS)
		{
			m_computer.setMemory(inputMemory);
		}

		return static_cast<int>(retVal);
	}

	int 
		CNeanderEmulation::saveMemory(const std::filesystem::path cOutputPath)
	{
		return static_cast<int>(CIOHelper::SaveProgramToFile(m_computer.getComputerState().m_memory, cOutputPath));
	}

	const SNeanderComputerState* 
		CNeanderEmulation::getComputerState()
	{
		return &m_computer.getComputerState();
	}
}