#include <math.h>
#include <vector>
#include <iostream>

#include "matrix.h"
 

class QRfactorization {
 private:
  Matrix *m_Q;
  Matrix m_R;
  real m_minnorm;
  void doFactorization();
 protected: 
  Matrix& getQ() {return *m_Q;}
  Matrix& getR() {return m_R;}
 public:
  real minNorm() const {return m_minnorm;}
  real Q(int r, int c) const;
  real R(int r, int c) const;
  const Matrix& R() const {return m_R;}
  const Matrix& Q() const {return *m_Q;}
  void updateAddOneRow(const MVector& row);
  QRfactorization(Matrix* Q) 
    : m_Q(Q), m_R(Q->cols(),Q->cols()), m_minnorm(1.0e-10) {}
  QRfactorization(Matrix* Q, int QR_cols) 
    : m_Q(Q), m_R(QR_cols,QR_cols), m_minnorm(1.0e-10) {}
  ~QRfactorization() {
  }
};

class QRfactorizationGaussJordan : public QRfactorization {
 private:
  Matrix m_local_Q;
  void factorGaussJordan(const Matrix& QR, Matrix& Q, Matrix& R);
 public:
  QRfactorizationGaussJordan(const Matrix& QR) 
    : m_local_Q(QR.rows(),QR.cols()), 
    QRfactorization(&m_local_Q, QR.cols()) {
    factorGaussJordan(QR,getQ(),getR());
  }

};


class QRfactorizationGramSchmidt : public QRfactorization {
 private:
  Matrix m_local_Q;
  void factorGramSchmidt(const Matrix& QR, Matrix& Q, Matrix& R);
 public:
  QRfactorizationGramSchmidt(const Matrix& QR) 
    : m_local_Q(QR.rows(),QR.cols()), 
    QRfactorization(&m_local_Q, QR.cols()) {
    factorGramSchmidt(QR,getQ(),getR());
  }

};


class QRfactorizationHouseholder : public QRfactorization {
 private:
  Matrix m_local_Q;
  void factorHouseholder(const Matrix& QR, Matrix& Q, Matrix& R);
 public:
  QRfactorizationHouseholder(const Matrix& QR) 
    : m_local_Q(QR.rows(),QR.cols()),
    QRfactorization(&m_local_Q, QR.cols()) {
    factorHouseholder(QR, getQ(),getR());
  }
};



     
 
      
 
      
    
  



     
 
      
 
      
    
  




     
 
   
 
     
       






class CholeskyDecomposition {
 private:
  Matrix m_L;
  real m_minnorm;
  void decomposeCholesky();
  Matrix& getL() {return m_L;}
 
 public:
  real minNorm() const {return m_minnorm;}
  real L(int r, int c) const;
  const Matrix& L() const {return m_L;}

  CholeskyDecomposition(Matrix &L) 
    : m_L(L), m_minnorm(1.0e-10) { 
    decomposeCholesky();
  }
  ~CholeskyDecomposition() {
  }
};


class EigenDecomposition {
 private:
  Matrix m_V;
  Matrix m_D;
  real m_minnorm;
  void eigenDecompose();
  Matrix& getV() {return m_V;}
  Matrix& getD() {return m_D;}
 public:
  real minNorm() const {return m_minnorm;}
  real V(int r, int c) const;
  real D(int r, int c) const;

  const Matrix& V() const {return m_V;}
  const Matrix& D() const {return m_D;}

  EigenDecomposition(Matrix &V) 
    : m_V(V), m_D(V.cols(),V.cols(),0.0), m_minnorm(1.0e-10) { 
    eigenDecompose();
  }
  ~EigenDecomposition() {
  }
};
