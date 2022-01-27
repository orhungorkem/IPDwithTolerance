#include "Agent.h"
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iostream>

Agent::Agent(int id, double cooperation_ratio, double optimism_threshold, double memory_ratio, SystemVariables& sys, SystemData* sysdata){
    this->id=id;
    this->cooperation_ratio=cooperation_ratio;
    this->optimism_threshold=optimism_threshold;
    this->memory_ratio=memory_ratio;
    double memory_capacity=sys.population_size*memory_ratio;
    Memory memory(memory_capacity);
    this->memory=memory;   //doldur
    this->sys=sys;
    this->sysdata=sysdata;
    this->total_payoff=0;
}

Agent::Agent(){
    return;
}

bool Agent::decideToPlay(Agent& opponent){
    
    if(sys.mem_ratio==0){    //means no data to evaluate
        if(cooperation_ratio == sys.coop_rate_coop){
            sysdata->number_of_default_play++;
        }
        return true;
    }
    
    bool decision;

    if(existsInMemory(opponent)){
        
        //Agents has not played sufficient games to eliminate bias yet
        //this code may chanage when recommendations are kept in memory
        if(this->memory.neighbors.at(opponent.id).number_of_plays < sys.number_of_toleration){

            sysdata->number_of_tolerance_play++;
            return true;

        }

        else{
            if(sys.recom_in_memory){   //option to keep recommendation data in memory
                decision = rememberOpponent(opponent, true) >= this->optimism_threshold;
            }
            else{    //only perceived data will be evaluated
                decision = rememberOpponent(opponent, false) >= this->optimism_threshold;
            }
        }


        if(cooperation_ratio == sys.coop_rate_coop){   //data count for cooperators
            if(decision){   //cooperator perception
                if(opponent.cooperation_ratio == sys.coop_rate_coop){   //opponent is cooperator
                    sysdata->coop_coop_by_perception++;
                }
                else{
                    sysdata->def_coop_by_perception++;
                }
            }
            else{
                if(opponent.cooperation_ratio == sys.coop_rate_coop){   //opponent is cooperator
                    sysdata->coop_def_by_perception++;
                }
                else{
                    sysdata->def_def_by_perception++;
                }
            }
        }

        return decision;
    }

    if(sys.self_assured_defectors){
        if(cooperation_ratio == sys.coop_rate_def){
            return true;    //take defectors self assured
        }
    }
    if(sys.self_assured_cooperators){
        if(cooperation_ratio == sys.coop_rate_coop){
            sysdata->number_of_default_play++;
            return true;    //take defectors self assured
        }
    }

    
    double coop_ratio_for_opponent;
    coop_ratio_for_opponent = (sys.recom_in_memory) ? receiveRecommendation(opponent, true) : receiveRecommendation(opponent, false);
    decision = coop_ratio_for_opponent >= optimism_threshold;
    
    return decision;
    
}

bool Agent::existsInMemory(Agent& opponent){

    return memory.checkAgent(opponent.id);
}

void Agent::forgetAgent(){

    

    bool not_over = false;

    if(sys.forgetting_strategy==0){   //FC

        int range = memory.cooperators.size();
        if(range>0){    //look for cooperators
            //removing from sets and map
            int id_to_forget=sys.randomElement(memory.cooperators);
            memory.neighbor_ids.erase(id_to_forget);
            memory.cooperators.erase(id_to_forget);
            memory.neighbors.erase(id_to_forget);

        }
        else{   //look for undefined
            range = memory.undefined.size();
            if(range>0){
                int id_to_forget=sys.randomElement(memory.undefined);
                memory.undefined.erase(id_to_forget);
                memory.neighbor_ids.erase(id_to_forget);
                memory.neighbors.erase(id_to_forget);
            }
            else{
                not_over = true;   //apply fr then
                
            }
        }

    }

    else if(sys.forgetting_strategy==2){  //FD

        
        int range = memory.defectors.size();
        if(range>0){    //look for defectors
            //removing from sets and map
            int id_to_forget=sys.randomElement(memory.defectors);
            memory.neighbor_ids.erase(id_to_forget);
            memory.defectors.erase(id_to_forget);
            memory.neighbors.erase(id_to_forget);

        }
        else{   //look for undefined
            range = memory.undefined.size();
            if(range>0){
                int id_to_forget=sys.randomElement(memory.undefined);
                memory.undefined.erase(id_to_forget);
                memory.neighbor_ids.erase(id_to_forget);
                memory.neighbors.erase(id_to_forget);
            }
            else{
                not_over = true;   //apply fr then
                
            }
        }
        

    }

    if(sys.forgetting_strategy==1 || not_over){   //FR   look for random

        int range = memory.neighbor_ids.size();
        int id_to_forget=sys.randomElement(memory.neighbor_ids);
        memory.neighbor_ids.erase(id_to_forget);
        memory.cooperators.erase(id_to_forget);
        memory.defectors.erase(id_to_forget);
        memory.neighbors.erase(id_to_forget);
        memory.undefined.erase(id_to_forget);
        
    }
    

}

