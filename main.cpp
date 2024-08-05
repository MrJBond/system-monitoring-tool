#include <iostream>
#include <thread>
#include <chrono>
#include "tool.h"

// Get the basic info (OS and Kernel version)
void getBasicInfo(){
    // /etc/issue -> OS
    // /proc/version -> kernel

   ifstream inf("/etc/issue");
   string input;
   while(getline(inf, input)){
       string outs = input.substr(0, 16);
       cout << outs << endl;
   }
   inf.close();
   ifstream inv("/proc/version");
   while(getline(inv, input)){
       string outs = input.substr(0, 30);
       cout << outs << endl;
   }
   inv.close();
   cout << endl;
}
void menu(Tool& tool);
void plots(Tool& tool);
void plotsMenu();
void properties(Tool& tool);
void propertiesMenu();

int main(){
   Tool tool = Tool();

  bool stop = false;
  do{
     if(stop){
        break;
      }
     system("clear");
     menu(tool);
     int answer;
     cin >> answer;
     switch(answer){
       case 1:
	tool.go();
	break;
       case 2:
        tool.printerNoTh(1);// procStat
        break;
       case 3:
        tool.printerNoTh(2); // diskStat
        break;
       case 4:
        plots(tool);
        break;
       case 5:
       properties(tool);
       break;
       case 6:
       tool.printInfo();
       break;
       case 0: // TODO more cases
         stop = true;
         break;
      default:
	cout << "The entered number is incorrect!" << endl;
        break;
     }
     }while(1);
}
void menu(Tool& tool){
   getBasicInfo();
   tool.getUpTime();
   cout << "View memory info (1)" << endl;
   cout << "Show process statistics (2)" << endl; 
   cout << "Show disk statistics (3)" << endl;
   cout << "Plots (4)" << endl;
   cout << "Properties (5)" << endl;

   // more statements...

   cout << "Used files (6)" << endl;
   cout << "Exit (0)" << endl;
}

