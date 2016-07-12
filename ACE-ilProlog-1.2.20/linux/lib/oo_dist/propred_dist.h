
#ifndef SCALAR_DISTANCE_INC
#define SCALAR_DISTANCE_INC



#include "oodist_base.h"

class ExactPredicateOODist : public ExactPrologOODist {
 private:
  GAPIC_functor m_dist_pred;
 public:
  virtual void compute_distance(GAPIC_term_ref obj1, GAPIC_term_ref obj2, double *dist) const;
  ostream& Print(ostream& os) const;
  ExactPredicateOODist(GAPIC_term_ref src);
  ~ExactPredicateOODist();
};


extern gapic_type GapicExactPredicateOODistType;
typedef GapicSubCollectable<ExactPredicateOODist,&GapicExactPredicateOODistType,ExactPrologOODistCollectable> 
	ExactPredicateOODistCollectable;

#endif
