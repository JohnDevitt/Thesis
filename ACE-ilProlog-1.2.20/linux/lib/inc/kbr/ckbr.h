

#ifndef CKBR_H_INC
#define CKBR_H_INC

#include <math.h>
#include "IdMan.h"

#define KBR_NUM_MODE_STD 0
#define KBR_NUM_MODE_LIN 1
#define KBR_NUM_MODE_CHOL 2
#define KBR_NUM_MODE_QR 3
#define KBR_NUM_MODE_QR_CHOL 4
#define KBR_NUM_MODE_QR_FAST 5

    
const int default_num_mode = KBR_NUM_MODE_STD;

class MatrixIndexing {
 public:
  inline static int ut_index(int i, int j) {
    return (j*(j+1)/2)+i;}
  inline static int sl_index(int i, int j) {
    return (j*(j+1)/2)+i;}
  inline static int index(int i,int j) {
    if(i>j) return (i*(i+1)/2)+j; else return (j*(j+1)/2)+i;}
};

const int safe_code=0;
inline void panic() {}


   
class ldvect {
 private:
  vector<long double> data;
 public:
  int checknan() {
    if(safe_code)
      for(int i=0;i<data.size();i++)
	if(isnan(data[i])) {
	  cerr << "NAN at " << i << " in check\n";
	  return 1;
	}
    return 0;
  }
  void push_back(long double d) {data.push_back(d);}
  long double& operator[](int i) {
    if(safe_code)
      if((i<0)||(i>=data.size())) {
	cerr << "ldvect operator[] out of range:" << i << ">=" 
	     << data.size() << endl; 
	panic();
      }
    return data[i];
    }
  void put(int i, long double d) {
    if(safe_code) {
      if(isnan(d)) cerr << "ldvect put i=" << i << ",d="<<d<< " a NAN\n";
    }
    data[i]=d;
  }
  const long double operator[](int i) const {
    if(safe_code)
      if((i<0)||(i>=data.size())) {
	cerr << "ldvect const operator[] out of range:" << i << ">=" 
	     << data.size() << endl; 
	panic();
      }
    return data[i];
  }
 void resize(int s) {data.resize(s);}
 void resize(int s, double d) {data.resize(s,d);}
 int size() const {return data.size();}
};

class ldmatrix {
 private:
  int m_cols;
  int m_decl_cols;
  int m_rows;
  vector<long double> m_data;
 public:
  int cols() const {return m_decl_cols;}
  int rows() const {return m_rows;}
  int checknan() {
    if(safe_code)
      for(int r=0;r<m_rows;r++)
	for(int c=0;c<m_decl_cols;c++)
	  if(isnan(m_data[r*m_cols+c])) {
	    cerr << "NAN at " << r << "," << c << " in check\n";
	    return 1;
	  }
    return 0;
  }
  void check_rc(int r,int c) const {
    if(r<0) {cerr << "ldmatrix::r="<<r<<"<0 (with c="<<c<<"\n";panic();}
    if(r>=m_rows) {cerr << "ldmatrix::r="<<r<<">=m_rows="<<m_rows
			<<" (c="<<c<<"\n";panic();}
    if(c<0) {cerr << "ldmatrix::c<0\n";panic();}
    if(c>=m_cols) {cerr << "ldmatrix::c>=m_cols\n";panic();}
  }
  inline long double& get(int r, int c) {
    if(safe_code) check_rc(r,c);
    return m_data[r*m_cols+c];
  }
  inline long double get(int r, int c) const {
    if(safe_code) check_rc(r,c);
    return m_data[r*m_cols+c];
  }
  inline void put(int r, int c, long double v) {
    if(safe_code) check_rc(r,c);
    m_data[r*m_cols+c]=v;
  }
  int cellCount() const {return m_data.size();}
  void resize(int cols,int rows, long double fill=0.0);
  ldmatrix() : m_cols(0), m_decl_cols(0), m_rows(0) {}
};

class safe_int_vect {
 private:
  vector<int> data;
 public:
  void push_back(int d) {data.push_back(d);}
  inline int& operator[](int i) {
    if(safe_code)
      if((i<0)||(i>=data.size())) {
	cerr << "ldvect operator[] out of range:" << i << ">=" 
	     << data.size() << endl; 
	panic();
      }
    return data[i];
  }
  inline const int operator[](int i) const {
    if(safe_code)
      if((i<0)||(i>=data.size())) {
	cerr << "ldvect const operator[] out of range:" << i << ">=" 
	     << data.size() << endl; 
	panic();
      }
    return data[i];
  }
  void clear() {data.clear();}
  void resize(int s) {data.resize(s);}
  void resize(int s, int d) {data.resize(s,d);}
  int size() const {return data.size();}
};

class QRBase : public MatrixIndexing {
 private:
  int m_num_mode;
  ldvect U;        
       
        
      
     
  vector<ldvect*> P;         
     
