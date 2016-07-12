
#ifndef GAPIC_H
#define GAPIC_H

#include <vector>
#include "c_prolog.h"

G_SO GAPIC_int listLength(GAPIC_term_ref list);

G_SO GAPIC_int fillIntegerList(GAPIC_term_ref list, GAPIC_int* lst, GAPIC_int max);

G_SO GAPIC_int fillPrologIDList(GAPIC_term_ref list, GAPIC_int* lst, GAPIC_int max);

G_SO GAPIC_int compareStringAtom(const char* str, GAPIC_term_ref ref, GAPIC_atom* atom);

G_SO extern GAPIC_int _gapic_talking;

#define gapic_talking() _gapic_talking


          
 
class CLASS_SO GapicListCons {
protected:
	GAPIC_term_ref m_Tail;
	GAPIC_term_ref m_Head;
public:
	GapicListCons();
	void addHeadToList();
	void newList();
	inline GAPIC_term_ref getHead() {
		return m_Head;
	}
	inline GAPIC_term_ref getTail() {
		return m_Tail;
	}
};


           
 
class CLASS_SO GapicListConsB : public GapicListCons {
protected:
	GAPIC_term_ref m_Temp1;
	GAPIC_term_ref m_Temp2;
public:
	GapicListConsB();
	void addListToList(GAPIC_term_ref list);
};


          
 
class CLASS_SO GapicListConsC : public GapicListCons {
protected:
	GAPIC_term_ref m_CrTail;
public:
	GapicListConsC();
	void addHeadToList();
	void finishList();
	inline GAPIC_term_ref getCrTail() {
		return m_CrTail;
	}
};

class CLASS_SO GapicTermTable {
protected:
	vector<GAPIC_term_ref> m_Terms;
	int m_Depth;
public:
	GapicTermTable();
	void inc() { m_Depth++; }
	void dec() { m_Depth--; }
	void inc(int nb) { m_Depth += nb; }
	void dec(int nb) { m_Depth -= nb; }
	GAPIC_term_ref get();
	GAPIC_term_ref get(int idx);
	G_SO GAPIC_term_ref getInc();
	int contains(int idx);
};

G_SO void GapicSelectTimer(int id);
G_SO void GAPICRegisterAllTypes();

#endif
