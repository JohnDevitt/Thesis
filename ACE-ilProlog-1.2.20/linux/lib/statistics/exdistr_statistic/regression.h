


#ifndef REGRESSION_INC
#define REGRESSION_INC

#include "../../gapic/gapicgc.h"
#include "ftest.h"
#include <dbms/dbms.h>
#include <kb/dbms/cdbms.h>


extern gapic_type StdRealESType;
extern gapic_type SlrRealESType;

 

class StdRealES : public SingleES {
protected:
	GAPIC_int m_Size;
	GAPIC_float* m_SumWeights;
	GAPIC_float* m_SumValues;
	GAPIC_float* m_SumSqValues;
public:
	StdRealES(GAPIC_term_ref ref);
	StdRealES(TargetValue* target);
	virtual void resize(GAPIC_int n, GAPIC_int s);
	virtual void reset();
	virtual void add(const ExdistrStatistic* s);
	virtual void scale(GAPIC_float s);
	virtual void update(GAPIC_int query, GAPIC_float weight);
	virtual void to_prolog(GAPIC_int idx, GAPIC_int query, GAPIC_term_ref prolog);
	virtual ExdistrStatistic* clone() const;
	virtual ostream& PrintStat(ostream& os) const;
	GAPIC_float totalWeight(GAPIC_int q);
	GAPIC_float expectedValue(GAPIC_int query);
	GAPIC_float expectedSqValue(GAPIC_int query);
	inline GAPIC_float sumValues(GAPIC_int query) const {
		return m_SumValues[query];
	}
	inline GAPIC_float sumSqValues(GAPIC_int query) const {
		return m_SumSqValues[query];
	}
	inline GAPIC_float sumWeights(GAPIC_int query) const {
		return m_SumWeights[query];
	}
	void copy(StdRealES* other) {}
};


     
class SlrRealES : public SingleES {
protected:
	GAPIC_int m_Size;    
	GAPIC_int m_NbRegrAttrs;                       
	GAPIC_int m_NbExamples;    
	GAPIC_float* m_SumWeights;
	GAPIC_float* m_SumTargets;
	GAPIC_float* m_SumSqTargets;
	GAPIC_float* m_SumPredictors;
	GAPIC_float* m_SumSqPredictors;
	GAPIC_float* m_SumTargetPredictor;
	GAPIC_float* m_BigTable;                 
	GAPIC_int* m_QueryTypes;       
	GAPIC_float* m_Weights;
	GAPIC_float* m_Targets;
	GAPIC_int* m_example_ids;
	GAPIC_int* m_pos_examples;                 
	GAPIC_int m_Stop;
	
public:
	SlrRealES(GAPIC_term_ref ref);
	SlrRealES(TargetValue* target);
	virtual void resize(GAPIC_int n, GAPIC_int s);
	virtual void reset();
	virtual void add(const ExdistrStatistic* s);
	virtual void scale(GAPIC_float s);
	virtual void update(GAPIC_int query, GAPIC_float weight);
	virtual void to_prolog(GAPIC_int idx, GAPIC_int query, GAPIC_term_ref prolog);
	virtual ExdistrStatistic* clone() const;
	virtual ostream& PrintStat(ostream& os) const;
	virtual void announce_regrvalues(GAPIC_int query, GAPIC_int example, GAPIC_float max, GAPIC_float min, GAPIC_float avg);
	virtual void announce_querytype(GAPIC_int query, GAPIC_int type);
	virtual void announce_example_ids(GAPIC_term_ref ex_list);
	GAPIC_float totalWeight(GAPIC_int q);
	GAPIC_float expectedValue(GAPIC_int query);
	GAPIC_float expectedSqValue(GAPIC_int query);
	GAPIC_int exid2number(GAPIC_int exid);
	
	inline GAPIC_float sumTargets(GAPIC_int query) const {
		return m_SumTargets[query];
	}
	inline GAPIC_float sumSqTargets(GAPIC_int query) const {
		return m_SumSqTargets[query];
	}
	inline TargetValue* target() const {
		return m_Target;
	}
	inline GAPIC_float sumPredictors(GAPIC_int query) const {
		return m_SumPredictors[query];
	}
	inline GAPIC_float sumSqPredictors(GAPIC_int query) const {
		return m_SumSqPredictors[query];
	}
	inline GAPIC_float sumTargetPredictor(GAPIC_int query) const {
		return m_SumTargetPredictor[query];
	}
	inline GAPIC_float sumWeights(GAPIC_int query) const {
		return m_SumWeights[query];
	}
	inline GAPIC_int queryType(GAPIC_int query) const {
		return m_QueryTypes[query];
	}
	inline GAPIC_float weight(GAPIC_int example) const {
		return m_Weights[example];
	}
	inline GAPIC_float target(GAPIC_int example) const {
		return m_Targets[example];
	}
	inline GAPIC_int exampleid(GAPIC_int index) const {
		return m_example_ids[index];
	}
	inline GAPIC_float bigTable(GAPIC_int query, GAPIC_int example, GAPIC_int regattr) const {
		return m_BigTable[query*m_NbExamples*m_NbRegrAttrs + example*m_NbRegrAttrs + regattr];
	}
	inline void setBigTable(GAPIC_int query, GAPIC_int example, GAPIC_int regattr, GAPIC_float value) {
		m_BigTable[query*m_NbExamples*m_NbRegrAttrs + example*m_NbRegrAttrs + regattr] = value;
	}
	inline GAPIC_int posExample(GAPIC_int query, GAPIC_int example) const {
		return m_pos_examples[query*m_NbExamples + example];
	}
	inline void setPosExample(GAPIC_int query, GAPIC_int example) {
		m_pos_examples[query*m_NbExamples + example] = 1;
	}
	inline GAPIC_int nbRegrAttrs() const {
		return m_NbRegrAttrs;
	}
	inline GAPIC_int nbExamples() const {
		return m_NbExamples;
	}
	void copy(SlrRealES* other) {}
	inline GAPIC_int getStop() const {
		return m_Stop;
	}
	inline void setStop(GAPIC_int stop) {
		m_Stop = stop;
	}
};


 

