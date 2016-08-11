#include <math.h>
#include <vector>
#include <iostream>

using namespace std;


typedef double real;
const int safe_matrix_code=1;
const int safe_vector_code=1;


class safe_real_vector : private vector<real> {
 public:
  inline real operator[](unsigned i) const {
    if(safe_vector_code)
      if((i<0)||(i>=size())) {
	cerr << "safe_real_vector: operator[](unsigned i) const : i (=" << i
	     << ") out of range\n"; 
	return 0;
      }
    return (*static_cast<const vector<real> *>(this))[i];
  }  
  inline real& operator[](unsigned i) {
    if(safe_vector_code)
      if((i<0)||(i>=size())) {
	cerr << "safe_real_vector: operator[](unsigned i) : i (=" << i
	     << ") out of range\n"; 
	return *((real*)NULL);
      }
    return (*static_cast<vector<real> *>(this))[i];
  }
  inline size_t size() const {
    return vector<real>::size();
  }
  inline void resize(size_t size) {
    vector<real>::resize(size);
  }
  inline void resize(size_t size, real fill) {
    vector<real>::resize(size,fill);
  }
  inline void pop_back() {
    if(safe_vector_code)
      if(vector<real>::size()==0) {
	cerr << "safe_real_vector::pop_back : vector is already empty\n";
	return;
      }
    vector<real>::pop_back();
  }
};


class Matrix;



class GivensRotation {
 private:
  int m_r1;
  int m_r2;
  real m_cos;
  real m_sin;
 public:
  real cos() const {return m_cos;}
  real sin() const {return m_sin;}
  int r1() const {return m_r1;}
  int r2() const {return m_r2;}
  ostream& write(ostream& os) const {
    return os << "Givens["<<m_r1<<'-'<<m_r2<<':'<<m_cos<<","<<m_sin<<"]";
  }
  GivensRotation(const Matrix& m, int rowtozero, int row2, int col);
  GivensRotation(int r1, int r2, real c, real s) 
    : m_r1(r1), m_r2(r2), m_cos(c), m_sin(s) {}
};



inline ostream& operator<<(ostream& os, const GivensRotation& G) {
  return G.write(os);
}






class MVector 
: public safe_real_vector {
     

  real unsafe_get(int i){
    return ((*this)[i]);
  }

 public:
  void scale(double f) {
    for(int i=0;i<size();i++) (*this)[i]*=f;
  }
  void add(const MVector& a) {
    int minsize=min(size(),a.size());
    for(int i=0;i<minsize;i++) (*this)[i]+=a[i];
  }
  void addScaled(const MVector& a, double f) {
    if(size()<a.size()) resize(a.size(),0.0);
    int minsize=min(size(),a.size());
    for(int i=0;i<minsize;i++) (*this)[i]+=a[i]*f;
  }

  real maxElement(){
    real max=0.0;
    for(int i=0;i<size();i++){
      if((*this)[i]>max)
	max=(*this)[i];
    }
    return(max);
  }


  void storeDotProduct(const MVector& v1, const MVector& v2);

  void storeMatrixProduct(const Matrix& m, const MVector& v);

  void storeMatrixProduct(const MVector& v, const Matrix& m);

  real get(int i){ return (unsafe_get(i));}

  void setNull() {for(int i=0;i<size();i++) (*this)[i]=0;}
  ostream& write(ostream& os) const {
    os << "MVector["; for (int i=0;i<size();i++) os << (*this)[i] << ' ';
    return os << "]";
  }
  MVector() {}
  MVector(int dim) {resize(dim);}
};








