#include "SystemVariables.h"
#include <stdlib.h>
#include <time.h>
#include <cmath>


SystemVariables::SystemVariables(){
    
    //SYSTEM VARIABLES  (initial configurations)
    this->population_size=100;
    this->defector_ratio=0.5;  //which ratio of the agents are defectors
    this->coop_rate_coop=0.9;   //cooperation rate of cooperators
    this->coop_rate_def=0.1;   //cooperation rate of defectors
    this->mem_ratio=0.3;  //ratio of the agents to keep in mind
    this->number_of_toleration=1;   
    this->mean_optimism_threshold=0.3;
    this->forgetting_strategy=0;    //0=fc 1=fr  2=fd gerekirse eklenir
    this->optimal_recommendations=false;   //recommendations giving the actual trait of the opponent
    this->self_assured_defectors=true;   //defectors not taking recommendations
    this->self_assured_cooperators=false;
    this->optimism_value_dist=1;   //0=uniform 1=fixed  2=uniform in fixed interval (0.2-0.6)
    this->payoff = createPayoffMatrix(0,1,3,5);    //0,1,3,5   or -7,-6,4,5
    this->recom_in_memory=false;   //recommendations are kept in memory also  
    this->lifetime=30;   //number of times that agent pairs ecnounter each other in one round
    this->number_of_realizations=20;

}


map<char,int> SystemVariables::createPayoffMatrix(int S, int P, int R, int T){
    map<char,int> m;
    m['S']=S; m['P']=P; m['R']=R; m['T']=T;
    
    return m;
}

int SystemVariables::random(int lower, int upper){   //lower, upper inclusive
    
    double X=((double)rand()/(double)RAND_MAX);
    X=((double)rand()/(double)RAND_MAX);
    return (X==1.0) ? (lower+ floor(X*(upper-lower+1))-1) : (lower+ floor(X*(upper-lower+1)));
}

int SystemVariables::randomElement(set<int>&s){   //returns random element from given set

    set<int>::iterator it = s.begin();
    int random = rand() % s.size();
    std::advance(it, random);
    return*it;

}


bool sortbysec(const pair<int, double> &a, const pair<int, double> &b) { 
    return (a.second > b.second); 
} 


//sorts given pair vector according to the second column with descending order
void SystemVariables::sortPairVector(vector< pair<int, double> >& v){
    sort(v.begin(), v.end(), sortbysec); 
}

double SystemVariables::mean(vector<double>&v){
    if(v.size()==0){
        return 0;
    }
    double sum=0.0;
    for(int i=0;i<v.size();i++){
        sum+=v[i];
    }
    return sum/v.size();
}