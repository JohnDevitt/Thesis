
#ifndef IDOIC_INC
#define IDOIC_INC




#include "../../math/math.h"
#include "../../gapic/gapicgc.h"
 

const int safe_ido_code=1;
const int trace_calc_poly=0;

class PatDayPolyData {
 private:
  static const double val_unknown=-1.0e-100;
  static const double unknown_cmp=-0.9e-100; 
  double m_value;
  double m_pred;
  double m_diff1;
 public:
  void set_unknown() {m_value=val_unknown;}
  int is_unknown() const {return m_value<unknown_cmp;}
  void put_value(double val) {  
    if(safe_ido_code)
      if(!is_unknown()) {
	cerr << "PatDayPolyData::put_value : pat/day has already a value\n";
	 
      }
    m_value=val;
  }
  double get_value() const {
    if(safe_ido_code)
      if(is_unknown()) {
	cerr << "PatDayPolyData::get_value : accessing unknown value\n";
	(*((int *)NULL))=0;
      }
    return m_value;
  }
  void put_pred(double pred) {m_pred=pred;}
  int has_pred() const {return m_pred>unknown_cmp;}
  double get_pred() const {
    if(safe_ido_code)
      if(!has_pred()) 
	cerr << "PatDayPolyData::get_pred : no prediction for this pat/day\n";
    return m_pred;
  }
  void put_diff1(double diff1) {m_diff1=diff1;}
  double get_diff1() const {return m_diff1;}
  PatDayPolyData() : m_value(val_unknown), m_pred(val_unknown), m_diff1(0.0) {}
};




class IdoPolyData;

class PatPolyData {
 private:
  IdoPolyData *m_parent;
  vector<PatDayPolyData> days;
  PatDayPolyData* get_day(int i) {
    if(safe_ido_code) 
      if((i<0)||(i>10000)) {
	cerr<<"PatPolyData::get_day: i (=" << i << ") out of range\n";
	return NULL;
      }
    if(days.size()<=i) days.resize(i+1);
    return &(days[i]);
  }
  const PatDayPolyData* get_day(int i) const {
    if(safe_ido_code) 
      if((i<0)||(i>=days.size())) {
	cerr<<"PatPolyData::get_day: i (=" << i << ") out of range\n";
	return NULL;
      }
    return &(days[i]);
  }
  void solve(const MVector& coef, const MVector& y, MVector& a);
 public:
  int size() const {return days.size();}
  int day_in_range(int day) const {
    return (day>=0)&&(day<size());
  }
  void put_value(int day, double value);
  int is_unknown(int day) const {
    return get_day(day)->is_unknown();
  }
  int is_unknown_or_out_icu(int day) const {
    if(!day_in_range(day)) return 1;
    return is_unknown(day);
  }
  double get_value(int day) const {
    return get_day(day)->get_value();
  }
  int has_pred(int day) const {
    return get_day(day)->has_pred();
  }
  int has_pred_and_icu(int day) const {
    if(!day_in_range(day)) return 0;
    return has_pred(day);
  }
  double get_pred(int day) const {
    return get_day(day)->get_pred();
  }
  double get_diff1(int day) const {
    return get_day(day)->get_diff1();
  } 
  int get_length_stay() const {
    return days.size();
  }
  void calc_poly();
  PatPolyData(IdoPolyData *parent) : m_parent(parent) {}
};


