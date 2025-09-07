#include "Process.h"


Process::Process(std::string name, double arrivalTime, double burstTime, uint priority) {   
    if(arrivalTime < 0 || burstTime < 0) 
        ;
    this -> name = name;
    this -> arrivalTime = arrivalTime;
    this -> burstTime = burstTime;
    this -> priority = priority;
}

double Process::getBurstTime() const{
    return burstTime;
}

double Process::getArrivalTime() const{
    return arrivalTime;
}

uint Process::getPriority() const{
    return priority;
}

std::string Process::getName() const{
    return name; 
}
