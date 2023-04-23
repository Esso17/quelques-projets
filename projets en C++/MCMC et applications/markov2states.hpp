
#ifndef MARKOV_TWO_STATES
#define MARKOV_TWO_STATES

class Markov2states {
	protected:
		int x;
		std::bernoulli_distribution Ua;
		std::bernoulli_distribution Ub;
	public:
		Markov2states(int x0=1,double a0=0.5,double b0=0.5): x(x0), Ua(a0), Ub(b0) {};
		template <class RNG> int operator()(RNG & G); // ne doit pas être const car x évolue
};

template <class RNG>
int Markov2states::operator ()(RNG & G) {
	if ( x== 1)
	{
		if ( Ua(G) ) x=2;	
	}
	else {
		if( Ub(G) ) x=1;
	}
	// la bonne loi est choisie selon la valeur initiale de x
	return x;
}

class Stat2states {
	protected:
		long unsigned visit1;
		long unsigned visit2;
		long unsigned normalisation; 
			
	public:
		Stat2states(): visit1(0), visit2(0), normalisation(1) {}
		void operator+=(int x) { if( x==1) visit1++; else visit2++; }
		void operator/=(long unsigned n) { normalisation *= n;} 
			
		double frequency_of_visits(int x) const { 
				if( x==1) return double(visit1)/double(normalisation); 
				else return double(visit2)/double(normalisation); 
			}
			/* c'est ici qu'on procède à la conversion finale en double	*/
};
#endif
