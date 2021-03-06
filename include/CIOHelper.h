#ifndef CIOHelper_h
#define CIOHelper_h

// C++ Includes
#include <array>
#include <filesystem>
#include <set>
// Homemade Includes
#include "CComputer.h"
#include "SRegisters.h"

namespace Neander
{
	class CIOHelper
	{
	public:
		enum EIOCode
		{
			SUCCESS = 0, // Successfully opened program file
			OPEN_ERROR = 1, // Error while trying to open file
			VALIDATION_ERROR = 2, // Not a valid neander file
			SAVE_ERROR = 3, // Could not save to file
			UNKNOWN_ERROR = 4, // Unknown error
		};

		// If program was successfully read, it will be on programRead
		static EIOCode LoadProgramFromFile(
			const std::filesystem::path& filePath, 
			std::array<uint8_t, CComputer::ms_cMemorySize>& programRead);

		// Save program to file
		static EIOCode SaveProgramToFile(
			const std::array<uint8_t, CComputer::ms_cMemorySize>& program,
			const std::filesystem::path& filePath);
	};
}

#endif // !CIOHelper