#include "Game.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;

Game::Game(SystemVariables* sys, map<int, Agent>&population, SystemData* sysdata){
    this->sys = sys;
    this->population = population;
    this->sysdata = sysdata;
}

// 2 random agents are picked to play for whole game 
map<int, Agent> Game::run(){
    
    int count=sys->lifetime*(sys->population_size)*(sys->population_size-1)/2;   //number of plays in one round
    for(int i=0;i<count;i++){
        
        //Pick 2 random agents
        int n1 = sys->random(0,population.size()-1);
        int n2 = sys->random(0,population.size()-1);
        while(n1==n2){
            n2 = sys->random(0,population.size()-1);
        }
        
        Agent& p1 = population.at(n1);
        Agent& p2 = population.at(n2);
        play(p1,p2);
    }
    return population;  //population is updated and returned
}

void Game::play(Agent& p1, Agent& p2){
    
    bool p1_play = p1.decideToPlay(p2);
    bool p2_play = p2.decideToPlay(p1);

    if(p1_play && p2_play){    //playing decision
        
        if(p1.cooperation_ratio == sys->coop_rate_coop && p2.cooperation_ratio == sys->coop_rate_coop){   //both cooperators
            sysdata->coop_vs_coop_accepted++;
        }
        else if(p1.cooperation_ratio != sys->coop_rate_coop && p2.cooperation_ratio == sys->coop_rate_coop){  //p1 defector
            sysdata->coop_vs_def_accepted++;
        }
        else if(p1.cooperation_ratio == sys->coop_rate_coop && p2.cooperation_ratio != sys->coop_rate_coop){  //p2 defector
            sysdata->coop_vs_def_accepted++;
        }
        else{   //both defectors
            sysdata->def_vs_def_accepted++;
        }
        
    }

    else{
        if(p1.cooperation_ratio == sys->coop_rate_coop && p2.cooperation_ratio == sys->coop_rate_coop){   //both cooperators
            sysdata->coop_vs_coop_rejected++;
        }
        else if(p1.cooperation_ratio != sys->coop_rate_coop && p2.cooperation_ratio == sys->coop_rate_coop){  //p1 defector
            sysdata->coop_vs_def_rejected++;
        }
        else if(p1.cooperation_ratio == sys->coop_rate_coop && p2.cooperation_ratio != sys->coop_rate_coop){  //p2 defector
            sysdata->coop_vs_def_rejected++;
        }
        else{
            sysdata->def_vs_def_rejected++;
        }

        return;    //game is rejected
    }

    
    double X=((double)rand()/(double)RAND_MAX);
    X=((double)rand()/(double)RAND_MAX);
    bool coop1= X <= p1.cooperation_ratio;   //p1 cooperated
    X=((double)rand()/(double)RAND_MAX);
    bool coop2= X <=p2.cooperation_ratio;   //p2 cooperated

    //Play outcomes
    if(coop1 && coop2){
        p1.total_payoff+=sys->payoff['R'];   p2.total_payoff+=sys->payoff['R'];
    }
    else if(coop1 && !coop2){
        p1.total_payoff+=sys->payoff['S'];   p2.total_payoff+=sys->payoff['T'];
    }
    else if(!coop1 && coop2){
        p1.total_payoff+=sys->payoff['T'];   p2.total_payoff+=sys->payoff['S'];
    }
    else{
        p1.total_payoff+=sys->payoff['P'];   p2.total_payoff+=sys->payoff['P'];
    }


    //Memory updates
    if(sys->mem_ratio==0){
        return;
    }

    if(p1.memory.capacity!=0){

        if(!p1.existsInMemory(p2)){   //p1 has data about p2 in memory already
        
            if(p1.memory.isFull()){   
                p1.forgetAgent();
            }
            p1.putIntoMemory(p2.id);     
        }

        MemorySlot& m1=p1.memory.neighbors.at(p2.id);
        m1.number_of_plays++;
        m1.number_of_cooperations_perceived += (coop2) ? 1: 0;
        m1.number_of_defections_perceived += (coop2) ? 0: 1;
        m1.coop_ratio_perceived = 
            (double)(m1.number_of_cooperations_perceived + 1) / (m1.number_of_cooperations_perceived + m1.number_of_defections_perceived + 2);

        p1.updateList(p2);

    }
    

    if(p2.memory.capacity!=0){
    
        if(!p2.existsInMemory(p1)){   
        
            if(p2.memory.isFull()){   
                p2.forgetAgent();
            }
            p2.putIntoMemory(p1.id);
        }

        MemorySlot& m2=p2.memory.neighbors.at(p1.id);
        m2.number_of_plays++;
        m2.number_of_cooperations_perceived += (coop1) ? 1: 0;
        m2.number_of_defections_perceived += (coop1) ? 0: 1;
        m2.coop_ratio_perceived = 
            (double)(m2.number_of_cooperations_perceived + 1) / (m2.number_of_cooperations_perceived + m2.number_of_defections_perceived + 2);

        p2.updateList(p1);
    
    }

    
    
    

}