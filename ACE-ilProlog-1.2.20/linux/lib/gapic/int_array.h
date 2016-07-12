
  
          
     
 






#ifndef C_PROLOG_H_INC
#error include c_prolog.h before int_array.h
#endif

#ifndef _int_array_INC
#define _int_array_INC
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

typedef GAPIC_int GAPIC_integer;

class int_array;
class int_matrix;
class int_weighted_array;

class int_array : public vector<GAPIC_int>,
  public MutableRefCountObject {
 private:
 public:
  void Clear() { clear(); };
  void Concat(const int_array *src);
  void Print() const;
  ostream& Print(ostream& os) const;
  GAPIC_result PutPrologList(GAPIC_term_ref source);
  GAPIC_result PutPrologList(GAPIC_term_ref source, int source_length);
  GAPIC_result PutPrologList(GAPIC_term_ref source, int start, int source_length);
	void setList(GAPIC_term_ref list);

	G_SO void pushback(GAPIC_int value);

	void addValue(GAPIC_int pos, GAPIC_int value);

	void set(GAPIC_int pos, GAPIC_int value);

	G_SO GAPIC_int get(GAPIC_int pos);

	G_SO int getSize();

	void removeAll();

	void expandTo(GAPIC_int count);

  int_array() {}
  int_array(GAPIC_term_ref src)
    throw(GapicObjectCreationError);
};

inline ostream& operator<<(ostream& os,const int_array& arr) {
  return arr.Print(os);
}

extern gapic_type gapic_int_array_type;

class gapic_int_array_collectable
: public GapicNonExMutCollectable<int_array,&gapic_int_array_type> {
 public:
  gapic_int_array_collectable(int_array *src) :
    GapicNonExMutCollectable<int_array,&gapic_int_array_type>(src) {};
};

typedef gapic_int_array_collectable int_arrayCollectable;
       
       

class weighted_int_array : public vector<pair<GAPIC_float,GAPIC_int> >,
  public MutableRefCountObject {
 private:
 public:
  void Clear() {clear();};
  void Concat(const weighted_int_array *src);
  void Print() const;
  ostream& Print(ostream& os) const;
  GAPIC_result PutPrologList(GAPIC_term_ref source);
  GAPIC_result PutPrologList(GAPIC_term_ref source, int source_length);
  GAPIC_result PutPrologList(GAPIC_term_ref source,
			     int start,
			     int source_length);
	GAPIC_result pushback(GAPIC_term_ref element);

  weighted_int_array() {}
  weighted_int_array(GAPIC_term_ref src)
    throw(GapicObjectCreationError);
};

inline ostream& operator<<(ostream& os,const weighted_int_array& arr) {
  return arr.Print(os);
}

extern gapic_type gapic_weighted_int_array_type;

class gapic_weighted_int_array_collectable
: public GapicNonExMutCollectable<weighted_int_array,&gapic_weighted_int_array_type> {
 public:
  gapic_weighted_int_array_collectable(weighted_int_array *src) :
    GapicNonExMutCollectable<weighted_int_array,&gapic_weighted_int_array_type>(src) {};
};

typedef gapic_weighted_int_array_collectable weighted_int_arrayCollectable;

class int_matrix : public MutableRefCountObject {
 private:
  int rows;
  int cols;
  int allocrows;
  GAPIC_int *data;
  void RawResize(int _rows,int _cols);
  void init(GAPIC_term_ref src) throw(GapicObjectCreationError);
  void init(int _rows,int _cols);
  void init();
 public:
	int GetSize() const {

    return cols*rows;}
	int GetCols() const {

    return cols;}
	int GetRows() const {

    return rows;}
	void Put(GAPIC_int row, GAPIC_int col, GAPIC_int value) {

    data[(row*cols)+col]=value;};
	void PutArray(GAPIC_int i, GAPIC_int value) {

    data[i]=value;};
	void Resize(GAPIC_int _rows, GAPIC_int _cols);

	GAPIC_int Get(GAPIC_int row, GAPIC_int col) const {

  	return data[(row*cols)+col];}
	void Add(GAPIC_int row, GAPIC_int col, GAPIC_int value) const {

  	data[(row*cols)+col] += value;}
	GAPIC_int GetArray(GAPIC_int i) const {

  	return data[i];}
	int AddRow();

	void SetSize(GAPIC_int _rows, GAPIC_int _cols, GAPIC_int fill);

	void Clear() {

    Resize(0,0);}
  int_matrix& operator =(const int_matrix& Src);
  void Clear(GAPIC_int filler) {
    for(int i=0;i<GetSize();i++) data[i]=filler;
  }
  GAPIC_int* GetRow(int row) const {
  	return &data[row*cols]; }
  GAPIC_result Row_PutPrologList(int _row,GAPIC_term_ref List);
  GAPIC_result PutPrologListList(GAPIC_term_ref List);
  void Print() const;
  ostream& Print(ostream& os) const;
  ~int_matrix() {GAPIC_free(data);};
  int_matrix()  {init();}
  int_matrix(GAPIC_term_ref src) throw(GapicObjectCreationError) {init(src);};
  int_matrix(int _rows,int _cols) {init(_rows,_cols);}
};

extern gapic_type gapic_int_matrix_type;

class gapic_int_matrix_collectable
: public GapicNonExMutCollectable<int_matrix,&gapic_int_matrix_type> {
 public:
  gapic_int_matrix_collectable(int_matrix *src) :
    GapicNonExMutCollectable<int_matrix,&gapic_int_matrix_type>(src) {};
};

typedef gapic_int_matrix_collectable int_matrixCollectable;

#endif
#endif
