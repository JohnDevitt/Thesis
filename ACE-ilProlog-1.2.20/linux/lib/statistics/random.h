#ifndef STATS_RANDOM
#define STATS_RANDOM

#include <stdlib.h>
#include <vector>





            

class RepeatableRandomSeq {
 public:
  enum {state_size=32};
 private:
  unsigned int m_seed;
  unsigned int m_idx;
  long int m_val;
  char m_state[state_size];
 public:
  long int get(unsigned int idx) {
    if(idx < m_idx) reset();
    if(m_idx == idx) return m_val;

    char *tmp = setstate(m_state);

    while(m_idx < idx) {
      m_val = random();
      m_idx++;
    }

    setstate(tmp);

    return m_val;
  }
  void reset() {
    m_idx = 0; 

    char *tmp = initstate(m_seed, m_state, state_size); 
    m_val = random();
    setstate(tmp);    



  }
  RepeatableRandomSeq() {m_seed=random();reset();}
  RepeatableRandomSeq(unsigned int seed) : m_seed(seed) {reset();}
};

class RandomPermutation {
 private:
  std::vector<unsigned> m_perm;
 public:
  void init(unsigned n) {
    m_perm.resize(n);
    m_perm[0]=0;
    for(unsigned i=1;i<n;i++) {
      unsigned j=random()%(i+1);
      m_perm[i]=m_perm[j];
      m_perm[j]=i;
    }
  }
  unsigned get(unsigned i) const {
    return m_perm[i];
  }
  RandomPermutation() {}
  RandomPermutation(unsigned n) {init(n);}
};

#endif
