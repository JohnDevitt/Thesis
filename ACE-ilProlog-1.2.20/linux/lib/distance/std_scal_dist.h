
#ifndef STD_SCALAR_DIST_INC
#define STD_SCALAR_DIST_INC

#include "scalar_dist.h"
#include "prolog_dist.h"


extern gapic_type StdFloatDistanceType;
typedef GapicCollectable<StdFloatDistance,&StdFloatDistanceType> 
StdFloatDistanceCollectable;


class PrologRealScalarDistance : public PrologRealDistance {
 private:
  inline GAPIC_result check_get_float(GAPIC_term_ref src,GAPIC_float* dst,
				      const char *method, int argnr) const;
 public:
  virtual ostream& Print(ostream& os) const;
  virtual GAPIC_result td_compute(GAPIC_term_ref a,GAPIC_term_ref b,
			       GAPIC_float *dist) const;
  virtual GAPIC_result compute(GAPIC_term_ref a,GAPIC_term_ref b,
			       GAPIC_term_ref dist) const;
  virtual GAPIC_float compute(GAPIC_float a,GAPIC_float b) const=0;
  virtual GAPIC_result id_compute(const cheap_id a,const cheap_id b,
				  domain_type *dist) const;
  PrologRealScalarDistance() {}
  virtual ~PrologRealScalarDistance() {}
};

extern gapic_type PrologRealScalarDistanceType;
typedef GapicSubCollectable<PrologRealScalarDistance,
  &PrologRealScalarDistanceType,
  PrologRealDistanceCollectable> PrologRealScalarDistanceCollectable;

class PrologStdFloatDistance : public PrologRealScalarDistance {
 public:
  static const StdFloatDistance stdfloat;
  typedef GAPIC_float element_type;
  virtual GAPIC_float compute(GAPIC_float a,GAPIC_float b) const;
  PrologStdFloatDistance() {}
  virtual ~PrologStdFloatDistance() {}
};

extern gapic_type PrologStdFloatDistanceType;
typedef GapicSubCollectable<PrologStdFloatDistance,
  &PrologStdFloatDistanceType,
  PrologRealScalarDistanceCollectable> PrologStdFloatDistanceCollectable;

#endif
