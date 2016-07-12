

#ifndef RIB_INC
#define RIB_INC



 

#include <vector>
#include "IdMan.h"
#include <gapic/gapicgc.h>
#include <gapic/int_array_eff.h>
 
#ifdef DEBUG_VECTOR
#include <debug/vector>
#define vector __gnu_debug_def::vector
#endif

typedef double RibValue;


  
 
    
   
 
       
         
         
     



class RibExStats {
 public:
  double D;
  double SD;
  double V;
  double A;
  RibExStats() : A(0.0), V(0.0), SD(0.0), D(0.0) {}
};

class RibMatrix {
 private:
  typedef double RibDistance;
  IdMan m_id_man;
  int m_dim;
  int m_size;
  int m_safe_lowerbound;
  bool m_extend;
  int m_number_of_queries;
  double m_min_error_diff;
  int m_min_sample_size;
  vector<vector<bool> > m_query_results;
  vector<RibDistance> m_dist;
  vector<int> m_ex_active;
  vector<RibValue> m_value;
  vector<RibExStats> m_ex_stats;
  
  int index(int i,int j) const {
    if(i>j) return (i*(i+1)/2)+j; else return (j*(j+1)/2)+i;}
  double m_max_diff;
  double m_max_ln_diff;
  void updateExStats(int i,int j,double dist,double diff);
  int getExCount() const {
    return m_id_man.idCount();}
  void predictDepends(int id,int query,double *value);
 public:
	void getStats(GAPIC_int* excount) const {

    *excount=getExCount();}
  int addElement(RibValue val);
	void addElement(GAPIC_float val, GAPIC_int* id) {

    *id=addElement(val);}
	void delElement(GAPIC_int id);

	void storeDistance(GAPIC_int i, GAPIC_int j, GAPIC_float dist);

	void buildQueryMatrix(GAPIC_int number);

	void addTniSettings(GAPIC_float errdiff, GAPIC_int samsize) {

    m_min_error_diff=errdiff; m_min_sample_size=samsize; }
	void addQueryResults(GAPIC_int i, GAPIC_int index, GAPIC_term_ref queryresults);

	GAPIC_result splitWanted(GAPIC_int* i);

	void split(GAPIC_int qid, GAPIC_term_ref newRIB, GAPIC_term_ref yesids, GAPIC_term_ref noids) const;

	void getDistance(GAPIC_int i, GAPIC_int j, GAPIC_float* dist) {

    *dist=m_dist[index(i,j)];}
	GAPIC_result isActive(GAPIC_int i) const {

    if(m_ex_active[i]) return GAPIC_SUCCESS; else return GAPIC_FAIL;}
	void predict(GAPIC_int id, GAPIC_float* value);

	void getDim(GAPIC_int* dim) const {

    *dim=m_dim;}
	void testDiff(GAPIC_int id, GAPIC_term_ref tobedeleted) const;

	void setMaxDiff(GAPIC_float mdiff) {

    m_max_diff=mdiff;}
	void setMaxLnDiff(GAPIC_float mdiff) {

    m_max_ln_diff=mdiff;}
	void getValue(GAPIC_int id, GAPIC_float* value) {

    *value=m_value[id];}
	void getSafeLowerbound(GAPIC_int* slb) const {

    *slb=m_safe_lowerbound;}
	void setSafeLowerbound(GAPIC_int slb) {

    m_safe_lowerbound=slb;}
  ostream& Print(ostream& os) const;
  RibMatrix() : m_dim(0), m_size(0), 
    m_safe_lowerbound(1), m_max_diff(-1), m_number_of_queries(-1) {}
  RibMatrix(IdMan id_man,int dim,int size,int safe_lowerbound,int number_of_queries,
            vector<vector<bool> > query_results,vector<RibDistance> dist,
            vector<int> ex_active,vector<RibValue> value,vector<RibExStats> ex_stats,
	    double max_diff, double max_ln_diff, double min_error_diff, int min_sample_size);
};

extern gapic_type GapicRibMatrixType;
typedef GapicNonExMutCollectable<RibMatrix,&GapicRibMatrixType> 
	RibMatrixCollectable;

#endif
