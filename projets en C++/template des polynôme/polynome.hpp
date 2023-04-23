#include <iostream>
#include <vector>
#include <cmath> // pour abs, pow
#include <complex>
#include <type_traits> // pour vérifier le type dans affiche_monome
#include <algorithm> // pour transform pour la somme
#include <numeric> // pour accumulate
#include <tuple>
#include <functional> // pour std::plus


// 1. template function elem neutre somme
template <typename T>
bool is_zero(T a){
    return (a==0);
}

template <>
bool is_zero(double a){
    return (std::abs(a)<1.e-5);
}
// ceci est spécialisation complète du template

template <typename T>
bool is_zero(std::complex<T>c){
    return is_zero<T>(c.real()) && is_zero<T>(c.imag());
}

template <typename T>
bool is_one(T a){
    return (a==1);
}

template <>
bool is_one<double>(double a){
    return (std::abs(a-1.)<1.e-5);
}

// overload pour les complexes
template <typename T>
bool is_one(std::complex<T> c){
    return is_one<T>(c.real()) && is_zero<T>(c.imag());
}

template <typename T> class Polynome;
template <typename T> std::ostream & operator<<(std::ostream & , const Polynome<T> &);
template <typename T> Polynome<T> operator+(const Polynome<T> &, const Polynome<T> &);
template <typename T> Polynome<T> operator-(const Polynome<T> &, const Polynome<T> &);
template <typename T> Polynome<T> operator*(const Polynome<T> &, const Polynome<T> &);
template <typename T> Polynome<T> operator/(const Polynome<T> &, const Polynome<T> &);
template <typename T> Polynome<T> operator%(const Polynome<T> &, const Polynome<T> &);

template <class T> 
struct result_euclid_div {
	Polynome<T> quotient;
    Polynome<T> remainder;
};

// 2. template class
template <typename T>
class Polynome {
    private:
        int n; // degree = coeff.size()-1
        std::vector<T> coeff; //coefficients

        // extend (utilisé pour les op de + et -)
        Polynome extend(int) const; 

        // pas de 0 à la fin de coeff (appel après opérations)
        void adjust();

    public:
        // 2. template class  constructeurs
        Polynome() : n(-1), coeff(0) {}
        Polynome(const T& a, int m=0): n(m), coeff(m+1,0) {coeff[n]=a; adjust();} // le monome a*x^n
        Polynome(const std::vector<T>& v): n(v.size()-1), coeff(v) {adjust();}
        Polynome(std::vector<T>&& v): n(v.size()-1), coeff(std::move(v)) {  std::cout << "Déplacement !\n";} // hors programme mais plus efficace

        // 3. accesseurs et mutateurs
        int degre() const {return n ;} 
        T operator[](int i) const {return (i>=0 && i <= n) ? coeff[i] : 0;} // accesseur
        
        T dominant() const { return coeff[n];} // coefficient de plus haut degré

		template <typename U>
        T operator()(const U&); // evaluation en un point
        
        friend std::ostream & operator<< < >(std::ostream & , const Polynome<T> &);

        friend Polynome<T> operator+ <T>(const Polynome<T> &, const Polynome<T> &);
        friend Polynome<T> operator- <T>(const Polynome<T> &, const Polynome<T> &);
        friend Polynome<T> operator* <T>(const Polynome<T> &, const Polynome<T> &);
        friend Polynome<T> operator/ <T>(const Polynome<T> &, const Polynome<T> &);
        friend Polynome<T> operator% <T>(const Polynome<T> &, const Polynome<T> &);
};

// les deux méthodes privées.
template <typename T>
void Polynome<T>::adjust()
{
  while ((n >= 0) && (is_zero(coeff[n])))
    n--;
  coeff.resize(n+1);
}

template <typename T>
Polynome<T> Polynome<T>::extend(int m) const
{
    std::vector<T> coeffs = coeff;
    coeffs.resize(std::max(n,m)+1,0);
    return Polynome<T>(coeffs);
   
}

