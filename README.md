# capstone - Project Submitted on May 15th, 2022
Source code for capstone project for EE459 - __Team 19: Jackson S, Alice G, and Ziang Q.__

## To Compile & Flash this code - Jackson S.
+ __NOTE__: All commands in this section should be called from the top-level directory `capstone/`, `team19_capstone_code/` - it's where the Makefile is.
+ Compiling
    - Type `make` into terminal. It should compile both minion-main and control-main which have both respective main functions for either node.
    - This generates control-main.hex, control-main.elf, minion-main.hex, and minion-main.elf FYI.
+ Flash
    - Type `make flash-control` to flash the control node and `make flash-min` to flash the minion node.

# Notes for additions
## Adding Files to the Makefile - Jackson S.
+ When you create a new c file in the src/ directory, you should add them in the following way:
    - say you coded `file.c` - if you want to compile this new file and link it, then add `$(OBJ)/file.o` to the line with the OBJECTS variable in the makefile.
    - when done correctly it should look like this: `OBJECTS = $(OBJ)/file1.o $(OBJ)/main.o ...`

+ When you create a new c file implementation of a driver, in the drivers/DEVICE_NAME/ directory, you should add them to the makefile for compilation in the following way:
    - say you coded `KEYPAD.c` in `drivers/KEYPAD/KEYPAD.c`, then you should add `$(DRIVERS)/KEYPAD/KEYPAD.o` to the line with the `DRIVER_OBJECTS` variable.
    - when done correctly, similar to the above bullet point, it should look like this: `DRIVER_OBJECTS = $(DRIVERS)/KEYPAD/KEYPAD.o $(DRIVERS)/DS1307/DS1037.o ...`
    - You also need to add the target manually: 
        `$(DRIVERS)/DS1307/%.o : $(DRIVERS)/DS1307/*.c`
            `$(COMPILE) -c $< -o $@`
