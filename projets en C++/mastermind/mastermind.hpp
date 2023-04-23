#ifndef MASTERMIND
#define MASTERMIND

#include <vector>
#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>


struct Response{
	int rr;
	int rw;
};

class Combination{
private:
    int n; 
    std::vector<int> values; 
    
public:
    // Constructeurs
    Combination(): n(0), values(std::vector<int>()){};
    Combination(const std::vector<int>& _values): n(_values.size()), values(_values){}; 
    
    // Accesseurs
    int size() const {return n;};
    int operator[](int i) const {return values[i];}; 
    
	// Entrées-sorties:
	friend std::ostream& operator<<( std::ostream&, const Combination&);
	friend std::istream& operator>>( std::istream&, Combination&);
    
    // Compteurs
    std::set<int> unique_values() const;
    int count_value(int) const;
    int count_right_place(int, const Combination&) const;
    int count_rr(const Combination&)  const; 
    int count_rw(const Combination&)  const; 
    
    // Jeu
    Response one_try(const Combination&) const;
    bool won(const Combination&) const;
};


class Mastermind{
private:
	int n_guess_max;
	int n_guess_count;
	Combination hidden;
	
public:
	Mastermind(int M, const Combination& C): n_guess_max(M), n_guess_count(0), hidden(C){};
	
	Combination user_guess() const;
	bool play();
};

#endif
