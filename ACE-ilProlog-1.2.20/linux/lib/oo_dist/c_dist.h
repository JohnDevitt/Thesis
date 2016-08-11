
#ifndef OODIST_C_INC
#define OODIST_C_INC



#include <vector.h>
#include <iostream.h>
#include <math.h>
#include "../gapic/gapicgc.h"

class IntDistance {
 public:
  virtual void compute_distance(int a,int b,double *dist) const=0;
  virtual ~IntDistance();
};



#endif
