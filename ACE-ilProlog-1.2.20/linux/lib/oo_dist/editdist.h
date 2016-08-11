
#ifndef EDIT_DISTANCE_INC
#define EDIT_DISTANCE_INC

#include "oodist_base.h"

template <class base_distance,class container>
class EditDistance {
 private:
  const base_distance *base;
 public:
  typedef typename base_distance::domain_type domain_type;
     
void compute_distance(const container &a,
			       const container &b,
			       domain_type *distance) const;
  EditDistance(const base_distance *_base) : base(_base) {}
};

#define debug_editdist 0

template <class base_distance,class container>
  
void
EditDistance<base_distance,container>::
compute_distance(const container &a,
		 const container &b,
		 domain_type *distance) const {
  vector<domain_type> adist;
  vector<domain_type> bdist;
  vector<domain_type> mindist;
  typename container::const_iterator iter_a;
  typename container::const_iterator iter_b;
  domain_type ijdist;
  if(debug_editdist) cerr << "editdist(ed) *1\n";
  iter_b=b.begin();
  bdist.push_back(0);
  while(iter_b!=b.end()) {
    base->compute_distance_null_to_x(*iter_b,&ijdist);
    iter_b++;
    bdist.push_back(ijdist);
  }
  mindist=bdist;
  iter_a=a.begin(); 
  while(iter_a!=a.end()) {
    base->compute_distance_x_to_null(*iter_a,&ijdist);
    iter_a++;
    adist.push_back(ijdist);
  }
  int ai=0,bi=0;
  if(debug_editdist) cerr << "ed *2\n";
  domain_type prevdist;
  iter_a=a.begin();
  while(iter_a!=a.end()) {
    if(debug_editdist) cerr << "ed *2a\n";
    prevdist=mindist[0]+adist[ai];
    bi=1;
    iter_b=b.begin();
    while(iter_b!=b.end()) {
      if(debug_editdist) cerr << "ed *2b\n";
      base->compute_distance(*iter_a,*iter_b,&ijdist);
      domain_type nextdist=mindist[bi-1]+ijdist;
      if(debug_editdist) cerr << "ed *2c\n";
      if(nextdist>prevdist+bdist[bi])
	nextdist=prevdist+bdist[bi];
      if(nextdist>mindist[bi]+adist[ai])
	nextdist=mindist[bi]+adist[ai];
      if(debug_editdist) cerr << "ed *2d\n";
      mindist[bi-1]=prevdist;
      prevdist=nextdist;
      iter_b++;
      bi++;
    }
    if(debug_editdist) cerr << "ed *2e\n";
  mindist[bi-1]=prevdist;
    iter_a++;
    ai++;
  }
  if(debug_editdist) cerr << "editdist(ed) *3\n";
  *distance=mindist[bi-1];
}

#endif
