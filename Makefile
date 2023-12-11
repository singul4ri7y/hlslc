default_target: build

# Make sure TDM GCC is installed. Haven't tested with Cygwin.

CC          = g++
SRC_FILES   = src/Main.cpp
OBJ_FILES   = bin/intm/Main.cpp.obj
OUTPUT_FILE = bin/hlslc.exe

.PHONY: 

build: create_dir $(SRC_FILES)
	g++ $^ -ld3dcompiler -o $(OUTPUT_FILE)

bin/intm/Main.cpp.obj: $(SRC_FILES)
	g++ -c $< -O2 -o $@

src/Main.cpp: src/termcolor.hpp

clean: 
	rm -rf ./bin

create_dir:
	mkdir -p bin/intm
