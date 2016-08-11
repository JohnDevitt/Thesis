#ifndef PTSET_DIST_INC
#define PTSET_DIST_INC

#include "std_coll_dist.h"
 
#include "flownet.h"



#include "ptset_base.h"


template <class base_distance_coll, class container> 
class Ptset_Collection_Distance     
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

template <class base_distance_coll> 
class Ptset_Vector_Distance 
: public Ptset_Collection_Distance<base_distance_coll,
  vector<typename base_distance_coll::basic_type::element_type> > {
  public:
  Ptset_Vector_Distance(const base_distance *_base,
		       Ptset_distance_method _method=PTSET_DIST_METHOD_MATCHING) 
    : Ptset_Collection_Distance<base_distance_coll,
    vector<typename base_distance_coll::basic_type::element_type> >
    ("Ptset_Vector_Distance",_base,_method) {}
};

template <class base_distance_coll,Ptset_distance_method method> 
class PtsetSpec_Vector_Distance 
: public Ptset_Vector_Distance<base_distance_coll> {
  public:
  PtsetSpec_Vector_Distance(const base_distance *_base) 
    : Ptset_Vector_Distance<base_distance_coll>(_base,method) {}
};

typedef PrologRealFloatArrayDistanceTemplate<
  StdFloatDistanceCollectable, 
  PtsetSpec_Vector_Distance<StdFloatDistanceCollectable,
  PTSET_DIST_METHOD_HAUSDORFF> > 
PrologRealFloatArrayPtsetHausdorffDistance;

extern gapic_type PrologRealFloatArrayPtsetHausdorffDistanceType;
typedef GapicSubCollectable<
  PrologRealFloatArrayPtsetHausdorffDistance,
  &PrologRealFloatArrayPtsetHausdorffDistanceType,
  PrologRealFloatArrayDistanceCollectable> 
PrologRealFloatArrayPtsetHausdorffDistanceCollectable;

typedef PrologRealFloatArrayDistanceTemplate<
  StdFloatDistanceCollectable, 
  PtsetSpec_Vector_Distance<StdFloatDistanceCollectable,
  PTSET_DIST_METHOD_LINK> > 
PrologRealFloatArrayPtsetLinkDistance;

extern gapic_type PrologRealFloatArrayPtsetLinkDistanceType;
typedef GapicSubCollectable<
  PrologRealFloatArrayPtsetLinkDistance,
  &PrologRealFloatArrayPtsetLinkDistanceType,
  PrologRealFloatArrayDistanceCollectable> 
PrologRealFloatArrayPtsetLinkDistanceCollectable;

typedef PrologRealFloatArrayDistanceTemplate<
  StdFloatDistanceCollectable, 
  PtsetSpec_Vector_Distance<StdFloatDistanceCollectable,
  PTSET_DIST_METHOD_SURJECTION> > 
PrologRealFloatArrayPtsetSurjectionDistance;

extern gapic_type PrologRealFloatArrayPtsetSurjectionDistanceType;
typedef GapicSubCollectable<
  PrologRealFloatArrayPtsetSurjectionDistance,
  &PrologRealFloatArrayPtsetSurjectionDistanceType,
  PrologRealFloatArrayDistanceCollectable> 
PrologRealFloatArrayPtsetSurjectionDistanceCollectable;

typedef PrologRealFloatArrayDistanceTemplate<
  StdFloatDistanceCollectable, 
  PtsetSpec_Vector_Distance<StdFloatDistanceCollectable,
  PTSET_DIST_METHOD_FAIR_SURJECTION> > 
PrologRealFloatArrayPtsetFairSurjectionDistance;

extern gapic_type PrologRealFloatArrayPtsetFairSurjectionDistanceType;
typedef GapicSubCollectable<
  PrologRealFloatArrayPtsetFairSurjectionDistance,
  &PrologRealFloatArrayPtsetFairSurjectionDistanceType,
  PrologRealFloatArrayDistanceCollectable> 
PrologRealFloatArrayPtsetFairSurjectionDistanceCollectable;

class PtsetMatching_Vector_Distance
: public PtsetSpec_Vector_Distance<StdFloatDistanceCollectable,
  PTSET_DIST_METHOD_MATCHING>  {
  public:
  PtsetMatching_Vector_Distance(const base_distance* _base) 
    : PtsetSpec_Vector_Distance<StdFloatDistanceCollectable,
  PTSET_DIST_METHOD_MATCHING>(_base) {}
  PtsetMatching_Vector_Distance(const base_distance *_base, GAPIC_term_ref src)
    : PtsetSpec_Vector_Distance<StdFloatDistanceCollectable,
  PTSET_DIST_METHOD_MATCHING>(_base) {
    GAPIC_float _M;
    GAPIC_get_float(src,&_M);
    SetM(_M);
  }
};

typedef PrologRealFloatArrayDistanceTemplate<
  StdFloatDistanceCollectable, 
  PtsetMatching_Vector_Distance > 
PrologRealFloatArrayPtsetMatchingDistance;

extern gapic_type PrologRealFloatArrayPtsetMatchingDistanceType;
typedef GapicSubCollectable<
  PrologRealFloatArrayPtsetMatchingDistance,
  &PrologRealFloatArrayPtsetMatchingDistanceType,
  PrologRealFloatArrayDistanceCollectable> 
PrologRealFloatArrayPtsetMatchingDistanceCollectable;


 
   
    
  

  
 
  
  
   