class IdoPolyData {
 private:
  vector<PatPolyData> patients;
  int m_poly_degree;
  double m_decay;
  vector<double> m_default_value;
  int m_predict_dist;
  double m_valstats_sum;
  double m_valstats_ss;
  int m_valstats_count;
  double m_global_default_value;
  PatPolyData* get_pat(int i) {
    if(safe_ido_code) 
      if((i<0)||(i>10000)) {
	cerr<<"ido_poly_data::get_pat: i out of range\n";
	return NULL;
      }
    if(patients.size()<=i) patients.resize(i+1,PatPolyData(this));
    return &(patients[i]);
  }
  const PatPolyData* get_pat(int i) const {
    if(safe_ido_code) 
      if((i<0)||(i>=patients.size())) {
	cerr<<"ido_poly_data::get_pat: i out of range\n";
	return NULL;
      }
    return &(patients[i]);
  }
  void calc_default_values() {
    m_default_value.resize(get_degree()+1);
    for(int k=0;k<get_degree()+1;k++) {
      double s=0.0;
      int n=0;
      for(int i=0;i<patients.size();i++)
	if(!patients[i].is_unknown_or_out_icu(k)) {
	  n++;
	  s+=patients[i].get_value(k);
	}
      if(n==0) {
	cerr << "PatPolyData::calc_default_values : no patients on day " 
	     << k << endl;
	m_default_value[k]=0.0;
      }
      else m_default_value[k]=s/n;
    }
    double s=0.0;
    int n=0;
    for(int i=0;i<patients.size();i++)
      for(int k=0;k<patients[i].get_length_stay();k++) 
	if(!patients[i].is_unknown(k)) {
	  n++;
	  s+=patients[i].get_value(k);
	}
    if(n==0) {
      cerr << "PatPolyData::calc_default_values : no patients at all \n";
      m_global_default_value=0.0;
    }
    else m_global_default_value=s/n;
  }
  void get_pred_stats(int firstday, double& error, 
		      double& default_error, int& count) const {
    error=0.0;
    default_error=0.0;
    count=0;
    firstday-=m_predict_dist;
    if(firstday<0)firstday=0;
    for(int i=0;i<patients.size();i++)
      for(int k=firstday;k<patients[i].get_length_stay();k++)
	if(patients[i].has_pred(k)) 
	  if(!patients[i].is_unknown_or_out_icu(k+m_predict_dist)){
	    count++;
	    double diff=patients[i].get_value(k+m_predict_dist)-patients[i].get_pred(k);
	    error+=diff*diff;
	    	       
	        
	       
	                              
	    
	    diff=patients[i].get_value(k+m_predict_dist)-m_global_default_value;
	    default_error+=diff*diff;
	  }
    	                
    	              
  }
  void print_pred_stats(int firstday) const {
    double error;
    double default_error;
    int count;
    get_pred_stats(firstday,error,default_error,count);
    double avgMSE=error/count;
    double default_avgMSE=default_error/count;
    cout << "Prediction stats\n";
    cout << "avg MSE         = " << avgMSE << endl;
    cout << "default avg MSE = " << default_avgMSE << endl;
    cout << "varienca        = " << m_valstats_ss/m_valstats_count << endl;
    cout << "Relative MSE = " << avgMSE/default_avgMSE << endl;
  }
 public:
  void valstats_add(double val) {
    m_valstats_count++;
    m_valstats_sum+=val;
    m_valstats_ss+=val*val;
  }
  double valstats_mean() const {
    return m_valstats_sum/m_valstats_count;
  }
  double valstats_var() const {
    return m_valstats_ss-(valstats_mean()*valstats_mean());
  }
  int get_degree() const {return m_poly_degree;}
  double get_decay() const {return m_decay;}
  double get_default_value(int k) const {return m_default_value[k];}
  int get_predict_dist() const {return m_predict_dist;}
  ostream& Print(ostream& os) const {
    return os << "[IdoPolyData:#pat="<<patients.size()<<"]";
  }
	void put_value(GAPIC_int pat, GAPIC_int pday, GAPIC_float val) {

    get_pat(pat)->put_value(pday-1,val);
  }
	void set_predict_dist(GAPIC_int dist){

    m_predict_dist=dist;
  }
	void calc_poly() {

    calc_default_values();
    for(int i=0;i<patients.size();i++) {
      if(trace_calc_poly) {
	cerr << "calc_poly("<<i<<"): ";
	for(int k=0;k<patients[i].get_length_stay();k++) {
	  if(patients[i].is_unknown(k))
	    cerr << "?";
	  else cerr << ".";
	}
	cerr << endl;
      }
      patients[i].calc_poly();
      if(trace_calc_poly) {
	cerr << "calc_poly("<<i<<"): ";
	for(int k=0;k<patients[i].get_length_stay();k++) {
	  if(patients[i].has_pred(k))
	    cerr << "+";
	  else cerr << "-";
	}
	cerr << endl;
      }
    }
  }
	void get_value(GAPIC_int pat, GAPIC_int pday, GAPIC_float* val) const {

    *val=get_pat(pat)->get_value(pday-1);
  }
	GAPIC_result get_value_or_fail(GAPIC_int pat, GAPIC_int pday, GAPIC_float* val) const {

    if(!get_pat(pat)->is_unknown(pday-1)) {
      *val=get_pat(pat)->get_value(pday-1);
      return GAPIC_SUCCESS;
    }
    return GAPIC_FAIL;
  }
	void get_pred(GAPIC_int pat, GAPIC_int pday, GAPIC_float* pred) const {

    *pred=get_pat(pat)->get_pred(pday-1);
  }
	GAPIC_result get_pred_or_fail(GAPIC_int pat, GAPIC_int pday, GAPIC_float* pred) const {

    if(get_pat(pat)->has_pred(pday-1)) {
      *pred=get_pat(pat)->get_pred(pday-1);
      return GAPIC_SUCCESS;
    }
    return GAPIC_FAIL;
  }
	void get_diff1(GAPIC_int pat, GAPIC_int pday, GAPIC_float* diff1) const {

    *diff1=get_pat(pat)->get_diff1(pday-1);
  }
	GAPIC_result get_diff1_or_fail(GAPIC_int pat, GAPIC_int pday, GAPIC_float* diff1) const {

    if(get_pat(pat)->has_pred(pday-1)) {
      *diff1=get_pat(pat)->get_diff1(pday-1);
      return  GAPIC_SUCCESS;
    }
    return GAPIC_FAIL;
  }
	void get_length_stay(GAPIC_int pat, GAPIC_int* len) const {

    *len=get_pat(pat)->get_length_stay();
  }
	void print_pred_stats() const {

    print_pred_stats(get_degree()+1);
  }
  IdoPolyData() : m_poly_degree(1), m_decay(0.92), m_predict_dist(1),
    m_valstats_sum(0.0), m_valstats_ss(0.0), m_valstats_count(0) {}
};

