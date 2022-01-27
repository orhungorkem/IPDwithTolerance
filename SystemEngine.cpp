#include <iostream>
#include <vector>
#include "Game.h"
#include <map>
#include <time.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>



using namespace std;


void totalsim(int forgetting);
void round(int round, int realization);
void oneRound(int realization);
map<int, Agent> initialPopulation();
void calculateMeanPayoff(int characteristic);
void calculateMeanThreshold();
void calculateMemoryUsage();
void calculateDecisionData();
void calculateRecommendationData();
void calculateSystemInfo();
void updateData();
void printData(int round, int realization);
void printLabels();
void sortByPayoff();



map<int, Agent> population;   //id object mappping for agents
SystemVariables sys;
set<int>cooperators;   //ids of cooperators
set<int>defectors;  //ids of defectors
SystemData sysdata;
ofstream myfile;






int main(){
    
    
    myfile.open ("results.csv", ios::app);
    srand((unsigned)time(NULL));

    sys.forgetting_strategy = 1; //fc=1, fr=2, fd=3
    totalsim(sys.forgetting_strategy);



    /*  FOR SIMULATING ONE SPECIFIC CONDITION
    printLabels();
    sys.mean_optimism_threshold = 0.5;
    sys.mem_ratio =0.2;
    sys.number_of_toleration = 1;
    int no_realizations = 1;
    oneRound(no_realizations);
    */
    
    myfile.close();


    
    return 0;
}




void totalsim(int forgetting){
    
    printLabels();

    sys.forgetting_strategy = forgetting;
    sys.number_of_toleration = 1;
    sys.mem_ratio = 0.0;
    sys.mean_optimism_threshold = 0.0;

for (int t = 0;t<3;t++){

    cout<<"Toleration: "<<sys.number_of_toleration<<"\n";
    
    

    for(int k=0;k<11;k++){

       
        cout<<"Memory Ratio: "<<sys.mem_ratio<<"\n";

        for(int i=0;i<11;i++){
            cout<<"Threshold: "<<sys.mean_optimism_threshold<<"\n";
        
        

            for(int m=0;m<sys.number_of_realizations;m++){
                
                oneRound(m+1);   
                population.clear();
                cooperators.clear();
                defectors.clear();
            }
            sys.mean_optimism_threshold+=0.1;
        }    
        sys.mean_optimism_threshold=0.0;
        sys.mem_ratio+=0.1;

    }
    sys.mem_ratio = 0.0;
    sys.number_of_toleration++;
} 

        
    
}


//for one round simulations
void oneRound(int realization){
    population = initialPopulation();
    Game game(&sys, population, &sysdata);
    for(int i=0;i<game.population.size();i++){   //agent objects should reach the other agents
        game.population.at(i).population = &game.population;
    }
    
    population = game.run();
    updateData();
    printData(1,realization);
    sysdata = sysdata.reset();
}








map<int, Agent> initialPopulation(){

    map <int, Agent> init;
    pair <int, Agent> p;
    set<int>available_ids;   //newly generated agents will be randomly assigned to these ids
    for(int i=0;i<sys.population_size;i++){
        available_ids.insert(i);
    }


    int number_of_defectors = (int) (sys.population_size * (sys.defector_ratio));
    double interval = 1.0 / (number_of_defectors-1); 
    for(int i=0;i<number_of_defectors;i++){   //defectors
        if(sys.payoff['S']==0){
            Agent x(sys.randomElement(available_ids), sys.coop_rate_def, 0, 0, sys, &sysdata);
            p.first=x.id;   p.second=x;
            init.insert(p);
            available_ids.erase(x.id);
            defectors.insert(x.id);
        }
        else{   //payoff matrix have negative indices, defectors should also have an optimism threshold
            if(sys.optimism_value_dist == 0){
                Agent x(sys.randomElement(available_ids), sys.coop_rate_def, interval*i, 0, sys, &sysdata);
                p.first=x.id;   p.second=x;
                init.insert(p);
                available_ids.erase(x.id);
                defectors.insert(x.id);
            }
            else{
                //bakarsın   (normal dist of thresholds)
            }
        }
    }


    int number_of_cooperators = (int) (sys.population_size * (1 -sys.defector_ratio));
    if(sys.optimism_value_dist == 0){
        double interval = 1.0 / (number_of_cooperators-1);  //distances of optimism thresholds of agents should be uniform
        for(int i=0;i<number_of_cooperators;i++){
            Agent x(sys.randomElement(available_ids), sys.coop_rate_coop, interval*i, sys.mem_ratio, sys, &sysdata);
            p.first=x.id;   p.second=x;
            init.insert(p);
            available_ids.erase(x.id);
            cooperators.insert(x.id);
        }
    }
    else if(sys.optimism_value_dist ==1){    //normal dist( imdilik var=0 yapıyorum)
        for(int i=0;i<number_of_cooperators;i++){
            Agent x(sys.randomElement(available_ids), sys.coop_rate_coop, sys.mean_optimism_threshold, sys.mem_ratio, sys, &sysdata);
            p.first=x.id;   p.second=x;
            init.insert(p);
            available_ids.erase(x.id);
            cooperators.insert(x.id);
        }
    }
    else{    //uniform between 0.2 - 0.4
        double interval = 0.2 / (number_of_cooperators-1);
        for(int i=0;i<number_of_cooperators;i++){
            Agent x(sys.randomElement(available_ids), sys.coop_rate_coop, 0.2 + interval*i, sys.mem_ratio, sys, &sysdata);
            p.first=x.id;   p.second=x;
            init.insert(p);
            available_ids.erase(x.id);
            cooperators.insert(x.id);
        }
    }
    return init;

}