void Agent::updateList(Agent& opponent){

    int id=opponent.id;

    if(sys.recom_in_memory){

       
    }
    else{
        MemorySlot data = memory.neighbors.at(id);
        if(data.number_of_plays < sys.number_of_toleration){  //add to undefined list
            if(data.number_of_plays == 1){
                memory.undefined.insert(id);
            }
        }
        else{   //now add to normal lists
            if(data.number_of_plays == sys.number_of_toleration){  //toleration ended
                if(data.number_of_plays != 1){  //if the agent is added to undefined some time
                    memory.undefined.erase(id);
                }
            }
            bool coop = data.coop_ratio_perceived >= optimism_threshold;
            if(coop){
                if(find(memory.cooperators.begin(), memory.cooperators.end(), id) == memory.cooperators.end()){  //agent should be added to cooperators
                    memory.cooperators.insert(id);
                }
                if(find(memory.defectors.begin(), memory.defectors.end(), id) != memory.defectors.end()){  //agent should removed from defectors
                    memory.defectors.erase(id);
                }
            }
            else{
                if(find(memory.defectors.begin(), memory.defectors.end(), id) == memory.defectors.end()){  //agent should be added to defectors
                    memory.defectors.insert(id);
                }
                if(find(memory.cooperators.begin(), memory.cooperators.end(), id) != memory.cooperators.end()){  //agent should removed from cooperators
                    memory.cooperators.erase(id);
                }
            }

        }
        

    }

}

//returns the cooperation ratio for the given opponent
double Agent::rememberOpponent(Agent& opponent, bool recom_in_memory){
    if(recom_in_memory){
        return this->memory.neighbors.at(opponent.id).coop_ratio_total;    //the coop ratio including recommendation data is returned
    }
    else{
        return this->memory.neighbors.at(opponent.id).coop_ratio_perceived;
    }
}


//Looks for neighbors, receives recommenndation, returns the predicted coop ratio for the oppponent
//if recom_in_memory option is true, recommendation data is also sstored in memory
double Agent::receiveRecommendation(Agent& opponent, bool recom_in_memory){

    set<int>& neighbors_set = this->memory.neighbor_ids;
    set<int>::iterator it = neighbors_set.begin();
    int id;

    int coop_count = 0;  //the data for cooperations heard from neighbors will be counted
    int def_count = 0;  //the data for cooperations heard from neighbors will be counted

    int num_coop = 0;  //temporary variables to keep adding terms
    int num_def = 0;
        
    while (it != neighbors_set.end()){
        Agent neighbor = (*population).at(*it); 
        
        
        if(neighbor.existsInMemory(opponent)){   //means the neighbor knows the opponent
            MemorySlot data = neighbor.memory.neighbors.at(opponent.id);
            num_coop = data.number_of_cooperations_perceived;
            num_def = data.number_of_defections_perceived;
            coop_count += num_coop;
            def_count += num_def;
            if(opponent.cooperation_ratio == sys.coop_rate_coop){    //opponent is cooperator
                sysdata->positive_recom_for_coop += num_coop;
                sysdata->negative_recom_for_coop += num_def;
            }
            else{   //opponent is defector
                sysdata->positive_recom_for_def += num_coop;
                sysdata->negative_recom_for_def += num_def;
            }
            
            if(recom_in_memory){
              
                if(!this->existsInMemory(opponent)){    //the recom data is kept in memory, thus a new memoryslot will be opened
                    if(this->memory.isFull()){   
                        this->forgetAgent();
                    }
                    this->putIntoMemory(opponent.id);
                }
                this->memory.neighbors.at(opponent.id).number_of_cooperations_heard += num_coop;
                this->memory.neighbors.at(opponent.id).number_of_defections_heard += num_def;
            }
        }
        
        it++;
    }
    
    if(( coop_count + def_count ) == 0){
        if(cooperation_ratio == sys.coop_rate_coop){
            sysdata->number_of_default_play++;
        }
        return 1;   //play anyway
    }
    
    double ratio = ((double)(coop_count + 1)) / (coop_count + def_count + 2);
    bool decision = ratio >= this->optimism_threshold;

    if(cooperation_ratio == sys.coop_rate_coop){   //data count for cooperators
        if(decision){   //cooperator perception
            if(opponent.cooperation_ratio == sys.coop_rate_coop){   //opponent is cooperator
                sysdata->coop_coop_by_recom++;
            }
            else{
                sysdata->def_coop_by_recom++;
            }
        }
        else{
            if(opponent.cooperation_ratio == sys.coop_rate_coop){   //opponent is cooperator
                sysdata->coop_def_by_recom++;
            }
            else{
                sysdata->def_def_by_recom++;
            }
        }
    }


    return ratio;

    
}

void Agent::putIntoMemory(int id){
    MemorySlot init;
    this->memory.neighbors.insert(pair<int, MemorySlot>(id, init));
    this->memory.neighbor_ids.insert(id);
}