
#ifndef QUERYUTIL_INC
#define QUERYUTIL_INC



void DollarRenamePredicates(GAPIC_term_ref in, GAPIC_term_ref out);

int ComputeTermHash(GAPIC_term_ref query);

unsigned int ComputeLiteralHash(GAPIC_term_ref term, GAPIC_term_ref temp);

class PrefixRenamer {
protected:
	char* m_Data;
	const char* m_Prefix;
	int m_Size, m_PreSize;
	GAPIC_atom m_TempAtom;
	GAPIC_functor m_TempFunctor;
	GAPIC_term_ref m_Scratch;
public:
	PrefixRenamer(const char* prefix);
	~PrefixRenamer();
	const char* rename(const char* name);
	void renameFromTo(GAPIC_term_ref from, GAPIC_term_ref to);
};

int computeStringHash(const char* strg, int arity);

class TermHashComputer {
protected:
	vector<int>    m_HashVals;
	int            m_Arity;
	     
	  
public:
	void hashAtom(GAPIC_term_ref atom);
	void hashFunctor(GAPIC_term_ref fct);
	void hashFloat(GAPIC_term_ref value);
	void hashInteger(GAPIC_term_ref value);
	void hashSimpleTerm(GAPIC_term_ref term);
	void computeHashRecursive(GAPIC_term_ref term);
	int computeEuivalenceHash();
	void printValues();
	inline int getArity() { return m_Arity; }
};

#endif