const int debug_calc_poly=0;
const int debug_solve=0;

inline void PatPolyData::put_value(int day, double value) {
  m_parent->valstats_add(value);
  get_day(day)->put_value(value);
}

inline void PatPolyData::calc_poly() {
  if(debug_calc_poly) cerr << "calc_poly:enter\n";
  int degree=m_parent->get_degree();
  if(debug_calc_poly) cerr << "calc_poly:degree="<<degree<<"\n";
  for(int i=0;i<degree+1;i++) 
    if(i<size()) {
        
      days[i].put_pred(m_parent->get_default_value(i));
      days[i].put_diff1(0.0);
    }
  if(debug_calc_poly) cerr << "calc_poly:*1\n";
  int start=0;
  while(is_unknown_or_out_icu(start)&&(start<size()))
    start++;
  if(size()>degree+1+start) {     
    double decay=m_parent->get_decay();
    if(debug_calc_poly) cerr << "calc_poly:*a\n";
    MVector coef(2*degree+1);
    if(debug_calc_poly) cerr << "calc_poly:*b\n";
    MVector y(degree+1);
    if(debug_calc_poly) cerr << "calc_poly:*c\n";
    for(int i=0;i<degree+1;i++) {
      y[i]=0.0;
      for(int k=0;k<degree+1;k++) {
	double x=k;
	double theval;
	if(is_unknown(k)&&(k>0)&&(!is_unknown(k-1)))
	  theval=days[start+k-1].get_value();
	else
	  theval=days[start+k].get_value();
	if(debug_solve) if(!i) cerr << "add:" << theval << endl;
	y[i] = (y[i]*decay) + theval*pow(x,i);
      }
    }
    if(debug_calc_poly) cerr << "calc_poly:*2\n";
    for(int j=0;j<2*degree+1;j++) {
      coef[j]=0.0;
      for(int k=0;k<degree+1;k++) {
	double x=k;
	coef[j] = (coef[j]*decay) + pow(x,j);
      }
    }
    if(debug_calc_poly) cerr << "calc_poly:*3\n";
    Polynomial a(degree+1);
    solve(coef,y,a);
      
    days[start+degree+1].put_pred(a.apply(degree+1.0));
    days[start+degree+1].put_diff1(a.nthDiffAtX(1,degree+1.0));
    for(int k=start+degree+2;k<size();k++) {
      double x=k;
      double theval;
      if((is_unknown(k))&&(k>0)&&(!is_unknown(k-1)))
	theval=days[k-1].get_value();
      else
	theval=days[k].get_value();
      if(debug_calc_poly) cerr << "add:" << theval << endl;
      for(int i=0;i<degree+1;i++) 
	y[i] = (y[i]*decay) + theval*pow(x,i);	
      for(int j=0;j<2*degree+1;j++) 
	coef[j] = (coef[j]*decay) + pow(x,j);  
      solve(coef,y,a);
        
      days[k].put_pred(a.apply(x+m_parent->get_predict_dist()));
      days[k].put_diff1(a.nthDiffAtX(1,x+m_parent->get_predict_dist()));
    }
  }
  if(debug_calc_poly) cerr << "calc_poly:done\n";
}

