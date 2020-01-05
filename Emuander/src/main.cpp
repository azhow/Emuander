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
		Neander::CIOHelper::RunNeander(args::get(inputPath), args::get(outputPath));
		retValue = 0;
	}
	else
	{
		std::cout << "Neander usage:" << std::endl;
		std::cout << parser;
		retValue = 0;
	}

	return retValue;
}