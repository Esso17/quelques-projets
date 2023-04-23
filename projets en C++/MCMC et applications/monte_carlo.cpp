#include "monte_carlo.hpp"


void Histogramme::operator+=(double x) {
	if ( (x > lbound) || (x<= ubound) ){
		unsigned k=floor((x-lbound)/box_width );
			// grâce au test, k est garanti d'être un indice valide.
		echantillon[k]+= 1.;
	}
	return;
}

void Histogramme::operator/=(double k) {
	for (int i = 0; i < echantillon.size(); i++)
	{
		echantillon[i] /= k;
	}
	/* ou bien, pour les aficionados de <algorithm> (ce que je vous
	 * encourage à devenir):
	 * 		std::transform(
	 * 			echantillon.begin(),echantillon.end(),
	 * 			echantillon.begin(),
	 * 			[k](double x) { return x/k; }
	 * 		);
	 * */
	return;
}

std::ostream & operator<< (std::ostream & o, const Histogramme & H) {
	for (int i = 0; i < H.echantillon.size(); i++)
	{
		o << H.lbound+H.box_width*0.5+i*H.box_width << "\t" << H.echantillon[i] << "\n";
	}
	return o;
}
