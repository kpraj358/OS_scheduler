#if !defined(_SCHEDULER)
#define _SCHEDULER

#include <vector>
#include "./Process.h"
#include "./ScheduleList.h"



class Scheduler
{
private:
    bool preemptive;
    int quantum;
protected:
    class ActiveProcess {
    public:
        double leftBurstTime;
        Process::ProcessPtr process;
        ActiveProcess(const Process::ProcessPtr& process) {
            leftBurstTime = process -> getBurstTime();
            this -> process = process;
        }
    };
    std::vector<Process::ProcessPtr> processContainer;
public:
    Scheduler() {
        preemptive = false;
        quantum = 1;
    }
    virtual ScheduleList schedule() = 0;
    void addProcess(const Process& process);
    unsigned int processCount();

    inline int getQuantum() {
        return quantum;
    };
    inline void setQuantum(unsigned int q) {
        if(q != 0)
            quantum = q;
    };

    inline bool isPreemptive() {
        return preemptive;
    };
    inline void setPreemptive(bool state) {
        preemptive = state;
    }

    enum Types {FCFS, SJF, PRIORITY, RB};
};

struct arrivalTime_cmp {
    bool operator () (const Process::ProcessPtr& p1, const Process::ProcessPtr& p2) const {
        return p1 -> getArrivalTime() < p2 -> getArrivalTime();
    }
};

struct burstTime_cmp {
    bool operator () (const Process::ProcessPtr& p1, const Process::ProcessPtr& p2) const {
        if(p1 -> getArrivalTime() == p2 -> getArrivalTime()) return p1 -> getBurstTime() < p2 -> getBurstTime();
        else return p1 -> getArrivalTime() < p2 -> getArrivalTime();
    }
};

struct priority_cmp {
    bool operator () (const Process::ProcessPtr& p1, const Process::ProcessPtr& p2) const {
        if(p1 -> getArrivalTime() == p2 -> getArrivalTime())
            return p1 -> getPriority() < p2 -> getPriority();
        else 
            return p1 -> getArrivalTime() < p2 -> getArrivalTime();
    }
};


#endif // _SCHEDULER
