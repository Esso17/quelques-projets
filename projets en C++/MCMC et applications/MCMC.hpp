#ifndef MCMC_HPP_INCLUDED
#define MCMC_HPP_INCLUDED

#include <random>
#include <iostream>
#include <cmath>

class Ising1D {
	protected:
		std::vector<int> spin;
		double beta;
		double h;
		std::uniform_int_distribution<int> random_index;
	public:
		Ising1D(unsigned int N0, double beta0, double h0): spin(N0,1), beta(beta0), h(h0),random_index(0,N0-1) {}
		template<class RNG> const std::vector<int> & operator()(RNG & G);
		
		const std::vector<int> & state() const { return spin;}
		

template<class RNG>
const std::vector<int> & Ising1D::operator()(RNG & G){
    unsigned int k = random_index(G);
    double p;
    if(k==0){	//sur le bord gauche
        p = exp(-2.*beta*(spin[k+1])*spin[k]-2*h*spin[k]);
    }
    else if(k==spin.size()-1){	// sur le bord droit
        p = exp(-2.*beta*(spin[k-1])*spin[k]-2*h*spin[k]);
    }
    else{	// au milieu
        p = exp(-2.*beta*(spin[k-1]+spin[k+1])*spin[k]-2.*h*spin[k]);
    }
    if(p>1.){	//calcul du min avec 1
        p=1.;
    }
    std::bernoulli_distribution Up(p);
    if(Up(G)){
        spin[k]= -spin[k];
    }
    return spin;
   
}


#endif
