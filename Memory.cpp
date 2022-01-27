#include "Memory.h"
#include "iostream"

Memory::Memory(int capacity){
    this->capacity=capacity;
}

Memory::Memory(){
    
}


bool Memory::isFull(){
    return this->capacity==this->neighbors.size();
}

bool Memory::checkAgent(int opponent_id){

    return neighbor_ids.count(opponent_id)==1;

}