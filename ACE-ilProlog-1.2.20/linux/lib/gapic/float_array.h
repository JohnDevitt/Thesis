
  
          
     
 






#ifndef C_PROLOG_H_INC
#error include c_prolog.h before float_array.h
#endif

#ifndef _float_array_INC
#define _float_array_INC
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

class float_array;
class float_matrix;
class float_weighted_array;

class float_array : public vector<GAPIC_float>,
  public MutableRefCountObject {
 private:
 public:
  void Clear() { clear(); };
  void Concat(const float_array *src);
  void Print() const;
  ostream& Print(ostream& os) const;
  GAPIC_result PutPrologList(GAPIC_term_ref source);
  GAPIC_result PutPrologList(GAPIC_term_ref source, int source_length);
  GAPIC_result PutPrologList(GAPIC_term_ref source, int start, int source_length);
	void setList(GAPIC_term_ref list);

	G_SO void pushback(GAPIC_float value);

	void addValue(GAPIC_int pos, GAPIC_float value);

	void set(GAPIC_int pos, GAPIC_float value);

	G_SO double get(GAPIC_int pos);

	G_SO int getSize();

	void removeAll();

	void expandTo(GAPIC_int count);

  float_array() {}
  float_array(GAPIC_term_ref src)
    throw(GapicObjectCreationError);
};

inline ostream& operator<<(ostream& os,const float_array& arr) {
  return arr.Print(os);
}

extern gapic_type gapic_float_array_type;

class gapic_float_array_collectable
: public GapicNonExMutCollectable<float_array,&gapic_float_array_type> {
 public:
  gapic_float_array_collectable(float_array *src) :
    GapicNonExMutCollectable<float_array,&gapic_float_array_type>(src) {};
};

typedef gapic_float_array_collectable float_arrayCollectable;
       
       

class weighted_float_array : public vector<pair<GAPIC_float,GAPIC_float> >,
  public MutableRefCountObject {
 private:
 public:
  void Clear() {clear();};
  void Concat(const weighted_float_array *src);
  void Print() const;
  ostream& Print(ostream& os) const;
  GAPIC_result PutPrologList(GAPIC_term_ref source);
  GAPIC_result PutPrologList(GAPIC_term_ref source, int source_length);
  GAPIC_result PutPrologList(GAPIC_term_ref source,
			     int start,
			     int source_length);
	GAPIC_result pushback(GAPIC_term_ref element);

  weighted_float_array() {}
  weighted_float_array(GAPIC_term_ref src)
    throw(GapicObjectCreationError);
};

inline ostream& operator<<(ostream& os,const weighted_float_array& arr) {
  return arr.Print(os);
}

extern gapic_type gapic_weighted_float_array_type;

class gapic_weighted_float_array_collectable
: public GapicNonExMutCollectable<weighted_float_array,&gapic_weighted_float_array_type> {
 public:
  gapic_weighted_float_array_collectable(weighted_float_array *src) :
    GapicNonExMutCollectable<weighted_float_array,&gapic_weighted_float_array_type>(src) {};
};

typedef gapic_weighted_float_array_collectable weighted_float_arrayCollectable;

class float_matrix : public MutableRefCountObject {
 private:
  int rows;
  int cols;
  int allocrows;
  GAPIC_float *data;
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
	void Put(GAPIC_int row, GAPIC_int col, GAPIC_float value) {

    data[(row*cols)+col]=value;};
	void PutArray(GAPIC_int i, GAPIC_float value) {

    data[i]=value;};
	void Resize(GAPIC_int _rows, GAPIC_int _cols);

	double Get(GAPIC_int row, GAPIC_int col) const {

  	return data[(row*cols)+col];}
	void Add(GAPIC_int row, GAPIC_int col, GAPIC_float value) const {

  	data[(row*cols)+col] += value;}
	double GetArray(GAPIC_int i) const {

  	return data[i];}
	int AddRow();

	void SetSize(GAPIC_int _rows, GAPIC_int _cols, GAPIC_float fill);

	void Clear() {

    Resize(0,0);}
  float_matrix& operator =(const float_matrix& Src);
  void Clear(GAPIC_float filler) {
    for(int i=0;i<GetSize();i++) data[i]=filler;
  }
  GAPIC_float* GetRow(int row) const {
  	return &data[row*cols]; }
  GAPIC_result Row_PutPrologList(int _row,GAPIC_term_ref List);
  GAPIC_result PutPrologListList(GAPIC_term_ref List);
  void Print() const;
  ostream& Print(ostream& os) const;
  ~float_matrix() {GAPIC_free(data);};
  float_matrix()  {init();}
  float_matrix(GAPIC_term_ref src) throw(GapicObjectCreationError) {init(src);};
  float_matrix(int _rows,int _cols) {init(_rows,_cols);}
};

extern gapic_type gapic_float_matrix_type;

class gapic_float_matrix_collectable
: public GapicNonExMutCollectable<float_matrix,&gapic_float_matrix_type> {
 public:
  gapic_float_matrix_collectable(float_matrix *src) :
    GapicNonExMutCollectable<float_matrix,&gapic_float_matrix_type>(src) {};
};

typedef gapic_float_matrix_collectable float_matrixCollectable;

#endif
#endif
