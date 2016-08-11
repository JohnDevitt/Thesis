
#ifndef DELTA_DIST_INC
#define DELTA_DIST_INC

template <class dom_type, class elem_type>
class DeltaDistance {
 private:
 public:
  typedef dom_type domain_type;
  typedef elem_type element_type;
  void compute_distance(element_type a,
			element_type b,
			domain_type *dist) const {
    if(a==b) *dist=0; else *dist=1;}
};

template <class element_type>
class IntDeltaDistance : public DeltaDistance<int,element_type> {};


template <class element_type>
class DoubleDeltaDistance : public DeltaDistance<double,element_type> {};


#endif

