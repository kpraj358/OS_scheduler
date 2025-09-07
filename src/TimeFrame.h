#if !defined(__TIMEFRAME__)
#define __TIMEFRAME__

#include <memory>
#include "Process.h"

class TimeFrame
{
private:
    
public:
    const double start;
    const double end;
    Process::ProcessPtr process;
    TimeFrame(double start, double end, const Process::ProcessPtr& process=nullptr): start(start), end(end),  process(process){

    };
    inline bool isIdle() const {
        return process == nullptr;
    }
    inline std::string getName() const{
        if(isIdle()) return "Idle";
        else return process -> getName();
    }
};



#endif // __TIMEFRAME__
