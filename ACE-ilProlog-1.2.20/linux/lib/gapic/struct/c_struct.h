
#ifndef GAPIC_C_STRUCT_INC
#define GAPIC_C_STRUCT_INC

#include <vector>
#include "../c_prolog.h"
#include "../prolog_vector.h"



 
    
     
     
     
   
     
     
         
      
      
     

class GapicInt : public RefCountObject {
 private:
  GAPIC_int data;
 public: 
  ostream& Print(ostream& os) const {return(os << data);}
	GAPIC_int Get() const {

    return data;}
	void add(GAPIC_int x) {data+=x;}

	void sub(GAPIC_int x) {data-=x;}

	void mult(GAPIC_int x) {data*=x;}

	void div(GAPIC_int x) {data/=x;}

	void mod(GAPIC_int x) {data%=x;}

	void bit_or(GAPIC_int x) {data=data|x;}

	void bit_and(GAPIC_int x) {data&=x;}

	void bit_xor(GAPIC_int x) {data^=x;}

  inline void operator+=(const GapicInt f) {data+=f.Get();}
  inline void operator-=(const GapicInt f) {data-=f.Get();}
  inline void operator/=(const GapicInt f) {data/=f.Get();}
  inline void operator*=(const GapicInt f) {data*=f.Get();}
  inline void operator%=(const GapicInt f) {data%=f.Get();}
  inline void operator&=(const GapicInt f) {data&=f.Get();}
  inline void operator|=(const GapicInt f) {data=data|f.Get();}
  inline void operator^=(const GapicInt f) {data^=f.Get();}
	void inc() {data++;}

	void dec() {data--;}

	GAPIC_int get_inc() {return data++;}

	GAPIC_int get_dec() {return data--;}

	GAPIC_int inc_get() {return ++data;}

	GAPIC_int dec_get() {return --data;}

  inline const GapicInt& operator++() {data++;return *this;}
  inline const GapicInt& operator--() {data--;return *this;}
  inline GAPIC_int operator++(int) {GAPIC_int tmp=data;data++;return tmp;}
  inline GAPIC_int operator--(int) {GAPIC_int tmp=data;data--;return tmp;}

  inline GapicInt() : data(0) {}
  inline GapicInt(const GapicInt& src) : data(src.data) {}
  inline GapicInt(GAPIC_term_ref src) 
    throw(GapicObjectCreationError) {
    if(!GAPIC_is_integer(src)) 
      throw GapicObjectCreationError("GapicInt:Construct:not an integer");
    GAPIC_get_integer(src,&data);
  }
};

extern gapic_type GapicIntType;
typedef GapicNonExMutCollectable<GapicInt,&GapicIntType> GapicIntCollectable;

class GapicFloat : public RefCountObject {
 private:
  GAPIC_float data;
 public:
  ostream& Print(ostream& os) const {
    os << data;
      
    return os;}
	double Get() const {

    return data;}
	void add(GAPIC_float x) {data+=x;}

	void sub(GAPIC_float x) {data-=x;}

	void mult(GAPIC_float x) {data*=x;}

	void div(GAPIC_float x) {data/=x;}

  inline void operator+=(const GapicFloat f) {data+=f.Get();}
  inline void operator-=(const GapicFloat f) {data-=f.Get();}
  inline void operator/=(const GapicFloat f) {data/=f.Get();}
  inline void operator*=(const GapicFloat f) {data*=f.Get();}

  inline GapicFloat() : data(0.0) {}
  inline GapicFloat(const GapicFloat& src) : data(src.Get()) {}
  inline GapicFloat(const GapicInt& src) : data(src.Get()) {}
  inline GapicFloat(GAPIC_term_ref src) 
    throw(GapicObjectCreationError) {
    if(!GAPIC_is_float(src)) 
      throw GapicObjectCreationError("GapicFloat:Construct:not a float");
    GAPIC_get_float(src,&data);
    
  }
};



