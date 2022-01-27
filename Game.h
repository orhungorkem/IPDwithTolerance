#include <vector>
#include "Agent.h"
#include <map>

using namespace std;

class Game{

    public:

    map<int, Agent>population;
    SystemVariables* sys;
    SystemData* sysdata;

    Game(SystemVariables* sys, map<int, Agent>&population, SystemData* sysdata);  //constructor
    void play(Agent& p1, Agent& p2);  //a dual game
    map<int, Agent> run();  //makes many plays, returns final population


};