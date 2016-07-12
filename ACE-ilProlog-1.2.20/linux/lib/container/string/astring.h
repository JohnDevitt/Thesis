
#ifndef GAPIC_ASTRING_INC
#define GAPIC_ASTRING_INC

#include "../../gapic/prolog_vector.h"

template <class AlphaCollClass> 
class Alphabetic_String {
 public:
  typedef typename AlphaCollClass::basic_type alphabet_type;
  typedef typename alphabet_type::element_type element_type;
  typedef typename alphabet_type::symbol_type symbol_type;
 private:
  typedef PrologVector<element_type> element_string_type;
  element_string_type data;
  RefCountPtr<alphabet_type> alphabet;
  void init(GAPIC_term_ref src) throw(GapicObjectCreationError);
 public:
  ostream& Print(ostream& os) const;
  element_type operator[](int idx) const {return data[i];}
  element_type GetElement(int idx) const {return data[i];}
  Alphabetic_String(GAPIC_term_ref src) throw(GapicObjectCreationError);
      
};

typedef Alphabetic_String<Enum_Atom_Alphabet_Collectable> Enum_Atom_String;

extern gapic_type Enum_Atom_String_Type;

class Enum_Atom_String_Collectable 
: public GapicCollectable<Enum_Atom_String,
  &Enum_Atom_String_Type> {
 public:
  Enum_Atom_String_Collectable(Enum_Atom_String *src) : 
    GapicCollectable<Enum_Atom_String,
    &Enum_Atom_String_Type>(src) {};
};

 

template <class AlphaCollClass> Alphabetic_String<AlphaCollClass>::
Alphabetic_String(GAPIC_term_ref src) throw(GapicObjectCreationError) {
  try {
    init(src);
  } catch(GapicObjectCreationError src) {
    cerr << "Alphabetic_String-construction from term: "
	 << src.GetMsg() << endl;
    throw GapicObjectCreationError();
  }
}
template <class AlphaCollClass> 
ostream& Alphabetic_String<AlphaCollClass>::Print(ostream& os) const {
  symbol_type symbol;
  
  element_string_type::const_iterator iter;
  os << "as[";
  for(iter=data.begin();iter<data.end();iter++) {
    alphabet->GetSymbol(*iter,&symbol);
    symbol.Print(os);
  }
  os << "]";
  return os;
}



template <class AlphaCollClass> 
void Alphabetic_String<AlphaCollClass>::init(GAPIC_term_ref src)
     throw(GapicObjectCreationError) {
 cerr << "as:init*1\n";
  if(!GAPIC_is_functor(src)) 
    throw GapicObjectCreationError("term is not a functor (should be ,/2)");
  GAPIC_functor f;
  GAPIC_get_functor(src,&f);
 cerr << "as:init*2\n";
  if(f!=get_GAPIC_functor_conj_2())
    throw GapicObjectCreationError("term should be ,/2");
  GAPIC_term_ref alpha=GAPIC_new_term_ref();
 cerr << "as:init*3\n";
  GAPIC_get_arg(1,src,alpha);
 cerr << "as:init*4\n";
  int alpha_id;
  if(PrologIDToInt(alpha,&alpha_id)!=GAPIC_SUCCESS) 
    throw GapicObjectCreationError("arg 1 of term should be "
				   "an (alphabet) ID");
  RefCountPtr<alphabet_type> _alphabet;
  cout << "alpha_id("<<alpha_id<<")\n";
  _alphabet=HeapGetRefCountPtr<AlphaCollClass>(alpha_id);
  if(_alphabet.IsNull()) 
    throw GapicObjectCreationError("arg 1 of term: is not of correct"
				   " alphabet type"); 
  GAPIC_term_ref head=GAPIC_new_term_ref();
  GAPIC_term_ref tail=GAPIC_new_term_ref();
  GAPIC_get_arg(2,src,tail);
  while(GAPIC_is_list(tail)) {
    GAPIC_get_list(tail,head,tail);
    try {
      symbol_type s(head);
      element_type e;
      _alphabet->Print(cout);
      _alphabet->Lookup(s,&e);
      data.push_back(e);
    } catch(GapicObjectCreationError err) {
      throw GapicObjectCreationError("arg 2 of term: reading element",err);
    }
  }
  GAPIC_atom endatom;
  if(!GAPIC_is_atom(tail)) 
    throw GapicObjectCreationError("arg 2: list does not end in atom (nil)");
  endatom.PutTerm(tail);
  if(endatom!=get_GAPIC_atom_nil_0()) 
    throw GapicObjectCreationError("arg 2: list does not end in nil");  
  alphabet=_alphabet;
}

#endif
