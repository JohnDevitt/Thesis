
#include "../matrix/matrix.h"


class Polynomial : public MVector {
 public:
  Polynomial() {}
  Polynomial(int degree) {resize(degree+1,0.0);}
  int degree() const {return size()-1;}
  double apply(double x) const {
    double ret=0;
    double xi=1.0;
    for(int i=0;i<=degree();i++) {
      ret+=xi*(*this)[i];
      xi*=x;
    }
    return ret;
  }

  void differentiate(){        
    for(int i=1;i<=degree();i++)
      (*this)[i-1]=(*this)[i]*i;  
    this->pop_back();
  }

  double getNthDiffCoef(int i, int n) {
    if(i<0) return 0.0;
    if(i+n>degree()) return 0.0;
    double coef=(*this)[i+n];
    for(int j=1;j<=n;j++) {coef*=(i+j);}
    return coef;
  }

  double nthDiffAtX(int n, double x){       
    double ret=0.0;
    double xi=1.0;
    for(int i=n;i<=degree();i++) {
      double coef=1.0;
      for(int j=0;j<n;j++) coef*=(i-j);
      ret+=xi*getNthDiffCoef(i-n,n);
        
      xi*=x;
    }
    
     
                 
        
     
      
        
	 
       
     
    
    return ret;
  }
  double getCoeff(int i){   
    if(i>degree()) {
      cout<<"At Polynomial::getNthCoeff -- The polynomial is not of the correct degree \n";
      return 0;
    }
    else {
      return((*this)[i]);
    }
  }

};






class PolynomeFit {
 public:
  int m_degree;
  Matrix m_coef;
  MVector m_y;
  void addPoint(double x, double y, double weight=1.0) {
    for(int i=0;i<=m_degree;i++) {
      for(int j=0;j<=m_degree;j++) 
	m_coef(i,j) += weight*pow(x,j+i);
      m_y[i]+=weight*y*pow(x,i);
    }
 
  }
  void getCurve(Polynomial& curve) const {
    Matrix inv(m_degree+1,m_degree+1);
    inv.storeInverse(m_coef);
    curve.storeMatrixProduct(inv,m_y); 
  }
  ostream& writeCoef(ostream& os) const {
    for(int i=0;i<=m_degree;i++) {
      for(int j=0;j<=m_degree;j++) {
	os << m_coef(i,j) << " ";
      }
      os << " | " << m_y[i] << endl;
    }
  }
  PolynomeFit(int degree) 
    : m_degree(degree), m_coef(degree+1,degree+1), m_y(degree+1) {}
};
