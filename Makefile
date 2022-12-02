CC=g++
CFLAGS=-pedantic -Wall -Werror -std=gnu99 -lsimlib
FILE=main
COMPILE_FILES=main.cpp

all:
	$(CC) $(FLAGS) $(COMPILE_FILES) -o $(FILE) -lsimlib

run:
	for number in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 ; do \
            ./$(FILE) -B $$number ; \
    done