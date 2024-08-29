#include "tool.h"

atomic<bool> memDone(false); // when two threads are done, start again
atomic<bool> procDone(false);

void Tool::printer(function<void()> func, int n){ // n is the function: 1 - memory 2 - process
    mx.lock();
   
    auto startWork = chrono::steady_clock::now();
    while(1){
        auto endWork = chrono::steady_clock::now();
        if(this->stop){
            break;
        }
        if(chrono::duration_cast<chrono::seconds>(endWork-startWork).count() > 30){ // work for 30 sec
           if(n == 1)
              memDone = true;
           else
             procDone = true;
 
            break;
        } 
        system("clear"); 
    
    	func();

	if(n == 2){ // for proc. info
           this_thread::sleep_for(chrono::seconds(10));
        }else // for memory info
           this_thread::sleep_for(chrono::milliseconds(500));
   }

    mx.unlock();
}
void Tool::start(){ 
     this->stop = false;
     // each thread works for 30 sec
      memoryThread = thread([&](){
 	        printer([&](){mm.read();}, 1);
      });

      processThread = thread([&](){
      		  printer([&](){pm.read();}, 2);
      });
     // another threads with another funcs.
}
void Tool::end(){
    this->stop = true;
    if(memoryThread.joinable()){   
      memoryThread.join();
    }
    if(processThread.joinable()){
       processThread.join();
    }
  // another threads
}
void Tool::go(){
   this->start();
   auto timeStart = chrono::steady_clock::now();
   while(true){
      auto end = chrono::steady_clock::now();
      if(chrono::duration_cast<chrono::seconds>(end - timeStart).count() > 120){ // work for two min
          this->end();
          break;
      }
      if(memDone && procDone){ // create again
	  if(memoryThread.joinable())
              memoryThread.join();
	  if(processThread.joinable())
              processThread.join();
              
          this->start();
      }
    }    

}
void Tool::printerNoTh(int n){
    auto start = chrono::steady_clock::now();

    while(true){
       auto end = chrono::steady_clock::now();
       system("clear");
       if(n == 1){
          this->pm.readStat();
       }       
       else if(n == 2){
           this->diskMonitor.read(); 
       } // another funcs...
       if(chrono::duration_cast<chrono::seconds>(end-start).count() > 30){
           break;
       }
       this_thread::sleep_for(chrono::milliseconds(500));    
   }
}
void Tool::getUpTime(){
    ifstream input("/proc/uptime");
    string line, number;
    getline(input, line);
    input.close();

    // till the white space (the first value is the time system has been up (in seconds))
    for(int i = 0; line[i] != ' '; ++i){
        number += line[i];
    }
    uint64_t n = stoll(number);
    uint64_t h = n / 3600;
    n -= h*3600;
    uint64_t m = n / 60;
    n -= m*60;
    uint64_t s = n;
    cout << "Up Time: "<< h << " : " << m << " : " << s << endl << endl;
}
void Tool::getSystemTime(){
    auto time_ = chrono::system_clock::to_time_t(chrono::system_clock::now());
    cout << "System time: " << ctime(&time_) << endl << endl;
}
