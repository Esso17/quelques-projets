#ifndef PI_HPP_INCLUDED
#define PI_HPP_INCLUDED
#include <random>

class SquareInDisk
{
	private:
		std::uniform_real_distribution<double> U;
	public :
		SquareInDisk(): U(0.,1.) {}
		template <class RNG> double operator()(RNG & G);
};

template <class RNG> double SquareInDisk::operator()(RNG & G) {
	double x=U(G);
	double y=U(G);
	return (x*x+y*y < 1.); // conversion implicite de bool vers double ici.
}
#endif // PI_HPP_INCLUDED
