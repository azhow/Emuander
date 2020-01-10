#include<iostream>
#include<fstream>

#include"args.hxx"
#include"CIOHelper.h"

int main(int argc, char **argv)
{
	// Main return value
	int retValue(0);

	// CLI arguments
	args::ArgumentParser parser("Neander Emulator", "Made by: Arthur Zachow Coelho");
	args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help" });
	args::ValueFlag<std::string> inputPath(parser, "input_program_file", "Input program file (.mem)", { 'i' });
	args::ValueFlag<std::string> outputPath(parser, "output_program_file", "Output memory file (.mem)", { 'o' });

	try
	{
		parser.ParseCLI(argc, argv);
	}
	catch (args::Help& e)
	{
		std::cout << parser;
		retValue = 0;
	}
	catch (args::ParseError& e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		retValue = 1;
	}
	catch (args::ValidationError& e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		retValue = 1;
	}

	if (inputPath && outputPath)
	{
		std::cout << "Running Neander..." << std::endl;
		std::cout << std::endl;
		Neander::CIOHelper::EIOCode neanderStatus = Neander::CIOHelper::RunNeander(args::get(inputPath), args::get(outputPath));
		std::cout << std::endl;
		switch (neanderStatus)
		{
		case Neander::CIOHelper::EIOCode::SUCCESS:
		{
			std::cout << "Neander ran successfully." << std::endl;
			retValue = 0;
		}
		break;
		case Neander::CIOHelper::EIOCode::SAVE_ERROR:
		{
			std::cout << "Neander could not save the memory file." << std::endl;
			retValue = 1;
		}
		break;
		case Neander::CIOHelper::EIOCode::OPEN_ERROR:
		{
			std::cout << "Neander could not open the input memory file." << std::endl;
			retValue = 1;
		}
		case Neander::CIOHelper::EIOCode::VALIDATION_ERROR:
		{
			std::cout << "The input file is not a Neander Memory file." << std::endl;
			retValue = 1;
		}
		break;
		case Neander::CIOHelper::EIOCode::UNKNOWN_ERROR:
		{
			std::cout << "An unknown error ocurred while running Neander." << std::endl;
			retValue = 1;
		}
		break;
		}
	}
	else
	{
		std::cout << "Neander usage:" << std::endl;
		std::cout << parser;
		retValue = 0;
	}

	return retValue;
}