class RegressionESH : public ExsplitNStatOwnerHeuristic<2> {
protected:
	GAPIC_int m_MinCases;
public:
	RegressionESH(GAPIC_term_ref ref);
	virtual void calculate(ExdistrStatistic* pos_s, ExdistrStatistic* tot_s, GAPIC_int query);
};

     

class SLRRegressionESH : public ExsplitNStatOwnerHeuristic<2> {
protected:
	GAPIC_int m_MinCases;
	GAPIC_int m_BestQuery;
	GAPIC_float m_RootSD;
public:
	SLRRegressionESH(GAPIC_term_ref ref);
	virtual void announce_best_test();

	virtual void announce_query2attr(GAPIC_int query, GAPIC_int attr, ExdistrStatistic* pos_s, ExdistrStatistic* tot_s);

	virtual void announce_querytype(GAPIC_int query, GAPIC_int type, ExdistrStatistic* pos_s, ExdistrStatistic* tot_s);

	virtual void announce_regrvalues(GAPIC_int query, GAPIC_int example, GAPIC_float max, GAPIC_float min, GAPIC_float avg, ExdistrStatistic* pos_s, ExdistrStatistic* tot_s);

	virtual void announce_example_ids(GAPIC_term_ref ex_list, ExdistrStatistic* stat);

	virtual void calculate(ExdistrStatistic* pos_s, ExdistrStatistic* tot_s, GAPIC_int query);
	virtual void Print(ostream& os) const;
	double residual_standard_deviation(GAPIC_float n, GAPIC_float sumY, GAPIC_float sumX, GAPIC_float sumXY, GAPIC_float sumY2, GAPIC_float sumX2);
};

class SSReductionESH : public NCompoundESH {
public:
	SSReductionESH(GAPIC_term_ref ref);
	virtual void calculate(ExdistrStatistic* pos_s, ExdistrStatistic* tot_s, GAPIC_int query);
};

        
class SLRSSReductionESH : public NCompoundESH {
public:
	SLRSSReductionESH(GAPIC_term_ref ref);
	virtual void calculate(ExdistrStatistic* pos_s, ExdistrStatistic* tot_s, GAPIC_int query);
};

class FTestESH : public SCompoundESH {
protected:
	GAPIC_int m_SLevel;
	GAPIC_int m_WCountIdx;
public:
	FTestESH(GAPIC_term_ref ref);
	virtual void calculate(ExdistrStatistic* pos_s, ExdistrStatistic* tot_s, GAPIC_int query);
};

class MIStdReal : public ModelInfo, VStatOwner {
public:
	MIStdReal(GAPIC_term_ref ref);
	virtual void get_info(ExdistrStatistic* tot_s, GAPIC_term_ref info);
	virtual void get_info(ExdistrStatistic* tot_s, GAPIC_term_ref cls, GAPIC_term_ref fct, GAPIC_term_ref info) {}
};

extern gapic_type RealErrorType;

class RealErrorComp {
public:
	RealErrorComp();
	double SumPi, SumSPi;
	double SumAi, SumSAi;
	double SumPiAi;
	double AE;
	double SE;
};

class RealError {
public:
	double m_Count;
	vector<RealErrorComp> m_Comp;
public:
	RealError();
	void update(GAPIC_term_ref target, GAPIC_term_ref predicted);

	void add(RealError* other);

	void get(GAPIC_term_ref err);

	inline RealErrorComp* getComp(int i) { return &m_Comp[i]; }
	inline int getSize() { return m_Comp.size(); }
	void expand(int i);
	ostream & Print(ostream & strm) const;
};

typedef GapicNonExMutCollectable<RealError, &RealErrorType> RealErrorCollectable;

extern gapic_type MIStdRealType;
extern gapic_type RegressionESHType;
extern gapic_type SLRRegressionESHType;
extern gapic_type SSReductionESHType;
extern gapic_type SLRSSReductionESHType;
extern gapic_type FTestESHType;

#endif
