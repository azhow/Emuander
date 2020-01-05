#include<iostream>

#include"CComputer.h"

int main()
{
	Neander::CComputer neanderEmulator;

	std::array<uint8_t, Neander::CComputer::ms_cMemorySize> exampleProgram;

	// Zero initialize memory
	std::fill(exampleProgram.begin(), exampleProgram.end(), 0);

	exampleProgram[0] = 32u;
	exampleProgram[1] = 128u;
	exampleProgram[2] = 255u;

	neanderEmulator.setMemory(exampleProgram);

	neanderEmulator.runProgram();

	Neander::SRegisters registersRead(neanderEmulator.getRegisters());

	std::cout << "ACC: " << static_cast<int>(registersRead.m_accumulator) << std::endl;
	std::cout << "PC : " << static_cast<int>(registersRead.m_programCounter) << std::endl;
	std::cout << "NEG: " << static_cast<int>(registersRead.m_negativeCondition) << std::endl;
	std::cout << "ZER: " << static_cast<int>(registersRead.m_zeroCondition) << std::endl;

	return 0;
}