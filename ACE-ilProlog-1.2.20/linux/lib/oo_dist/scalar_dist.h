
#ifndef SCALAR_DISTANCE_INC
#define SCALAR_DISTANCE_INC



#include "oodist_base.h"

class IntPrologOODist : public ExactPrologOODist {
 public:
  virtual void compute_distance(GAPIC_term_ref obj1, GAPIC_term_ref obj2, double *dist) const;
  ostream& Print(ostream& os) const;
  ~IntPrologOODist();
};


extern gapic_type GapicIntPrologOODistType;
typedef GapicSubCollectable<IntPrologOODist,&GapicIntPrologOODistType,ExactPrologOODistCollectable> 
	IntPrologOODistCollectable;

class FloatPrologOODist : public ExactPrologOODist {
 public:
  virtual void compute_distance(GAPIC_term_ref obj1, GAPIC_term_ref obj2, double *dist) const;
  ostream& Print(ostream& os) const;
  ~FloatPrologOODist();
};


extern gapic_type GapicFloatPrologOODistType;
typedef GapicSubCollectable<FloatPrologOODist,&GapicFloatPrologOODistType,ExactPrologOODistCollectable> 
	FloatPrologOODistCollectable;

#endif