 public:
  const int *m_dim;
  long double& u(int i, int j) {
    if(safe_code) {
      if(i>j) {cerr << "u(i,j) with i>j\n";panic();}
      if(ut_index(i,j)>=U.size()) {cerr << "u(i,j) out of range\n";panic();}
    }
    return U[ut_index(i,j)];
  }
  long double& p(int i, int j) {
    if(safe_code) {
      if(i>j) {cerr << "p(i,j) with i>j\n";panic();}
      if(j>=P.size()) {cerr << "j>=Prows.size()\n";panic();}
      if(i>=P[j]->size()) {cerr << "p(i,j) out of range\n";panic();}
    }
     
    return (*(P[j]))[i];
  }
  long double p(int i, int j) const {
    if(safe_code) {
      if(j>i) {cerr << "p(i,j) with i>j\n";panic();}
      if(j>=P.size()) {cerr << "j>=Prows.size()\n";panic();}
    }
    if(i>=P[j]->size())
      return 0.0;
    return (*(P[j]))[i];
  }
  int uSize() const {return U.size();}
  int pSize() const {return P.size();}
  const ldvect* m_kernel_val;
  ldvect m_fast_kernel_val;
  safe_int_vect m_kern_idx;
  long double min_norm_for_new;
  
  safe_int_vect indep;         
            
    
  safe_int_vect order;         
  long double decomp0(int n, long double *n_decomp);   
  long double decomp0_p(int n);   
  long double decomp0_pf(int n, int& m);   
  void decomp(int n);          
  int decomp_fast(int n, int& m); 
           
  QRBase(int nm, const ldvect& kernel_val, const int *dim) 
    : m_num_mode(nm), m_dim(dim), m_kernel_val(&kernel_val), 
	min_norm_for_new(0.0001) {}
};

class QR_Chol : public QRBase {
 public:
  ldvect *S;
  ldvect m_S[2];
  int currs;
  int S_dim;
  QR_Chol(int nm,const ldvect& kernel_val, const int *dim) 
    : QRBase(nm,kernel_val, dim), S(&(m_S[0])), currs(0), S_dim(0) {}
};

class QR_qr : public QRBase {
 public:
  ldmatrix Q;
  ldmatrix R;
   
   
  long double qr_fast_y;
  int qr_cols;
  void do_qr(int n, long double value);
  void do_qr_fast(int n, int m, long double value);
  long double& q(int i, int j) {
    if(safe_code) {
      if(i>=*m_dim) {cerr << "q(i,j) with i>=m_dim\n";panic();}
      if(j>qr_cols) {
	cerr << "q(i,j) with j="<<j<<">qr_cols"<<qr_cols<<"\n";panic();}
    }
    return Q.get(i,j);
  }
  long double& r(int i, int j) { 
    if(safe_code) {
      if(i>qr_cols) {cerr << "r(i,j) with i>qr_cols\n";panic();}
      if(j>=qr_cols) {cerr << "r(i,j) with j>=qr_cols\n";panic();}
    }
    return R.get(i,j);
  }
  QR_qr(int nm,const ldvect& kernel_val, const int *dim) 
    : QRBase(nm,kernel_val,dim),qr_cols(0),qr_fast_y(0.0) {
    Q.resize(1,1,0.0);
  }
};

class CKBRTheory : public MatrixIndexing {
 private:
  typedef struct {
    ldvect S;
    vector<int> Sindep; 
    int Sdim;
  } Chol;
   
  IdMan m_id_man;
  int m_dim;
  int m_size;
  int m_num_mode;
  QR_Chol m_qrc;
  Chol m_chol;
  QR_qr m_qrqr;
  ldvect m_kernel_val;
  ldvect m_kernel_inv;
   
  safe_int_vect m_ex_active;
 
  safe_int_vect m_val_count;
  ldvect m_val_sy;
  ldvect m_val_sx;
  ldvect m_val_sxy;
  ldvect m_val_syy;
  ldvect m_val_sxx;
  ldvect m_value;
  int getExCount() const {
    return m_id_man.idCount();}
  void compute_m_mu_cni_k(int n,long double *m,long double *cni_k,long double *mu) const;
 public:
  void getBaseExamples(safe_int_vect& base) {
    if((m_num_mode==KBR_NUM_MODE_QR)||(m_num_mode==KBR_NUM_MODE_QR_FAST)) 
      base=m_qrqr.indep;
    else {
      base.clear();
      for(int i=0;i<m_dim;i++) base.push_back(i);
    }
  }
  int addElement(long double val);
  void addElement(long double val, int *id) {
    *id=addElement(val);}
  int finalizeAdd();
  void retractAdd(int id);
  void delElement(int id);
  void storeKernelValue(int i,int j,double val) { 
    if((m_num_mode==KBR_NUM_MODE_QR_FAST)) {
        
      m_qrqr.m_fast_kernel_val[min(i,j)]=val;
      return;
    }
        
    if( (i==0)||(j==0) ){
                          
    }
          
	              
	  
	  
    m_kernel_val[index(i,j)]=val;
  }
  void getKernelValue(int i,int j,double *dist) {
    *dist=m_kernel_val[index(i,j)];}
  int isActive(int i) const {
    return m_ex_active[i];}
  void maxPredict(int id,double *value);
  void probPredict(int id,double value,double *prob);
  void getDim(int *dim) const {
    *dim=m_dim;}
  void getValue(int id,double *value) {
    *value=m_value[id];}
  ostream& Print(ostream& os) const;
  CKBRTheory() : m_num_mode(default_num_mode),
    m_dim(0), m_size(0),
    m_qrc(default_num_mode,m_kernel_val, &m_dim), 
    m_qrqr(default_num_mode,m_kernel_val, &m_dim) { 
     }
};

#endif
