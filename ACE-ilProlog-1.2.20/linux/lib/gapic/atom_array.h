
  
          
       
 






#ifndef C_PROLOG_H_INC
#error include c_prolog.h before atom_array.h
#endif

#ifndef _atom_array_INC
#define _atom_array_INC
#ifndef __cplusplus
#error should be using C++ for including this file
#else

#include "gapic_alloc.h"
#include <vector>
#include <iostream>
#include "gapicgc.h"

#ifdef GAPIC_COMMON_INC 
#define _INCOMMON 
#define _INITNULL =NULL
#else
#define _INCOMMON extern
#define _INITNULL 
#endif

class atom_array;
class atom_matrix;
class atom_weighted_array;

class atom_array : public vector<GAPIC_atom>,
  public MutableRefCountObject {
 private:
 public:
  void Clear() {clear();};
  void Concat(const atom_array *src);
  void Print() const;
  ostream& Print(ostream& os) const;
  GAPIC_result PutPrologList(GAPIC_term_ref source);
  GAPIC_result PutPrologList(GAPIC_term_ref source, int source_length);
  GAPIC_result PutPrologList(GAPIC_term_ref source, 
			     int start,
			     int source_length);
	GAPIC_result pushback(GAPIC_term_ref element);

  atom_array() {}
  atom_array(GAPIC_term_ref src) 
    throw(GapicObjectCreationError);
};

inline ostream& operator<<(ostream& os,const atom_array& arr) {
  return arr.Print(os);
}

extern gapic_type gapic_atom_array_type;

class gapic_atom_array_collectable 
: public GapicNonExMutCollectable<atom_array,&gapic_atom_array_type> {
 public:
  gapic_atom_array_collectable(atom_array *src) : 
    GapicNonExMutCollectable<atom_array,&gapic_atom_array_type>(src) {};
};

typedef gapic_atom_array_collectable atom_arrayCollectable;
       
        

class weighted_atom_array : public vector<pair<GAPIC_float,GAPIC_atom> >,
  public MutableRefCountObject {
 private:
 public:
  void Clear() {clear();};
  void Concat(const weighted_atom_array *src);
  void Print() const;
  ostream& Print(ostream& os) const;
  GAPIC_result PutPrologList(GAPIC_term_ref source);
  GAPIC_result PutPrologList(GAPIC_term_ref source, int source_length);
  GAPIC_result PutPrologList(GAPIC_term_ref source, 
			     int start,
			     int source_length);
	GAPIC_result pushback(GAPIC_term_ref element);

  weighted_atom_array() {}
  weighted_atom_array(GAPIC_term_ref src) 
    throw(GapicObjectCreationError);
};

inline ostream& operator<<(ostream& os,const weighted_atom_array& arr) {
  return arr.Print(os);
}

extern gapic_type gapic_weighted_atom_array_type;

class gapic_weighted_atom_array_collectable 
: public GapicNonExMutCollectable<weighted_atom_array,&gapic_weighted_atom_array_type> {
 public:
  gapic_weighted_atom_array_collectable(weighted_atom_array *src) : 
    GapicNonExMutCollectable<weighted_atom_array,&gapic_weighted_atom_array_type>(src) {};
};

typedef gapic_weighted_atom_array_collectable weighted_atom_arrayCollectable;

class atom_matrix : public MutableRefCountObject {
 private:
  int rows;
  int cols;
  GAPIC_atom *data;
  void RawResize(int _rows,int _cols);
  void init(GAPIC_term_ref src) throw(GapicObjectCreationError);
  void init(int _rows,int _cols);
 public:
	int GetSize() const {

    return cols*rows;}
	int GetCols() const {

    return cols;}
	int GetRows() const {

    return rows;}
	void Put(GAPIC_int row, GAPIC_int col, const GAPIC_atom& value) {

    data[(row*cols)+col]=value;};
  GAPIC_result Row_PutPrologList(int _row,GAPIC_term_ref List);
  GAPIC_result PutPrologListList(GAPIC_term_ref List);
  void Print() const;  
  ostream& Print(ostream& os) const;
	void Resize(GAPIC_int _rows, GAPIC_int _cols);

  atom_matrix& operator =(const atom_matrix& Src);
  GAPIC_atom Get(int row, int col) const {return data[(row*cols)+col];}
	void Get(GAPIC_int row, GAPIC_int col, GAPIC_atom* val) const {

    *val=Get(row,col);
  }
  void Clear(GAPIC_atom filler) {
    for(int i=0;i<GetSize();i++) data[i]=filler;
  }
	void Clear() {

    Resize(0,0);}
  ~atom_matrix() {GAPIC_free(data);};
  atom_matrix()  {init(0,0);}
  atom_matrix(GAPIC_term_ref src) throw(GapicObjectCreationError) {init(src);};
  atom_matrix(int _rows,int _cols) {init(_rows,_cols);}
};

extern gapic_type gapic_atom_matrix_type;

class gapic_atom_matrix_collectable 
: public GapicNonExMutCollectable<atom_matrix,&gapic_atom_matrix_type> {
 public:
  gapic_atom_matrix_collectable(atom_matrix *src) : 
    GapicNonExMutCollectable<atom_matrix,&gapic_atom_matrix_type>(src) {};
};

typedef gapic_atom_matrix_collectable atom_matrixCollectable;

#endif
#endif
