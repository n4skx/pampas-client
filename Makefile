CXX := clang++
FLAGS := -ggdb

all:
	$(CXX) src/Main.cpp $(FLAGS) -o main.elf
