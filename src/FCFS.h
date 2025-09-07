#if !defined(__FCFC_SCH__)
#define __FCFC_SCH__    

#include "./Scheduler.h"
#include <algorithm>

class FCFS : public Scheduler
{
private:

public:
    inline ScheduleList schedule();
};

ScheduleList FCFS::schedule() {
    std::sort(processContainer.begin(), processContainer.end(), arrivalTime_cmp());
    ScheduleList schList;
    double curTime = 0;
    for(auto& curProcess: processContainer) {

        // If current process arrival time is ahead of the last time frame 
        // creates an idle time frame which ends at arrival time of current process  
        if(curTime < curProcess -> getArrivalTime()) {
            schList.addTimeFrame(TimeFrame(curTime, curProcess -> getArrivalTime(), nullptr));
            curTime = curProcess -> getArrivalTime();
        }

        TimeFrame curTimeFrame(curTime, curProcess -> getBurstTime() + curTime, curProcess);
        schList.addTimeFrame(curTimeFrame);
        curTime += curProcess -> getBurstTime();
    }
    return schList;
}




#endif // __FCFC_SCH__
