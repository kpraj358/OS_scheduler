#if !defined(RR_SCH)
#define RR_SCH

#include "Scheduler.h"
#include <algorithm>
#include <queue>
class RR: public Scheduler
{
private:
    /* data */
public:
    inline ScheduleList schedule();
};


ScheduleList RR::schedule() {
    ScheduleList schList;
    if(processContainer.empty()) 
        return schList;

    std::sort(processContainer.begin(), processContainer.end(), arrivalTime_cmp());
    std::queue<ActiveProcess> readyQueue;
    double curTime = 0;
    int arrivedProcess = 0;

    while(arrivedProcess < processContainer.size() || !readyQueue.empty()) {
        // Adding Idle TimeFrame
        if(readyQueue.empty() && arrivedProcess < processContainer.size() && processContainer[arrivedProcess] -> getArrivalTime() > curTime) {
            schList.addTimeFrame(TimeFrame(curTime, processContainer[arrivedProcess] -> getArrivalTime()));
            curTime = processContainer[arrivedProcess] -> getArrivalTime();
        }
        // Adding arrived processes to waitingProcesses
        while(arrivedProcess < processContainer.size() && processContainer[arrivedProcess] -> getArrivalTime() <= curTime) {
            readyQueue.push(ActiveProcess(processContainer[arrivedProcess]));
            arrivedProcess++;
        }
        
        ActiveProcess curProcess = readyQueue.front();
        readyQueue.pop();
        double usedBurstTime = -1;
        if(curProcess.leftBurstTime <= getQuantum()) {
            usedBurstTime = curProcess.leftBurstTime;
        } else {
            usedBurstTime = getQuantum();
            curProcess.leftBurstTime -= usedBurstTime;
            while(arrivedProcess < processContainer.size() && processContainer[arrivedProcess] -> getArrivalTime() <= curTime + usedBurstTime) {
                readyQueue.push(ActiveProcess(processContainer[arrivedProcess]));
                arrivedProcess++;
            }
            readyQueue.push(curProcess);
        }
        schList.addTimeFrame(TimeFrame(curTime, curTime + usedBurstTime, curProcess.process));
        curTime += usedBurstTime;

    }
    return schList;
}

#endif // RR_SCH
