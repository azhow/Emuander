import ctypes
import struct
from sys import platform


class NeanderRegisters(ctypes.Structure):
    _fields_ = [("accumulator", ctypes.c_uint8), 
                ("program_counter", ctypes.c_uint8), 
                ("zero_condition", ctypes.c_bool), 
                ("negative_condition", ctypes.c_bool)]

    def __str__(self):
        ret_str = ""
        for field_name, field_type in self._fields_:
            ret_str += "{0}: {1}".format(field_name, getattr(self, field_name)) + "\n"
        
        return ret_str

class NeanderComputerState(ctypes.Structure):
    _fields_ = [("memory_access_counter", ctypes.c_uint64),
                ("instruction_counter", ctypes.c_uint64),
                ("memory", ctypes.c_uint8 * 256),
                ("registers", NeanderRegisters)]

    def __str__(self):
        ret_str = ""
        for field_name, field_type in self._fields_:
            # For the memory we print each memory position
            if field_name == "memory":
                ret_str += "memory:"
                for idx, val in enumerate(getattr(self, field_name)):
                    ret_str += "\t{0}: {1}".format(idx, val) + "\n"
            else:
                ret_str += "{0}: {1}".format(field_name, getattr(self, field_name)) + "\n"
        
        return ret_str


def execute_neander(input_memory_path, output_memory_path, neander_dll):
    # Execution result
    result = 0
    # Set function argument type
    emuander_so.load_memory_file.argtypes = [ctypes.c_char_p]
    # Set input memory
    load_result = emuander_so.load_memory_file(input_memory_path.encode('utf-8'))

    # If loaded successfully
    if load_result == 0:
        emuander_so.get_computer_state.restype = ctypes.POINTER(NeanderComputerState)
        # Step result will be 0 when the program finishes
        step_result = 1
        # Run steps till the end
        while step_result != 0:
            # Set function argument type
            step_result = emuander_so.run_step()
            # Print the registers at each step
            state_p = emuander_so.get_computer_state()
            print(state_p.contents)

        # Save program
        # Set function argument type
        emuander_so.save_memory_file.argtypes = [ctypes.c_char_p]
        # Set input memory
        result = emuander_so.save_memory_file(output_memory_path.encode('utf-8'))
    else:
        result = 1

    return result


# Input path
input_path = "F:\\Projects\\Emuander\\examples\\NeanderMemExample.MEM"
# Output path
output_path = "F:\\Temp\\ran.MEM"

# Test code
shared_lib_path = "./Emuander.so"
if platform.startswith('win32'):
    shared_lib_path = "./../../Output/Binary/x64/Release/Emuander Windows.dll"
try:
    emuander_so = ctypes.CDLL(shared_lib_path)
    print("Successfully loaded ", emuander_so)

    emulation_result = execute_neander(input_path, output_path, emuander_so)

    # Parses result
    if emulation_result == 0:
        print("Neander ran successfully!")
    else:
        print("Error occurred!")

except Exception as e:
    print(e)