#include "./ScheduleList.h"

void ScheduleList::addTimeFrame(const TimeFrame& timeFrame) {
    timeline.push_back(timeFrame);
    if(!timeFrame.isIdle()) {
        double turnAroundTime = timeFrame.end - timeFrame.process -> getArrivalTime();
        double waitingTime = turnAroundTime - timeFrame.process -> getBurstTime();
        processMetrics[timeFrame.process] = PMetric(waitingTime, turnAroundTime);
    }
}

std::vector<Process::ProcessPtr> ScheduleList::getProcesses() const {
    std::vector<Process::ProcessPtr> processes;
    for(const auto& item: processMetrics) {
        processes.push_back(item.first);
    }
    return processes;
}
std::vector<TimeFrame> ScheduleList::getTimeline() const {
    return timeline;
}

PMetric ScheduleList::getProcessMetric(const Process::ProcessPtr& process) const{
    if(processMetrics.find(process) != processMetrics.end())
        ;

    return processMetrics.at(process);
}

int ScheduleList::numberOfProcesses() const {
    return processMetrics.size();
}
