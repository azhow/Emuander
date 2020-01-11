# Emuander
Neander emulator implemented using C++.

![](https://github.com/azhow/Emuander/workflows/Windows%20Build%20CI/badge.svg)

![](https://github.com/azhow/Emuander/workflows/Unitests/badge.svg)

## Features
1. Cross-platform support (Windows and Linux);
2. Command Line Interface to use;

## How to use:
```
Emuander.exe [-d] -i input_memory_file.mem -o output_memory_file.mem
```

Where:
* -h, --help                        Display this help menu
* -i[input_program_file], --input=[input_program_file]      Input program file (.mem)
* -o[output_program_file], --output=[output_program_file]    Output memory file (.mem)
* -d, --debug                       Uses the neander in debug mode

Upon successful execution, the application will display the computer register`s contents and hardware counters.

### Neander Registers
The registers are the following:
* ACC = Accumulator;
* PC  = Program Counter;
* NEG = Negative Flag, true if the content of ACC is < 0;
* ZER = Zero Flag, true if the content of ACC is == 0;
