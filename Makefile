# ENCE260 embedded systems project
# Group 108
# Drogo Shi, Lucas Payne
#
# Makefile. Based partly off of Michael Hayes' given makefile.

# Root location of the AVR files relative to this makefile
DEPTH=../..

# Relevant files (everything else should not be committed)
GAME_HEADERS = game.h input.h utility.h graphics.h gametasks.h
GAME_SOURCES = game.c input.c utility.c graphics.c gametasks.c
GAME_OBJECTS = input.o utility.o graphics.o gametasks.o
FILES = $(GAME_HEADERS) $(GAME_SOURCES) Makefile README create_bitmaps.py create_gameobjects.py play.sh

# Michael Hayes' AVR library
AVR_DRIVERS = $(DEPTH)/drivers/avr
DRIVERS = $(DEPTH)/drivers
UTILS = $(DEPTH)/utils
AVR_OBJECTS  = pio.o system.o timer.o pacer.o ledmat.o display.o navswitch.o ir_uart.o timer0.o usart1.o prescale.o

# Commands and flags
CC=avr-gcc
CFLAGS=-mmcu=atmega32u2 -Os -Wall -Wextra -g -I$(DRIVERS) -I$(AVR_DRIVERS) -I$(UTILS)
OBJCOPY=avr-objcopy
SIZE=avr-size
DEL=rm

# Compile an object given this format for the rule:
#   Compiled object is the rule name
#   Leftmost name in rule is the main C file to be compiled
#   The rest are 
COMPILE_OBJECT = $(CC) -c $(CFLAGS) $< -o $@


# Play the game
.PHONY: play
play: clean bitmaps gameobjects game.hex play.sh
	sh play.sh

# play: game.hex bitmaps
#	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash game.hex; dfu-programmer atmega32u2 start


# Create the gameobjects
.PHONY: gameobjects
gameobjects: create_gameobjects.py
	python3 create_gameobjects.py

# Create the bitmaps
.PHONY: bitmaps
bitmaps: create_bitmaps.py
	python3 create_bitmaps.py


# Create hex file for programming from executable file.
game.hex: game.out
	$(OBJCOPY) -O ihex game.out game.hex


# Create the executable game, and view the size of the linked object.
game.out: game.o $(AVR_OBJECTS) $(GAME_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@

game.o: game.c
	$(COMPILE_OBJECT)
input.o: input.c input.h
	$(COMPILE_OBJECT)
utility.o: utility.c utility.h
	$(COMPILE_OBJECT)
graphics.o: graphics.c graphics.h utility.h game.h
	$(COMPILE_OBJECT)

pio.o: $(AVR_DRIVERS)/pio.c
	$(COMPILE_OBJECT)
ledmat.o: $(DRIVERS)/ledmat.c
	$(COMPILE_OBJECT)
system.o: $(AVR_DRIVERS)/system.c
	$(COMPILE_OBJECT)
timer.o: $(AVR_DRIVERS)/timer.c
	$(COMPILE_OBJECT)
pacer.o: $(UTILS)/pacer.c
	$(COMPILE_OBJECT)
display.o: $(DRIVERS)/display.c
	$(COMPILE_OBJECT)
navswitch.o: $(DRIVERS)/navswitch.c
	$(COMPILE_OBJECT)
ir_uart.o: $(AVR_DRIVERS)/ir_uart.c
	$(COMPILE_OBJECT)
timer0.o: $(AVR_DRIVERS)/timer0.c
	$(COMPILE_OBJECT)
usart1.o: $(AVR_DRIVERS)/usart1.c
	$(COMPILE_OBJECT)
prescale.o: $(AVR_DRIVERS)/prescale.c
	$(COMPILE_OBJECT)


# Utilities
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex

# Lazily commit relevant files to eng-git
.PHONY: commit
commit:
	git commit $(FILES)

# Add all resources to the index
.PHONY: git_resources
git_resources:
	git add bitmaps/*
	git add gameobjects/*
