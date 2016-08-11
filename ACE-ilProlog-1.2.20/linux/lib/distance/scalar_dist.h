
#ifndef GAPIC_DISTANCE_SCALAR_INC
#define GAPIC_DISTANCE_SCALAR_INC

#include <math.h>
#include "base_dist.h"

inline GAPIC_float float_sqr(GAPIC_float x) {return x*x;}
inline GAPIC_float float_sqrt(GAPIC_float x) {return sqrt(x);}
inline GAPIC_float float_abs(GAPIC_float x) {return fabs(x);}

class StdFloatDistance : public Distance {
 public:
  typedef GAPIC_float element_type;
  typedef GAPIC_float domain_type;
  domain_type compute_distance(element_type x,element_type y) const {
    return float_abs(x-y);
  }
  domain_type compute_sqr_distance(element_type x,element_type y) const {
    return float_sqr(x-y);}
  StdFloatDistance() : Distance("FloatDistance",DISTANCE_STRICTMETRIC) {};
};



inline GAPIC_int int_sqr(GAPIC_int x) {return x*x;}
inline GAPIC_float int_sqrt(GAPIC_int x) {return sqrt(1.0*x);}
inline GAPIC_int int_abs(GAPIC_int x) {return (x>=0)?(x):(-x);}

class Int_Distance : public Distance {
 public:
  typedef GAPIC_int element_type;
  typedef GAPIC_int domain_type;
  domain_type compute_distance(element_type x,element_type y) const {
    return int_abs(x-y);
  }
  domain_type compute_sqr_distance(element_type x,element_type y) const {
    return int_sqr(x-y);}
  Int_Distance() : Distance("Int_Distance",DISTANCE_STRICTMETRIC) {};
};

#endif
