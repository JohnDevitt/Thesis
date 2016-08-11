
#ifndef CTRANSFORM_INC
#define CTRANSFORM_INC



#define CTQ_LITERAL	0
#define CTQ_SET		1

typedef unsigned char TR_BYTE;
typedef TR_BYTE* TR_BYTE_PTR;

typedef struct {
	int parent;
	int rank;
	int map;
} DisjointSetForestElem;

class DisjointSetForest {
protected:
	int m_NbElemMax;
	DisjointSetForestElem* m_DSForest;
	int* m_ComprTable;
public:
	DisjointSetForest(int nbelem, int* memory);
	void makeSet(int var);
	void computeUnion(int e1, int e2);
	void makeLink(int e1, int e2);	
	int findSet(int e);
	int getMap(int e);
	void setMap(int e, int v);	
	void showAll();
};

class ExpandableByteArray {
protected:
	vector<TR_BYTE> m_Bytes;
	int m_Count, m_Current, m_UsedBytes;
public:
	ExpandableByteArray();
	void reset();
	void addEntry();
	void createIndex();
	void addBytes(int toadd);
	void setBytes(int total);
	void delBytes(int todel);
	inline int incSize() { return m_Count++; }
	inline void setSize(int size) { m_Count = size; }
	inline void decSize() { m_Count--; }
	inline void movePointer(int toadd) { m_Current += toadd; }
	inline void decPointer(int todec) { m_Current -= todec; }	
	inline void resetPointer() { m_Current = 0; }
	inline void* currentPointer() { return (void*)&m_Bytes[m_Current]; }
	inline void* initialPointer() { return (void*)&m_Bytes[0]; }
	inline void* getPointer(int idx, int size) { return (void*)&m_Bytes[idx*size]; }
	inline int size() const { return m_Count; }
	inline int getNbBytes() const { return m_UsedBytes; }
};

class ExpandableIndexByteArray : public ExpandableByteArray {
protected:
	vector<int> m_Index;
	int m_UsedIndex;
public:
	void reset();
	void addEntry();
	inline void* getPointer(int idx) { return (void*)&m_Bytes[m_Index[idx]]; }
};

typedef struct {
	string* m_Name;
	int m_Arity;
	int m_Vars;
	int m_Index;
} CTransLiteral;

typedef struct {
	TR_BYTE m_Type;
	int m_Index;
} CTransSetElem;

class CTransSet {
protected:
	         
	ExpandableByteArray m_Vars;
	ExpandableByteArray m_Elements;
public:
	void reset();
	void createInitial(int size, int maxvar);
	void createInitial(int from, int to, int maxvar);
	void initVars(int maxvars);	
	void addLiteral(int index);
	void addSet(int index);
	void addElement(CTransSetElem* add);
	void addVar(int var);
	void setVars(int* vars, int nb);
	void mergeVars(int* vars, int nb);
	void mergeLiterals(CTransSet* set);
	void deleteElem(int i);
	void deleteTo(int i);
	inline int getNbVars() { return m_Vars.size(); }
	inline int getNbElems() { return m_Elements.size(); }
	inline int* getVars() { return (int*)m_Vars.initialPointer(); }
	inline CTransSetElem* getElems() { return (CTransSetElem*)m_Elements.initialPointer(); }
	inline CTransSetElem* getElem(int i) { return (CTransSetElem*)m_Elements.getPointer(i, sizeof(CTransSetElem)); }
};

class CTransSetList {
protected:
	int m_UsedSets;
	vector<CTransSet*> m_Sets;
public:
	void reset();
	CTransSet* addSet();
	inline int size() { return m_UsedSets; }
	inline CTransSet* getSet(int i) { return m_Sets[i]; }
};

#define CTRANS_LIT_VARS(l) (int*)((TR_BYTE*)l+sizeof(CTransLiteral))


        
 
              
          
                
                
 
          
         
 
 

