
#ifndef OODIST_BASE_INC
#define OODIST_BASE_INC



#include <vector.h>
#include <iostream.h>
#include <math.h>
#include "../gapic/gapicgc.h"

extern GAPIC_functor_static exact1_functor;

class PrologOODist {
 public:
	virtual void compute_distance(GAPIC_term_ref obj1, GAPIC_term_ref obj2, GAPIC_float* dist) const=0;

	virtual void approx_distance(GAPIC_term_ref obj1, GAPIC_term_ref obj2, GAPIC_term_ref hint, GAPIC_term_ref result) const=0;

  ostream& Print(ostream& os) const;
  ~PrologOODist();
};

extern gapic_type GapicPrologOODistType;
typedef GapicCollectable<PrologOODist,&GapicPrologOODistType>
	PrologOODistCollectable;

class ExactPrologOODist : public PrologOODist {
 public:
  virtual void approx_distance(GAPIC_term_ref obj1, GAPIC_term_ref obj2, GAPIC_term_ref hint, GAPIC_term_ref result) const;
  ~ExactPrologOODist();
};

extern gapic_type GapicExactPrologOODistType;
typedef GapicSubCollectable<ExactPrologOODist,&GapicExactPrologOODistType,PrologOODistCollectable>
	ExactPrologOODistCollectable;

#endif
