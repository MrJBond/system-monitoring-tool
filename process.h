#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "disk.h"

namespace systemMonitor{

  class ProcessMonitor : public FileReader
  {
    private:
        void readProcInfo(); // cpuinfo
        int getMaxProc();
        int maxProc;
    public:
	ProcessMonitor(){
           this->maxProc = this->getMaxProc();
        };
	~ProcessMonitor() = default;
	void read() override;
        void readStat();// stat
        void runningProcPlot();
        void procPlot();
  };
}