inline void PatPolyData::solve(const MVector& coef,
			       const MVector& y, 
			       MVector& a) {
  int degree=m_parent->get_degree();
  double C[degree+1][degree+1];
  for(int i=0;i<degree+1;i++) 
    for(int j=0;j<degree+1;j++)
      C[i][j]=coef[i+j];
  double Y[degree+1];
  for(int i=0;i<degree+1;i++)
    Y[i]=y[i];
  int rows[degree+1];
  int cols[degree+1];
  for(int k=0;k<degree+1;k++)
    rows[k]=cols[k]=k;
  if(debug_solve) {
    for(int i=0;i<degree+1;i++) {
      for(int j=0;j<degree+1;j++)
	cerr << C[i][j] << " ";
      cerr << "*a= " << Y[i] << endl;
    }
    cerr << "rows=";
    for(int i=0;i<degree+1;i++) 
      cerr << rows[i] << " ";
    cerr << "\ncols=";
    for(int i=0;i<degree+1;i++) 
      cerr << cols[i] << " ";
    cerr << endl;
  }
  for(int k=0;k<degree+1;k++) {
    double m=-1.0;
    int maxi=-1;
    int maxj=-1;
    for(int i=k;i<degree+1;i++)
      for(int j=k;j<degree+1;j++) {
	double acij=fabs(C[rows[i]][cols[j]]);
	if(acij>m) {maxi=i;maxj=j;m=acij;}
      }
    int tr=rows[maxi];
    rows[maxi]=rows[k];
    rows[k]=tr;
    int tc=cols[maxj];
    cols[maxj]=cols[k];
    cols[k]=tc;
  
    for(int i=0;i<degree+1;i++) 
      if(i!=k) {
	int ri=rows[i];
	double f=-C[ri][tc]/C[tr][tc];
	for(int j=k;j<degree+1;j++)
	  C[ri][cols[j]]+=C[tr][cols[j]]*f;
	Y[ri]+=Y[tr]*f;
      }
    if(debug_solve) {
      for(int i=0;i<degree+1;i++) {
	for(int j=0;j<degree+1;j++)
	  cerr << C[i][j] << " ";
	cerr << "*a= " << Y[i] << endl;
      }
      cerr << "rows=";
      for(int i=0;i<degree+1;i++) 
	cerr << rows[i] << " ";
      cerr << "\ncols=";
      for(int i=0;i<degree+1;i++) 
	cerr << cols[i] << " ";
      cerr << endl;
    }
  }
  for(int k=0;k<degree+1;k++)
    a[cols[k]]=Y[rows[k]]/C[rows[k]][cols[k]];
  if(debug_solve) { 
    cerr << "solve\ncoef=" << coef << "\ny=" << y << "\na=" << a << endl;
    cerr << "delta=";
    for(int k=0;k<degree+1;k++) {
      double check=0.0;
      for(int i=0;i<degree+1;i++) 
	check += a[i]*coef[i+k];
      cerr << (check-y[k]) << " ";
    }
    cerr << endl;
  }
}

extern gapic_type GapicIdoPolyDataType;
typedef GapicNonExMutCollectable<IdoPolyData,&GapicIdoPolyDataType> 
	IdoPolyDataCollectable;

#endif
