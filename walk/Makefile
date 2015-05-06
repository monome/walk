CC=avr-gcc
CFLAGS=-g -Os -Wall -mcall-prologues -mmcu=attiny24
ALL_CFLAGS = -I. $(CFLAGS)
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref	
OBJ2HEX=avr-objcopy 


####### Files:

SOURCES       = walk.c 

OBJECTS	      = walk.o 

TARGET=	walk


####### Implicit rules

.SUFFIXES: .c .o .cpp .cc .cxx .C


.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

%.elf:	$(OBJECTS)


####### Build rules

$(TARGET):	$(OBJECTS)
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)
		$(CC) $(ALL_CFLAGS) $(OBJECTS) --output $(TARGET).elf $(LDFLAGS)	
		$(OBJ2HEX) -R .eeprom -O ihex $(TARGET) $(TARGET).hex

erase:
	avrdude -p m325 -b 115200 -P usb -c avrispmkII -e 
	#avarice --erase --jtag /dev/tty.usbserial-1B1 --jtag-bitrate 500KHz

fuses: 
	avrdude -c avrisp2 -P /dev/tty.usbmodem00088031 -p t24 -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
	#avarice -2 --write-fuses ff99ff --jtag usb --jtag-bitrate 500KHz

flash: $(TARGET).hex
	avrdude -c avrisp2 -P /dev/tty.usbmodem00088031 -p t24 -e -V -D -U flash:w:$(TARGET).hex:i

####### Compile

phonon.o:		phonon.c
