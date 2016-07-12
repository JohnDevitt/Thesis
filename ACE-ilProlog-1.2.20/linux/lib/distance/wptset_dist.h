#ifndef WPTSET_DIST_INC
#define WPTSET_DIST_INC

#include "std_coll_dist.h"
#include "flownet.h"



#include "ptset_base.h"

template <class base_distance_coll, class container> 
class WPtset_Collection_Distance     
: public Collection_Distance<base_distance_coll> {
 private:
  const Ptset_distance_method method;
  GAPIC_float M;     
 public:
  Ptset_distance_method GetMethod() const {return method;}
  typedef typename base_distance_coll::basic_type base_distance;
  typedef container element_type;
  typedef typename base_distance::domain_type domain_type;
  domain_type compute_distance(const element_type& x, 
			       const element_type& y) const;
  domain_type compute_dist_hausdorff(const element_type& x, 
				     const element_type& y) const;
  domain_type compute_dist_link(const element_type& x, 
				const element_type& y) const;
  domain_type compute_dist_surjection(const element_type& x, 
				      const element_type& y) const;
  domain_type compute_dist_fair_surjection(const element_type& x, 
					   const element_type& y) const;
  domain_type compute_dist_matching(const element_type& x, 
				    const element_type& y) const;
  domain_type compute_sqr_distance(const element_type& x, 
				   const element_type& y) const {
    return float_sqr(compute_distance(x,y));
  }
  GAPIC_float GetM() const { 
    return M;}
  void SetM(GAPIC_float _M) {
    M=_M;}
  Ptset_Collection_Distance(const char *name,const base_distance* _base,
			   Ptset_distance_method _method) 
    : Collection_Distance<base_distance_coll>(name,_base),method(_method),
    M(1.0) {};
};



#endif
