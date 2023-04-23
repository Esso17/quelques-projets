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
			/* le strict nécessaire pour être appelable dans MonteCarlo en deuxième argument */
		// accesseur à l'état de la chaîne:
		const std::vector<int> & state() const { return spin;}
		/* ATTENTION: UNE REMARQUE SUR LE TYPE DE RETOUR DE L'ACCESSEUR ET DE operator():
		 * 
		 * nous dérogeons à la règle disant que le prototype de l'accesseur est T accesseur() const car ici T est 
		 * un objet potentiellement très gros qui serait copié. Or, l'accesseur est là pour assurer la lecture seule. On peut
		 * donc sans problème mettre un prototype en const T & accesseur() const qui permet de donner accès au champ privé sans le recopier.
		 * 
		 * */
};

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
    /* Remarque: le calcul ci-dessus est directement tiré des formules mathématiques.
     * Néanmoins, une énorme perte d'efficacité a lieu:
     * 		- les trois cas du if/else if/else demande un calcul d'exponentielle pour calculer p
     * 		- or, ligne 42, on se rend compte que si p>1, ce calcul ne sert à rien.
     * Il aurait donc mieux valu retirer les exponentielles,
     * rester avec une variable logp, comparer celle-ci à 0, et, dans le cas où elle est 
     * négative, calculer son exponentielle pour l'utiliser ligne 45.
     * */
}




#endif
