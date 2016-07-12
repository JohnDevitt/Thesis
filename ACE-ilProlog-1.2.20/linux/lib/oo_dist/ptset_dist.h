
#ifndef PTSET_OO_DIST_INC
#define PTSET_OO_DIST_INC

#include "../gapic/gapicgc.h"
#include "../gapic/float_array.h"
#include "flownet.h"

typedef enum {
  PTSET_DIST_METHOD_HAUSDORFF,
  PTSET_DIST_METHOD_LINK,
  PTSET_DIST_METHOD_SURJECTION,
  PTSET_DIST_METHOD_FAIR_SURJECTION,
  PTSET_DIST_METHOD_MATCHING
} Ptset_distance_method;

int GetPtsetMethodFromTerm(GAPIC_term_ref src,Ptset_distance_method* method);

Ptset_distance_method GetPtsetMethodFromTermForCreation(GAPIC_term_ref src) throw(GapicObjectCreationError);


template <class base_distance, class container> 
class PtsetDistance {
 public:
  typedef container element_type;
  typedef typename base_distance::domain_type domain_type;
 private:
  const base_distance *base;
  const Ptset_distance_method method;
  domain_type M;     
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
 public:
  Ptset_distance_method GetMethod() const {return method;}
  domain_type compute_distance(const element_type& x, 
			       const element_type& y) const;
  void compute_distance(const element_type& x, 
			       const element_type& y,
			domain_type* dist) const {
    *dist=compute_distance(x,y);}
  domain_type GetM() const { 
    return M;}
       
    
  PtsetDistance(const base_distance* _base,
		Ptset_distance_method _method) 
    : base(_base),method(_method),
    M(1) {};
  PtsetDistance(const base_distance* _base,
		Ptset_distance_method _method, domain_type _M) 
    : base(_base),method(_method),
    M(_M) {};
};

template <class base_distance, class container>  
typename  base_distance::domain_type 
PtsetDistance<base_distance,container>::
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



template <class base_distance, class container>  
typename  base_distance::domain_type 
PtsetDistance<base_distance,container>::
compute_dist_hausdorff(const element_type& x,const  element_type& y) const {

  typename element_type::const_iterator i,j;
  domain_type minx[x.size()];
  domain_type miny[y.size()];
  domain_type *xptr,*yptr;
  domain_type xydist;
  domain_type max;

  for(((i=x.begin()),(xptr=minx));i<x.end();(i++,xptr++)) 
    *xptr=base->compute_distance(*i,*(y.begin()));
  miny[0]=minx[0];

  for(((j=y.begin()+1),(yptr=miny));j<y.end();(j++,yptr++)) 
    *yptr=base->compute_distance(*j,*(x.begin()));

  for(((i=x.begin()+1),(xptr=minx));i<x.end();(i++,xptr++)) 
    for(((j=y.begin()+1),(yptr=miny));j<y.end();(j++,yptr++)) {
      xydist=base->compute_distance(*i,*j);
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


template <class base_distance, class container>  
typename  base_distance::domain_type 
PtsetDistance<base_distance,container>::
compute_dist_link(const element_type& x, const element_type& y) const {
  return 0;
}


template <class base_distance, class container>  
typename  base_distance::domain_type 
PtsetDistance<base_distance,container>::
compute_dist_surjection(const element_type& x,const  element_type& y) const {
  return 0;
}


template <class base_distance, class container>  
typename  base_distance::domain_type 
PtsetDistance<base_distance,container>::
compute_dist_fair_surjection(const element_type& x, const element_type& y) const {
  return 0;
}


template <class base_distance, class container>  
typename  base_distance::domain_type 
PtsetDistance<base_distance,container>::
compute_dist_matching(const element_type& x, const element_type& y) const {
  int an=x.size();
  int bn=y.size();
  int size=an+bn+2;
  FlowNetworkStructure fns(size);
  for(int i=0;i<an;i++) 
    for(int j=0;j<bn;j++) {
      fns.GetEdge(i,an+j)->weight=
	min(base->compute_distance(x[i],y[j]),GetM());
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
  
  return (domain_type)(flow.GetTotalCost()+(abs(an-bn)*GetM()));
  
    
    
    
    
    
     
    
    
    
}

template <class base_distance> 
class Ptset_Vector_Distance 
: public PtsetDistance<base_distance,
  vector<typename base_distance::element_type> > {
  public:
  Ptset_Vector_Distance(const base_distance *_base,
		       Ptset_distance_method _method=PTSET_DIST_METHOD_MATCHING, typename base_distance::domain_type _M=1) 
    : PtsetDistance<base_distance,
    vector<typename base_distance::element_type> >
    (_base,_method, _M) {}
};

template <class base_distance,Ptset_distance_method method> 
class PtsetSpec_Vector_Distance 
: public Ptset_Vector_Distance<base_distance> {
  public:
  PtsetSpec_Vector_Distance(const base_distance *_base) 
    : Ptset_Vector_Distance<base_distance>(_base,method) {}
  PtsetSpec_Vector_Distance(const base_distance *_base, typename base_distance::domain_type _M)
    : Ptset_Vector_Distance<base_distance>(_base,method,_M) {}
};

 
   
  
    


  
 
  
  
   


 
   
  
    


  
 
  
  
   


 
   
  
    


  
 
  
  
   


 
   
  
    


  
 
  
  
   



 
  
    
  
     
     
   
      
     
   
     
    
    
  




 
   
    


  
 
  
  
   






 
   
    
  

  
 
  
  
   






#endif
