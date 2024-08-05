#include "disk.h"
void systemMonitor::DiskMonitor::read(){
    cout << "Disk info:" << endl;
    this->readDiskStat(); 
}
// /proc/diskstat
void systemMonitor::DiskMonitor::readDiskStat(){
     ifstream infile("/proc/diskstats");
     string input;
     while(getline(infile, input)){
         string sub = input.substr(0, 100);
         cout << sub << endl;
     }
     infile.close();
}
