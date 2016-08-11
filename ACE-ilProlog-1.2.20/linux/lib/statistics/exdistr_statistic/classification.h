


#ifndef CLASSIFICATION_INC
#define CLASSIFICATION_INC

#include "../../gapic/gapicgc.h"

extern gapic_type StdNominalESType;


class StdNominalES : public SingleES {
protected:
	GAPIC_int m_NbClasses, m_Size;
	int m_chi_signif;
	GAPIC_float* m_ClassCounts;
public:
	StdNominalES(GAPIC_term_ref ref);
	StdNominalES(TargetValue* target);
	virtual ~StdNominalES();
	virtual void resize(GAPIC_int n, GAPIC_int s);
	virtual void reset();
	virtual void add(const ExdistrStatistic* s);
	virtual void scale(GAPIC_float s);
	virtual void update(GAPIC_int query, GAPIC_float weight);
	virtual void to_prolog(GAPIC_int idx, GAPIC_int query, GAPIC_term_ref prolog);
	virtual void rule_info(GAPIC_term_ref prolog);
	virtual ExdistrStatistic* clone() const;
	virtual ostream& PrintStat(ostream& os) const;
	virtual GAPIC_float getSumWeight(GAPIC_int q);	
	inline int getNbClasses() const {
		return m_NbClasses;
	}
	void copy(StdNominalES* other);
	void setNbClasses(GAPIC_int nbClasses);
	GAPIC_float classWeight(GAPIC_int q, GAPIC_int cl);
	double totalWeight(GAPIC_int which);

	GAPIC_float entropy(GAPIC_int which, GAPIC_float total);	
	double entropy(GAPIC_int which);

	GAPIC_float entropyDifference(GAPIC_int which, StdNominalES* other, GAPIC_float total);
	GAPIC_float gini(GAPIC_int which, GAPIC_float total);	
	GAPIC_float giniDifference(GAPIC_int which, StdNominalES* other, GAPIC_float total);
	GAPIC_int nbClassesMoreThan(GAPIC_int which, GAPIC_float thr, GAPIC_float* max);
	void getMajorityClass(GAPIC_int which, GAPIC_term_ref ref);
	int getMajorityClassWeight(GAPIC_int which, GAPIC_float* weight);
	void getClassDistr(GAPIC_int which, GAPIC_term_ref ref);	
	int get_chi_acceptability() {

		return m_chi_signif;
	}
	void reset_chi_acceptability() {

		m_chi_signif = 0;
	}
	void update_chi_significance(int currentquerysignif) {
		if (currentquerysignif == 1) m_chi_signif = 1;
	}		
private:
	void unifyClassIndex(GAPIC_int idx, GAPIC_term_ref ref);
};

class ClassSingleStatESH : public ExsplitHeuristic, public SingleStatOwner {
protected:
	GAPIC_int m_MinCases;
public:
	ClassSingleStatESH(GAPIC_term_ref ref);
};

class GainESH : public ClassSingleStatESH {
public:
	GainESH(GAPIC_term_ref ref);
	virtual void calculate(ExdistrStatistic* pos_s, ExdistrStatistic* tot_s, GAPIC_int query);
};

extern gapic_type GainESHType;


class MdlESH : public ClassSingleStatESH {
protected:
        int m_stop_criterion;             
	GAPIC_float m_DL_leaf;
	GAPIC_float m_BIC_penalty_perparam;
	int m_mdl_criterion;   
	int m_gainratio_splitting;                 
	int m_gini_splitting;                 

public:
	MdlESH(GAPIC_term_ref ref);
	virtual void calculate(ExdistrStatistic* pos_s, ExdistrStatistic* tot_s, GAPIC_int query);
	GAPIC_float computeMdlPenalty(ExdistrStatistic* pos_s, ExdistrStatistic* tot_s, GAPIC_int query);
	void setMdlCriterion(GAPIC_int nb);     

	void setDlLeaf(GAPIC_float nb);

	void setBicpenaltyPerparam(GAPIC_float nb);	

	void setGainratioSplitting(GAPIC_int nb) { m_gainratio_splitting=nb; };

	void setGiniSplitting(GAPIC_int nb) { m_gini_splitting=nb; };

	void setMdlStopCriterion(GAPIC_int nb) { m_stop_criterion=nb; };

	double getDlLeaf() { return m_DL_leaf; }

	double getBicpenaltyPerparam() { return m_BIC_penalty_perparam; }


};

extern gapic_type MdlESHType;



class ChiESH : public ClassSingleStatESH {
protected:
        int m_stop_criterion;             
	int m_gainratio_splitting;                 
	int m_gini_splitting;                 
        GAPIC_float m_SignificanceLevel;
public:
	ChiESH(GAPIC_term_ref ref);
	virtual void calculate(ExdistrStatistic* pos_s, ExdistrStatistic* tot_s, GAPIC_int query);
	void setChiStopCriterion(GAPIC_int nb) { m_stop_criterion=nb; };

	void setGainratioSplitting(GAPIC_int nb) { m_gainratio_splitting=nb; };

	void setGiniSplitting(GAPIC_int nb) { m_gini_splitting=nb; };

	GAPIC_float computeChiSquareScore(ExdistrStatistic* pos_s, ExdistrStatistic* tot_s, GAPIC_int query);	
        GAPIC_float computePFromChiSquare(GAPIC_float chisquare, int degreesfr); 
        GAPIC_float computeChiSquareFromP(GAPIC_float pvalue, int degreesfr);
};

extern gapic_type ChiESHType;


class GainRatioESH : public ClassSingleStatESH {
public:
	GainRatioESH(GAPIC_term_ref ref);
	virtual void calculate(ExdistrStatistic* pos_s, ExdistrStatistic* tot_s, GAPIC_int query);
};

extern gapic_type GainRatioESHType;


class GiniESH : public ClassSingleStatESH {
public:
	GiniESH(GAPIC_term_ref ref);
	virtual void calculate(ExdistrStatistic* pos_s, ExdistrStatistic* tot_s, GAPIC_int query);
};

extern gapic_type GiniESHType;


class SingleNominalESC : public ExdistrStopCrit {
protected:
	GAPIC_int m_Stat;
	GAPIC_int m_MinCases;
	GAPIC_float m_Accuracy;
public:
	SingleNominalESC(GAPIC_term_ref ref);
	virtual bool stopCrit(ExdistrStatistic* stat) const;
};

extern gapic_type SingleNominalESCType;

class MIStdNominal : public ModelInfo, VStatOwner {
public:
	MIStdNominal(GAPIC_term_ref ref);
	virtual void get_info(ExdistrStatistic* tot_s, GAPIC_term_ref info);
	virtual void get_info(ExdistrStatistic* tot_s, GAPIC_term_ref cls, GAPIC_term_ref fct, GAPIC_term_ref info) {}
};

extern gapic_type MIStdNominalType;

#endif
