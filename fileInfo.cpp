#include "fileInfo.h"

void systemMonitor::InfoReader::info(){
    cout << "/etc/issue" << endl;
    cout << "The /etc/issue is a text file which contains \n";  
    cout << "a message or system identification to be printed \n before the login prompt." << endl << endl;
    cout << "/proc/version" << endl;
    cout << "This file specifies the version of the Linux kernel,\n the version of gcc used to compile the kernel,\n";
    cout << " and the time of kernel compilation. \nIt also contains the kernel compiler's ";
    cout << "user name (in parentheses)." << endl << endl;
    cout << "/proc/diskstats" << endl;
    cout << "This file displays the I/O statistics of block devices. Each line contains the following fields:\n";
    cout << "1  major number\n2  minor number\n3  device name\n4  reads completed successfully\n5  reads merged\n";
    cout << "6  sectors read\n7  time spent reading (ms)\n8  writes completed\n9  writes merged\n";
    cout << "10  sectors written\n\n11  time spent writing (ms)\n12  I/Os currently in progress\n";
    cout << "13  time spent doing I/Os (ms)\n14  weighted time spent doing I/Os (ms)\n";
    cout << "15  discards completed successfully\n16  discards merged\n17  sectors discarded\n";
    cout << "18  time spent discarding\n19  flush requests completed successfully\n20  time spent flushing" << endl << endl;
    cout << "/proc/meminfo" << endl;
    cout << "The report about memory on the system" << endl << endl;
    cout << "/proc/cpuinfo" << endl;
    cout << "Contains information about all CPUs present on the system" << endl << endl;
    cout << "/proc/stat" << endl;
    cout << "user: normal processes executing in user mode\n";
    cout << "nice: niced processes executing in user mode\n";
    cout << "system: processes executing in kernel mode\n";
    cout << "idle: twiddling thumbs\n";
    cout << "iowait: waiting for I/O to complete\n";
    cout << "irq: servicing interrupts\n";
    cout << "softirq: servicing softirqs" << endl << endl;
    cout << "/proc/sys/kernel/pid_max   - To get the max number of processes" << endl << endl;
    cout << "/proc/vmstat provides information on virtual memory usage of both system and process" << endl << endl;
    cout << "/proc/uptime - contains information detailing how long the system has been on since its last restart"
    << endl << endl;
   // Other used files...
}
