#include "monte_carlo.hpp"
#include "chi2.hpp"
#include "pi.hpp"
#include "markov2states.hpp"
#include "MCMC.hpp"
#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <fstream>
#include <random>

int main(){
	long unsigned int nb_samples = 1000000;
	std::mt19937 G(time(NULL));

//Estimation de pi
	double pi_approx=0.;
	auto fois4= [](double x) { return 4*x;};
	SquareInDisk A;

	MonteCarlo(pi_approx,A,fois4,G,nb_samples);
        /*****
         * fois4 pourrait etre aussi :
         * - une fonction globale définie hors du main:
         *   double fois4(double x) { return 4*x;}
         * - un objet d'une classe (par exemple Mult4) qui aurait
         *   un operateur d'évaluation :
         *   class Mult4 {
         *      public:
         *          double operator()(double x) { return 4*x;}
         *   };
         *   ...
         *   Mult4 fois4;
         *   Rem : le 4 pourrait etre remplacé par k en champ privé ou en
         *   paramètre de template...
         *   */
	std::cout << "Valeur moyenne de Pi : " << pi_approx << "\n-----------------------------------\n";


//Calcul de l'estimation de pi et son intervalle de confiance:
	DoubleMeanVar pi_mv;

	MonteCarlo(pi_mv,A,fois4,G,nb_samples);
	std::cout << "Valeur moyenne de Pi : " << pi_mv.mean() << " +/- " << 1.96*sqrt(pi_mv.variance())/sqrt(nb_samples) << "\n-----------------------------------\n";

//Histogramme de la loi normale
	double a=-3.;
	double b=3.;
	double boxes=50;
	Histogramme Hnormal(a,b,boxes);
	std::normal_distribution<double> N(0.,1.);

	MonteCarlo(Hnormal,N,[](double x){return x;},G,nb_samples);
	std::ofstream file("normal_histo.dat");
	file << Hnormal;
	file.close();
	/* on affiche sur gnuplot via la commande suivante :
	plot 'normal_histo.dat' using 1:2 with boxes
	*/

//Histogramme de la loi du chi-deux;
	// Paramétrage:
	a=0.;
	b=10.;
	boxes=50;
	Chi2_distribution<double,3> chi2_3;
			/* Lors de la déclaration d'une variable d'un template de classe,
			 * c'est le seul cas où l'instanciation du template doit être 
			 * explicitement faite (contrairement à l'usage de MonteCarlo,
			 * ou de templates de méthodes)
			 * */
	Histogramme Hchi2(a,b,boxes);
	
	// Calcul:
	MonteCarlo(Hchi2,chi2_3,[](double x){return x;},G,nb_samples);
	
	// Sauvegarde:
	file.open("chi2_histo.dat");
	file << Hchi2;
	file.close();
	
	// Paramétrage
	Chi2_distribution<double,6> chi2_6;
	Histogramme Hchi2prime(a,b,boxes);
	
	//Calcul:
	MonteCarlo(Hchi2prime,chi2_6,[](double x){return x;},G,nb_samples);
	
	// Sauvegarde:
	file.open("chi2_histo2.dat");
	file << Hchi2prime;
	file.close();

//Estimation d'une intégrale simple int_0^1 ln(1+x^2)dx
	
	std:: uniform_real_distribution<double> X0(0.,1.);
        // par la méthode du rejet dans le carré [0,1]x[0,1]
        // f : indicatrice d'etre sous le graphe de x->log(1+x^2)
	auto f = [](std::pair<double,double> p){return double(p.second<=log(1+p.first*p.first));};
	auto CoupleXY = [&](std::mt19937 & G) { return std::make_pair( X0(G), X0(G) );};
	DoubleMeanVar u0;
	MonteCarlo(u0,CoupleXY,f,G,nb_samples);
        // ou directement comme E(log(1+X0^2))
        // MonteCarlo(u0, X0, [](double x){return log(1+x*x);},G,nb_samples);
	std::cout << "Estimation de l'integrale: " << u0.mean() << "+/-" << 1.96*sqrt(u0.variance())/sqrt(nb_samples) << "\n-----------------------------------\n";

//Estimation d'une intégrale double int_{R_+ x [0,1]} ln(1+xy) e^(-x) dxdy
	std::exponential_distribution<double> X(1.);
	std::uniform_real_distribution<double> Y(0.,1.);
	auto CoupleXY2 = [&](std::mt19937 & G) { return std::make_pair( X(G), Y(G) );};
	auto Function_to_evaluate = [](std::pair<double,double> p) { return log(1.+p.first*p.second);};
	/*	en C++14, on a même le droit à:
	 *
	 * auto CoupleXY = [&](auto & G) { return std::make_pair( X(G), Y(G) );};
	 * auto Function_to_evaluate = [](auto p) { return log(1.+p.first*p.second);};
	 *
	 * et la détermination de tous les types est faite par le compilateur.
	 * */
	DoubleMeanVar u;
	MonteCarlo(u,CoupleXY2,Function_to_evaluate,G,nb_samples);
	std::cout << "Estimation de l'integrale: " << u.mean() << " +/- " << 1.96*sqrt(u.variance())/sqrt(nb_samples) << "\n-----------------------------------\n";

//Chaîne de Markov à deux états
	/* On fait varier a et b entre 0 et 1 par pas de 0.1
     * et on affiche les mesures invariantes associées dans un fichier
     * sous forme de matrice dont le coefficient (i,j) correspond à pi(a_i,b_j)
     * */
	double aa=2./3.;
	double bb=1./2.;
	Markov2states Z(1,aa,bb);
	Stat2states V;
	MonteCarlo(V,Z,[](auto x){return x;},G,nb_samples);
	std::cout << "Estimation de la mesure invariance par theoreme ergodique:\n";
	std::cout << "\tpi(1): " << V.frequency_of_visits(1) << " (valeur attendue: 0.4286 )\n";
	std::cout << "\tpi(2): " << V.frequency_of_visits(2) << " (valeur attendue: 0.5714 )\n";
	std::cout << "-----------------------------------\n";

//Ising 1D
	
	unsigned N0 = 1000;
	auto x500 = [](const std::vector<int> & v){return v[499];};
	std::cout << "Modele d'Ising: quelques valeurs selon beta et h:\n";
	for (double beta = 0.; beta < 2.; beta+=0.4)
	{
		for (double h = 0.; h < 1.; h += 0.2)
		{
			Ising1D Ising(N0,beta,h);
			double m=0.;
			MonteCarlo(m,Ising,x500,G,nb_samples);
			std::cout << "\tbeta= " << beta << "\th= " << h << "\tmoyenne de x_500= "<< m << "\n";
		}
	}
	std::cout << "-----------------------------------\n";

}