template <class base_distance_coll, class container>  
typename  base_distance_coll::basic_type::domain_type 
Ptset_Collection_Distance<base_distance_coll,container>::
compute_distance(const element_type& x, const element_type& y) const {

  switch(method) {
  case PTSET_DIST_METHOD_HAUSDORFF: 
    return compute_dist_hausdorff(x,y);
  case PTSET_DIST_METHOD_LINK:
    return compute_dist_link(x,y);
  case PTSET_DIST_METHOD_SURJECTION:
    return compute_dist_surjection(x,y);
  case PTSET_DIST_METHOD_FAIR_SURJECTION:  
    return compute_dist_fair_surjection(x,y);
  case PTSET_DIST_METHOD_MATCHING:
    return compute_dist_matching(x,y);
  }
}



template <class base_distance_coll, class container>  
typename  base_distance_coll::basic_type::domain_type 
Ptset_Collection_Distance<base_distance_coll,container>::
compute_dist_hausdorff(const element_type& x,const  element_type& y) const {

  typename element_type::const_iterator i,j;
  domain_type minx[x.size()];
  domain_type miny[y.size()];
  domain_type *xptr,*yptr;
  domain_type xydist;
  domain_type max;

  for(((i=x.begin()),(xptr=minx));i<x.end();(i++,xptr++)) 
    *xptr=GetBase()->compute_distance(*i,*(y.begin()));
  miny[0]=minx[0];

  for(((j=y.begin()+1),(yptr=miny));j<y.end();(j++,yptr++)) 
    *yptr=GetBase()->compute_distance(*j,*(x.begin()));

  for(((i=x.begin()+1),(xptr=minx));i<x.end();(i++,xptr++)) 
    for(((j=y.begin()+1),(yptr=miny));j<y.end();(j++,yptr++)) {
      xydist=GetBase()->compute_distance(*i,*j);
      if(*xptr>xydist) *xptr=xydist;
      if(*yptr>xydist) *yptr=xydist;
    }

  max=0;
  for(xptr=minx;xptr<&(minx[x.size()]);xptr++)
    if(max<*xptr) max=*xptr;
  for(yptr=miny;yptr<&(miny[y.size()]);yptr++)
    if(max<*yptr) max=*yptr;

  return max;
}


template <class base_distance_coll, class container>  
typename  base_distance_coll::basic_type::domain_type 
Ptset_Collection_Distance<base_distance_coll,container>::
compute_dist_link(const element_type& x, const element_type& y) const {
  return 0;
}


template <class base_distance_coll, class container>  
typename  base_distance_coll::basic_type::domain_type 
Ptset_Collection_Distance<base_distance_coll,container>::
compute_dist_surjection(const element_type& x,const  element_type& y) const {
  return 0;
}


template <class base_distance_coll, class container>  
typename  base_distance_coll::basic_type::domain_type 
Ptset_Collection_Distance<base_distance_coll,container>::
compute_dist_fair_surjection(const element_type& x, const element_type& y) const {
  return 0;
}


template <class base_distance_coll, class container>  
typename  base_distance_coll::basic_type::domain_type 
Ptset_Collection_Distance<base_distance_coll,container>::
compute_dist_matching(const element_type& x, const element_type& y) const {
  int an=x.size();
  int bn=y.size();
  int size=an+bn+2;
  FlowNetworkStructure fns(size);
  for(int i=0;i<an;i++) 
    for(int j=0;j<bn;j++) {
      fns.GetEdge(i,an+j)->weight=
	min(GetBase()->compute_distance(x[i],y[j]),GetM());
      fns.GetEdge(an+j,i)->weight=1e100;
      fns.GetEdge(i,an+j)->cap=1;
      fns.GetEdge(an+j,i)->cap=0;
    }
  for(int i=0;i<an;i++) {
    fns.GetEdge(an+bn,i)->weight=0;
    fns.GetEdge(an+bn,i)->cap=1;
    fns.GetEdge(i,an+bn)->weight=1e100;
    fns.GetEdge(i,an+bn)->cap=0;
    fns.GetEdge(an+bn+1,i)->weight=1e100;
    fns.GetEdge(an+bn+1,i)->cap=0;
    fns.GetEdge(i,an+bn+1)->weight=1e100;
    fns.GetEdge(i,an+bn+1)->cap=0;
    for(int j=0;j<an;j++) {
      fns.GetEdge(i,j)->weight=1e100;
      fns.GetEdge(i,j)->cap=0;
    }
  }	
  for(int i=0;i<bn;i++) {
    fns.GetEdge(i+an,an+bn+1)->weight=0;
    fns.GetEdge(i+an,an+bn+1)->cap=1;
    fns.GetEdge(an+bn+1,i+an)->weight=1e100;
    fns.GetEdge(an+bn+1,i+an)->cap=0;
    fns.GetEdge(i+an,an+bn)->weight=1e100;
    fns.GetEdge(i+an,an+bn)->cap=0;
    fns.GetEdge(an+bn,i+an)->weight=1e100;
    fns.GetEdge(an+bn,i+an)->cap=0;
    for(int j=0;j<bn;j++) {
      fns.GetEdge(an+i,an+j)->weight=1e100;
      fns.GetEdge(an+i,an+j)->cap=0;
    }
  }
  for(int i=0;i<2;i++)	
    for(int j=0;j<2;j++) {
      fns.GetEdge(an+bn+i,an+bn+j)->weight=1e100;
      fns.GetEdge(an+bn+i,an+bn+j)->cap=0;
    }
#ifdef TEST
  printf("minmatch show %d x %d\n",an,bn);
  show_network(n,an+bn,an+bn+1);
#endif
  FlowNetworkFlow flow(&fns);
  flow.MaxFlow(an+bn,an+bn+1);	
  
  return flow.GetTotalCost()+(abs(an-bn)*GetM());
  
    
    
    
    
    
     
    
    
    
}

#endif
