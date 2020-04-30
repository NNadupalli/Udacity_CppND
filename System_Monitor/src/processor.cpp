
#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    vector<string> jiffies_list = LinuxParser::CpuUtilization();

    vector<double> jiffies(jiffies_list.size(),0);
    for(int i =0; i< jiffies_list.size(); i++){
        jiffies[i] = stod(jiffies_list[i]); 
    }
    

    return 0.0; 
}