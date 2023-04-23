#ifndef CHI_HPP_INCLUDED
#define CHI_HPP_INCLUDED
#include <random>

/* --- 	le paramètre REAL correspond au choix 
 * 			float/double/long double (pour laisser la décision à l'utilisateur,
 * 			comme par exemple pour les lois std::***_distribution de <random>
 * --- 	le paramètre entier k est celui de la loi. L'intérêt de le passer 
 * 		en paramètre de templates est la possibilité de laisser au compilateur
 *		le soin d'optimiser la boucle for ci-dessous.
 * */
template<class REAL, int k>
class Chi2_distribution
{
	private:
		std::normal_distribution<REAL> N;
	public :
		Chi2_distribution(): N(0.,1.) {}
		template <class RNG> REAL operator()(RNG & G);
		/* l'intérêt du template de méthode à l'intérieur du template
		 * de classe est la possibilité de déclarer une variable de type
		 * Chi2_distribution SANS DEVOIR immédiatement spécifier le type
		 * de générateur: autant de méthodes que de générateurs utilisés 
		 * seront compilées pour la même classe.
		 * */
};

template<class REAL,int k>
template <class RNG>	//double template de classe puis de méthode
REAL Chi2_distribution<REAL,k>::operator()(RNG & G) {
	REAL s(0.);
	REAL x;
	for (int i = 0; i < k; i++)
	{
		x=N(G);
		s+= x*x;
		/* attention de ne pas remplacer les deux lignes par s+=N(G)*N(G) 
		 * en croyant faire plus simple !!!
		 * */
	}
	return s;
}
#endif // CHI_HPP_INCLUDED
