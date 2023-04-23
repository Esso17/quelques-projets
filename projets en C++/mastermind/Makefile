
all: test.exe

mastermind.o: mastermind.hpp mastermind.cpp
	g++ -c mastermind.cpp -O2 -std=c++11 -Wall -pedantic
	
test.exe: mastermind.o test_mastermind.cpp
	g++ mastermind.o test_mastermind.cpp -O2 -std=c++11 -o test.exe -Wall -pedantic

