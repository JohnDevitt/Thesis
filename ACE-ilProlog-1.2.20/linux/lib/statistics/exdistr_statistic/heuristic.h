


#ifndef HEURISTIC_INC
#define HEURISTIC_INC

#include "statistic.h"

typedef struct {
	GAPIC_float Value;
	GAPIC_float SSPos, SSNeg, SSTot;
	bool Acceptable;
} HeuristicValue;

inline int operator<(const HeuristicValue& a, const HeuristicValue& b) {
  if(!b.Acceptable) return 0;
  if(!a.Acceptable) return 1;
  return a.Value < b.Value;
}


extern gapic_type ExsplitHeuristicType;

class ExsplitHeuristic {
public:
	HeuristicValue m_Value;
	int m_NbQueries;
public:
	virtual void announce_query2attr(GAPIC_int query, GAPIC_int attr, ExdistrStatistic* pos_s, ExdistrStatistic* tot_s);

	virtual void announce_best_test();

	virtual void announce_querytype(GAPIC_int query, GAPIC_int type, ExdistrStatistic* pos_s, ExdistrStatistic* tot_s);

	virtual void announce_regrvalues(GAPIC_int query, GAPIC_int example, GAPIC_float max, GAPIC_float min, GAPIC_float avg, ExdistrStatistic* pos_s, ExdistrStatistic* tot_s);

	virtual void announce_example_ids(GAPIC_term_ref ex_list, ExdistrStatistic* stat);

	void setNbQueries(GAPIC_int nb) {

	  m_NbQueries=nb;}
	virtual void calculate(ExdistrStatistic* pos_s, ExdistrStatistic* tot_s, GAPIC_int query) = 0;

	void getValue(GAPIC_term_ref value) const;

	virtual void globalAccept(GAPIC_term_ref value, GAPIC_int* acc);

	ostream& Print(ostream& os) const;
	inline const HeuristicValue* getData() const {
		return &m_Value;
	};
};

typedef ExsplitHeuristic* ESHPtr;

typedef GapicNonExMutCollectable<ExsplitHeuristic, &ExsplitHeuristicType> ExsplitHeuristicCollectable;

class SCompoundESH : public ExsplitHeuristic {
protected:
	ExsplitHeuristic* m_Child;
public:
	SCompoundESH();
	virtual ~SCompoundESH();
	virtual void announce_query2attr(GAPIC_int query, GAPIC_int attr, ExdistrStatistic* pos_s, ExdistrStatistic* tot_s);
	virtual void announce_best_test();
	virtual void announce_querytype(GAPIC_int query, GAPIC_int type, ExdistrStatistic* pos_s, ExdistrStatistic* tot_s);
	virtual void announce_regrvalues(GAPIC_int query, GAPIC_int example, GAPIC_float max, GAPIC_float min, GAPIC_float avg, ExdistrStatistic* pos_s, ExdistrStatistic* tot_s);
	virtual void announce_example_ids(GAPIC_term_ref exlist, ExdistrStatistic* stat);
	virtual void calculate(ExdistrStatistic* pos_s, ExdistrStatistic* tot_s, GAPIC_int query);
	void initialize(GAPIC_term_ref children);
};

class NCompoundESH : public ExsplitHeuristic {
protected:
	GAPIC_int m_NbChildren;
	ESHPtr* m_Children;
public:
	NCompoundESH();
	virtual ~NCompoundESH();
	virtual void announce_query2attr(GAPIC_int query, GAPIC_int attr, ExdistrStatistic* pos_s, ExdistrStatistic* tot_s);
	virtual void announce_best_test();
	virtual void announce_querytype(GAPIC_int query, GAPIC_int type, ExdistrStatistic* pos_s, ExdistrStatistic* tot_s);
	virtual void announce_regrvalues(GAPIC_int query, GAPIC_int example, GAPIC_float max, GAPIC_float min, GAPIC_float avg, ExdistrStatistic* pos_s, ExdistrStatistic* tot_s);
	virtual void announce_example_ids(GAPIC_term_ref ex_list, ExdistrStatistic* stat);
	virtual void calculate(ExdistrStatistic* pos_s, ExdistrStatistic* tot_s, GAPIC_int query);
	void initialize(GAPIC_term_ref children);
};

template <unsigned N> class ExsplitNStatOwnerHeuristic 
: public ExsplitHeuristic, protected NStatOwner<N> {
  virtual void announce_query2attr(GAPIC_int query, GAPIC_int attr, ExdistrStatistic* pos_s, ExdistrStatistic* tot_s);
  virtual void announce_best_test();
  virtual void announce_querytype(GAPIC_int query, GAPIC_int type, ExdistrStatistic* pos_s, ExdistrStatistic* tot_s);
  virtual void announce_regrvalues(GAPIC_int query, GAPIC_int example, GAPIC_float max, GAPIC_float min, GAPIC_float avg, ExdistrStatistic* pos_s, ExdistrStatistic* tot_s);
  virtual void announce_example_ids(GAPIC_term_ref exlist, ExdistrStatistic* stat);
};
template <unsigned N> void ExsplitNStatOwnerHeuristic<N>::
announce_query2attr(GAPIC_int query, GAPIC_int attr, ExdistrStatistic* pos_s, ExdistrStatistic* tot_s) {
  for(unsigned i=0;i<N;i++) {
    pos_s->getStatistic(i)->announce_query2attr(query,attr);
    tot_s->getStatistic(i)->announce_query2attr(query,attr);
  }
}
template <unsigned N> void ExsplitNStatOwnerHeuristic<N>::
announce_best_test() {}
template <unsigned N> void ExsplitNStatOwnerHeuristic<N>::
announce_querytype(GAPIC_int query, GAPIC_int type, ExdistrStatistic* pos_s, ExdistrStatistic* tot_s) {
  for(unsigned i=0;i<N;i++) {
    pos_s->getStatistic(i)->announce_querytype(query,type);
    tot_s->getStatistic(i)->announce_querytype(query,type);
  }
}
template <unsigned N> void ExsplitNStatOwnerHeuristic<N>::
announce_regrvalues(GAPIC_int query, GAPIC_int example, GAPIC_float max, GAPIC_float min, GAPIC_float avg, ExdistrStatistic* pos_s, ExdistrStatistic* tot_s) {
  for(unsigned i=0;i<N;i++) {
    pos_s->getStatistic(i)->announce_regrvalues(query,example,max,min,avg);
    tot_s->getStatistic(i)->announce_regrvalues(query,example,max,min,avg);
  }
}
template <unsigned N> void ExsplitNStatOwnerHeuristic<N>::
announce_example_ids(GAPIC_term_ref ex_list, ExdistrStatistic* stat) {
  for(unsigned i=0;i<N;i++) {
    stat->getStatistic(i)->announce_example_ids(ex_list);
  }
}

#endif
