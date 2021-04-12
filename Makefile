CXX=clang

all: compile

compile:
	$(CXX) -g -o riscvdasm -Wall *.c