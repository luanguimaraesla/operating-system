COMPILER := avr-gcc
SOURCE := main
MICROCONTROLLER := atmega328p
PORT := /dev/ttyUSB0

all: $(SOURCE).o $(SOURCE).elf $(SOURCE).hex 
	sudo avrdude -F -V -c arduino -p ATMEGA328P -P $(PORT) -b 57600 -U flash:w:$(SOURCE).hex

$(SOURCE).hex: $(SOURCE).o $(SOURCE).elf
	avr-objcopy -O ihex -R .eeprom $(SOURCE).elf $(SOURCE).hex

$(SOURCE).elf: $(SOURCE).o
	$(COMPILER) -mmcu=atmega328p $(SOURCE).o -o $(SOURCE).elf

$(SOURCE).o:
	$(COMPILER) -Os -DF_CPU=16000000UL -mmcu=$(MICROCONTROLLER) -c -o $(SOURCE).o $(SOURCE).c

.PHONY: clean
clean:
	rm -f *.hex
	rm -f $(SOURCE)
	rm -f *.o
	rm -f *.elf	
