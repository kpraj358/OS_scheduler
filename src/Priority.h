#if !defined(PRIORITY_SCH)
#define PRIORITY_SCH

#include "./Scheduler.h"
#include <algorithm>
#include <queue>

class Priority : public Scheduler
{
private:
    /* data */
public:
    inline ScheduleList schedule();
};

ScheduleList Priority::schedule() {
    
    ScheduleList schList;
    if(processContainer.empty())
        return schList;        
    sort(processContainer.begin(), processContainer.end(), priority_cmp());

    double curTime = 0;
    int arrivedProcess = 0;
    auto cmp = [] (const ActiveProcess& p1, const ActiveProcess& p2) {
        if(p1.process -> getPriority() == p2.process -> getPriority()) {
            return p1.process -> getArrivalTime() > p2.process -> getArrivalTime();
        } else {
            return p1.process -> getPriority() >= p2.process -> getPriority();
        }
    };
    std::priority_queue<ActiveProcess, std::vector<ActiveProcess>, decltype(cmp) > readyQueue(cmp);

    while(arrivedProcess < processContainer.size() || !readyQueue.empty()) {

        // Add Idle TimeFrame
        if(readyQueue.empty() && processContainer[arrivedProcess] -> getArrivalTime() > curTime) {
            schList.addTimeFrame(TimeFrame(curTime, processContainer[arrivedProcess] -> getArrivalTime()));
            curTime = processContainer[arrivedProcess] -> getArrivalTime();
        }
        // Adding process that have arrived to the readyQueue
        while(arrivedProcess < processContainer.size() && processContainer[arrivedProcess] -> getArrivalTime() <= curTime) {
            readyQueue.push(ActiveProcess(processContainer[arrivedProcess]));
            arrivedProcess++;
        }

        ActiveProcess curProcess = readyQueue.top();
        readyQueue.pop();
        double usedBurstTime = curProcess.leftBurstTime;
        if(isPreemptive()) {
            int newArrivals = arrivedProcess;
            // Checking if higher priority processes have arrived during the left burst time of the current process 
            while(newArrivals < processContainer.size() && processContainer[newArrivals] -> getArrivalTime() < curTime + curProcess.leftBurstTime) {
                // Preempting current process if true
                if(processContainer[newArrivals] -> getPriority() < curProcess.process -> getPriority()) {
                    usedBurstTime = processContainer[newArrivals] -> getArrivalTime() - curTime;
                    curProcess.leftBurstTime -= usedBurstTime;
                    readyQueue.push(curProcess);
                    break;
                }
                newArrivals++;
            }
        }

        schList.addTimeFrame(TimeFrame(curTime, curTime + usedBurstTime, curProcess.process));
        curTime += usedBurstTime;
    }
    return schList;
}



#endif // PRIORITY_SCH