//data for each round can be put here
void updateData(){
    calculateMeanPayoff(1); calculateMeanPayoff(2);
    calculateMeanThreshold();
    calculateMemoryUsage();
    calculateDecisionData();
    calculateRecommendationData();
    calculateSystemInfo();
    
}

void printData(int round, int realization){

        
        int winner = (sysdata.coop_mean>=sysdata.def_mean)? 1 : 0;    //1 if coops win
        myfile<< realization << ", "<< round << ","<<sys.forgetting_strategy<<",";
        myfile<<sys.mem_ratio<<","<<sysdata.coop_mean_optimism_threshold<<","<<sysdata.def_mean_optimism_threshold<<","<<sys.number_of_toleration<<",";
        myfile<<cooperators.size()<<","<< defectors.size()<<","<< winner << "," << sysdata.coop_mean<<","<<sysdata.def_mean<<","<<sysdata.coop_vs_coop_accepted<<",";
        myfile<<sysdata.coop_vs_def_accepted<<","<< sysdata.def_vs_def_accepted<<","<< sysdata.coop_vs_coop_rejected<<","<<sysdata.coop_vs_def_rejected<<",";
        myfile<<sysdata.def_vs_def_rejected<<","<< sysdata.memory_usage << "," << sysdata.coop_in_memory << "," << sysdata.def_in_memory <<","<<sysdata.undef_in_memory<<",";
        myfile<<sysdata.actual_coop_in_memory<<","<<sysdata.actual_def_in_memory<<",";
        myfile<<sysdata.coop_coop_by_perception<<","<<sysdata.coop_def_by_perception<<","<<sysdata.def_coop_by_perception<<","<<sysdata.def_def_by_perception<<","
        <<sysdata.coop_coop_by_recom<<","<<sysdata.coop_def_by_recom<<","<<sysdata.def_coop_by_recom<<","<<sysdata.def_def_by_recom<<","<<
        sysdata.perception_success<<","<<sysdata.recommendation_success<<","<<sysdata.positive_recom_for_coop<<","<<sysdata.negative_recom_for_coop<<","
        <<sysdata.positive_recom_for_def<<","<<sysdata.negative_recom_for_def<<","<<sysdata.system_recom_for_coop<<","<<sysdata.system_recom_for_def<<","
        <<sysdata.number_of_percepted_evaluation<<","<<sysdata.number_of_recommendation<<","
        <<sysdata.number_of_tolerance_play<<","<<sysdata.number_of_default_play<<","<<sysdata.error_in_coop<<","<<sysdata.error_in_def<<","<<sysdata.error_in_total<<","<<
        sysdata.system_perception_coop<<","<<sysdata.system_perception_def;
        myfile<<"\n";


}

void calculateMeanPayoff(int characteristic) {   //1=coop  2=def

    double aim = (characteristic == 1) ? (sys.coop_rate_coop) : (sys.coop_rate_def);
    vector<double>v;
    for(int i=0;i<population.size();i++){
        Agent current = population.at(i);
        if(current.cooperation_ratio == aim){
            v.push_back((double)current.total_payoff);
        }
    }
    if(characteristic == 1) sysdata.coop_mean = sys.mean(v);
    else sysdata.def_mean = sys.mean(v);
}

