CC=g++
CFLAGS=-pedantic -Wall -Werror -std=gnu99 -lsimlib -g
FILE=warehouse
COMPILE_FILES=warehouse.cpp

all:
	$(CC) $(FLAGS) $(COMPILE_FILES) -o $(FILE) -lsimlib