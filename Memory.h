#include <vector>
#include <set>
#include <map>
#include "MemorySlot.h"

using namespace std;

class Memory{

    public:
    int capacity;
    map<int, MemorySlot>neighbors;   //mapping of agent id and the related data
    set<int>neighbor_ids;
    set<int>defectors;
    set<int>cooperators;
    set<int>undefined;  //to eliminate bias

    Memory(int capacity);
    Memory();  //default constructor
    bool isFull();   
    bool checkAgent(int opponent_id);

};