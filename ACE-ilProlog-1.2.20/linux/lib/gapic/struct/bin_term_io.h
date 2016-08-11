
#ifndef BIN_TERM_IO_H
#define BIN_TERM_IO_H

using namespace std;
using namespace __gnu_cxx;

class GapicSymTableElem {
public:
	virtual ~GapicSymTableElem();
	virtual void put(GAPIC_term_ref ref) = 0;
	virtual void save(FILE* f) = 0;
	virtual dlong getHashKey() = 0;
	virtual void getString(string* str) const = 0;
};

class GapicSymTableAtom : public GapicSymTableElem {
protected:
	GAPIC_atom m_Atom;
public:
	GapicSymTableAtom(const char* name);
	GapicSymTableAtom(GAPIC_atom& atom);
	virtual ~GapicSymTableAtom();	
	virtual void put(GAPIC_term_ref ref);
	virtual void save(FILE* f);	
	virtual dlong getHashKey();	
	virtual void getString(string* str) const;
};

class GapicSymTableFunctor : public GapicSymTableElem {
protected:
	GAPIC_functor m_Functor;
public:
	GapicSymTableFunctor(const char* name, int arity);
	GapicSymTableFunctor(GAPIC_functor& fct);
	virtual ~GapicSymTableFunctor();		
	virtual void put(GAPIC_term_ref ref);
	virtual void save(FILE* f);	
	virtual dlong getHashKey();	
	virtual void getString(string* str) const;
};

struct EqSymbolHashKey {
	inline bool operator() (const dlong& s1, const dlong& s2) const {
	    return s1 == s2;
	}
};

struct HashSymbolHashKey {
	inline size_t operator() (const dlong& s) const {
		return hash<dlong>()(s);
	}
};

class GapicSymbolKeyPair : public pair<dlong, int> {
public:
	GapicSymbolKeyPair(dlong key, int value);
};

class GapicSymbolHash :	public hash_map<dlong, int, HashSymbolHashKey, EqSymbolHashKey> {
public:
	int try_get(dlong key) const;
	void add_item(dlong key, int elem);
};

class CLASS_SO GapicSymTable {
protected:
	vector<GapicSymTableElem*> m_Table;
	GapicSymbolHash m_Hash;
public:
	GapicSymTable();
	~GapicSymTable();
	int contains(int idx);
	void add(int idx, const char* name, int arity);
	void put(int idx, GAPIC_term_ref ref);
	GapicSymTableElem* get(int idx);
	void expand(int idx);
	int tryAddAtom(GAPIC_atom& atm);
	int tryAddFunctor(GAPIC_functor& fct);
	void clearSymTable();
	G_SO void save(FILE* f);
	G_SO void load(FILE* f);
	void show();
	inline int getSize() { return m_Table.size(); }
};

#define GAPIC_BIN_ATOM    0
#define GAPIC_BIN_VAR     1
#define GAPIC_BIN_INT     2
#define GAPIC_BIN_FLOAT   3
#define GAPIC_BIN_FUNCTOR 4
#define GAPIC_BIN_LIST    5

class CLASS_SO GapicBinReader {
protected:
	FILE* m_File;
	GapicSymTable* m_SymTab;
	GapicTermTable m_Terms;
	GapicTermTable m_Vars;
public:
	GapicBinReader(FILE* f, GapicSymTable* syms);
	G_SO void read(GAPIC_term_ref ref);
	void readAtom(GAPIC_term_ref ref);
	void readVar(GAPIC_term_ref ref);
	void readInt(GAPIC_term_ref ref);
	void readFloat(GAPIC_term_ref ref);
	void readFunctor(GAPIC_term_ref ref);
	void readList(GAPIC_term_ref ref);
};

class CLASS_SO GapicGroundBinWriter {
protected:
	FILE* m_File;
	GapicSymTable* m_SymTab;
	GapicTermTable m_Terms;
public:
	GapicGroundBinWriter(FILE* f, GapicSymTable* syms);
	G_SO void write(GAPIC_term_ref ref);
	void writeAtom(GAPIC_term_ref ref);
	void writeVar(GAPIC_term_ref ref);
	void writeInt(GAPIC_term_ref ref);
	void writeFloat(GAPIC_term_ref ref);
	void writeFunctor(GAPIC_term_ref ref);
	void writeList(GAPIC_term_ref ref);
};

#endif