void calculateMeanThreshold(){
    vector<double>v1;
    vector<double>v2;
    for(int i=0;i<population.size();i++){
        Agent current = population.at(i);
        if(current.cooperation_ratio == sys.coop_rate_coop){
            v1.push_back(current.optimism_threshold);
        }
        else{
            v2.push_back(current.optimism_threshold);
        }
    }
    sysdata.coop_mean_optimism_threshold = sys.mean(v1);
    sysdata.def_mean_optimism_threshold = sys.mean(v2);
}


void calculateMemoryUsage(){

    int count = 0;
    double coop_rate = 0.0;
    double def_rate = 0.0;
    double undefined_rate = 0.0;
    double actual_coop_rate = 0.0;
    double actual_def_rate = 0.0;

    double mem_size = sys.mem_ratio * sys.population_size;
    if(mem_size == 0.0){
        return;
    }

    for(int i=0;i<population.size();i++){
        Agent current = population.at(i);
        
        if(current.cooperation_ratio == sys.coop_rate_coop){    //memory matters for cooperators
            count++;  //counting number of cooperators
            coop_rate += current.memory.cooperators.size() / mem_size;
            def_rate += current.memory.defectors.size() / mem_size;
            undefined_rate += current.memory.undefined.size() / mem_size;
            
            int actual_coop = 0;
            int actual_def = 0;
            set<int>::iterator it = current.memory.neighbor_ids.begin();
            while (it != current.memory.neighbor_ids.end()){   //count the actual characteristic
                
                Agent neighbor = population.at(*it);
                if(neighbor.cooperation_ratio == sys.coop_rate_coop){   //neighbor is actually a cooperator
                    actual_coop++;
                }
                else{
                    actual_def++;
                }
                it++;
            }
            actual_coop_rate += actual_coop / mem_size;
            actual_def_rate += actual_def / mem_size;
        }

    }

    sysdata.coop_in_memory = coop_rate / count;
    sysdata.def_in_memory = def_rate / count;
    sysdata.undef_in_memory = undefined_rate /count;
    sysdata.actual_coop_in_memory = actual_coop_rate /count;
    sysdata.actual_def_in_memory = actual_def_rate / count;
    sysdata.memory_usage = sysdata.coop_in_memory + sysdata.def_in_memory + sysdata.undef_in_memory;
}

void calculateDecisionData(){
    
    sysdata.number_of_percepted_evaluation = sysdata.coop_coop_by_perception + sysdata.def_def_by_perception + sysdata.coop_def_by_perception + sysdata.def_coop_by_perception;
    if(sysdata.number_of_percepted_evaluation!=0){
        sysdata.perception_success = (sysdata.coop_coop_by_perception + sysdata.def_def_by_perception) / (double)sysdata.number_of_percepted_evaluation;
    }
    sysdata.number_of_recommendation = sysdata.coop_coop_by_recom + sysdata.coop_def_by_recom + sysdata.def_coop_by_recom + sysdata.def_def_by_recom;
    if(sysdata.number_of_recommendation!=0){
        sysdata.recommendation_success = (sysdata.coop_coop_by_recom + sysdata.def_def_by_recom) / (double)sysdata.number_of_recommendation;
    }
    
}

void calculateRecommendationData(){
    
    sysdata.system_recom_for_coop = (sysdata.positive_recom_for_coop + 1) / (double)(sysdata.positive_recom_for_coop + sysdata.negative_recom_for_coop + 2);
    sysdata.system_recom_for_def = (sysdata.positive_recom_for_def + 1) / (double)(sysdata.positive_recom_for_def + sysdata.negative_recom_for_def + 2);
    
}


