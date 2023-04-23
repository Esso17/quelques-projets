#include "polynome.hpp"
#include <iostream>

using namespace std;

int main(){
//Première partie
	Polynome<double> q;
	std::cout << "Degre du polynome construit par defaut : " << q.degre() << endl;
			
	vector<int> v1{6, 3, 0, 1, 5};
	Polynome<int> p1(v1);

    vector<int> v2{1,0,1};
    Polynome<int> p2(v2);
    
    
	std::complex<double> a (2.0,1.0);
	std::complex<double> b (0.0,1.0);
	vector<std::complex<double>> vc{a,b};
	
	Polynome<std::complex<double>> pc(vc);
	
	std::complex<int> one = 1;
	std::cout << "Is one one : " <<  is_one(one) << endl;
	
	std::cout << is_zero(a) << endl;
	affiche_monome(std::cout, a, 3);
	std::cout << std::endl;
	
	std::cout << "p1 : " << p1 << std::endl;
	
//Deuxième partie
	// Somme, différénce, produit
	Polynome<int> sum = p1+p2;
	Polynome<int> diff = p1-p2;
	Polynome<int> prod = p1*p2;
	std::cout << "Somme : " << sum << std::endl;
	std::cout << "Différence : " << diff << std::endl;
	std::cout << "Produit : " << prod << std::endl;
	
	// Division et reste
	Polynome<int> div = p1/p2;
	Polynome<int> reste = p1%p2;
	std::cout << "Quotient : " << div << std::endl;
	std::cout << "Reste : " << reste << std::endl;

	// Evaluation en un point
	std::cout << "p1(2) : "  <<  p1(2) << std::endl << "p2(3) " << p2(3) << std::endl; 
	
	return 0;	
}
