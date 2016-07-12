 
#ifndef GAPIC_ALPHABET_INC
#define GAPIC_ALPHABET_INC

#include <typeinfo>
#include "../../../gapic/c_prolog.h"
#include "../../../gapic/gapicgc.h"
#include "../../../gapic/atom_array.h"

  

     
            
            
    
             
            
    
            
            


template <class symbol_class,class element_class> 
class Alphabet : public RefCountObject {
 public:
  typedef symbol_class symbol_type;
  typedef element_class element_type;
  virtual GAPIC_result GetSymbol(element_type element,
				 symbol_type *symbol) const;
  virtual GAPIC_result Lookup(symbol_type symbol,
			      element_type *element) const;
  virtual GAPIC_result PrologToElement(element_type* element,
				       GAPIC_term_ref term) const;  
  virtual ostream& Print(ostream& os) const;
  virtual ostream& PrintElement(ostream& os,const element_type element) const;
};


template <class symbol_class,class element_class> 
GAPIC_result Alphabet<symbol_class,element_class>::
GetSymbol(element_type element,symbol_type *symbol) const {
  return GAPIC_FAIL;
}

template <class symbol_class,class element_class> 
GAPIC_result Alphabet<symbol_class,element_class>::
Lookup(symbol_type symbol, element_type *element) const {
  return GAPIC_FAIL;
}

template <class symbol_class,class element_class> 
GAPIC_result Alphabet<symbol_class,element_class>::
PrologToElement(element_type* element,
		GAPIC_term_ref term) const {
  symbol_type symbol;
  if(symbol.PutTerm(term)!=GAPIC_SUCCESS) 
    return GAPIC_FAIL;
  return Lookup(symbol,element);
}


template <class symbol_class,class element_class> 
ostream& Alphabet<symbol_class,element_class>::
Print(ostream& os) const {
  return (os << "[Unprintable Alphabet]");
}

template <class symbol_class,class element_class> 
ostream& Alphabet<symbol_class,element_class>:: 
PrintElement(ostream& os,const element_type element) const {
  symbol_type symbol;
  if(GetSymbol(element,&symbol)==GAPIC_SUCCESS) {
    return symbol.Print(os);
  }  
  os << "*NaAE*";
  return os;
}

template <class symbol_class,class vector_class>
class Enum_Alphabet : 
public Alphabet<symbol_class,GAPIC_int> {
 private:
  vector_class symbolset;
 public:
  virtual GAPIC_result GetSymbol(element_type element,
				 symbol_type *symbol) const;
  virtual GAPIC_result Lookup(symbol_type symbol,
			      element_type *element) const;
  virtual ostream& Print(ostream& os) const;
  virtual ostream& PrintElement(ostream& os,const element_type element) const;

  int size() const {return symbolset.size();}
  Enum_Alphabet(GAPIC_term_ref term) throw(GapicObjectCreationError);
  Enum_Alphabet(const vector_class src) {symbolset=src;}
  Enum_Alphabet(const Enum_Alphabet& src) {symbolset=src.symbolset;};
};
  
template <class S,class V>
GAPIC_result Enum_Alphabet<S,V>::GetSymbol(element_type element,
					   symbol_type *symbol) const {
  *symbol=symbolset[element];
}

template <class S,class V>
GAPIC_result Enum_Alphabet<S,V>::Lookup(symbol_type symbol, 
					element_type *element) const {
  for(int i=0;i<size();i++) {
    if(symbolset[i]==symbol) {*element=i;
    return GAPIC_SUCCESS;}
  }
  cerr << "Enum_Alphabet::unknown_symbol\n";
  return GAPIC_FAIL;
}

template <class S,class V>
ostream& Enum_Alphabet<S,V>::PrintElement(ostream& os,
					  element_type element) const {
  symbol_type symbol;
  GetSymbol(element,&symbol);
  return symbol.Print(os);
}

template <class S,class V>
ostream& Enum_Alphabet<S,V>::Print(ostream& os) const {
  os << "alph(" << symbolset << ",ref=" << GetOwnerCount();
  return os;
}

template <class S,class V>
Enum_Alphabet<S,V>::Enum_Alphabet(GAPIC_term_ref term) 
     throw(GapicObjectCreationError) {
  if(symbolset.PutPrologList(term)!=GAPIC_SUCCESS) {
    cerr << "failed to initialise alphabet\n";
    symbolset.Clear();
    throw GapicObjectCreationError();
  }
}

template <class S,class V>
inline ostream& operator<<(ostream& os,
			   const Enum_Alphabet<S,V>& alpha) {
  return alpha.Print(os);
}

typedef Enum_Alphabet<GAPIC_atom,atom_array> Enum_Atom_Alphabet;

extern gapic_type Enum_Atom_Alphabet_Type;

class Enum_Atom_Alphabet_Collectable 
: public GapicCollectable<Enum_Atom_Alphabet,
  &Enum_Atom_Alphabet_Type> {
 public:
  Enum_Atom_Alphabet_Collectable(Enum_Atom_Alphabet *src) : 
    GapicCollectable<Enum_Atom_Alphabet,
    &Enum_Atom_Alphabet_Type>(src) {};
};


template <class T> class Direct_Alphabet : Alphabet<T,T> {
 private:
 public:
  typedef T element_type;
  typedef T symbol_type;
  virtual GAPIC_result GetSymbol(element_type element,
				 symbol_type *symbol) const;
  virtual GAPIC_result Lookup(symbol_type symbol,
			      element_type *element) const;
  virtual ostream& Print(ostream& os) const;
  virtual ostream& PrintElement(ostream& os,const element_type element) const;

  Direct_Alphabet(GAPIC_term_ref term) {};
  Direct_Alphabet(const Direct_Alphabet& src) {};
};

template <class T>
GAPIC_result Direct_Alphabet<T>::GetSymbol(element_type element,
					symbol_type *symbol) const {
  *symbol=element;
}

template <class T>
GAPIC_result Direct_Alphabet<T>::Lookup(symbol_type symbol, 
					   element_type *element) const {
  *element=symbol;
}

template <class T>
ostream& Direct_Alphabet<T>::PrintElement(ostream& os,element_type element) const {
  return element.Print(os);
}

template <class T>
ostream& Direct_Alphabet<T>::Print(ostream& os) const {
  os << "alph(direct:" << typeid(T)->type_name() << ",ref=" << GetOwnerCount();
  return os;
}

template <class T>
inline ostream& operator<<(ostream& os,
			   const Direct_Alphabet<T>& alpha) {
  return alpha.Print(os);
}


  
   

  
  
  
    
 
     
    
     
    



#endif


