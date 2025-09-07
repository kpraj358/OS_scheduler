#if !defined(_PROCESS)
#define _PROCESS

#include <string>
#include <memory>
#include <vector>
class Process
{
private:
    std::string name;
    double arrivalTime;
    double burstTime;
    uint priority;
public:
    typedef std::shared_ptr<Process> ProcessPtr;
    Process(std::string name, double arrivalTime = 0, double burstTime = 0, uint priority = 0);
    std::string getName() const;
    double getArrivalTime() const;
    double getBurstTime() const;
    uint getPriority() const;
};



#endif // _PROCESS
