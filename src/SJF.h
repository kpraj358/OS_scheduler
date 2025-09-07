#if !defined(__SJF_SCH_)
#define __SJF_SCH_

#include "./Scheduler.h"
#include <algorithm>
#include <queue>



class SJF : public Scheduler
{
private:
    
public:
    inline ScheduleList schedule();
};

ScheduleList SJF::schedule() {
    ScheduleList schList;

    if(processContainer.empty())
        return schList;

    std::sort(processContainer.begin(), processContainer.end(), burstTime_cmp());
    auto cmp = [] (const ActiveProcess& p1, const ActiveProcess& p2) {
        if (p1.leftBurstTime == p2.leftBurstTime) {
            return p1.process -> getArrivalTime() >= p2.process -> getArrivalTime();
        } else {
            return p1.leftBurstTime >= p2.leftBurstTime;
        }
    };
    std::priority_queue<ActiveProcess, std::vector<ActiveProcess>, decltype( cmp )> readyQueue( cmp );
    double curTime = 0;

    int arrivedProcess = 0; 
    while(!readyQueue.empty() || arrivedProcess < processContainer.size()){
        
        // Adding Idle timeframe
        if(readyQueue.empty() && processContainer[arrivedProcess] -> getArrivalTime() > curTime) {
            schList.addTimeFrame(TimeFrame(curTime, curTime + processContainer[arrivedProcess] -> getArrivalTime()));
            curTime  += processContainer[arrivedProcess] -> getArrivalTime();
        }
        // Adding process that have arrived to the readyQueue
        while(arrivedProcess < processContainer.size() && processContainer[arrivedProcess] -> getArrivalTime() <= curTime) {
            readyQueue.push(ActiveProcess(processContainer[arrivedProcess]));
            arrivedProcess++;
        }

        ActiveProcess curProcess = readyQueue.top();
        readyQueue.pop();
        // Assuming at first that it will use all burst time
        double usedBurstTime = curProcess.leftBurstTime;
        if(isPreemptive()) {
            // Checking if shorter processes have arrived during the burst time of the current process 
            int newArrivals = arrivedProcess;
            while(newArrivals < processContainer.size() && processContainer[newArrivals] -> getArrivalTime() < curTime + curProcess.leftBurstTime) {
                // Preempting current process
                if(processContainer[newArrivals] -> getBurstTime() < curProcess.leftBurstTime - (processContainer[newArrivals] -> getArrivalTime() - curTime) ) {
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
        


    };
    return schList;
}


#endif // __SJF_SCH_
