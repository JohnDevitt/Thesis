
#ifndef ALPHA_DIST_INC
#define ALPHA_DIST_INC

#include "../../gapic/atom_array.h"
#include "../../gapic/float_array.h"
#include "../../gapic/int_array.h"
#include "../../gapic/alloc_table.h"
#include "../../gapic/refcount.h"
#include "alphabet.h"
#include "../../distance/base_dist.h"

    
       
  
      
   

 
 

extern GAPIC_functor alphabet_distance_functor;

class Enum_distance_float_matrix : public float_matrix {
 public:
  GAPIC_result CheckPositive(int write_warning) const;
  GAPIC_result CheckReflexive(int write_warning) const;
  GAPIC_result CheckSymmetric(int write_warning) const;
  GAPIC_result CheckTransitive(int write_warning) const;
  GAPIC_result CheckSimilarity(int write_warning) const;
  GAPIC_result CheckCost(int write_warning) const;
  GAPIC_result CheckMetric(int write_warning) const;
  GAPIC_result CheckStrictMetric(int write_warning) const;
  distance_properties FindProperties() const;
  
  Enum_distance_float_matrix() {};
  Enum_distance_float_matrix(const float_matrix& src) :
    float_matrix(src) {}
};

template <class Enum_Alph_Coll> class Enum_Alphabet_Distance 
: public Distance {
 public:
  typedef typename Enum_Alph_Coll::basic_type Enum_Alph;
  typedef Enum_Alph_Coll alpha_coll_type;
  typedef typename Enum_Alph::element_type element_type;
  typedef GAPIC_float domain_type;
  typedef RefCountPtr<Enum_Alph> Alphabet_Ptr;
 private:
  Alphabet_Ptr alphabet;
  Enum_distance_float_matrix distances;
  GAPIC_result __PutProlog(GAPIC_term_ref dist_matrix);
  void Set(const Enum_Alphabet_Distance& src);
    
  void init() {}
 public:
  void Clear(); 
  Enum_Alphabet_Distance& operator=(const Enum_Alphabet_Distance& src) {
    Set(src);return *this;};
  GAPIC_result PutProlog(GAPIC_term_ref dist_matrix);
  int GetSize() const {
    if(alphabet.IsNull()) return 0;
    return alphabet->size();
  }
  GAPIC_float compute_distance(element_type& a,element_type& b) const {
    return distances.Get(a.i,b.i);
  }
  void SetProperties() {SetProperties(&distances);}
  void SetProperties(Enum_distance_float_matrix* src) {
    Distance::SetProperties(src->FindProperties());}
  void Print() const {Print(cout);}
  ostream& Print(ostream& os) const;
       
     
  Enum_Alphabet_Distance(Enum_Alphabet_Distance &src)
    : Distance(src) {init();Set(src);}
  Enum_Alphabet_Distance(GAPIC_term_ref src) throw(GapicObjectCreationError);
  Enum_Alphabet_Distance(Enum_Alph* alph,
			 float_matrix *dist);
  Enum_Alphabet_Distance(Alphabet_Ptr alph,float_matrix *dist);

  ~Enum_Alphabet_Distance() {Clear();}
};

  

template <class Enum_Alph_Coll> Enum_Alphabet_Distance<Enum_Alph_Coll>::
Enum_Alphabet_Distance(Alphabet_Ptr alph,float_matrix *dist)
  : distances(*dist),alphabet(alph),
  Distance("Enum_Alphabet_Distance") {
  CHECK(distances.size()==alphabet->size());
  SetProperties();
}

template <class Enum_Alph_Coll> Enum_Alphabet_Distance<Enum_Alph_Coll>::
Enum_Alphabet_Distance(Enum_Alph* alph,
		       float_matrix *dist)
  : distances(*dist),alphabet(alph),
  Distance("Enum_Alphabet_Distance") {
  CHECK(distances.size()==alphabet->size());
  SetProperties();
}

#define EAD_IC "alpha_dist.C:Enum_Alphabet_Distance::InitConstructor:"

