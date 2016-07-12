


#ifndef PROLOG_DISTANCE_INC
#define PROLOG_DISTANCE_INC

#include <string.h>
#include <iostream.h>
#include "../gapic/gapicgc.h"
#include "../gapic/struct/c_struct.h"



  
         

class PrologDistance : public RefCountObject {
        
 private:
 public: 
	virtual GAPIC_result compute(GAPIC_term_ref a, GAPIC_term_ref b, GAPIC_term_ref dist) const = 0;

         
         
  virtual ostream& Print(ostream& os) const = 0;
  PrologDistance() {}
  virtual ~PrologDistance() {}
};

extern gapic_type PrologDistanceType;
typedef GapicCollectable<PrologDistance,&PrologDistanceType> 
PrologDistanceCollectable;

        
    
        

template <class domain>
class PrologTypedDistance : public PrologDistance {
 public:
  typedef domain domain_type;
  virtual GAPIC_result td_compute(GAPIC_term_ref a,GAPIC_term_ref b,
			       domain* dist) const = 0;
  virtual GAPIC_result id_compute(const cheap_id a,const cheap_id b,
				  domain *dist) const=0;
  PrologTypedDistance() {}
  virtual ~PrologTypedDistance() {}
};

    

class PrologRealDistance : public PrologTypedDistance<GAPIC_float> {
          
 private:
 public:
  virtual GAPIC_result td_compute(GAPIC_term_ref a,GAPIC_term_ref b,
			       domain_type* dist) const = 0;
  virtual GAPIC_result compute(GAPIC_term_ref a,GAPIC_term_ref b,
			       GAPIC_term_ref dist) const;
  PrologRealDistance() {}
  virtual ~PrologRealDistance() {}
};

extern gapic_type PrologRealDistanceType;
typedef GapicSubCollectable<PrologRealDistance,&PrologRealDistanceType,
  PrologDistanceCollectable> PrologRealDistanceCollectable;

#include "std_scal_dist.h"

template <class typed_dist_class, class coll_type>
class PrologTypedDistanceTemplate : public typed_dist_class {
 public:
  typedef typename typed_dist_class::domain_type domain_type;
  typedef typename coll_type::basic_type element_type;
 private:
  inline const element_type *check_get_element_from_term(GAPIC_term_ref src, 
							 const char *method, 
							 int argnr) const;
  inline const element_type *check_get_element_from_id(cheap_id id, 
						       const char* method,
						       int argnr) const;
 public:
  virtual ostream& Print(ostream& os) const;
  const gapic_type* GetElementType() const {return coll_type::get_type();}
  virtual GAPIC_result td_compute(GAPIC_term_ref a,GAPIC_term_ref b,
			       domain_type *dist) const;
  virtual GAPIC_result compute(GAPIC_term_ref a,GAPIC_term_ref b,
			       GAPIC_term_ref dist) const=0;
  virtual GAPIC_result compute(const element_type *a, 
			 const element_type *b, domain_type *dist) const=0;
  virtual GAPIC_result id_compute(const cheap_id a,const cheap_id b,
				  domain_type *dist) const;
  PrologTypedDistanceTemplate();
  virtual ~PrologTypedDistanceTemplate();
};

template <class coll_type>
class PrologRealDistanceTemplate 
: public PrologTypedDistanceTemplate<PrologRealDistance,coll_type> {
 private:
 public:
  virtual GAPIC_result compute(GAPIC_term_ref a,GAPIC_term_ref b,
			       GAPIC_term_ref dist) const;
  PrologRealDistanceTemplate() {}
  virtual ~PrologRealDistanceTemplate() {}
};

class PrologRealGapicFloatDistance 
: public PrologRealDistanceTemplate<GapicFloatCollectable> {
 private:
  static const StdFloatDistance stdfloat;
 public:
  virtual GAPIC_result compute(const element_type *a, 
			 const element_type *b, domain_type *dist) const;
  PrologRealGapicFloatDistance() {}
  virtual ~PrologRealGapicFloatDistance() {}
};


extern gapic_type PrologRealGapicFloatDistanceType;
typedef GapicSubCollectable<PrologRealGapicFloatDistance,
  &PrologRealGapicFloatDistanceType,
  PrologRealDistanceCollectable> PrologRealGapicFloatDistanceCollectable;

  


template <class typed_dist_type,class coll_type>
PrologTypedDistanceTemplate<typed_dist_type,coll_type>::
PrologTypedDistanceTemplate() {}

template <class typed_dist_type,class coll_type>
PrologTypedDistanceTemplate<typed_dist_type,coll_type>::
~PrologTypedDistanceTemplate() {}

 
template <class typed_dist_type,class coll_type>
ostream& PrologTypedDistanceTemplate<typed_dist_type,coll_type>::
Print(ostream& os) const {
  return (os << "[typed_distance_template]");
}

template <class typed_dist_type,class coll_type>
inline const typename coll_type::basic_type *
PrologTypedDistanceTemplate<typed_dist_type,coll_type>::
check_get_element_from_term(GAPIC_term_ref src, 
			    const char *method, 
			    int argnr) const {
  cheap_id id;
  if(PrologIDToInt(src,&id)!=GAPIC_SUCCESS) {
    cerr << "PrologTypedDistanceTemplate"
	 <<"::" << method << ": arg "<<argnr << " (";
    print_term(cout,src);
    cerr << "): is not a valid c object ID; failing ...\n";
    return NULL;
  }
  return check_get_element_from_id(id,method,argnr);
}

template <class typed_dist_type,class coll_type>
inline const typename coll_type::basic_type *
PrologTypedDistanceTemplate<typed_dist_type,coll_type>::
check_get_element_from_id(cheap_id id, 
			  const char *method, 
			  int argnr) const {
  const element_type *el;
  el=HeapGetData<coll_type>(id);
  if(el!=NULL)
    return el;
  cerr << "PrologTypedDistanceTemplate"
       <<"::" << method << ": arg "<<argnr << " (id=" << id << ")"
       << ": is a c object of incorrect type (should be"
       << coll_type::GetType()->GetName() << "); failing ...\n";
  return NULL;
}

template <class typed_dist_type,class coll_type>
GAPIC_result PrologTypedDistanceTemplate<typed_dist_type,coll_type>::
td_compute(GAPIC_term_ref a,GAPIC_term_ref b,domain_type* dist) const {
  const element_type *ae,*be;

  ae=check_get_element_from_term(a,"compute(term,term,dom)",1);
  be=check_get_element_from_term(b,"compute(term,term,dom)",2);
  if((ae==NULL)||(be==NULL))
    return GAPIC_FAIL; 
  return compute(ae,be,dist);
}

template <class typed_dist_type,class coll_type> GAPIC_result
PrologTypedDistanceTemplate<typed_dist_type,coll_type>::
id_compute(const cheap_id a,const cheap_id b,domain_type* dist) const {
  const element_type *ae,*be;


  ae=check_get_element_from_id(a,"id_compute(id,id,dom)",1);
  be=check_get_element_from_id(b,"id_compute(id,id,dom)",2);
  if((ae==NULL)||(be==NULL))
    return GAPIC_FAIL; 
  return compute(ae,be,dist);
}

template <class coll_type>
GAPIC_result PrologRealDistanceTemplate<coll_type>::
compute(GAPIC_term_ref a,GAPIC_term_ref b,
	GAPIC_term_ref dist) const {
  domain_type dom_dist;

  if(td_compute(a,b,&dom_dist)!=GAPIC_SUCCESS)
    return GAPIC_FAIL;
  GAPIC_put_float(dist,dom_dist);
  return GAPIC_SUCCESS;
}

#endif
