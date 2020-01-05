#include<iostream>
#include<fstream>

#include"CComputer.h"
#include"CIOHelper.h"

int main()
{
	Neander::CComputer neanderComputer;

	std::array<uint8_t, Neander::CComputer::ms_cMemorySize> exampleProgram;

	Neander::CIOHelper::EIOCode loadStatus = 
		Neander::CIOHelper::LoadProgramFromFile("/home/lain/projects/Emuander/teste.mem", exampleProgram);

	if (loadStatus == Neander::CIOHelper::EIOCode::SUCCESS)
	{
		neanderComputer.setMemory(exampleProgram);

		neanderComputer.runProgram();

		Neander::SRegisters registersRead(neanderComputer.getRegisters());

		Neander::CIOHelper::PrintRegisters(registersRead);

		Neander::CIOHelper::SaveProgramToFile(
			neanderComputer.getMemory(),
			"/home/lain/projects/Emuander/testeOut2.mem");
	}

	return 0;
}