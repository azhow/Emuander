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

	void
		CIOHelper::PrintRegisters(const SRegisters& registersToPrint)
	{
		std::cout << "ACC: " << static_cast<int>(registersToPrint.m_accumulator) << std::endl;
		std::cout << "PC : " << static_cast<int>(registersToPrint.m_programCounter) << std::endl;
		std::cout << "NEG: " << static_cast<int>(registersToPrint.m_negativeCondition) << std::endl;
		std::cout << "ZER: " << static_cast<int>(registersToPrint.m_zeroCondition) << std::endl;
	}

	void
		CIOHelper::PrintCounters(const uint64_t memAccesses, const uint64_t instructionsExecuted)
	{
		std::cout << "Memory Accesses      : " << memAccesses << std::endl;
		std::cout << "Instructions Executed: " << instructionsExecuted << std::endl;
	}

	CIOHelper::EIOCode
		CIOHelper::RunNeander(const std::filesystem::path& inputFile, const std::filesystem::path& outputFile)
	{
		// Return value
		EIOCode retVal(EIOCode::UNKNOWN_ERROR);

		// Neander computer instance
		CComputer neanderComputer;

		// Input memory
		std::array<uint8_t, CComputer::ms_cMemorySize> inputMemory;

		retVal = CIOHelper::LoadProgramFromFile(inputFile, inputMemory);

		if (retVal == CIOHelper::EIOCode::SUCCESS)
		{
			neanderComputer.setMemory(inputMemory);

			neanderComputer.runProgram();

			CIOHelper::PrintRegisters(neanderComputer.getRegisters());
			CIOHelper::PrintCounters(neanderComputer.getMemoryAccesses(), neanderComputer.getInstructionsExecuted());

			retVal = CIOHelper::SaveProgramToFile(neanderComputer.getMemory(), outputFile);
		}

		return retVal;
	}

	// Run Neander Debug mode
	CIOHelper::EIOCode
		CIOHelper::RunNeanderDebug(
			const std::filesystem::path& inputFile, const std::filesystem::path& outputFile)
	{
		// Return value
		EIOCode retVal(EIOCode::UNKNOWN_ERROR);

		// Breakpoints
		std::set<uint8_t> breakpoints;

		// Neander computer instance
		CComputer neanderComputer;

		// Input memory
		std::array<uint8_t, CComputer::ms_cMemorySize> inputMemory;

		retVal = CIOHelper::LoadProgramFromFile(inputFile, inputMemory);

		if (retVal == CIOHelper::EIOCode::SUCCESS)
		{
			CComputer::EProgramEnd programStatus(CComputer::EProgramEnd::CONTINUE);

			neanderComputer.setMemory(inputMemory);

			// User input
			std::string userCommand;

			std::cout << "Type 'h' for help" << std::endl;
			do
			{
				// Prompts user
				std::cout << "Neander Debug> ";
				// Read user command
				std::cin >> userCommand;
				std::cout << std::endl;

				// Pre process user command
				std::for_each(userCommand.begin(), userCommand.end(), [](char & c) { c = ::tolower(c); });

				// Run a step
				if (userCommand == "s")
				{
					programStatus = ProcessStepCommand(neanderComputer);
				}
				// Runs the program till the halt or end of memory
				else if (userCommand == "r")
				{
					programStatus = ProcessRunCommand(neanderComputer);
				}
				// Runs the program till the end or breakpoint
				else if (userCommand == "c")
				{
					programStatus = ProcessContinueCommand(neanderComputer, breakpoints);
				}
				// Adds new breakpoint
				else if (userCommand == "b")
				{
					// Address to print
					uint8_t addToPrint(0);

					if (ReadAddress(addToPrint))
					{
						breakpoints.insert(static_cast<uint8_t>(addToPrint));
						std::cout << "Breakpoint added at " << static_cast<int>(addToPrint) << std::endl;
						std::cout << std::endl;
					}
				}
				// Print content of memory address
				else if (userCommand == "p")
				{
					// Address to print
					uint8_t addToPrint(0);

					if (ReadAddress(addToPrint))
					{
						std::cout << "Address content: " << static_cast<int>(neanderComputer.getMemory()[addToPrint]) << std::endl;
						std::cout << std::endl;
					}
				}
				// Help
				else if (userCommand == "h")
				{
					PrintHelp();
				}
			} while ((userCommand != "q") && (programStatus == CComputer::EProgramEnd::CONTINUE));

			retVal = CIOHelper::SaveProgramToFile(neanderComputer.getMemory(), outputFile);
		}

		return retVal;
	}

	void
		CIOHelper::PrintHelp()
	{
		std::cout << "Commands List:" << std::endl;
		std::cout << "s -> Executes one instruction." << std::endl;
		std::cout << "r -> Runs the program until the end." << std::endl;
		std::cout << "c -> Runs until the next breakpoint or the end." << std::endl;
		std::cout << "b -> Adds new breakpoint" << std::endl;
		std::cout << "p -> Prints contents of the add given." << std::endl;
		std::cout << "h -> Displays this message." << std::endl;
		std::cout << "q -> Terminates execution." << std::endl;
		std::cout << std::endl;
	}

	bool
		CIOHelper::ReadAddress(uint8_t& readAdd)
	{
		// Return Value
		bool retVal(false);

		// Address string
		std::string addString;

		std::cout << "Address: ";
		std::cin >> addString;

		try
		{
			const uint64_t c_address = std::stoi(addString);
			if ((c_address >= 0) && (c_address <= 255))
			{
				readAdd = static_cast<uint8_t>(c_address);
				retVal = true;
			}
			else
			{
				throw std::out_of_range("The number entered should be between 0 and 255.");
			}
		}
		catch (std::invalid_argument)
		{
			std::cout << "Invalid number entered." << std::endl;
		}
		catch (std::out_of_range& e)
		{
			std::cout << e.what() << std::endl;
		}

		return retVal;
	}

	CComputer::EProgramEnd 
		CIOHelper::ProcessStepCommand(CComputer& neanderComputer)
	{
		CComputer::EProgramEnd programStatus = neanderComputer.runStep();
		
		CIOHelper::PrintRegisters(neanderComputer.getRegisters());
		CIOHelper::PrintCounters(neanderComputer.getMemoryAccesses(), neanderComputer.getInstructionsExecuted());
		std::cout << std::endl;

		return programStatus;
	}

	CComputer::EProgramEnd
		CIOHelper::ProcessRunCommand(CComputer& neanderComputer)
	{
		neanderComputer.runProgram();
		CComputer::EProgramEnd programStatus = CComputer::EProgramEnd::HALT;

		CIOHelper::PrintRegisters(neanderComputer.getRegisters());
		CIOHelper::PrintCounters(neanderComputer.getMemoryAccesses(), neanderComputer.getInstructionsExecuted());
		std::cout << std::endl;

		return programStatus;
	}

	CComputer::EProgramEnd
		CIOHelper::ProcessContinueCommand(CComputer& neanderComputer, const std::set<uint8_t>& breakpoints)
	{
		CComputer::EProgramEnd programStatus(CComputer::EProgramEnd::CONTINUE);

		// While not end of program and have not hit break point
		do
		{
			programStatus = neanderComputer.runStep();
		} while ((programStatus != CComputer::EProgramEnd::HALT) &&
			((breakpoints.find(neanderComputer.getRegisters().m_programCounter) == breakpoints.end())));

		if (programStatus != CComputer::EProgramEnd::HALT)
		{
			std::cout << "Breakpoint at " << static_cast<int>(neanderComputer.getRegisters().m_programCounter) << " hit" << std::endl;
		}
		else
		{
			std::cout << "Program Halted" << std::endl;
		}

		CIOHelper::PrintRegisters(neanderComputer.getRegisters());
		CIOHelper::PrintCounters(neanderComputer.getMemoryAccesses(), neanderComputer.getInstructionsExecuted());
		std::cout << std::endl;

		return programStatus;
	}
}