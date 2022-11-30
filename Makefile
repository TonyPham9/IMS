CC=g++
CFLAGS=-pedantic -Wall -Werror -std=gnu99 -lsimlib -g
FILE=main
COMPILE_FILES=main.cpp

all:
	$(CC) $(FLAGS) $(COMPILE_FILES) -o $(FILE) -lsimlib