
#ifndef C_PROLOG_ARRAY_H
#define C_PROLOG_ARRAY_H

#include <vector>
typedef unsigned int CPArrayIdx;

#define CPARRAY_MAX_DEPTH 5

class CPrologArray {
protected:
	CPArrayIdx m_Size;
	CPArrayIdx m_Branch;
	int m_Depth;
	vector<CPArrayIdx> m_FreeList;
public:
	G_SO GAPIC_term_ref create();
	G_SO CPArrayIdx add(GAPIC_term_ref elem, GAPIC_term_ref scratch, GAPIC_term_ref array);
	G_SO void get(CPArrayIdx i, GAPIC_term_ref array, GAPIC_term_ref elem);
	G_SO void set(CPArrayIdx idx, GAPIC_term_ref array, GAPIC_term_ref scratch, GAPIC_term_ref elem);
	G_SO void remove(CPArrayIdx i);
	G_SO void removePut(CPArrayIdx i, GAPIC_term_ref array, GAPIC_term_ref s1, GAPIC_term_ref s2);
	G_SO void removePut(CPArrayIdx i, GAPIC_term_ref array, GAPIC_term_ref* scr);
	G_SO void printSizeInfo(ostream& os) const;
	inline int getDepth() const { return m_Depth; };
	inline int getSize() const { return m_Size; };
	inline int getBranch() const { return m_Branch; };
	inline int getFreeSize() const { return m_FreeList.size(); }
};

class CPrologVector {
 private:
  static const unsigned s_default_branch;
  static const unsigned s_default_depth;
  static const GAPIC_atom *s_empty_element;
  unsigned m_branch;
  unsigned m_depth;
  GAPIC_functor m_functor;
  unsigned m_size;
         
  void expand(GAPIC_term_ref pa, unsigned size); 
         
  void shrink(GAPIC_term_ref pa, unsigned size);
  GAPIC_term_ref navigate(GAPIC_term_ref pa, unsigned idx, unsigned *low_idx) const;
 public:
  unsigned size() const {return m_size;}
  void set(GAPIC_term_ref pa, unsigned idx, GAPIC_term_ref item);
  void set_empty(GAPIC_term_ref pa, unsigned idx);
  GAPIC_term_ref get(GAPIC_term_ref pa, unsigned idx) const;
  void get(GAPIC_term_ref pa, unsigned idx, GAPIC_term_ref item) const;
  GAPIC_term_ref get_not_empty(GAPIC_term_ref pa, unsigned idx, 
			       int *not_empty) const;
  int is_empty(GAPIC_term_ref pa, unsigned idx) const;
  void resize(GAPIC_term_ref pa, unsigned size);
  CPrologVector(GAPIC_term_ref pa, 
		unsigned branch = s_default_branch, 
		unsigned depth = s_default_depth);
  static void init_module();
};

#endif