template <typename T>
void affiche_monome(std::ostream & s, const T & a, int n)
{
    char var;

    // on pourrait également écrire une spécification de cette méthode pour les complexes
    if constexpr (std::is_same<T, std::complex<float>>::value || std::is_same<T, 
                 std::complex<double>>::value || std::is_same<T, std::complex<long double>>::value)
        var = 'z';
    else
        var = 'x';

    s << a;
        
    if(n>1)
        s << "*" << var << "^" << n  << " ";
    else if (n==1)
        s << "*" << var;
}

template <typename T>
std::ostream & operator<<(std::ostream & out, const Polynome<T> & p) {
    if (p.n <0) return out;
    for (int i=p.n; i>0; --i){
        if(!is_zero(p.coeff[i])){
            affiche_monome(out, p.coeff[i], i);
            out << " + " ;
        }
    }
    if(!is_zero(p.coeff[0]))
        out << p.coeff[0];

    return out;
}

template<typename T>
Polynome<T> operator+(const Polynome<T> & p, const Polynome<T> & q){
    int m = std::max(p.n, q.n);
    Polynome<T> p_ext = p.extend(m);
    Polynome<T> q_ext = q.extend(m);
    Polynome<T> P;
    P.coeff.resize(m+1);
    P.n = m;
    std::transform(p_ext.coeff.begin(), p_ext.coeff.end(), q_ext.coeff.begin(), P.coeff.begin(), [](const T& t1, const T& t2){return t1+t2;});
    P.adjust();
    return P;

}

template<typename T>
Polynome<T> operator-(const Polynome<T> & p, const Polynome<T> & q){
    auto polynome_ref_pair =  std::minmax(p,q, [](const Polynome<T> & u,const Polynome<T> & v) { return u.degre()<v.degre(); });
    Polynome<T> result(polynome_ref_pair.second); // copie du plus grand dans le résultat.
    if( & polynome_ref_pair.first == &p ) {// pour trouver qui est celui qui porte le - entre le min et le max!
        std::transform(
            polynome_ref_pair.first.coeff.begin(), polynome_ref_pair.first.coeff.end(),
            result.coeff.begin(),
            result.coeff.begin(),
            std::minus<T>()); // l'ordre des arguments permet de ne parcourir que jusqu'au degré du plus petit polynôme.
    } else {
        std::transform(
            polynome_ref_pair.first.coeff.begin(), polynome_ref_pair.first.coeff.end(),
            result.coeff.begin(),
            result.coeff.begin(),
            std::minus<T>()); // l'ordre des arguments permet de ne parcourir que jusqu'au degré du plus petit polynôme.
    }
    result.adjust();
    return result;
}

template<typename T>
Polynome<T> operator*(const Polynome<T> &p, const Polynome<T> & q) {
    if (p.n <0 || q.n <0) {
        return Polynome<T>();
    }
    int m = p.n + q.n;
    std::vector<T> coeffs(m+1);
    for(int i=0; i<= p.degre(); ++i) {
        for(int j=0; j <= q.degre(); ++j) {
            coeffs[i+j] += p[i]*q[j];
        }
    }
    return Polynome<T>(std::move(coeffs)); 
}

template<typename T>
result_euclid_div<T> euclid_div(const Polynome<T>& a, const Polynome<T>& b) {
    Polynome<T> q; // construit par défaut -> 0
    Polynome<T> r(a);

    while (r.degre() >= b.degre()) {
        int n = r.degre()-b.degre();
        Polynome<T> q1 (r.dominant()/b.dominant(), n); // attention à rester dans T !!
        r = r-b*q1;
        q = q+q1;
    }

    return result_euclid_div(q,r);
}

template<typename T>
Polynome<T> operator/(const Polynome<T> &p, const Polynome<T> &q) {
    return euclid_div(p,q).quotient;
}


template<typename T>
Polynome<T> operator%(const Polynome<T> &p, const Polynome<T> &q) {
    return euclid_div(p,q).remainder;
}

template<typename T>
template<typename U>
T Polynome<T>::operator()(const U& x){

    if (n>0){
        auto horner = [&x](T s, T a) {return x*s+a;};
        T res = std::accumulate(std::next(coeff.rbegin()), coeff.rend(), coeff[n], horner);
        return res;
    }
    else if (n==0) return coeff[0];
    else return 0;
}