class Matrix 
: public safe_real_vector {
      
  int m_rows;
  int m_cols;
  int m_rowmem;
  int m_colmem;
  real unsafe_get(int i,int j) const {
    return (*this)[m_cols*i+j];
  }
  real& unsafe_get(int i,int j) {
    return (*this)[m_cols*i+j];
  }
  void destroy_resize(int r, int c) {
    m_rows=r; m_cols=c;
    safe_real_vector::resize(r*c);
    
  }
  void resized() {
    safe_real_vector::resize(m_rows*m_cols);
  }
  void resized(real fill) {
    safe_real_vector::resize(m_rows*m_cols, fill);
     
  }
 public:
  void removeRowsAtEnd(int n);
  void removeRows(int r, int n);
  void addRowsAtEnd(int n, real fill=0.0);
  void removeCols(int c, int n);
  void removeColsAtEnd(int n);
  void addColsAtEnd(int n, real fill=0.0);

  void switchrows(int row1, int row2);
  void resize(int r, int c) {
    if(r<m_rows) m_rows=r;
    if(c<m_cols) 
      removeColsAtEnd(m_cols-c);
    else if(c>m_cols) 
      addColsAtEnd(c-m_cols);
    if(r>m_rows) addRowsAtEnd(r-m_rows);
  }
  void put(int i, int j, real r) {
	(*this)[m_cols*i+j]=r;
  }
  Matrix() {}
  
  Matrix(const Matrix& src) 
    : m_rows(src.m_rows), m_cols(src.m_cols),
     
    safe_real_vector(src)
    {}
  Matrix(int rows, int cols) : m_rows(rows), m_cols(cols) {
    safe_real_vector::resize(m_rows*m_cols);
    
  }
  Matrix(int rows, int cols, real fill) : m_rows(rows), m_cols(cols) {
    safe_real_vector::resize(m_rows*m_cols,fill);
    
  }
  ~Matrix() {}
  ostream& write(ostream& os) const {
    for(int r=0;r<rows();r++) {
      if(r) os << "  "; else os << "[ ";
      for(int c=0;c<cols();c++) 
	os << unsafe_get(r,c) << ' ';
      if(r==rows()-1) os << ']';
      os << endl;
    }
    return os;
  }

  real operator()(int i,int j) const {
    if(safe_matrix_code) {
      if((i<0)||(i>m_rows)) { cerr << "....";}
       
    }
    return unsafe_get(i,j);
  }
  real& operator()(int i,int j) {
    if(safe_matrix_code) {
      if((i<0)||(i>m_rows)) { cerr << "....";}
       
    }
    return unsafe_get(i,j);
  }
  int rows() const {return m_rows;}
  int cols() const {return m_cols;}
   
  void add(const Matrix& m) {
    if(safe_matrix_code) {
      if(m.rows()!=rows()) cerr << " ....";
    }
    
  }
  void scaleRow(int r, real f) {
    for(int i=0;i<m_cols;i++)
      unsafe_get(r,i)*=f;
  }
  void scaleCol(int c, real f) {
    for(int i=0;i<m_rows;i++)    
      unsafe_get(i,c)*=f; 
  }
  void rowScaledAdd(int dst, int src, real f) {
    for(int i=0;i<m_cols;i++)
      unsafe_get(dst,i)+=unsafe_get(src,i)*f;
  }

  void colScaledAdd(int dst, int src, real f) {
    for(int i=0;i<m_rows;i++)
      unsafe_get(i,dst)+=unsafe_get(i,src)*f;
  }


  real normCol(int c){
    real norm=0.0;
    for(int i=0;i<m_rows;i++)
      norm+=unsafe_get(i,c)*unsafe_get(i,c);
    norm=sqrt(norm); 
    return(norm);
  }

  real normRow(int r){
    real norm=0.0;
    for(int j=0;j<m_cols;j++)
      norm+=unsafe_get(r,j)*unsafe_get(r,j);
    norm=sqrt(norm);
    return(norm);
  }

  real dotProductCols(int c1, int c2){
    real prod=0.0;
    for(int i=0;i<m_rows;i++)
      prod+=unsafe_get(i,c1)*unsafe_get(i,c2);
    return(prod);
  }

  real norm1(){
    real norm=0.0;
    real acc;
    for(int j=0;j<m_cols;j++){
      acc=0.0;
      for(int i=0;i<m_rows;i++)
	acc+=fabs((*this)(i,j));
      if(acc>norm)
	norm=acc;
    }
    return(norm);
  }

  real norminf(){
    real norm=0.0;
    real acc;
    for(int i=0;i<m_rows;i++){
      acc=0.0;
      for(int j=0;j<m_cols;j++)
	acc+=fabs((*this)(i,j));
      if(acc>norm)
	norm=acc;
    }
    return(norm);
  }

  real normFrobenius(){
    real norm=0.0;
    for(int i=0;i<m_rows;i++){
      for(int j=0;j<m_cols;j++)
	norm+=((*this)(i,j)*(*this)(i,j));
    }
    norm=sqrt(norm);
    return(norm);
  }

  real spectralNorm(){
    real norm=0.0;
    Matrix Copy(*this);
    square_ata(Copy);
    MVector E(m_cols);
    E=this->eigenValues();
    norm=E.maxElement();
    (*this)=Copy;
    return(sqrt(norm));
  }



  Matrix& operator+=(const Matrix& m) {
    add(m);
    return *this;
  }

  Matrix& operator=(const Matrix& m) {
    destroy_resize(m.m_rows,m.m_cols);
    for(int i=0;i<m_cols*m_rows;i++) 
      (*this)[i]=m[i];
  }

  void zeros() {
    for(int i=0;i<m_rows;i++){
      for(int j=0;j<m_cols;j++)
	put(i,j,0.0);
    }
  }

  void multiply(const Matrix& a, const Matrix& b);
           
  void subtract(const Matrix& a, const Matrix& b);
           
  void square_ata(const Matrix& a);
  void square_aat(const Matrix& a);
  void storeInverse(const Matrix& a);
  void storeInverseDestroyingOriginal(Matrix& input);
  
  int inverse(const Matrix& a);
  int inverseDestroyingOriginal(Matrix& input);

  void transpose(const Matrix& original);
  void preMultiplyGivens(const GivensRotation& G);    
  void postMultiplyGivens(const GivensRotation& G);    
  void preMultiplyGivensTranspose(const GivensRotation& G);    
  void postMultiplyGivensTranspose(const GivensRotation& G);    
  void Givens(int i, int j, Matrix& G);
  void Householder(int col, Matrix& H);
  MVector eigenValues();
};

inline ostream& operator<<(ostream& os, const Matrix& m) {
  return m.write(os);
}






inline ostream& operator<<(ostream& os, const MVector& m) {
  return m.write(os);
}

inline GivensRotation::GivensRotation(const Matrix& M, 
				      int rowtozero, int row2, int col) 
  : m_r1(rowtozero), m_r2(row2) {
  
  real a=M(row2,col);
  real b=M(rowtozero,col);
  real r;


  if(b==0) {
    m_sin=0.0;
    m_cos=(a>=0.0 ? 1.0 : -1.0);
  }
  else {
  
    if(fabs(b)>=fabs(a)) {
      r = -a/b;
      m_sin =-1/sqrt(1+(r*r));
      if (b<0) m_sin*=-1;
      m_cos = r*m_sin; 
    }
    else {
      r = -b/a;
      m_cos = 1/sqrt(1+(r*r));
      if (a<0) m_cos*=-1;
      m_sin = r*m_cos;
    }
 
  }
}

