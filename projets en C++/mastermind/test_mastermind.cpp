#include "mastermind.hpp"

using namespace std;

int main(){
    
    // Constructeur par defaut
    Combination C1;
    cout << C1 << endl;
    
    vector<int> v = {2,0,1,2,4};
    Combination C2(v);
    cout << "La combinaison C2 : " << C2 << endl;
    
    /*
    cout << "C_test : " << endl;
    Combination C_test;
    cin >> C_test;
    cout << C_test << endl;
    */
    
    // Flux 
    Combination a_deviner;
    ifstream fichier("combinaison_4.dat");
	fichier >> a_deviner;
    fichier.close();
    
    cout << "La combinaison a deviner est : " ;
    cout << a_deviner << endl;
    
    // Méthodes
    Combination C3({2,2,4,3,1});
    cout << "La combinaison C3 : " << C3 << endl;
    cout << "Count rr : " << C2.count_rr(C3) << endl;
    cout << "Count 2 : " << C2.count_value(2) << endl;
    cout << "Count right place 2 : " << C2.count_right_place(2,C3) << endl;
    cout << "Count rw : " << C2.count_rw(C3) << endl;

    Mastermind game(10,a_deviner);
    
    bool won = game.play();
    
    if (won) cout << "Vous avez gagné." << endl;
    else cout << "Vous avez perdu." << endl;
		
    return 0;	
}
