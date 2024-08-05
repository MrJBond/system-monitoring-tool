#include <iostream>
#include "memoryManager.h"
#include <thread>
#include <chrono>
#include <functional>
#include <mutex>
#include <atomic>


using namespace systemMonitor;

class Tool{
   private:
      thread memoryThread;
      thread processThread;
      // another threads...

      MemoryManager mm;
      ProcessMonitor pm;
      DiskMonitor diskMonitor;
      InfoReader infoReader;
      bool stop = false;
      mutex mx; // to lock the printer function

     void start();
     void end();
     void printer(function<void()> func, int n); // will use it as the shared resource (is used to work with threads)

     const string meminfo = "/proc/meminfo";
     const string stat = "/proc/stat";
   public:
     Tool(){
      this->pm = ProcessMonitor();
     };
     ~Tool(){
         this->end();
     };
     void go();
     void printerNoTh(int n); // is used without threads (1 - procStat, 2 - diskStat)
     void printInfo(){
        while(1){
           char input;
           this->infoReader.info();
           cout << "\nPress any key to continue...\n";
           cin >> input;
          if(input){
             break;
          }
        }
     }
     void getUpTime();

    /* ***************************
                PLOTS
   *******************************/

     void activeMemoryPlot(){
           this->mm.activeMemoryPlot();
     }
     void freeMemoryPlot(){
         this->mm.freeMemoryPlot();
     }
     void runningProcPlot(){
         this->pm.runningProcPlot();
     }
     void procPlot(){
        this->pm.procPlot();
     }
     void vmallocUsedPlot(){
        this->mm.vmallocUsedPlot();
     }
     void percpuPlot(){
       this->mm.percpuPlot();
     }
     void availableMemPlot(){
       this->mm.availableMemPlot();
     }
     void committedASPlot(){
       this->mm.committedAsPlot();
     }
     void cachedMemPlot(){
       this->mm.cachedPlot();
     }
     void swapFreePlot(){
       this->mm.swapFreePlot();
     }
     void inactiveMemPlot(){
        this->mm.inactiveMemPlot();
     }
     void activeAnonPlot(){
        this->mm.activeAnonPlot();
     }
     void activeFilePlot(){
        this->mm.activeFilePlot();
     }
     void anonPagesPlot(){
       this->mm.anonPagesPlot();
     }
     void mappedMemPlot(){
        this->mm.mappedPlot();
     }
     void slabMemPlot(){
        this->mm.slabPlot();
     }
     void freePagesPlot(){
        this->mm.freePagesPlot();
     }
     void writtenPagesPlot(){
        this->mm.writtenPagesPlot();
     }

     /**********************************************
                  AVERAGE + DISPERSION
     ************************************************/

     void  avgAvailMem(){
       this->mm.countAverage("MemAvailable", meminfo);
     }
     void diperAvailMem(){
       this->mm.countDispersion("MemAvailable", meminfo);
     }
     void avgActiveMem(){
       this->mm.countAverage("Active", meminfo);
     }
     void disperActiveMem(){
       this->mm.countDispersion("Active", meminfo);
     }
     void avgRunProc(){
       this->pm.countAverage("procs_running", stat);
     }
     void disperRunProc(){
       this->pm.countDispersion("procs_running", stat);
     }
     void avgFreeMem(){
       this->mm.countAverage("MemFree", meminfo);
     }
     void disperFreeMem(){
       this->mm.countDispersion("MemFree", meminfo);
     }
     void avgProc(){
       this->pm.countAverage("processes", stat);
     }
     void dispersionProc(){
        this->pm.countDispersion("processes", stat);
     }

     /*****************************************************
                        MEDIAN + MODE
     ******************************************************/

     void medianAvailMem(){
         this->mm.countMedian("MemAvailable", meminfo);
     }
     void modeAvailMem(){
         this->mm.countMode("MemAvailable", meminfo);
     }
     void medianActiveMem(){
         this->mm.countMedian("Active", meminfo);
     }
     void modeActiveMem(){
         this->mm.countMode("Active", meminfo);
     }
     void medianRunProc(){
         this->mm.countMedian("procs_running", stat);
     }
     void modeRunProc(){
         this->mm.countMode("procs_running", stat);
     }
    void medianFreeMem(){
         this->mm.countMedian("MemFree", meminfo);
     }
     void modeFreeMem(){
         this->mm.countMode("MemFree", meminfo);
     }
     void medianProc(){
        this->pm.countMedian("processes", stat);
     }
     void modeProc(){
       this->pm.countMode("processes", stat);
     }

    /****************************************************
                          Max Min
   *******************************************************/
   void maxMinAvailMem(){
      this->mm.countMaxMin("MemAvailable", meminfo);
   }
   void maxMinActiveMem(){
      this->mm.countMaxMin("Active", meminfo);
   }
   void maxMinRunProc(){
     this->pm.countMaxMin("procs_running", stat);
   }
   void maxMinFreeMem(){
      this->mm.countMaxMin("MemFree", meminfo);
   }
   void maxMinProc(){
      this->pm.countMaxMin("processes", stat);
   }
   void maxMinMemPercpu(){
       this->mm.countMaxMin("Percpu", meminfo);
   }
   void maxMinMemCommitted(){
       this->mm.countMaxMin("Committed_AS", meminfo);
   }
   void maxMinMemCached(){
       this->mm.countMaxMin("Cached", meminfo);
   }
   void maxMinMemInactive(){
      this->mm.countMaxMin("Inactive", meminfo);
   }
   void maxMinMemActiveAnon(){
      this->mm.countMaxMin("Active(anon)", meminfo);
   }
   void maxMinMemMapped(){
      this->mm.countMaxMin("Mapped", meminfo);
   }
   void maxMinMemKernelStack(){
      this->mm.countMaxMin("KernelStack", meminfo);
   }
  /******************************************************
               Asymmetry and kurtosis
  *******************************************************/
  void asymmetryKurAvailMem(){
     this->mm.countAsymmetryKurtosis("MemAvailable", meminfo);
  }
   void asymmetryKurActiveMem(){
     this->mm.countAsymmetryKurtosis("Active", meminfo);
  }
  void asymmetryKurRunProc(){
     this->pm.countAsymmetryKurtosis("procs_running", stat);
  }
  void asymmetryKurFreeMem(){
     this->mm.countAsymmetryKurtosis("MemFree", meminfo);
  }
  void asymmetryKurProc(){
     this->pm.countAsymmetryKurtosis("processes",stat);
  }
  void asymmetryKurMemPercpu(){
     this->mm.countAsymmetryKurtosis("Percpu", meminfo);
  }
  void asymmetryKurMemCommitted(){
     this->mm.countAsymmetryKurtosis("Committed_AS", meminfo);
  }
  void asymmetryKurMemCached(){
      this->mm.countAsymmetryKurtosis("Cached", meminfo);
  }
  void asymmetryKurMemInactive(){
      this->mm.countAsymmetryKurtosis("Inactive", meminfo);
  }
  void asymmetryKurMemActiveAnon(){
      this->mm.countAsymmetryKurtosis("Active(anon)", meminfo);
  }
  void asymmetryKurMemMapped(){
     this->mm.countAsymmetryKurtosis("Mapped", meminfo);
  }
  void asymmetryKurMemKernelStack(){
     this->mm.countAsymmetryKurtosis("KernelStack", meminfo);
  }
  /**********************************************************
                       GRAPH
  *************************************************************/
  void MSTAvailMem(){
     this->mm.countMST("MemAvailable", meminfo);
  }
  void MSTRunningProc(){
     this->pm.countMST("procs_running", stat);
  }
};
