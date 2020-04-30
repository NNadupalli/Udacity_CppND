#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector> 
#include <string>
#include "linux_parser.h"

using std::vector;
using std::string;
using std::stod;

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
};

#endif