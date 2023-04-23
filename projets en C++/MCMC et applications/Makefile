all: simulations.exe

monte_carlo.o: monte_carlo.hpp monte_carlo.cpp 
	g++ monte_carlo.cpp -c

simulations.exe: monte_carlo.o simulations.cpp chi2.hpp monte_carlo.hpp markov2states.hpp MCMC.hpp
	g++ monte_carlo.o simulations.cpp -O2 -std=c++14 -o simulations.exe



