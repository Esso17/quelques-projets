#ifndef MONTE_CARLO_HPP_INCLUDED
#define MONTE_CARLO_HPP_INCLUDED
#include <iostream>
#include <cmath>
#include <vector>

//=============================
//Annonce des classes, fonctions et templates
//=============================
class Histogramme;
class DoubleMeanVar;

std::ostream & operator<<(std::ostream &, const Histogramme &);

template <class Statistique, class RandomVariable, class Measurement, class RNG>
	void MonteCarlo(Statistique & res, RandomVariable & X, const Measurement & f, RNG & G, long unsigned int n);
//==========================
//Déclaration des templates
//==========================


//MonteCarlo
template <class Statistique, class RandomVariable, class Measurement, class RNG>
void MonteCarlo(Statistique & res, RandomVariable & X, const Measurement & f, RNG & G, long unsigned int n)
{
	// génération des échantillons:
    for(long unsigned int i=0; i<n; i++){
        res+=f(X(G));
    }
    res/=double(n); //normalisation par le nombre d'échantillons
    
};


//DoubleMeanVar
class DoubleMeanVar{
protected:
    double m;//Somme et Moyenne
    double v;//Somme et moyenne du second moment
public:
    DoubleMeanVar(double x=0.): m(x), v(x) {}
    double mean() const {return m;}	//accesseur à m
    double variance() const {return v-m*m;}//accesseur modifié à v
    void operator+=(double x){m+=x; v+=x*x; }
    void operator/=(double n){m/=n; v/=n; }
};

//Histogramme
class Histogramme{
protected:
    std::vector<double> echantillon; //statistiques par boîte
    unsigned int nb_boxes;	//nombre de boîtes
    double lbound;	//début de l'intervalle
    double ubound;	//fin de l'intervalle
    double box_width;	//taille d'une boîte

public:
    Histogramme(double min_intervalle, double max_intervalle,unsigned int n):
			echantillon(n,0.), nb_boxes(n),
			lbound(min_intervalle), ubound(max_intervalle),
			box_width( (max_intervalle-min_intervalle)/double(nb_boxes)) {}
    void operator+=(double);
    void operator/=(double);
    friend std::ostream & operator<< (std::ostream &, const Histogramme &);
};


//==========================================================================


#endif
