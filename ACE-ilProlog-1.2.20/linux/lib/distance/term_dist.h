
#ifndef TERM_DIST_INC
#define TERM_DIST_INC

#include "base_dist.h"
#include "prolog_dist.h"
#include "ptset_dist.h"

class AtomSubst {
 public:
  GAPIC_atom a;
  GAPIC_atom b;
  int IsSubstituted(const GAPIC_atom& x,const GAPIC_atom& y) const {
    return (a==x)&&(b==y);
  }
};

class IDSubst {
 public:
  int a;
  int b;
  int IsSubstituted(int x,int y) const {
    return (a==x)&&(b==y);}
  IDSubst(int _a,int _b) : a(_a),b(_b) {}
  IDSubst() {}
};

typedef const AtomSubst *AtomSubstConstPtr;

class AtomSubstList : public vector<AtomSubstConstPtr> {  
 public:
  int IsSubstituted(const GAPIC_atom& a,const GAPIC_atom& b) const;
};

class IDSubstList : public vector<IDSubst> {
 public:
  int IsSubstituted(int a,int b) const;
};

extern IDSubstList empty_atomsubst;

class ShncTermDistance : public Distance {
 public:
  typedef GAPIC_term_ref element_type;
  typedef GAPIC_float domain_type;
  domain_type compute_distance(element_type x,element_type y,
			       const IDSubstList *subst) const;
  domain_type compute_distance(element_type x,element_type y) const {
    return compute_distance(x,y,&empty_atomsubst);
  }
  domain_type compute_sqr_distance(element_type x,element_type y) const {
    GAPIC_float d=compute_distance(x,y);return d*d;
  }
  ShncTermDistance()
    : Distance("ShncTermDistance",DISTANCE_STRICTMETRIC) {}
};

class PrologRealShncTermDistance 
: public PrologRealDistance {
 private:
  ShncTermDistance dist;
 public:
  virtual ostream& Print(ostream& os) const;
  virtual GAPIC_result td_compute(GAPIC_term_ref a,GAPIC_term_ref b,
				  GAPIC_float* _dist) const;
  virtual GAPIC_result id_compute(cheap_id a, cheap_id b, 
				  GAPIC_float* _dist) const;  
  PrologRealShncTermDistance() {}
  virtual ~PrologRealShncTermDistance();
};


extern gapic_type PrologRealShncTermDistanceType;
typedef GapicSubCollectable<PrologRealShncTermDistance,
  &PrologRealShncTermDistanceType,
  PrologRealDistanceCollectable> PrologRealShncTermDistanceCollectable;

class PtsetClauseDistance : public Distance {
 private:
  static const GAPIC_functor greedy_functor;
  const ShncTermDistance term_dist;
  char greedy;
  int max_ungreedy_count;
 public:
  const ShncTermDistance *GetTermDist() const {
    return &term_dist;}
  typedef GAPIC_term_ref element_type;
  typedef GAPIC_float domain_type;
  domain_type compute_distance(element_type x,element_type y,
			       const IDSubstList *subst) const;
  domain_type compute_distance(element_type x,element_type y) const {
    return compute_distance(x,y,&empty_atomsubst);
  }
  domain_type compute_sqr_distance(element_type x,element_type y) const {
    GAPIC_float d=compute_distance(x,y);return d*d;
  }
  int IsGreedy() const {
    return greedy;}
  int MaxUngreedyCount() const {
    return max_ungreedy_count;}
  PtsetClauseDistance()
    : greedy(0),max_ungreedy_count(0),
    Distance("ShncTermDistance",DISTANCE_STRICTMETRIC) {}
  PtsetClauseDistance(GAPIC_term_ref src)
    throw(GapicObjectCreationError);
};

class PrologRealPtsetClauseDistance 
: public PrologRealDistance {
 private:
  PtsetClauseDistance dist;
 public:
  virtual ostream& Print(ostream& os) const;
  virtual GAPIC_result td_compute(GAPIC_term_ref a,GAPIC_term_ref b,
				  GAPIC_float* _dist) const;
  virtual GAPIC_result id_compute(cheap_id a, cheap_id b, 
				  GAPIC_float* _dist) const;  
  PrologRealPtsetClauseDistance() {}
  PrologRealPtsetClauseDistance(GAPIC_term_ref src) 
    : dist(src) {} 
  virtual ~PrologRealPtsetClauseDistance();
};



extern gapic_type PrologRealPtsetClauseDistanceType;
typedef GapicSubCollectable<PrologRealPtsetClauseDistance,
  &PrologRealPtsetClauseDistanceType,
  PrologRealDistanceCollectable> PrologRealPtsetClauseDistanceCollectable;

#endif

