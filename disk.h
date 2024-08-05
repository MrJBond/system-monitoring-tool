#include "fileInfo.h"

namespace systemMonitor{
     class DiskMonitor : public FileReader{
          private:
            void readDiskStat(); // diskstat 
          public:
	    DiskMonitor() = default;
            ~DiskMonitor() = default;
            void read() override;
     };
};
