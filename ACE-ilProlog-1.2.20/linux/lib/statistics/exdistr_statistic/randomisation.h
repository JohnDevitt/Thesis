




#ifndef RANDOMISATION_INC
#define RANDOMISATION_INC

#include <assert.h>
#include <ext/hash_map>
using namespace __gnu_cxx;

#include "../../gapic/gapicgc.h"

#include "../random.h"
#include "exdistr_stat.h"

extern gapic_type RandomisationESHType;
extern gapic_type RandomisationESType;

class RandomisationVector {
 private:
  unsigned m_min_ex_id;
  unsigned m_max_ex_id;
  unsigned m_rand_count;
  bool m_reg_done;
  vector<GAPIC_int> m_ex_ids;
  typedef hash_map<GAPIC_int, GAPIC_int, hash<GAPIC_int> > id2nr_hash;
  id2nr_hash m_id2nr;
  vector<GAPIC_float> m_best_query;                 
  vector<RandomPermutation> m_rand_list;
 public:
         
  void clear() {m_ex_ids.clear(); m_rand_list.clear(); m_id2nr.clear();}
             
  void register_ex(GAPIC_int ex_id) {
            
    if(m_reg_done) {clear();m_reg_done=false;}
    m_ex_ids.push_back(ex_id);
  }
         
  void finish_ex_registration();
         
  bool registration_ok() const {return !m_rand_list.empty();}
                
              
      
  GAPIC_int get_rnd(GAPIC_int rnd_idx, GAPIC_int ex_id) {
    id2nr_hash::const_iterator iter=m_id2nr.find(ex_id);
    assert(iter!=m_id2nr.end());
    return m_ex_ids[m_rand_list[rnd_idx].get(iter->second)];
  }
  void set_rand_count(unsigned rc) {m_rand_count=rc;clear();}
  RandomisationVector() {m_reg_done = true;} 
  void update_best_rnd_query(unsigned i, GAPIC_float heurval) {if (heurval>m_best_query[i]) m_best_query[i]=heurval;} 
  GAPIC_float get_heur_significance(GAPIC_float heurval);                   
};



class RandomisationES : public SingleES {
 private:
  ExdistrStatistic* m_orig_stat;
  std::vector<ExdistrStatistic*> m_rand_stats;
  RandomisationVector m_ex_rnd;
 public:
  unsigned rand_stat_count() const {return m_rand_stats.size();}
  const ExdistrStatistic* rand_stats(unsigned i)const {return m_rand_stats[i];}
  const ExdistrStatistic* orig_stat() const {return m_orig_stat;}
  ExdistrStatistic* rand_stats(unsigned i) {return m_rand_stats[i];}
  ExdistrStatistic* orig_stat() {return m_orig_stat;}
  virtual ~RandomisationES();
  RandomisationES(GAPIC_term_ref src) throw(GapicObjectCreationError);
  RandomisationES(const RandomisationES& src);
  void update_best_rnd_query(unsigned i, GAPIC_float heurval) {m_ex_rnd.update_best_rnd_query(i,heurval);} 
	void get_heur_significance(GAPIC_float heurval, GAPIC_float* conf) {

    *conf=m_ex_rnd.get_heur_significance(heurval);}
  virtual void resize(GAPIC_int n, GAPIC_int s);
  virtual void reset();
  virtual void add(const ExdistrStatistic* s);
  virtual void scale(GAPIC_float s);
  virtual void update(GAPIC_int query, GAPIC_float weight);
  virtual ExdistrStatistic* clone() const;
  virtual ostream& PrintStat(ostream& os) const;
  virtual void to_prolog(GAPIC_int idx, GAPIC_int query, GAPIC_term_ref prolog);
  virtual void rule_info(GAPIC_term_ref prolog);
  virtual void needs_ex_reg(GAPIC_int *yn);     
  virtual void register_ex(GAPIC_int exid);    
  virtual void finish_ex_reg();    
};


  
	    
		 
	
	  
	  
	    
	    	
	      
	      
	    
	    
	    

	    
  
  
  


class RandomisationESH : public ExsplitHeuristic, public SingleStatOwner {
 private:
  ExsplitHeuristic *base;  
  GAPIC_float m_Significance;
  int m_LocalRnd;         
 protected:
  template <class SpecClass> void rnd_esh_init(GAPIC_term_ref src) {
    GAPIC_term_ref rnd_t = GAPIC_new_term_ref();
    GAPIC_term_ref base_t = GAPIC_new_term_ref();
    GAPIC_get_arg(1,src,rnd_t);
    GAPIC_get_arg(2,src,base_t);
    base = new typename SpecClass::base_type(base_t);
    SingleStatOwner::initialize(rnd_t);
  }
 public:	
  virtual void calculate(ExdistrStatistic* pos_s, ExdistrStatistic* tot_s, GAPIC_int query);
  virtual ~RandomisationESH();
  RandomisationESH(GAPIC_term_ref src)    
    throw(GapicObjectCreationError);
};

#endif

