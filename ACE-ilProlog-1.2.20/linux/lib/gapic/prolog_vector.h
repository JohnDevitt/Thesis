
#ifndef PROLOG_VECTOR_INC
#define PROLOG_VECTOR_INC

#include <vector>
#include <iostream>
#include "c_prolog.h"
#include "except.h"

template <class T>
class PrologVector : public vector<T> {
 public:
  ostream& Print(ostream& os) const;
  PrologVector(int n, const T& init) : vector<T>(n,init) {};
  PrologVector() {}
  PrologVector(GAPIC_term_ref src) throw(GapicObjectCreationError);
};
template <class T> ostream& PrologVector<T>::Print(ostream& os) const {
  os << "PrologVector[";
  typename PrologVector<T>::const_iterator iter;
  iter=this->begin();
  if(iter<this->end()) {
    iter->Print(os);
    iter++;
  }
  for(iter=this->begin();iter<this->end();iter++) {
    os << ",";
    iter->Print(os);
  }
  os << "]";
  return os;
};

template <class T> PrologVector<T>::PrologVector(GAPIC_term_ref src) 
     throw(GapicObjectCreationError) {
  GAPIC_term_ref head=GAPIC_new_term_ref();
  GAPIC_term_ref tail=GAPIC_new_term_ref();
  GAPIC_put_term(tail,src);
  while(GAPIC_is_list(tail)) {
    GAPIC_get_list(tail,head,tail);
    this->push_back(head);
  }
  GAPIC_atom endatom;
  if(!GAPIC_is_atom(tail)) {
    this->clear();
    throw GapicObjectCreationError();
  }
  endatom.PutTerm(tail);
  if(endatom!=get_GAPIC_atom_nil_0()) {
    this->clear();
    throw GapicObjectCreationError();
  }
}

template <class T1,class T2>
class PrologPair {
 protected:
  T1 first;
  T2 second;
 public:
  typedef T1 first_type;
  typedef T2 second_type;
  const T1& GetFirst() const {return first;}
  const T2& GetSecond() const {return second;}
  ostream& Print(ostream& os) const {
    os<<"("; first.Print(os); os<<","; second.Print(os); os<<")"; return os;}
  PrologPair() {}
  PrologPair(GAPIC_term_ref src) throw(GapicObjectCreationError)
    : first(GetTermArgForConstruct("PrologPairConstruct",
				   src,get_GAPIC_functor_conj_2(),1)),
    second(GetTermArgForConstruct("PrologPairConstruct",
				  src,get_GAPIC_functor_conj_2(),2)) {
          
  }
};


#endif
