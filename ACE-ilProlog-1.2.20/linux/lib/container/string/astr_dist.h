

#include <ostream.h>
#include "alpha_dist.h"
#include "astring.h"
#include "../../distance/edit_dist.h"


template <class base_distance_coll> 
class Edit_AlphaString_Distance 
: public Edit_Collection_Distance<base_distance_coll,
  Alphabetic_String<typename 
base_distance_coll::basic_type::alpha_coll_type> > {
 private:
  const Enum_Atom_Alphabet_Distance *BaseIDFromTerm(GAPIC_term_ref src) 
    throw(GapicObjectCreationError);
 public:
  ostream& Print(ostream& os) const;
  Edit_AlphaString_Distance() throw(GapicObjectCreationError) :  
    Edit_Collection_Distance<base_distance_coll,
    Alphabetic_String<typename base_distance::alpha_coll_type> >("",NULL) {
  throw GapicObjectCreationError("Edit_AlphaString_Distance "
				 "has no default constructor");}
  Edit_AlphaString_Distance(GAPIC_term_ref src) 
    throw(GapicObjectCreationError);
  Edit_AlphaString_Distance(const base_distance *_base) 
    : Edit_Collection_Distance<base_distance_coll,
    Alphabetic_String<typename base_distance::alpha_coll_type> >
    ("Edit_AlphaString_Distance",base) {}
};

typedef Edit_AlphaString_Distance<Enum_Atom_Alphabet_Distance_Coll> 
Edit_Enum_Atom_Alphabet_String_Distance;

extern gapic_type Edit_Enum_Atom_Alphabet_String_Distance_Type;
					    
typedef GapicCollectable<Edit_Enum_Atom_Alphabet_String_Distance,
  &Edit_Enum_Atom_Alphabet_String_Distance_Type> 
  Edit_Enum_Atom_Alphabet_String_Distance_Coll;

  

template <class base_distance_coll> 
ostream& Edit_AlphaString_Distance<base_distance_coll>::
Print(ostream& os) const {
  os<<"Edit_String_Distance[";
  GetBase()->Print(os);
  os<<"]";
  return os;
}

#define EASD_IC "astr_dist.C:Edit_AlphaString_Distance::InitConstructor:"
  
template <class base_distance_coll> 
Edit_AlphaString_Distance<base_distance_coll>::
Edit_AlphaString_Distance(GAPIC_term_ref src) 
     throw(GapicObjectCreationError) 
  : Edit_Collection_Distance<base_distance_coll,
  Alphabetic_String<typename base_distance::alpha_coll_type> >
("Edit_AlphaString_Distance",BaseIDFromTerm(src)) {}

template <class base_distance_coll> 
const Enum_Atom_Alphabet_Distance* 
Edit_AlphaString_Distance<base_distance_coll>::
BaseIDFromTerm(GAPIC_term_ref src) throw(GapicObjectCreationError) {
  cheap_id base_id;
  if(PrologIDToInt(src,&base_id)!=GAPIC_SUCCESS)
    throw GapicObjectCreationError(EASD_IC " arg1:"
				   "should be valid c object id");
  const Enum_Atom_Alphabet_Distance* base_dist;
  base_dist=HeapGetData<Enum_Atom_Alphabet_Distance_Coll>(base_id);
  if(base_dist==NULL)
     throw GapicObjectCreationError(EASD_IC " arg1:"
				   "is a c object id but is not of type "
				    "enum_atom_alphabet_distance");   
  return base_dist;
}

#undef EASD_IC     
