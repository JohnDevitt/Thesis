


#ifndef PACKSINFO_INC
#define PACKSINFO_INC

#include "../../gapic/gapicgc.h"

extern gapic_type PacksInfoType;

class PacksInfo : public GapicIDObject {
protected:
	GAPIC_int m_Size;
	GAPIC_int m_AllowReuse;
	GAPIC_int m_ReuseEnable;
	unsigned int m_CrNbQueries;
	unsigned int m_Count;
	unsigned long m_Total;
	unsigned long m_TotalEx;
	unsigned long m_TotalExQueries;	
public:
	PacksInfo(GAPIC_term_ref ref);
	ostream& Print(ostream& os) const;
	boolean isReuseEnabled() const;

	boolean isReuseAllowed() const;

	void setReuseEnable(GAPIC_int enable);

	void addPack(GAPIC_int size, GAPIC_int nbex);

	void getStatistics(GAPIC_int* total, GAPIC_int* count, GAPIC_int* ex, GAPIC_int* exquery);

	int getSize() const;

	void setAllowReuse(GAPIC_int allow);

	int getCrNbQueries() { return m_CrNbQueries; }

};

typedef GapicNonExMutCollectable<PacksInfo, &PacksInfoType> PacksInfoCollectable;

#endif
