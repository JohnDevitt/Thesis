


#ifndef STOPCRIT_INC
#define STOPCRIT_INC

#include "../../gapic/gapicgc.h"

extern gapic_type ExdistrStopCritType;

class ExdistrStopCrit {
public:
	virtual bool stopCrit(ExdistrStatistic* stat) const = 0;

	ostream& Print(ostream& os) const { return os; }
};

typedef GapicNonExMutCollectable<ExdistrStopCrit, &ExdistrStopCritType> ExdistrStopCritCollectable;

class MinimalCasesESC : public ExdistrStopCrit {
protected:
	GAPIC_int m_MinCases;
	GAPIC_int m_StatisticIdx;
public:
	MinimalCasesESC(GAPIC_term_ref ref);
	virtual bool stopCrit(ExdistrStatistic* stat) const;
};

extern gapic_type MinimalCasesESCType;

#endif