void plots(Tool& tool){
   bool stop = false;
   do{
   if(stop){
      break;
    }
   system("clear");
   plotsMenu();
   int answer;
   cin >> answer;
   switch(answer){
    case 1:
    tool.activeMemoryPlot();
    break;
    case 2:
    tool.freeMemoryPlot();
    break;
    case 3:
    tool.runningProcPlot();
    break;
    case 4:
    tool.vmallocUsedPlot();
    break;
    case 5:
    tool.percpuPlot();
    break;
    case 6:
    tool.availableMemPlot();
    break;
    case 7:
    tool.committedASPlot();
    break;
    case 8:
    tool.cachedMemPlot();
    break;
    case 9:
    tool.swapFreePlot();
    break;
    case 10:
    tool.inactiveMemPlot();
    break;
    case 11:
    tool.activeAnonPlot();
    break;
    case 12:
    tool.activeFilePlot();
    break;
    case 13:
    tool.anonPagesPlot();
    break;
    case 14:
    tool.mappedMemPlot();
    break;
    case 15:
    tool.slabMemPlot();
    break;
    case 16:
    tool.procPlot();
    break;
    case 17:
    tool.freePagesPlot();
    break;
    case 18:
    tool.writtenPagesPlot();
    break;
    case 0:
    stop = true;
    break;
    default:
    cout << "The entered number is incorrect!" << endl;
    break;
   }
   }while(1);
}
void plotsMenu(){
   cout << "Active memory plot (1)" << endl;
   cout << "Free memory plot (2)" << endl;
   cout << "Running processes plot (3)" << endl;
   cout << "Used Vmalloc plot (4)" << endl;
   cout << "Memory used per cpu plot (5)" << endl;
   cout << "Available memory plot (6)" << endl;
   cout << "Memory committed_AS plot (7)" << endl;
   cout << "Cached memory plot (8)" << endl;
   cout << "Free swap plot (9)" << endl;
   cout << "Inactive memory plot (10)" << endl;
   cout << "Active(anon) memory plot (11)" << endl;
   cout << "Anonymous memory that has been used recently and is not typically paged out" << endl;
   cout << "Active(file) memory plot (12)" << endl;
   cout << "Pagecache memory that has been recently used and is typically not released until needed" << endl;
   cout << "AnonPages plot (13)" << endl;
   cout << "Non-file supported pages mapped to user space page tables" << endl;
   cout << "Mapped memory plot (14)" << endl;
   cout << "Files that have been mapped, such as libraries." << endl;
   cout << "Slab memory plot (15)" << endl;
   cout << "Cache of data structures inside the kernel" << endl;
   cout << "Processes plot (16)" << endl;
   cout << "The 'processes' line gives the number of processes and threads created" << endl;
   cout << "Number of free memory pages plot (17)" << endl;
   cout << "Number of written pages plot (18)" << endl;
   cout << "Number of pages that have been written to disk" << endl;
   cout << "Exit (0)" << endl;
}
void properties(Tool& tool){
   bool stop = false;
   do{
   if(stop){
      break;
    }
   system("clear");
   propertiesMenu();
   int answer;
   cin >> answer;
   switch(answer){
   case 1:
   tool.avgAvailMem();
   break;
   case 2:
   tool.diperAvailMem();
   break;
   case 3:
   tool.medianAvailMem();
   break;
   case 4:
   tool.modeAvailMem();
   break;
   case 5:
   tool.avgActiveMem();
   break;
   case 6:
   tool.disperActiveMem();
   break;
   case 7:
   tool.medianActiveMem();
   break;
   case 8:
   tool.modeActiveMem();
   break;
   case 9:
   tool.avgRunProc();
   break;
   case 10:
   tool.disperRunProc();
   break;
   case 11:
   tool.medianRunProc();
   break;
   case 12:
   tool.modeRunProc();
   break;
   case 13:
   tool.avgFreeMem();
   break;
   case 14:
   tool.disperFreeMem();
   break;
   case 15:
   tool.medianFreeMem();
   break;
   case 16:
   tool.modeFreeMem();
   break;
   case 17:
   tool.avgProc();
   break;
   case 18:
   tool.dispersionProc();
   break;
   case 19:
   tool.medianProc();
   break;
   case 20:
   tool.modeProc();
   break;
   case 21:
   tool.maxMinAvailMem();
   break;
   case 22:
   tool.maxMinActiveMem();
   break;
   case 23:
   tool.maxMinRunProc();
   break;
   case 24:
   tool.maxMinFreeMem();
   break;
   case 25:
   tool.maxMinProc();
   break;
   case 26:
   tool.maxMinMemPercpu();
   break;
   case 27:
   tool.maxMinMemCommitted();
   break;
   case 28:
   tool.maxMinMemCached();
   break;
   case 29:
   tool.maxMinMemInactive();
   break;
   case 30:
   tool.maxMinMemActiveAnon();
   break;
   case 31:
   tool.maxMinMemMapped();
   break;
   case 32:
   tool.maxMinMemKernelStack();
   break;
   case 33:
   tool.asymmetryKurAvailMem();
   break;
   case 34:
   tool.asymmetryKurActiveMem();
   break;
   case 35:
   tool.asymmetryKurRunProc();
   break;
   case 36:
   tool.asymmetryKurFreeMem();
   break;
   case 37:
   tool.asymmetryKurProc();
   break;
   case 38:
   tool.asymmetryKurMemPercpu();
   break;
   case 39:
   tool.asymmetryKurMemCommitted();
   break;
   case 40:
   tool.asymmetryKurMemCached();
   break;
   case 41:
   tool.asymmetryKurMemInactive();
   break;
   case 42:
   tool.asymmetryKurMemActiveAnon();
   break;
   case 43:
   tool.asymmetryKurMemMapped();
   break;
   case 44:
   tool.asymmetryKurMemKernelStack();
   break;
   case 45:
   tool.MSTAvailMem();
   break;
   case 46:
   tool.MSTRunningProc();
   break;
   case 0:
   stop = true;
   break;
   default:
   cout << "The entered number is incorrect!" << endl;
   break;
   }
    }while(1);

}
void propertiesMenu(){
   cout << "Average available memory (1)" << endl;
   cout << "Dispersion available memory (2)" << endl;
   cout << "Median available memory (3)" << endl;
   cout << "Mode available memory (4)" << endl;
   cout << "-------------------------------" << endl;
   cout << "Average active memory (5)" << endl;
   cout << "Dispersion active memory (6)" << endl;
   cout << "Median active memory (7)" << endl;
   cout << "Mode active memory (8)" << endl;
   cout << "--------------------------------" << endl;
   cout << "Average running processes (9)" << endl;
   cout << "Dispersion running processes (10)" << endl;
   cout << "Median running processes (11)" << endl;
   cout << "Mode running processes (12)" << endl;
   cout << "-----------------------------" << endl;
   cout << "Average free  memory (13)" << endl;
   cout << "Dispersion free memory (14)" << endl;
   cout << "Median free memory (15)" << endl;
   cout << "Mode free memory (16)" << endl;
   cout << "--------------------------------" << endl;
   cout << "Average processes (17)" << endl;
   cout << "Dispersion processes (18)" << endl;
   cout << "Median processes (19)" << endl;
   cout << "Mode processes (20)" << endl;
   cout << "--------------------------------" << endl;
   cout << "Max and min available memory (21)" << endl;
   cout << "Max and min active memory (22)" << endl;
   cout << "Max and min running processes (23)" << endl;
   cout << "Max and min free memory (24)" << endl;
   cout << "Max and min processes (25)" << endl;
   cout << "Max and min memory per cpu (26)" << endl;
   cout << "Max and min memory committed as (27)" << endl;
   cout << "Max and min memory cached (28)" << endl;
   cout << "Max and min memory inactive (29)" << endl;
   cout << "Max and min memory active(anon) (30)" << endl;
   cout << "Max and min memory mapped (31)" << endl;
   cout << "Max and min memory kernel stack (32)" << endl;
   cout << "--------------------------------" << endl;
   cout << "Asymmetry and kurtosis available memory (33)" << endl;
   cout << "Asymmetry and kurtosis active memory (34)" << endl;
   cout << "Asymmetry and kurtosis running processes (35)" << endl;
   cout << "Asymmetry and kurtosis free memory (36)" << endl;
   cout << "Asymmetry and kurtosis processes (37)" << endl;
   cout << "Asymmetry and kurtosis memory per cpu (38)" << endl;
   cout << "Asymmetry and kurtosis memory committed as (39)" << endl;
   cout << "Asymmetry and kurtosis memory cached (40)" << endl;
   cout << "Asymmetry and kurtosis inactive memory (41)" << endl;
   cout << "Asymmetry and kurtosis active(anon) memory (42)" << endl;
   cout << "Asymmetry and kurtosis mapped memory (43)" << endl;
   cout << "Asymmetry and kurtosis kernel stack memory (44)" << endl;
   cout << "The amount of memory used by the kernel stack allocations done for each task in the system" << endl;
   cout << "--------------------------------------" << endl;
   cout << "The minimum spanning tree of available memory numbers (45)" << endl;
   cout << "The minimum spanning tree of running processes numbers (46)" << endl;
   cout << "--------------------------------------" << endl;
  // more statements

   cout << "Exit (0)" << endl;
}
