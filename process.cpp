#include "process.h"

// cat /proc/cpuinfo
void systemMonitor::ProcessMonitor::readProcInfo(){
    ifstream infile("/proc/cpuinfo");
    string line;
    while(getline(infile, line)){
        string sub = line.substr(0, 100);
           // general info
        cout << sub << endl;
    }
    infile.close();
}
void systemMonitor::ProcessMonitor::read(){
    cout << "Process info: " << endl;
    readProcInfo();
}

// proc/stat
void systemMonitor::ProcessMonitor::readStat(){
    ifstream infile("/proc/stat");
    cout << "The amount of time CPU has spent performing different kinds of work (hundredths of second)" << endl;
    cout << "    user " << "nice " << "sys "  << "idle " << "iowait " 
    << "irq " << "softirq " << "steal " << "guest" << endl; 
    string line;
    while(getline(infile, line)){
       string sub = line.substr(0, 100);
       cout << sub << endl;
    }
    infile.close();
}
int systemMonitor::ProcessMonitor::getMaxProc (){
    ifstream ifs("/proc/sys/kernel/pid_max");
    string line;
    getline(ifs, line);
    string sub = line.substr(0,6);
    int maxProc = stoi(sub);
    return maxProc;
}
void systemMonitor::ProcessMonitor::runningProcPlot(){
    this->visualizePlot("procs_running", "/proc/stat", maxProc);
}
void systemMonitor::ProcessMonitor::procPlot(){
    this->visualizePlot("processes", "/proc/stat", maxProc);
}
