DEVICE     = atmega328p
CLOCK      = 7372800
PROGRAMMER = -c usbtiny -P usb

FUSES      = -U hfuse:w:0xd9:m -U lfuse:w:0xe0:m

# ADD NEW FILES TO BE COMPILED TO THE FOLLOWING 2 LINES
OBJECTS_CONTROL    = $(OBJ)/Serial.o $(OBJ)/I2C.o $(DRIVER_OBJECTS) $(OBJ)/control-main.o
OBJECTS_MINION 	   = $(OBJ)/Serial.o $(OBJ)/I2C.o $(DRIVER_OBJECTS) $(OBJ)/minion-main.o
DRIVER_OBJECTS = $(DRIVERS)/DS1307/DS1307.o $(DRIVERS)/KEYPAD/KEYPAD.o

SRC = src
INC =-Idrivers/ATMEGA328P -Idrivers/DS1307 -Idrivers/KEYPAD -Idrivers/XBEE
BUILD = build
OBJ = obj

DRIVERS = drivers

DIRS = $(BUILD) $(OBJ)
# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -Wl,-Map=$(BUILD)/myprog.map -Wl,--cref $(INC)

# symbolic targets:
all: $(DIRS) control-main.hex minion-main.hex

$(BUILD):
	mkdir $(BUILD)

$(OBJ):
	mkdir $(OBJ)

$(OBJ)/%.o : $(SRC)/%.c
	$(COMPILE) -c $< -o $@

$(DRIVERS)/DS1307/%.o : $(DRIVERS)/DS1307/*.c
	$(COMPILE) -c $< -o $@

$(DRIVERS)/KEYPAD/%.o : $(DRIVERS)/KEYPAD/*.c
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

# Flash the control device
flash-control: all 
	$(AVRDUDE) -U flash:w:$(BUILD)/minion-main.hex:i

# Flash the minion device
flash-min: all 
	$(AVRDUDE) -U flash:w:$(BUILD)/control-main.hex:i


# old flash target
# flash:	all
# 	$(AVRDUDE) -U flash:w:$(BUILD)/main.hex:i

# run this otherwise runs at 1/8 frequency !!!!!!!!!!!!!!!!
fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -rf $(DIRS) $(DRIVER_OBJECTS)
	
# main.hex main.elf 

control-main.elf: $(OBJECTS_CONTROL)
	$(COMPILE) -o $(BUILD)/control-main.elf $(OBJECTS_CONTROL)

control-main.hex: control-main.elf 
	rm -f $(BUILD)/control-main.hex
	avr-objcopy -j .text -j .data -O ihex $(BUILD)/control-main.elf $(BUILD)/control-main.hex
	avr-size --format=avr --mcu=$(DEVICE) $(BUILD)/control-main.elf

minion-main.elf: $(OBJECTS_MINION)
	$(COMPILE) -o $(BUILD)/minion-main.elf $(OBJECTS_MINION)

minion-main.hex: minion-main.elf 
	rm -f $(BUILD)/minion-main.hex
	avr-objcopy -j .text -j .data -O ihex $(BUILD)/minion-main.elf $(BUILD)/minion-main.hex
	avr-size --format=avr --mcu=$(DEVICE) $(BUILD)/minion-main.elf
	
# # file targets:
# main.elf: $(OBJECTS)
# 	$(COMPILE) -o $(BUILD)/main.elf $(OBJECTS)

# main.hex: main.elf
# 	rm -f $(BUILD)/main.hex
# 	avr-objcopy -j .text -j .data -O ihex $(BUILD)/main.elf $(BUILD)/main.hex
# 	avr-size --format=avr --mcu=$(DEVICE) $(BUILD)/main.elf


# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf
