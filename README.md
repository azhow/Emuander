# Emuander
Neander emulator implemented using C++.

![](https://github.com/azhow/Emuander/workflows/Windows%20Build%20CI/badge.svg)

![](https://github.com/azhow/Emuander/workflows/Unitests/badge.svg)

## Features
1. Cross-platform support (Windows and Linux);
2. Command Line Interface to use;

## How to use:
```
Emuander.exe -i input_memory_file.mem -o output_memory_file.mem
```

After successfully running the memory file, it will display the register`s contents and hardware counters.

The registers are the following:
* ACC = Accumulator;
* PC  = Program Counter;
* NEG = Negative Flag, true if the content of ACC is < 0;
* ZER = Zero Flag, true if the content of ACC is == 0;
