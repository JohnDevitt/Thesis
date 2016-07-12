

using namespace std;
#include <vector>
#include <iostream>
 
#include "../matrix/QR.h"



   

class KernExample {
 public: 
  virtual ostream& write(ostream& os) const;
  KernExample() {}
  virtual ~KernExample();
};

class Kernel {
 private:
 public: 
  virtual int is_trivial() const;
  virtual ostream& write(ostream& os) const=0;
  virtual double compute(const KernExample& a, const KernExample& b) const=0;
  virtual double compute2(const KernExample& a, const KernExample& b) const=0;
  virtual int goodExample(const KernExample& a) const=0;
              
             
  Kernel() {}
  virtual ~Kernel();
};

class KernExampleLabeled;

class KernDatabase : public vector<const KernExampleLabeled*> {

 public:
  void addOneExample(const KernExampleLabeled* exlab);


};

class KernModel {
 public:
  virtual ostream& write(ostream& os) const=0;
  virtual double predict(const KernExample& a) const=0;
         
  virtual ~KernModel();
};

class KernOptimizationMethod {
 public:
  virtual ostream& write(ostream& os) const=0;
  virtual KernModel *optimize(const Kernel& m_kernel, const KernDatabase& data) const=0;
  virtual ~KernOptimizationMethod();
};

   
   

class KernExampleLabeled {
 private:
  double m_label;
  const KernExample* m_ex;
 public:
  KernExampleLabeled() {}
  virtual ostream& write(ostream& os) const;
  double label() const {return m_label;}  
  const KernExample* example() const {return m_ex;}
  KernExampleLabeled& operator=(const KernExampleLabeled& src) {
    m_label=src.m_label;m_ex=src.m_ex;}
  KernExampleLabeled(double l, const KernExample* ex) : m_label(l), m_ex(ex) {}
  KernExampleLabeled(const KernExampleLabeled& src) 
    : m_label(src.m_label), m_ex(src.m_ex) {}
  virtual ~KernExampleLabeled();
};

class KernelLabeled {
 private:
  const Kernel *base;
  const KernExampleLabeled* castExample(const KernExample* a) const {
    return dynamic_cast<const KernExampleLabeled*>(a);}
 public:
  virtual ostream& write(ostream& os) const;
  virtual double compute(const KernExample& a, const KernExample& b) const;
  virtual int goodExample(const KernExample& a) const;
  KernelLabeled() {}
  virtual ~KernelLabeled();
};

   
     

class KernExampleVectorSpace : public KernExample {
 public:
  virtual void setNull()=0;
  virtual void scale(double f)=0;
  virtual void add(const KernExampleVectorSpace& a)=0;
  virtual void addScaled(const KernExampleVectorSpace& a, double f)=0;
  virtual void storeDotProduct(const MVector& v, const KernExampleVectorSpace& a)=0;
  virtual KernExampleVectorSpace *constructNull() const=0;
  KernExampleVectorSpace() {}
  ~KernExampleVectorSpace();
};

class KernExampleDoubleVector : public MVector, public KernExampleVectorSpace {
 public:
  virtual KernExampleDoubleVector* constructNull() const {
    return new KernExampleDoubleVector();
  }
  virtual void setNull();
  virtual void scale(double f);
  virtual void add(const KernExampleVectorSpace& a);
  virtual void addScaled(const KernExampleVectorSpace& a, double f);
  virtual void storeDotProduct(const MVector& v, const KernExampleVectorSpace& a);
  virtual ostream& write(ostream& os) const;
  KernExampleDoubleVector() {}
  virtual ~KernExampleDoubleVector();
};

class KernelDoubleVector : public Kernel {
 private:
  vector<double> m_weights;
  const KernExampleDoubleVector* castExample(const KernExample* a) const {
    return dynamic_cast<const KernExampleDoubleVector*>(a);
  }
 public:
  int defaultWeights() const {return m_weights.size()==0;}
  virtual ostream& write(ostream& os) const;
  virtual double compute(const KernExample& a, const KernExample& b) const;
  virtual double compute2(const KernExample& a, const KernExample& b) const;
  virtual int goodExample(const KernExample& a) const;
  KernelDoubleVector() {}
  virtual ~KernelDoubleVector();
};




    

  
          
     
        

  




class KernExampleFeatureCoordinates  {
 private:
  Matrix m_coor;
  void generateFeatureCoordinates(const Kernel& kernel, const KernDatabase& data,
				  Matrix& coor);
 public:
  Matrix coordinates() {return m_coor;}
  KernExampleFeatureCoordinates(const Kernel& kernel, const KernDatabase& data) 
    : m_coor(data.size(),data.size(),0.0) {
    generateFeatureCoordinates(kernel,data,m_coor);
  }

};


   
 
   
        
				   
 
     
        
       
    
  




    

class KernModelSupportVectors : public KernModel {
 private:
  vector<const KernExample*> m_supvectors;
  vector<double> m_alpha;
 public:

};


class KernOptSVM : public KernOptimizationMethod {
 public:
  virtual ostream& write(ostream& os) const;
  virtual KernModel *optimize(const Kernel& kernel, 
			      const KernDatabase& data) const;
  virtual ~KernOptSVM();
};


class KernModelCoordinateWeights : public KernModel {
 private:
  const Kernel* m_kernel;
  KernExampleVectorSpace *m_weights;
 public:
  virtual double predict(const KernExample& a) const;
  KernExampleVectorSpace*& weights() {return m_weights;}
  virtual ostream& write(ostream& os) const;
  KernModelCoordinateWeights(const Kernel* kernel) 
    : m_kernel(kernel), m_weights(NULL) {}
  ~KernModelCoordinateWeights();
};

class KernModelCoordinateWeightsQRUpdatable : public KernModelCoordinateWeights {
 private:
  QRfactorization* m_QRfact; 
 public:  
  QRfactorization* getQRfact() {return m_QRfact;}
  void updateAddOneExampleLabeled(const KernDatabase& data); 
  KernModelCoordinateWeightsQRUpdatable(const Kernel* kernel,
					QRfactorization* fact) 
    : KernModelCoordinateWeights(kernel), m_QRfact(fact) {}
  ~KernModelCoordinateWeightsQRUpdatable();
};

class KernOptimizationMethodInversion : public KernOptimizationMethod {
 public:
  virtual ostream& write(ostream& os) const;
  virtual KernModel *optimize(const Kernel& kernel, 
			      const KernDatabase& data) const;
  KernOptimizationMethodInversion() {}
  virtual ~KernOptimizationMethodInversion();
};

class KernOptimizationMethodSimpleQR : public KernOptimizationMethod {

 private:

 protected:

 public:

  virtual ostream& write(ostream& os) const;

  virtual KernModelCoordinateWeightsQRUpdatable *optimize(const Kernel& kernel, 
			      const KernDatabase& data) const;
  KernOptimizationMethodSimpleQR() {}
     
  virtual ~KernOptimizationMethodSimpleQR();
};

class KernOptimizationMethodQR : public KernOptimizationMethodSimpleQR{

 private:

 protected:

 public:

  virtual ostream& write(ostream& os) const;

  virtual KernModelCoordinateWeightsQRUpdatable *optimize(const Kernel& kernel, 
			      const KernDatabase& data) const=0;
  KernOptimizationMethodQR() {}
     
  virtual ~KernOptimizationMethodQR();
};
