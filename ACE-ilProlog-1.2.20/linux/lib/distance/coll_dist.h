
#ifndef GAPIC_DISTANCE_COLLECTION_INC
#define GAPIC_DISTANCE_COLLECTION_INC

#include <vector>
#include "../gapic/collections.h"
#include "base_dist.h"


template <class base_distance_coll> 
class Collection_Distance : public Distance {
 public:
  typedef typename base_distance_coll::basic_type base_distance;
  typedef typename base_distance::element_type base_element_type;
 private:
  const base_distance *base;
 protected:
  void set_base(const base_distance* _base) {
    base=_base;}
 public:
  const base_distance *GetBase() const {return base;}
  ostream& PrintName(ostream& os) const;
  ostream& Print(ostream& os) const {return PrintName(os);}
  Collection_Distance(const char *name,const base_distance* _base)
    :  base(_base),Distance(name,_base->GetProperties()) {

  }

};

 
   

template <class base_distance_coll, class container> 
class Euclid_Collection_Distance 
: public Collection_Distance<base_distance_coll> {
 public:
  typedef container element_type;
  typedef typename base_distance_coll::basic_type base_distance;
  typedef typename base_distance::domain_type domain_type;
  domain_type compute_sqr_distance(const element_type& x, 
				   const element_type& y) const {
    domain_type dist=0.0;
    int dim=min(x.size(),y.size());
    for(int i=0;i<dim;i++) dist+=float_sqr(GetBase()->
					  compute_distance(x[i],y[i]));
    return dist;
  }
  domain_type compute_distance(const element_type& x, 
			       const element_type& y) const {
    return float_sqrt(compute_sqr_distance(x,y));
  }
  Euclid_Collection_Distance(const char *name,const base_distance* _base) 
    : Collection_Distance<base_distance_coll>(name,_base) {

  };
};

template <class base_distance_coll> 
class Euclid_Vector_Distance 
: public Euclid_Collection_Distance<base_distance_coll,
  vector<typename base_distance_coll::basic_type::element_type> > {
 public:
  Euclid_Vector_Distance(const base_distance *_base) 
    : Euclid_Collection_Distance<base_distance_coll,
    vector<base_element_type> >("Euclid_Vector_Distance",_base)  {

  }
};

template <class base_distance_coll, int array_dimension> 
class Euclid_FixedArray_Distance 
: public Euclid_Collection_Distance<base_distance_coll,
  FixedArray<typename base_distance_coll::basic_type::element_type,
  array_dimension> > {
 public:
  ostream& PrintName(ostream& os) const;
  ostream& Print(ostream& os) const {return PrintName(os);}
  Euclid_FixedArray_Distance(const base_distance *_base) 
    : Euclid_Collection_Distance<base_distance_coll,
    FixedArray<base_element_type,
    array_dimension> >("Euclid_FixedArray_Distance",_base) {

  }
};


 
   

template <class base_distance_coll, class container> 
class Manhatten_Collection_Distance 
: public Collection_Distance<base_distance_coll> {
 public:
  typedef container element_type;
  typedef typename base_distance_coll::basic_type base_distance;
  typedef typename base_distance::domain_type domain_type;
  domain_type compute_distance(const element_type& x, 
			       const element_type& y) const {
    domain_type dist=0.0;
    int dim=min(x.size(),y.size());
    for(int i=0;i<dim;i++) dist+=fabs(GetBase()->compute_distance(x[i],y[i]));
    return dist;
  }
  domain_type compute_sqr_distance(const element_type& x, 
				   const element_type& y) const {
    return float_sqr(compute_distance(x,y));
  }
  Manhatten_Collection_Distance(const char *name,const base_distance* _base) 
    : Collection_Distance<base_distance_coll>(name,_base) {};
};

template <class base_distance_coll> 
class Manhatten_Vector_Distance 
: public Manhatten_Collection_Distance<base_distance_coll,
  vector<typename base_distance_coll::basic_type::element_type> > {
 public:
  Manhatten_Vector_Distance(const base_distance *_base) 
    : Manhatten_Collection_Distance<base_distance_coll,
    vector<base_element_type> >
    ("Manhatten_Vector_Distance",_base) {}
};

template <class base_distance_coll, int array_dimension> 
class Manhatten_FixedArray_Distance 
: public Manhatten_Collection_Distance<base_distance_coll,
  FixedArray<typename base_distance_coll::basic_type::element_type,
  array_dimension> > {
 public:
  ostream& PrintName(ostream& os) const;
  ostream& Print(ostream& os) const {return PrintName(os);}
  Manhatten_FixedArray_Distance(const base_distance *_base) 
    : Manhatten_Collection_Distance<base_distance_coll,
    FixedArray<base_element_type,
    array_dimension> >("Manhatten_FixedArray_Distance",_base) {}
};

 
 

 
     

template <class base_distance_coll> 
ostream& Collection_Distance<base_distance_coll>::
PrintName(ostream& os) const {
  return os << GetName() << "(" << GetBase()->GetName() << ")";
}

 
     

template <class base_distance_coll, int array_dimension> 
ostream& Euclid_FixedArray_Distance<base_distance_coll,array_dimension>
::PrintName(ostream& os) const {
  return os << GetName() << "[" << array_dimension  
	    << "](" << GetBase()->GetName() << ")";
}


 
     

template <class base_distance_coll, int array_dimension> 
ostream& Manhatten_FixedArray_Distance<base_distance_coll,array_dimension>
::PrintName(ostream& os) const {
  return os << GetName << "[" << array_dimension  
	    << "](" << GetBase()->GetName() << ")";
}

#endif