void calculateSystemInfo(){
    

    int popsize = 0;
    popsize = population.size();
    vector<vector<int> > agent_experiences(population.size(), vector<int>(2));
    //int agent_experiences[popsize][2] = {}; //keeps the experience about agent i in ith index
    map<int,MemorySlot>::iterator it;
    for(int i = 0; i< population.size();i++){
        Agent cur = population.at(i);
        map<int,MemorySlot>cur_neighbors = cur.memory.neighbors;
        it = cur_neighbors.begin();
        while (it != cur_neighbors.end()){
            
            int id = it->first;
            int coop = it->second.number_of_cooperations_perceived + it->second.number_of_cooperations_heard;
            int def = it->second.number_of_defections_perceived + it->second.number_of_defections_heard;
            agent_experiences[id][0] +=coop;
            agent_experiences[id][1] +=def;
            it++;
            
        }
        
    }
    
    //calculate ratios for each agent
    vector<double>coop_experiences;
    vector<double>def_experiences;

    int number_of_defectors = (int)(sys.population_size * sys.defector_ratio);
    for(int i = 0;i<population.size();i++){
        int coop = agent_experiences[i][0];
        int def = agent_experiences[i][1];
        double ratio = (double)(coop+1) / (coop+def+2);
        if(defectors.find(i) != defectors.end()){
            def_experiences.push_back(ratio);
        }
        else{
            coop_experiences.push_back(ratio);
        }
    }


    //calculate errors and means
    double sum_coop = 0;
    double sum_def = 0;
    double error_coop = 0;
    double error_def = 0;
    
    for(int i = 0;i<coop_experiences.size();i++){
        
        sum_coop += coop_experiences[i];
        error_coop += pow(coop_experiences[i]-sys.coop_rate_coop,2);
    }

    
    for(int i = 0;i<def_experiences.size();i++){
        
        sum_def += def_experiences[i];
        error_def += pow(def_experiences[i]-sys.coop_rate_def,2);
    }

    sysdata.error_in_coop = sqrt(error_coop) / sqrt(coop_experiences.size());
    sysdata.error_in_def = sqrt(error_def) / sqrt(def_experiences.size());
    sysdata.error_in_total = sqrt(error_coop+error_def) /sqrt(population.size());
    sysdata.system_perception_coop = sum_coop / (double)coop_experiences.size();
    sysdata.system_perception_def = sum_def / (double)def_experiences.size();

    

}



void printLabels(){

        myfile<<"Realization,";
        myfile<<"Round,";
        myfile<<"Forgetting Strategy,";
        myfile<<"Memory Ratio,";
        myfile<<"Cooperator mean optimism threshold,";
        myfile<<"Defector mean optimism threshold,";
        myfile<<"Toleration,";
        myfile<<"Num of cooperators,";
        myfile<<"Num of defectors,";
        myfile<<"Winner,";
        myfile<<"Cooperator payoff mean,";
        myfile<<"Defector payoff mean,";
        myfile<<"Coop vs Coop games played,"; 
        myfile<<"Coop vs Def games played," ;
        myfile<<"Def vs Def games played," ;
        myfile<<"Coop vs Coop games rejected," ;
        myfile<<"Coop vs Def games rejected,";
        myfile<<"Def vs Def games rejected,";
        myfile<<"Memory usage,";
        myfile<<"Cooperators in memory,";
        myfile<<"Defectors in memory,";
        myfile<<"Undefined in memory,";
        myfile<<"Actual cooperators in memory,";
        myfile<<"Actual defectors in memory,";
        myfile<<"Perception of coop as coop,";
        myfile<<"Perception of coop as def,";
        myfile<<"Perception of def as coop,";
        myfile<<"Perception of def as def,";
        myfile<<"Recommendation of coop as coop,";
        myfile<<"Recommendation of coop as def,";
        myfile<<"Recommendation of def as coop,";
        myfile<<"Recommendation of def as def,";
        myfile<<"Perception success,";
        myfile<<"Recommendation success,";
        myfile<<"Coop games about cooperators(by recom),";  //means how many cooperations are asserted in recommendaitons for cooperators
        myfile<<"Def games about cooperators(by recom),";
        myfile<<"Coop games about defectors(by recom),";
        myfile<<"Def games about defectors(by recom),";
        myfile<<"Coop ratio for cooperators with recom,";
        myfile<<"Coop ratio for defectors with recom,";
        myfile<<"Number of percepted evaluations,";
        myfile<<"Number of recommendation,";
        myfile<<"Number of tolerance play,";
        myfile<<"Number of default plays,";
        myfile<<"Error of coop info in system,";
        myfile<<"Error of def info in system,";
        myfile<<"Error total in system,";
        myfile<<"Mean perception for cooperators in system,";
        myfile<<"Mean perception for defectors in system,";
        myfile<<"\n";


        

}