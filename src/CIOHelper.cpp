// C++ Headers
#include <fstream>
#include <iostream>
// Homemade Headers
#include "CIOHelper.h"

namespace Neander
{
	CIOHelper::EIOCode
		CIOHelper::LoadProgramFromFile(
			const std::filesystem::path& filePath,
			std::array<uint8_t, CComputer::ms_cMemorySize>& programRead)
	{
		// Return value
		EIOCode retVal(EIOCode::UNKNOWN_ERROR);

		// Program file stream
		std::ifstream memFile(filePath, std::ios::binary | std::ios::ate);

		if (memFile.is_open())
		{
			// Position of the pointer in the stream
			std::ifstream::pos_type pos = memFile.tellg();

			// Raw Program
			std::vector<char>  rawProgram(pos);
			// Put at beginning of stream
			memFile.seekg(0, std::ios::beg);
			// Read whole stream
			memFile.read(&rawProgram[0], pos);
			memFile.close();
			// Check if Neander  program
			bool isValidNeanderProgram = (rawProgram[0] == 0x3);
			isValidNeanderProgram &= (rawProgram[1] == 0x4E);
			isValidNeanderProgram &= (rawProgram[2] == 0x44);
			isValidNeanderProgram &= (rawProgram[3] == 0x52);
			isValidNeanderProgram &= (rawProgram.size() == ((Neander::CComputer::ms_cMemorySize * 2) + 4));

			if (isValidNeanderProgram)
			{
				// Counter for copying every other element
				std::size_t counter = 0;

				// Copy every other element
				std::copy_if(rawProgram.begin() + 4, rawProgram.end(), programRead.begin(),
					[&counter](int i)->bool { return ++counter % 2; });

				retVal = EIOCode::SUCCESS;
			}
			// Invalid header or size
			else
			{
				retVal = EIOCode::VALIDATION_ERROR;
			}
		}
		// Could not open file
		else
		{
			retVal = EIOCode::OPEN_ERROR;
		}

		return retVal;
	}

	CIOHelper::EIOCode
		CIOHelper::SaveProgramToFile(
			const std::array<uint8_t, CComputer::ms_cMemorySize>& program,
			const std::filesystem::path& filePath)
	{
		// Return value
		EIOCode retVal(EIOCode::UNKNOWN_ERROR);

		// File output stream
		std::ofstream finalFile(filePath, std::ofstream::out);

		if (finalFile.is_open())
		{
			// Header size
			const std::size_t c_headerSize = 4;
			// Program to be saved as string
			std::string programString((Neander::CComputer::ms_cMemorySize * 2) + c_headerSize, '\0');
			// Neander header
			programString[0] = 0x3; // ETX
			programString[1] = 0x4E; // N
			programString[2] = 0x44; // D
			programString[3] = 0x52; // R

			// Convert memory to string
			std::size_t programStrIt = c_headerSize;
			for (std::size_t idx = 0; idx < program.size(); idx++)
			{
				programString[programStrIt] = program[idx];
				programStrIt++;
				programStrIt++;
			}

			finalFile << programString;

			finalFile.close();

			retVal = EIOCode::SUCCESS;
		}
		else
		{
			retVal = EIOCode::SAVE_ERROR;
		}

		return retVal;
	}
}