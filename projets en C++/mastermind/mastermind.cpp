#include "mastermind.hpp"

/*
#include <string>
#include <sstream>
*/

using namespace std;

ostream& operator<<( ostream& out, const Combination& C) {
	for (int i=0; i<C.size(); i++){
		out << C.values[i] << " ";
	}
	
	return out;
}

istream& operator>>( istream& in, Combination& C){
	vector<int> values;
	int val;
	while( in >> val)
		values.push_back(val);
	
	C.values = values;
	C.n = values.size(); 
	
	/*
	string line;
	getline(in,line);
	istringstream iss(line);
	
	vector<int> values;
	int val;
	while( iss >> val)
		values.push_back(val);
	
	C.values = values;
	C.n = values.size(); 
	*/
	
	return in;
}

int Combination::count_rr(const Combination& C) const{  
    int count = 0;
	
	for (int i=0; i<n; ++i){
		if (C[i] == values[i])
			count++;
	}
	
	return count;
}

set<int> Combination::unique_values() const{
	set<int> unique_vals;
    for (int i=0; i<n; ++i)
		unique_vals.insert(values[i]);
		
	return unique_vals;
}

int Combination::count_value(int c) const{
	return std::count(values.begin(), values.end(), c);	
}

int Combination::count_right_place(int c, const Combination& C) const {
	int counter = 0;
	for (int i=0; i<n; ++i){
		if	(C[i] == c && c == values[i])
			counter++;
	}
	return counter;
}

int Combination::count_rw(const Combination& C) const{    
    int count_wrong = 0;
    
    set<int> unique_vals = C.unique_values();
    
    for (auto it=unique_vals.begin(); it!=unique_vals.end(); ++it){
		int count_this = count_value(*it);
		int count_user = C.count_value(*it);
		int count_right = count_right_place(*it, C);
		
		count_wrong += min(count_this, count_user) - count_right;
	}

	return count_wrong;
}

Response Combination::one_try(const Combination& C) const{
	Response rep;
	rep.rr = count_rr(C);
	rep.rw = count_rw(C);
	
	return rep;
}

bool Combination::won(const Combination& C) const{
	Response rep = one_try(C);
	return n==rep.rr;
}

Combination Mastermind::user_guess() const{	
	cout << n_guess_count+1 << ") Enter " << hidden.size() << " integers for your guess: " ;
	
	/*
	Combination C;
	cin >> C;
	*/
	
	vector<int> v;
	int v_i;
	for (int i=0; i<hidden.size(); ++i){
		cin >> v_i; 
		v.push_back(v_i);
	}
	Combination C(v);
	
	return C;
}

bool Mastermind::play(){
	bool guessed = false;
	while (!guessed && n_guess_count != n_guess_max){
		Combination C(user_guess());
		Response rep = hidden.one_try(C);
		guessed = hidden.won(C);	
		
		cout << endl <<  "Answer : " << rep.rr << " rr and " << rep.rw << " rw" << endl;
		
		n_guess_count++;	
	}
	
	return guessed;
}
