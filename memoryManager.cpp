#include "memoryManager.h"

void systemMonitor::MemoryManager::read(){
    cout << endl;
    cout << "Memory: " << endl;
    getMemInfo();
}
void systemMonitor::MemoryManager::getMemInfo(){
    // cat /proc/meminfo
    ifstream info("/proc/meminfo");
    string line;
    int countIter = 3;
    while(getline(info, line)){
        string sub = line.substr(0, 30);
   	//  params. are being changed
          if(countIter % 3 == 0 || countIter % 3 == 1){
              cout  << sub << "       ";
          }
          else{
            cout << sub << endl;
          }
        countIter++; 
      } 
      info.close();
    }

void systemMonitor::MemoryManager::activeMemoryPlot(){
    this->visualizePlot("Active", meminfo, maxRam);
}
void systemMonitor::MemoryManager::freeMemoryPlot(){
    this->visualizePlot("MemFree", meminfo, maxRam);
}

void systemMonitor::MemoryManager::vmallocUsedPlot(){
    long vmallocTotal = this->findNumber("VmallocTotal", meminfo);    
    this->visualizePlot("VmallocUsed", meminfo, vmallocTotal); 
}
void systemMonitor::MemoryManager::percpuPlot(){
    this->visualizePlot("Percpu", meminfo, maxRam);
}
void systemMonitor::MemoryManager::availableMemPlot(){
    this->visualizePlot("MemAvailable", meminfo, maxRam);
}
void systemMonitor::MemoryManager::committedAsPlot(){
   long commitLimit = this->findNumber("CommitLimit", meminfo);
   this->visualizePlot("Committed_AS", meminfo, commitLimit);
}
void systemMonitor::MemoryManager::cachedPlot(){
   this->visualizePlot("Cached", meminfo, maxRam);
}
void systemMonitor::MemoryManager::loadRam(){
  long size = 2000000000;
   try{
    ar = new int[size];
    for (int i = 0; i < size; i++) {
        ar[i] = i;
    }
   }catch (bad_alloc &e) {
        cerr << "Memory allocation failed: " << e.what() << endl;
    }
}
void systemMonitor::MemoryManager::freeRam(){ 
   if(ar != nullptr){
      delete[] ar;
      ar = nullptr;
   }
}
void systemMonitor::MemoryManager::swapFreePlot(){ 
    long swapTotal = this->findNumber("SwapTotal", meminfo);
    this->visualizePlot("SwapFree", meminfo, swapTotal);
    cout << "Do you want to load the RAM (y/n)" << endl;
    char a;
    cin >> a;
    if(a == 'y'){
     this->loadRam();
     this->visualizePlot("SwapFree", meminfo, swapTotal);
     this->freeRam();
    }else{
      return;
    }
}
void systemMonitor::MemoryManager::inactiveMemPlot(){
    this->visualizePlot("Inactive", meminfo, maxRam);
}
void systemMonitor::MemoryManager::activeAnonPlot(){
    this->visualizePlot("Active(anon)", meminfo, maxRam);
}
void systemMonitor::MemoryManager::activeFilePlot(){
    this->visualizePlot("Active(file)", meminfo, maxRam);
}
void systemMonitor::MemoryManager::anonPagesPlot(){
    this->visualizePlot("AnonPages", meminfo, maxRam);
}
void systemMonitor::MemoryManager::mappedPlot(){
    this->visualizePlot("Mapped", meminfo, maxRam);
}
void systemMonitor::MemoryManager::slabPlot(){
    this->visualizePlot("Slab", meminfo, maxRam);
}
void systemMonitor::MemoryManager::freePagesPlot(){
    this->visualizePlot("nr_free_pages", vmstat, maxPages);
}
void systemMonitor::MemoryManager::writtenPagesPlot(){
    this->visualizePlot("nr_written", vmstat, maxPages);

    cout << "Do you want to flush some pages to the disk? (y/n)" << endl;
    char a;
    cin >> a;
    if(a == 'y'){
         int fd = open("file.txt", O_WRONLY | O_CREAT, 0644);
         if (fd == -1) {
            cerr << "File opening error!" << endl;
         }
          // write some data to the file
          for(int i = 0; i<50000000; ++i){
              const char* s = "Hello, world!";
              write(fd, s, 13);
         }

          // Flush the file's dirty pages to disk
          fsync(fd);

          close(fd);

          this->visualizePlot("nr_written", vmstat, maxPages);
    }else{
      return;
    }
}

