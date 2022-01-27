#include <vector>
#include <map>
#include <set>
//#include<bits/stdc++.h> 

using namespace std;

class SystemVariables{

    public:
    //SYSTEM VARIABLES  /initial configurations)
    int population_size;
    double defector_ratio;  //which ratio of the agents are defectors
    double coop_rate_coop;   //cooperation rate of cooperators
    double coop_rate_def;   //cooperation rate of defectors
    double mem_ratio;  //ratio of the agents to keep in mind
    int forgetting_strategy;    //0=fc 1=fr gerekirse eklenir
    bool optimal_recommendations;   //recommendations giving the actual trait of the opponent
    bool self_assured_defectors;
    bool self_assured_cooperators;
    int optimism_value_dist;   //0=uniform 1=normal
    double mean_optimism_threshold;
    map <char, int> payoff;
    bool recom_in_memory;   //recommendations are kept in memory also
    int lifetime;   //number of times that agent pairs ecnounter each other in one round
    int num_of_generations;   //number of rounds
    double mutation_rate;   //possiblity of a mutation
    int number_of_realizations;
    int number_of_toleration;   //number of games to play to with an opponent to reach a consequence


    

    SystemVariables();
    map<char,int> createPayoffMatrix(int S, int P, int R, int T);

    int random(int lower, int upper);
    int randomElement(set<int>&s);
    void sortPairVector(vector< pair<int, double> >& v);
    double mean(vector<double>&v);

};