template <class Enum_Alph_Coll> Enum_Alphabet_Distance<Enum_Alph_Coll>::
Enum_Alphabet_Distance(GAPIC_term_ref src) throw(GapicObjectCreationError)
  : Distance("Enum_Alphabet_Distance") {
  GAPIC_term_ref prolog_alphabet=GAPIC_new_term_ref();
  GAPIC_term_ref dist_matrix=GAPIC_new_term_ref();
  

  if(!GAPIC_is_functor(src))
    throw GapicObjectCreationError(EAD_IC " arg1 should be"
				   " functor alphabet_distance/2"); 

  if(!alphabet_distance_functor.CmpTerm(src)) 
    throw GapicObjectCreationError(EAD_IC " arg1 should be"
				   " functor alphabet_distance/2");

  GAPIC_get_arg(1,src,prolog_alphabet);

  cheap_id alpha_id;
  if(PrologIDToInt(prolog_alphabet,&alpha_id)!=GAPIC_SUCCESS)
    throw GapicObjectCreationError(EAD_IC " arg1 of arg1:"
				   "should be valid c object id");

  alphabet=HeapGetRefCountPtr<Enum_Alph_Coll>(alpha_id);
  if(alphabet.IsNull())
    throw GapicObjectCreationError(EAD_IC " arg1 of arg1:"
				   "should be a correctly typed alphabet id");
  distances.Resize(alphabet->size(),alphabet->size());

  GAPIC_get_arg(2,src,dist_matrix);

  cheap_id dist_matrix_id;
  if(PrologIDToInt(dist_matrix,&dist_matrix_id)==GAPIC_SUCCESS) {

    const float_matrix *fm;
    fm=HeapGetData<gapic_float_matrix_collectable>(dist_matrix_id);
    if(fm==NULL) 
      throw GapicObjectCreationError(EAD_IC " arg2 of arg1:"
				   "is a c object id but "
				    "not a float_matrix id");
    distances=*fm;
  }
  else if(distances.PutPrologListList(dist_matrix)==GAPIC_SUCCESS) {

  }
  else
    throw GapicObjectCreationError(EAD_IC " arg2 of arg1: should be a"
				   "float_matrix id or a float matrix"
				   "init term");
  
  if(distances.GetRows() != distances.GetCols()) {
    char buf[80];
    sprintf(buf,EAD_IC " Distance matrix is not square (%d!=%d)",
	    distances.GetRows(),distances.GetCols());
    throw GapicObjectCreationError(buf);
  }
  
  if(alphabet->size() != distances.GetRows()) {
    char buf[80];
    sprintf(buf,EAD_IC " Distance matrix size is not equal"
	    " to the alphabet cardinality  (%d!=%d)",
	    distances.GetRows(),alphabet->size());
    throw GapicObjectCreationError(buf);
  }
  SetProperties();
}

#undef EAD_IC     


template <class Enum_Alph_Coll> 
void Enum_Alphabet_Distance<Enum_Alph_Coll>::Clear() {
  alphabet.ClearPtr();
  distances.Clear();
}

template <class Enum_Alph_Coll> 
void Enum_Alphabet_Distance<Enum_Alph_Coll>::
Set(const Enum_Alphabet_Distance& Src) {
  Clear();
  alphabet=Src.alphabet;
  distances=Src.distances;
}

template <class Enum_Alph> 
ostream& Enum_Alphabet_Distance<Enum_Alph>::Print(ostream& os) const {
  os << "Symbol distance table \n Alphabet (#=%d) : \n" << GetSize();
  alphabet->Print(os);
  os << "\nMatrix:\n";
  distances.Print(os);
}

typedef Enum_Alphabet_Distance<Enum_Atom_Alphabet_Collectable> 
Enum_Atom_Alphabet_Distance;

extern gapic_type Enum_Atom_Alphabet_Distance_Type;
					    
typedef GapicCollectable<Enum_Atom_Alphabet_Distance,
  &Enum_Atom_Alphabet_Distance_Type> Enum_Atom_Alphabet_Distance_Coll;
  
#endif
 
