#include "./Scheduler.h"


void Scheduler::addProcess(const Process& process) {
    Process::ProcessPtr copiedProcess(std::make_shared<Process>(process));
    processContainer.push_back(copiedProcess);
}

uint Scheduler::processCount() {
    return processContainer.size();
}