extern gapic_type GapicFloatType;
typedef GapicNonExMutCollectable<GapicFloat,&GapicFloatType> 
GapicFloatCollectable;

class GapicAtom {
 private:
  GAPIC_atom data;
 public:
  ostream& Print(ostream& os) const {return data.Print(os);}
	void Get(GAPIC_atom* dst) const { *dst=data; }     

  inline GapicAtom(const GapicAtom& src) {src.Get(&data);}
  inline GapicAtom(GAPIC_term_ref src) 
    throw(GapicObjectCreationError) : data(src) {}
};



extern gapic_type GapicAtomType;
typedef GapicCollectable<GapicAtom,&GapicAtomType> 
GapicAtomCollectable;

class RefCountCollectableReference {
 private:
  cheap_id id;
  inline Collectable *GetPtr() const {return the_gapic_heap.GetPtr(id);}
  inline int _Set(cheap_id _id) {
    id=_id;return GetPtr()->IncRefCount();}
 public:
  cheap_id Get() const {return id;}
  ostream& Print(ostream& os) const;
  inline void Clear() {
    if(id!=-1) {GetPtr()->DecRefCount();id=-1;}}
  inline int Set(cheap_id _id) {
    Clear();return _Set(_id);}
  inline RefCountCollectableReference(cheap_id _id) {_Set(id);}
  RefCountCollectableReference(GAPIC_term_ref) 
    throw(GapicObjectCreationError);
  ~RefCountCollectableReference() {Clear();}
};

extern gapic_type RefCountCollectableReferenceType;
typedef GapicCollectable<RefCountCollectableReference,
  &RefCountCollectableReferenceType> RefCountCollectableReferenceCollectable;

typedef PrologVector<RefCountCollectableReference> RefCountCollRefArray;
extern gapic_type RefCountCollRefArrayType;
typedef GapicCollectable<RefCountCollRefArray,
  &RefCountCollRefArrayType> RefCountCollRefArrayCollectable;

template <class T> class PrologDouble : public PrologPair<T,T> {
 public:
  PrologDouble() {}
  PrologDouble(GAPIC_term_ref src) throw(GapicObjectCreationError)
    : PrologPair<T,T>(src) {}
};

class RefCountCollRefDouble 
: public PrologDouble<RefCountCollectableReference> {
 public:
	GAPIC_result get(GAPIC_int idx, GAPIC_term_ref el) const;

  RefCountCollRefDouble(GAPIC_term_ref src)  throw(GapicObjectCreationError)
    : PrologDouble<RefCountCollectableReference>(src) {}
};

extern gapic_type RefCountCollRefDoubleType;
typedef GapicCollectable<RefCountCollRefDouble,
  &RefCountCollRefDoubleType> RefCountCollRefDoubleCollectable;

class RefCountCollRefTuple3 
: public PrologPair<RefCountCollectableReference,RefCountCollRefDouble> {
 public:
	GAPIC_result get(GAPIC_int idx, GAPIC_term_ref el) const;

  RefCountCollRefTuple3(GAPIC_term_ref src)  throw(GapicObjectCreationError)
    : PrologPair<RefCountCollectableReference,RefCountCollRefDouble>(src) {}
};
extern gapic_type RefCountCollRefTuple3Type;
typedef GapicCollectable<RefCountCollRefTuple3,
  &RefCountCollRefTuple3Type> RefCountCollRefTuple3Collectable;

class RefCountCollRefTuple4
: public PrologPair<RefCountCollectableReference,RefCountCollRefTuple3> {
 public:
	GAPIC_result get(GAPIC_int idx, GAPIC_term_ref el) const;

  RefCountCollRefTuple4(GAPIC_term_ref src)  throw(GapicObjectCreationError)
    : PrologPair<RefCountCollectableReference,RefCountCollRefTuple3>(src) {}
};
extern gapic_type RefCountCollRefTuple4Type;
typedef GapicCollectable<RefCountCollRefTuple4,
  &RefCountCollRefTuple4Type> RefCountCollRefTuple4Collectable;

#endif