class CTransQuery {
protected:
	CTransSetList m_Query;
		       
	ExpandableIndexByteArray m_Literals;
		         
	ExpandableByteArray m_Vars;
		         
	ExpandableByteArray m_Terms;
		      
	ExpandableByteArray m_Heap;
		    
	CTransSet* m_Scratch;
		       
	int m_Prefix;
public:
	void reset();
	void printLiterals();
	void printLiteral();
	void printLiteral(CTransLiteral* lit);
	void selectFirstLit();
	void selectNextLit();
	CTransLiteral* addLiteral();
	void addLiteral(GAPIC_term_ref lit);
	void addLiteralVars(GAPIC_term_ref term);
	void addLiteralVar(GAPIC_int varidx);
	void sortLitVars();
	CTransSet* createInitialSet();
	CTransSet* createPrefixSet();
	CTransSet* createTailSet();
	void printSet(CTransSet* set);
	int createSingletonSetNoVars(CTransLiteral* lit);
	CTransSet* computeIndependentSets(CTransSet* set);
	CTransSet* computeIndependentSetsNR(CTransSet* set);
	void computeIndependentSets(CTransSet* set, CTransSet* result);
	void computeIndependentSetsNR(CTransSet* set, CTransSet* result);
	CTransSet* performOnceTransform(CTransSet* set);
	CTransSet* performOnceTransformNR(CTransSet* set);
	CTransSet* performCutTransform(CTransSet* set);
	CTransSet* performCutTransformNR(CTransSet* set);
	CTransSet* performSmartTransform(int addHead);
	void  performOnceTransformSub(CTransSet* set);
	void  performOnceTransformSubNR(CTransSet* set);
	void removeHeadVars(CTransSet* head, CTransSet* set);
	void removeHeadVars(CTransSet* set);
	void computeSetVars(CTransSet* set);
	int mergeInSets(CTransLiteral* lit, CTransSet* result);
	void mergeLastSubset(CTransSet* set);
	int addNewVar();
	void queryToProlog(CTransSet* set, GAPIC_term_ref body1, GAPIC_term_ref body2);
	GAPIC_term_ref setToProlog(CTransSet* set);
	CTransSet* allocateScratch();
	int* allocHeap(int cells);
	void popHeap(int cells);
	inline CTransSet* addSet() { return m_Query.addSet(); }
	inline int getTotalNbVars() const { return m_Vars.getNbBytes(); }
	inline CTransLiteral* currentLiteral() { return (CTransLiteral*)m_Literals.currentPointer(); }
	inline CTransLiteral* getLiteral(int i) { return (CTransLiteral*)m_Literals.getPointer(i); }
	inline CTransSet* getSet(int i) { return m_Query.getSet(i); }
	inline CTransSet* getScratch() { return m_Scratch; }
	inline int size() { return m_Literals.size(); }
	inline int isSmartFormat() { return m_Prefix != 0; }
	inline int getSmartPrefix() { return m_Prefix; }
	inline void setSmartPrefix(int prefix) { m_Prefix = prefix; }
};

void CTransInitialize();
void CTransSetQuery(GAPIC_term_ref head, GAPIC_term_ref body);
void CTransSetQuerySmart(GAPIC_term_ref head, GAPIC_term_ref body);
void CTransOnceTransform(GAPIC_term_ref body1, GAPIC_term_ref body2);
void CTransOnceTransformNR(GAPIC_term_ref body1, GAPIC_term_ref body2);
void CTransCutTransform(GAPIC_term_ref body1, GAPIC_term_ref body2);
void CTransCutTransformNR(GAPIC_term_ref body1, GAPIC_term_ref body2);
void CTransSmartTransform(GAPIC_term_ref body1, GAPIC_term_ref body2);
void CTransSmartOnceTransform(GAPIC_term_ref body1, GAPIC_term_ref body2);
void CTransDummyTransform(GAPIC_term_ref body1, GAPIC_term_ref body2);

#endif
