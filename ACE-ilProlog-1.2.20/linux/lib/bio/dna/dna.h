#ifndef DNA_INC
#define DNA_INC


 


#include "../../gapic/gapicgc.h"
#include "../../gapic/refcount.h"

extern GAPIC_atom *one_char_atom[96];

class ActgString : public RefCountObject {
 private:
  vector<char> data;
  void init(const char* filename);
 public:
      
	void Get(GAPIC_int idx, GAPIC_term_ref dst) const;

	void Len(GAPIC_int* len) const;

  virtual ostream& Print(ostream& os) const;
  ActgString();
  ActgString(const char* filename) {init(filename);}
  ActgString(GAPIC_term_ref src) throw(GapicObjectCreationError);
};

extern gapic_type GapicActgStringType;
typedef GapicCollectable<ActgString,&GapicActgStringType> 
	ActgStringCollectable;

typedef struct {
  char sign;
  long bound;
} RnaSecStructItem;

class RnaSecStruct : public RefCountObject {
 private:
  vector<RnaSecStructItem> data;
  void init(const char* filename);
 public:
      
	void Get(GAPIC_int idx, GAPIC_term_ref sign, GAPIC_int* bound);

	void Len(GAPIC_int* len) const;

  virtual ostream& Print(ostream& os) const;
  RnaSecStruct();
  RnaSecStruct(const char* filename) {init(filename);}
  RnaSecStruct(GAPIC_term_ref src) throw(GapicObjectCreationError);
};

extern gapic_type GapicRnaSecStructType;
typedef GapicNonExMutCollectable<RnaSecStruct,&GapicRnaSecStructType> 
	RnaSecStructCollectable;



#endif

