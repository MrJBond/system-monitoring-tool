#include <iostream>
#include <string>
#include <fstream>
#include "process.h"
#include <cmath>
#include <iomanip>

namespace systemMonitor{

  class MemoryManager : public FileReader{

    private:
        void getMemInfo();
        const string meminfo = "/proc/meminfo";
        const string vmstat = "/proc/vmstat";
        const int maxRam = this->findNumber("MemTotal", meminfo);
        const uint64_t pageSize = sysconf(_SC_PAGESIZE);
        const uint64_t maxPages = maxRam/(pageSize/1024); // convert the size to KB
        int *ar = nullptr;
        void loadRam();
        void freeRam();
    public:
        MemoryManager() = default;
        ~MemoryManager(){
           if(ar != nullptr){
             this->freeRam();
           }
        };
        void read() override;
        void activeMemoryPlot();
        void freeMemoryPlot();
        void vmallocUsedPlot();
        void percpuPlot();
        void availableMemPlot();
        void committedAsPlot();
        void cachedPlot();
        void swapFreePlot();
        void inactiveMemPlot();
        void activeAnonPlot();
        void activeFilePlot();
        void anonPagesPlot();
        void mappedPlot();
        void slabPlot();
        void freePagesPlot();
        void writtenPagesPlot(); 
  };
}
