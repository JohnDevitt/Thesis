
#ifndef TYPED_TERM_INC
#define TYPED_TERM_INC

#include "../gapic/refcount.h"

#define PROLOG_TERM_TYPE_UNTYPED 0x0001
#define PROLOG_TERM_TYPE_NOTFORPREDICATES 0x0002
#define PROLOG_TERM_TYPE_NOTFORFUNCTORS 0x0004
#define PROLOG_TERM_TYPE_INT 0x0008
#define PROLOG_TERM_TYPE_FLOAT 0x0010

class PrologTermType;
typedef RefCountPtr<PrologTermType> PrologTermTypeRCPtr;

class PrologTermType {
 private:
  const int flags;
 public:
  virtual ostream& Print(ostream& os) const=0;
  virtual int CheckTerm(GAPIC_term_ref term) const=0;
  int IsTyped() const {
    return (flags&PROLOG_TERM_TYPE_UNTYPED)==0;}
  int IsFunctorType() const {
    return (flags&PROLOG_TERM_TYPE_NOTFORFUNCTORS)==0;}
  int IsPredicateType() const {
    return (flags&PROLOG_TERM_TYPE_NOTFORPREDICATES)==0;}
  PrologTermType(int _flags) 
    : flags(_flags) {}
};

class PrologTermTypeUntyped : public PrologTermType {
 public:
  virtual ostream& Print(ostream& os) const;
  virtual int CheckTerm(GAPIC_term_ref term) const;
  PrologTermTypeUntyped()
    : PrologTermType(PROLOG_TERM_TYPE_UNTYPED) {}
  PrologTermTypeUntyped(int _flags) 
    : PrologTermType(_flags|PROLOG_TERM_TYPE_UNTYPED) {}
};

class PrologTermTypeInt  : public PrologTermType {
 public:
  virtual ostream& Print(ostream& os) const;
  virtual int CheckTerm(GAPIC_term_ref term) const;  
  PrologTermTypeInt() 
    :  PrologTermType(PROLOG_TERM_TYPE_INT) {}
  PrologTermTypeInt(int _flags) 
    : PrologTermType(_flags|PROLOG_TERM_TYPE_INT) {}
};

class PrologTermTypeFloat : public PrologTermType {
 public:
  virtual ostream& Print(ostream& os) const;
  virtual int CheckTerm(GAPIC_term_ref term) const;
  PrologTermTypeFloat() 
    :  PrologTermType(PROLOG_TERM_TYPE_INT) {}
  PrologTermTypeFloat(int _flags) 
    : PrologTermType(_flags|PROLOG_TERM_TYPE_INT) {}
};

class PrologTermTypeAtom : public PrologTermType {
 public:
  virtual ostream& Print(ostream& os) const;
  virtual int CheckTerm(GAPIC_term_ref term) const;
  PrologTermTypeAtom() 
    :  PrologTermType(PROLOG_TERM_TYPE_ATOM) {}
  PrologTermTypeAtom(int _flags) 
    : PrologTermType(_flags|PROLOG_TERM_TYPE_ATOM) {}
};

class PrologTermTypeSpecAtom 
: public PrologTermTypeAtom {
 private:
  GAPIC_atom atom;
 public:
  virtual ostream& Print(ostream& os) const;
  virtual int CheckTerm(GAPIC_term_ref term) const;
  PrologTermTypeSpecAtom(const GAPIC_atom& _atom) 
    : atom(_atom) {}
  PrologTermTypeSpecAtom(const GAPIC_atom& _atom, int _flags) 
    : atom(_atom), PrologTermTypeAtom(_flags) {}
  PrologTermTypeSpecAtom(GAPIC_term_ref src) 
    throw(GapicObjectCreationError); 
}



class PrologTermTypeFunctor : public PrologTermType {
  GAPIC_functor funct;
  PrologTermTypeRCPtr *argtypes;
 public:
  virtual ostream& Print(ostream& os) const;
  virtual int CheckTerm(GAPIC_term_ref term) const;
  PrologTermTypeFunctor(GAPIC_term_ref src);
};

class PrologTermTypeUnion : public PrologTermType {

};

extern PrologTermTypeUntyped untyped_prologtermtype;

#endif
