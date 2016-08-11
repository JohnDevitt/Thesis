
#ifndef CPLOGIC_INC
#define CPLOGIC_INC



#include <cptheory.h>

extern gapic_type ACECPTheoryType;

enum ACECPTheoryTermType {
	ACE_CP_THEORY_TERM_ATOM,
	ACE_CP_THEORY_TERM_INT,
	ACE_CP_THEORY_TERM_FUNCTOR,
	ACE_CP_THEORY_TERM_LIST
};

typedef union {
	dlong          SymbolVal;
	GAPIC_int      IntVal;
	double         DoubleVal;
} ACECPTheoryTermValue;

class ACECPTheoryTerm {
public:
	ACECPTheoryTermType m_Type;
	ACECPTheoryTermValue m_Value;
	void* m_SubTerms;
public:
	inline ACECPTheoryTerm() {}
	inline ACECPTheoryTerm(const ACECPTheoryTerm& other) {
		m_Type = other.m_Type; m_Value = other.m_Value;
		if (other.m_Type >= ACE_CP_THEORY_TERM_FUNCTOR) copyFunctor(other);
	}
	inline ~ACECPTheoryTerm() {
		if (m_Type >= ACE_CP_THEORY_TERM_FUNCTOR) deleteRecursive();
	}
	void copyFunctor(const ACECPTheoryTerm& other);
	void deleteRecursive();
	bool equals(const ACECPTheoryTerm& other) const;
	size_t hash() const;
	void print(ostream& out) const;
};

struct ACECPTheoryTermEquals {
	inline bool operator() (const ACECPTheoryTerm& k1, const ACECPTheoryTerm& k2) const {
		return k1.equals(k2);
	}
};

struct ACECPTheoryTermHash {
	inline size_t operator() (const ACECPTheoryTerm& key) const {
		return key.hash();
	}
};

typedef unordered_map<ACECPTheoryTerm, Atom*, ACECPTheoryTermHash, ACECPTheoryTermEquals> ACECPTheoryAtomMap;

typedef unordered_map<ACECPTheoryTerm, int, ACECPTheoryTermHash, ACECPTheoryTermEquals> ACECPTheoryAtomIntMap;

class ACECPTheoryGroundEvent {
public:
	int m_Size;
	size_t m_Hash;
	void* m_Terms;
public:
	ACECPTheoryGroundEvent(int nbTerms);
	ACECPTheoryGroundEvent(const ACECPTheoryGroundEvent& other);
	~ACECPTheoryGroundEvent();
	ACECPTheoryTerm* getTerm(int i);
	void computeHash();
	bool equals(const ACECPTheoryGroundEvent& other) const;
};

struct ACECPTheoryGroundEventEquals {
	inline bool operator() (const ACECPTheoryGroundEvent& k1, const ACECPTheoryGroundEvent& k2) const {
		return k1.equals(k2);
	}
};

struct ACECPTheoryGroundEventHash {
	inline size_t operator() (const ACECPTheoryGroundEvent& key) const {
		return key.m_Hash;
	}
};

typedef unordered_set<ACECPTheoryGroundEvent, ACECPTheoryGroundEventHash, ACECPTheoryGroundEventEquals> ACECPTheoryGroundEventSet;

class ACECPTheory {
protected:
	CPTheory m_Theory;
	ACECPTheoryAtomMap m_AtomMap;
	ACECPTheoryAtomIntMap m_AtomTable;
	          
	vector<int> m_FONbAtoms;
	        
	vector<ACECPTheoryGroundEventSet*> m_FOGroundEvents;
public:
	ACECPTheory();
	ACECPTheory(GAPIC_term_ref ref);
	~ACECPTheory();
	void atomFromTermBuildKey(GAPIC_term_ref term, ACECPTheoryTerm* key, GapicTermTable* terms);
	Atom* atomFromTerm(const ACECPTheoryTerm& key);
	Atom* atomFromTerm(GAPIC_term_ref term, GapicTermTable* terms);
	void processEvidence(GAPIC_term_ref evidence, AtomBody& query_evidence, GapicTermTable& terms);
	int atomInTable(GAPIC_term_ref atom);

	void atomTableSuccess(GAPIC_term_ref atom);

	void addEvent(GAPIC_term_ref event);

	void addGroundEvent(GAPIC_term_ref event);

	void addFoEvent(GAPIC_int id, GAPIC_int nbatoms);

	void compile();

	void save(GAPIC_term_ref fname);

	double query(GAPIC_term_ref qatom, GAPIC_term_ref evidence);

	double querySilent(GAPIC_term_ref qatom, GAPIC_term_ref evidence);

	void setSettings(GAPIC_int ccnodes, GAPIC_int breakcycles, const GAPIC_atom& transtype, const GAPIC_atom& inftype, const GAPIC_atom& ordtype);

	ostream& Print(ostream& os) const;
	void printGraphviz(const GAPIC_atom& filename);

};

typedef GapicNonExMutCollectable<ACECPTheory, &ACECPTheoryType> ACECPTheoryCollectable;

#endif

