#include <vector>
#include <map>
#include "Memory.h"
#include "SystemVariables.h"
#include "SystemData.h"
using namespace std;

class Agent{

    public:

    Agent(int id, double cooperation_ratio, double optimism_threshold, double memory_ratio, SystemVariables& sys, SystemData* sysdata);
    Agent(); //default constructor

    int id;
    double cooperation_ratio;
    double optimism_threshold;
    double memory_ratio;
    int total_payoff;
    Memory memory;
    SystemVariables sys;
    SystemData* sysdata;
    map<int, Agent>* population;


    bool decideToPlay(Agent& opponent);   //evaluate opponent and return the decision to play
    void putIntoMemory(int id);  //assigns a memory slot in neighbors for a new agent
    void forgetAgent();  
    double rememberOpponent(Agent& opponent, bool recom_in_memory);   //return the opponent coop ratio by looking memory
    double receiveRecommendation(Agent& opponent, bool recom_in_memory);    //seek for recommendation from neighbors and return their perception
    bool existsInMemory(Agent& opponent);   //checks whether the opponent is in memory or not
    void updateList(Agent& opponent);  //opponents can be transfered from coop list to update list or vice versa


};