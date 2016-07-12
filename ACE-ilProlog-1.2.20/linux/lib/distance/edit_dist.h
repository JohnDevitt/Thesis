
#ifndef GAPIC_DISTANCE_EDIT_INC
#define GAPIC_DISTANCE_EDIT_INC

#include "coll_dist.h"

 
   

template <class base_distance_coll, class container> 
class Edit_Collection_Distance 
: public Collection_Distance<base_distance_coll> {
 public:
  typedef typename base_distance_coll::basic_type base_distance;
  typedef container element_type;
  typedef typename base_distance::domain_type domain_type;
  domain_type compute_distance(element_type& x, element_type& y) const;
  domain_type compute_sqr_distance(element_type& x, element_type& y) const {
    return float_sqr(compute_distance(x,y));
  }
  Edit_Collection_Distance(const char *name,const base_distance* _base) 
    : Collection_Distance<base_distance_coll>(name,_base) {};
};

template <class base_distance_coll> 
class Edit_Vector_Distance 
: public Edit_Collection_Distance<base_distance_coll,
  vector<typename base_distance_coll::basic_type> > {
 public:
  Edit_Vector_Distance(const typename base_distance_coll::basic_type *_base) 
    : Edit_Collection_Distance<typename base_distance_coll::basic_type,
    vector<typename base_distance_coll::basic_type> >("Edit_Vector_Distance",
						      base) {}
};

template <class base_distance_coll, int array_dimension> 
class Edit_FixedArray_Distance 
: public Edit_Collection_Distance<base_distance_coll,
  FixedArray<typename base_distance_coll::basic_type,array_dimension> > {
 public:
  ostream& PrintName(ostream& os) const;
  Edit_FixedArray_Distance(const typename base_distance_coll::basic_type *_base) 
    : Edit_Collection_Distance<typename base_distance_coll::basic_type,
    FixedArray<typename base_distance_coll::basic_type,
    array_dimension> >("Edit_FixedArray_Distance",_base) {}
};

 
 

 
      

template <class base_distance_coll, class container>  
typename  base_distance_coll::basic_type::domain_type 
Edit_Collection_Distance<base_distance_coll,container>::
compute_distance(element_type& x, element_type& y) const {
  const domain_type zero=base->GetZero();
  domain_type tmp_dist[s1.size()+1];
  for(int i1=0;i1<s1.size();i1++) {
    tmp_dist[i1+1]=tmp_dist[i1]+
      base->compute_distance(s1.Get(i1),zero);

  }
  for(int i2=0;i2<s2.size();i2++) {
    domain_type add=tmp_dist[0];
    domain_type change;
    tmp_dist[0]=add+base->compute_distance(zero,s2.Get(i2));
    for(int i1=0;i1<s1.size();i1++) {
      change=add;
      add=tmp_dist[i1+1];
      tmp_dist[i1+1]=min(tmp_dist[i1]+base->compute_distance(s1.Get(i1),zero),
			 min(change+base->compute_distance(s1.Get(i1),
							   s2.Get(i2)),
			     add+base->compute_distance(zero,s2.Get(i2))));


    }
  }
  return tmp_dist[s1.size()];
}

#endif
