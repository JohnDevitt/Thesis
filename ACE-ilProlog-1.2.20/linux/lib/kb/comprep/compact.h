


#ifndef COMPACT_INC
#define COMPACT_INC

#define KBG_MEM_LIMIT 1048576000L
   
  

#define MY_VECTOR_ADD  5
#define MY_VECTOR_MULT 2

template <class type> class my_vector {
protected:
	char* m_Data;
	int m_Count;
	int m_Size;
public:
	my_vector() {
		m_Data = NULL;
		m_Count = 0;
		m_Size = 0;
	}

	~my_vector() {
		if (m_Data != NULL) free(m_Data);
	}

	void reserve(int nb) {
		int wanted = (nb + 1) * sizeof(type);
		if (wanted > m_Size) {
			while (m_Size < wanted) {
				m_Size = MY_VECTOR_MULT * m_Size + MY_VECTOR_ADD * sizeof(type);
			}
			m_Data = (char*)realloc(m_Data, m_Size);
		}
	}

	type* inc() {
		m_Count++;
		reserve(m_Count);
		return ((type*)m_Data) + m_Count - 1;
	}

	type* get(int idx) {
		return ((type*)m_Data) + idx;
	}

	const type* getConst(int idx) const {
		return ((type*)m_Data) + idx;
	}

	void pop_back() {
		m_Count--;
	}

	void clear() {
		m_Count = 0;
	}

	inline int size() { return m_Count; }
};

extern gapic_type KbgType;

class Kbg;

class KbgNodeName : public PTermComposed {
private:
	string m_name;
	int m_NbTempl;
public:
	KbgNodeName(const string& name, const TokenizerPos& pos);
	virtual ProTypeEnum getType() const;
	virtual ostream& write(ostream &os, const PTermWriter* wrt) const;
	virtual void writeBin(FILE* f, PTermSymVarTable* symtab) const;
	inline void setName(string& name) { m_name = name; };
	inline const string& getName() const { return m_name; };
	inline string& setName() { return m_name; };
	inline int getNbTempl() const { return m_NbTempl; }
	inline void setNbTempl(int nbtempl) { m_NbTempl = nbtempl; }
	int getTemplInt(int idx);
};

#define KBG_TYPE_UNKNOWN   0
#define KBG_TYPE_SIMPLE    1
#define KBG_TYPE_META      2
#define KBG_TYPE_TEMPLATE  3
#define KBG_TYPE_COMPILED  4

#define KBG_TODO_NOTHING   0
#define KBG_TODO_RETRIEVE  1
#define KBG_TODO_TEMPLATE  2
#define KBG_TODO_META      3
#define KBG_TODO_COMPILED  4
#define KBG_TODO_PARENTS   5
#define KBG_TODO_ASSERT    6

#define KBG_FLAG_PARCOMP    8
#define KBG_FLAG_EXAMPLE    16
#define KBG_FLAG_REACHABLE  32
#define KBG_FLAG_EXPANDED   64
#define KBG_FLAG_VISITED    128

#define KBG_FLAG_ASSERTED   1
#define KBG_FLAG_SEMI_LOCK  2

class KbgNodeActive {
protected:
	char m_Flags;
	int m_Lock;
	int m_ArrayID;
	int m_Size;
	int m_NodeID;
	KbgNodeActive* m_Next;
	KbgNodeActive* m_Prev;
	vector<int>* m_Disjoint;
public:
	KbgNodeActive(int id);
	int getMySize();
	void insertAfter(KbgNodeActive* active);
	void deleteFromList();
	inline int hasFlag(int flag) const { return (m_Flags & flag) != 0; }
	inline int noFlag(int flag) const { return (m_Flags & flag) == 0; }
	inline int getFlags() const { return m_Flags; }
	inline void setFlags(int flags) { m_Flags = flags; }
	inline void setFlag(int flag) { m_Flags |= flag; }
	inline void clearFlag(int flag) { m_Flags &= (0xFF ^ flag); }
	inline int getArrayID() const { return m_ArrayID; }
	inline void setArrayID(int id) { m_ArrayID = id; }
	inline int getSize() const { return m_Size; }
	inline void setSize(int size) { m_Size = size; }
	inline int getLock() const { return m_Lock; }
	inline void lock() { m_Lock++; }
	inline void unLock() { m_Lock--; }
	inline int getNodeID() const { return m_NodeID; }
	inline vector<int>* getDisjoint() { return m_Disjoint; }
	inline void setDisjoint(vector<int>* compiled) { m_Disjoint = compiled; }
	inline int getTotalSize() { return getSize() + getMySize(); }
	inline KbgNodeActive* getPrev() { return m_Prev; }
	inline KbgNodeActive* getNext() { return m_Next; }
	inline void setPrev(KbgNodeActive* prev) { m_Prev = prev; }
	inline void setNext(KbgNodeActive* next) { m_Next = next; }
};

class KbgNode {
protected:
	char m_Flags;
	char m_Set;
	int  m_ID;
	int  m_Parents;
	int  m_Childs;
	long m_DiskRef;
	KbgNodeActive* m_Active;
public:
	KbgNode();
	KbgNode(const KbgNode& other);
	void init();
	void deActivate();
	int easyActivatable();
	inline void setDiskRef(long pos) { m_DiskRef = pos; }
	inline long getDiskRef() { return m_DiskRef; }
	inline int isActive() { return m_Active != NULL; }
	inline int isInActive() { return m_Active == NULL; }
	inline KbgNodeActive* getActiveData() const { return m_Active; }
	inline void setActiveData(KbgNodeActive* data) { m_Active = data; }
	inline int getType() { return m_Flags & 0x07; }
	inline void setType(int type) { m_Flags = (m_Flags & 0xF8) | type; }
	inline int hasFlag(int flag) const { return (m_Flags & flag) != 0; }
	inline int noFlag(int flag) const { return (m_Flags & flag) == 0; }
	inline int getFlags() const { return m_Flags; }
	inline void setFlags(int flags) { m_Flags = flags; }
	inline void setFlag(int flag) { m_Flags |= flag; }
	inline void clearFlag(int flag) { m_Flags &= (0xFF ^ flag); }
	inline void setSet(int set) { m_Set = set; }
	inline void setSet(int set, int id) { m_Set = set; m_ID = id; }
	inline int getID() const { return m_ID; }
	inline void setID(int id) { m_ID = id; }
	inline int getSet() const { return m_Set; }
	inline void setParents(int parents) { m_Parents = parents; }
	inline int getParents() { return m_Parents; }
	inline void setChilds(int childs) { m_Childs = childs; }
	inline int getChilds() { return m_Childs; }
	inline void lock() { if (m_Active != NULL) m_Active->lock(); }
	inline void unLock() { if (m_Active != NULL) m_Active->unLock(); }
	inline void setExampleFlag(int isEx) { if (isEx == 1) setFlag(KBG_FLAG_EXAMPLE); }
	inline int isAsserted() { return getActiveData()->hasFlag(KBG_FLAG_ASSERTED); }
	inline int setAsserted() { getActiveData()->setFlag(KBG_FLAG_ASSERTED); }
};

class CPTermSymTable : public PTermSymTable {
protected:
	GapicSymTable* m_SymTab;
public:
	CPTermSymTable(GapicSymTable* symtab);
	virtual ~CPTermSymTable();
	virtual SymbolID getSym(const string& name, int arity);
};

#define KBG_NODE_NAME_SINGLE 0
#define KBG_NODE_NAME_ARRAY 1

class KbgNodeNameEntry {
protected:
	string m_Name;
	int m_Arity;
public:
	KbgNodeNameEntry(const char* name, int arity);
	virtual ~KbgNodeNameEntry();
	virtual int getType() = 0;
	inline string& getName() { return m_Name; }
	inline int getArity() { return m_Arity; }
};

class KbgNodeNameArray : public KbgNodeNameEntry {
protected:
	int m_NbTempl;
	MDVectorNode m_Node;
public:
	KbgNodeNameArray(MDVector* main, const char* name, int arity);
	virtual ~KbgNodeNameArray();
	virtual int getType();
	inline int getNbTempl() { return m_NbTempl; }
	inline void setNbTempl(int nb) { m_NbTempl = nb; }
	inline void add(MDVectorKey* key, int value) { m_Node.add(key, value); }
	inline int get(MDVectorKey* key) { return m_Node.get(key); }
	inline void write(int level, const string& prefix, MDVectorNodeWriter* writer) const {
		m_Node.write(level, prefix, writer);
	}
	inline void save(FILE* f) {
		m_Node.save(f);
	}
	inline void load(FILE* f) {
		m_Node.load(f);
	}
	inline void forEach(MDVectorForEach* foreach) {
		m_Node.forEach(foreach);
	}
};

class KbgNodeNameSingle : public KbgNodeNameEntry {
protected:
	int m_Index;
public:
	KbgNodeNameSingle(int index, const char* name, int arity);
	virtual ~KbgNodeNameSingle();
	virtual int getType();
	inline int getIndex() { return m_Index; }
};

class KbgSymbolKeyPair : public pair<dlong, KbgNodeNameEntry*> {
public:
	KbgSymbolKeyPair(dlong key, KbgNodeNameEntry* value);
};

class KbgActiveNodeWriter : public MDVectorNodeWriter {
protected:
	ostream* m_OS;
	const Kbg* m_KBG;
public:
	KbgActiveNodeWriter(ostream* os, const Kbg* kbg);
	virtual void write(const string& prefix, int value) const;
};

class KbgSingleNodeWriter : public KbgActiveNodeWriter {
protected:
	int m_Node;
public:
	KbgSingleNodeWriter(ostream* os, const Kbg* kbg, int node);
	virtual void write(const string& prefix, int value) const;
};

class KbgSymbolHash :	public hash_map<dlong, KbgNodeNameEntry*, HashSymbolHashKey, EqSymbolHashKey> {
public:
	MDVector* m_Main;
public:
	KbgSymbolHash(MDVector* main);
	KbgNodeNameEntry* try_get(dlong key) const;
	void add_item(dlong key, KbgNodeNameEntry* item);
	inline void write(ostream& os) const {
		MDVectorSimpleNodeWriter wrt(&os);
		write(&wrt);
	}
	void write(MDVectorNodeWriter* writer) const;
	void save(FILE* f) const;
	void load(FILE* f);
};

class KbgVectorKey : public MDVectorKey {
protected:
	int m_NbTempl;
public:
	KbgVectorKey();
	inline void setNbTempl(int nb) { m_NbTempl = nb; }
	virtual dlong getMainKey() = 0;
	virtual int computeNbTempl() = 0;
	virtual const char* getName() = 0;
	virtual int getArity() = 0;
	inline int getNbTempl() { return m_NbTempl; }
};

class KbgTemplateInfo {
protected:
	long m_FilePos;
	int m_ArrayID;
public:
	KbgTemplateInfo();
	KbgTemplateInfo(long pos);
	void init(long pos);
	inline void setFilePos(long pos) { m_FilePos = pos; }
	inline long getFilePos() { return m_FilePos; }
	inline void setArrayID(int idx) { m_ArrayID = idx; }
	inline int getArrayID() { return m_ArrayID; }
};

class KbgSet {
protected:
	string m_FName;
	int  m_Index;
	int  m_Options;
	int  m_ModelCount;
	int  m_Predicates;
	long m_PredListPos;
	MyOutputFile m_Models;
public:
	KbgSet(int idx);
	void ensureOpen();
	inline const string& getFName() const { return m_FName; }
	inline void setFName(const string& fname) { m_FName = fname; }
	inline int setOptions(int options) { m_Options = options; }
	inline int getOptions() { return m_Options; }
	inline int getOption(int option) { return (m_Options & option) != 0; }
	inline void setOption(int option) { m_Options |= option; }
	inline void open(const char* fname) { m_Models.open(fname); }
	inline void open(const string& fname) { m_Models.open(fname.c_str()); }
	inline void close() { m_Models.close(); }
	inline int hasFile() { return m_Models.isOpen(); }
	inline int getIndex() { return m_Index; }
	inline int getModelCount() { return m_ModelCount; }
	inline void incModelCount() { m_ModelCount++; }
	inline void setModelCount(int count) { m_ModelCount = count; }
	inline ostream& os() { return m_Models.get(); }
	inline int getPredicates() const { return m_Predicates; }
	inline void setPredicates(int preds) { m_Predicates = preds; }
	inline void setPredListPos(long pos) { m_PredListPos = pos; }
	inline long getPredListPos() { return m_PredListPos; }
};

class NodeExtraInfo {
protected:
	vector<int> m_Childs;
	int m_Root;
	int m_Depth;
	char m_PComp;
public:
	NodeExtraInfo();
	inline int getDepth() { return m_Depth; }
	inline void setDepth(int depth) { m_Depth = depth; }
	inline int getRoot() { return m_Root; }
	inline void setRoot(int root) { m_Root = root; }
	inline void addChild(int child) { m_Childs.push_back(child); }
	inline int getNbChilds() { return m_Childs.size(); }
	inline int getChild(int i) { return m_Childs[i]; }
	inline int getPComp() { return m_PComp; }
	inline void setPComp(int comp) { m_PComp = comp; }
};

#define KBG_RESET_ALL     1
#define KBG_INIT_ITER     2
#define KBG_SELECT_ALL    4
#define KBG_SELECT_SAME   8
#define KBG_SELECT_SOME   16
#define KBG_SELECT_WEIGHT 32

#define KBG_PHASE_ACTIVE    0
#define KBG_PHASE_EXPENSIVE 1
#define KBG_PHASE_CHEAP     2
#define KBG_PHASE_DONE      3

class IterStackElem {
protected:
	int m_Node;
	int m_Child;
public:
	IterStackElem(int node, int child);
	void init(int node, int child);
	inline int getNode() { return m_Node; }
	inline int getChild() { return m_Child; }
};

class Kbg {
protected:
	FILE* m_BinFile;
	my_vector<KbgTemplateInfo> m_Templates;
	vector<long> m_Clauses;
	my_vector<KbgNode> m_Nodes;
	vector<KbgSet*> m_Sets;
	vector<int> m_Parents;
	KbgSet* m_CrSet;
	PTermPtr m_Current;
	CPTermSymTable* m_PSymTab;
	GapicSymTable m_GSymTab;
	string m_FileName;
	CPrologArray m_PArray;
	KbgSymbolHash m_NameHash;
	MDVector m_NameDims;
	vector<int> m_Examples;
	vector<int> m_ChildInfo;
	int m_Unreachable;
	int m_MaxDepth;
	int m_Selected;
	int m_Constructed;
	unsigned long m_MemSize;
	unsigned long m_MemMax;
	unsigned long m_MemLimit;
	vector<NodeExtraInfo*> m_Extra;
	vector<int> m_CrExamples;
	int m_Phase;
	int m_Node;
	int m_Child;
	int m_ExID;
	int m_InTree;
	int m_ToConstruct;
	int m_ShouldGC;
	my_vector<IterStackElem> m_Stack;
	KbgNodeActive* m_CacheHead;
	KbgNodeActive* m_CacheTail;
public:
	Kbg();
	~Kbg();
	void load(GAPIC_term_ref fname);

	void reload(GAPIC_term_ref fname);

	void save();

	void postInit();

	int getNbTemplates();

	void getTemplate(GAPIC_int i, GAPIC_term_ref meta);

	void setTemplateData(GAPIC_int i, GAPIC_term_ref pp, GAPIC_term_ref data);

	int getNbClauses();

	void getClause(GAPIC_int i, GAPIC_term_ref clause);

	void getFileName(GAPIC_int i, GAPIC_term_ref fname);

	void checkTheoryAssert(GAPIC_term_ref name, GAPIC_term_ref pp, GAPIC_int* index, GAPIC_int* activate, GAPIC_int* retrieve);

	void checkTheoryConstruct(GAPIC_term_ref name, GAPIC_term_ref pp, GAPIC_int* index, GAPIC_int* activate);

	void checkTheoryActivate(GAPIC_int index, GAPIC_term_ref pp, GAPIC_int* todo);

	void checkNextExample(GAPIC_int* activate, GAPIC_term_ref pp, GAPIC_int* index);

	bool atEnd();

	void toAssert(GAPIC_int index, GAPIC_term_ref assert);

	void assertBeforeFindall(GAPIC_int index);

	void getAllDisjoint(GAPIC_int index, GAPIC_term_ref disjoint);

	void retrieveTheory(GAPIC_int index, GAPIC_term_ref pp, GAPIC_term_ref theory);

	void retrieveTheorySet(GAPIC_int index, GAPIC_term_ref pp, GAPIC_term_ref theory, GAPIC_int* set, GAPIC_int* id);

	void retrieveFindall(GAPIC_int index, GAPIC_term_ref pp, GAPIC_term_ref preds);

	void activateCompiledTheory(GAPIC_int index);

	void loadSimpleTheory(GAPIC_int index, GAPIC_term_ref pp);

	void loadTemplateTheory(GAPIC_int index, GAPIC_term_ref pp, GAPIC_term_ref templ);

	void loadMetaTheory(GAPIC_int index, GAPIC_term_ref theory);

	void deactivateTheory(GAPIC_term_ref name);

	void lockTheory(GAPIC_int index);

	void activateLockParents(GAPIC_int index, GAPIC_term_ref parents, GAPIC_int* todo);

	void cacheTheory(GAPIC_int name, GAPIC_term_ref pp, GAPIC_term_ref theory);

	void createArray(GAPIC_term_ref pp);

	void addGeneratedNode(GAPIC_int index, GAPIC_int setid, GAPIC_int flags, GAPIC_int foreach, GAPIC_term_ref vars, GAPIC_term_ref uses);

	void addCompiledNode(GAPIC_int index, GAPIC_int setid, GAPIC_int modelid, GAPIC_int flags, GAPIC_term_ref add);

	void setNodeParents(GAPIC_int index, GAPIC_term_ref add);

	int incModelCount(GAPIC_int set);

	void getCompiledSets(GAPIC_term_ref sets);

	void setPredicates(GAPIC_int set, GAPIC_term_ref pp, GAPIC_term_ref preds);

	void loadPredicateList(GAPIC_int setid, GAPIC_term_ref preds);

	void showInfo();

	void getTheories(GAPIC_int type, GAPIC_term_ref theories);

	void selectExamples(GAPIC_int type, GAPIC_term_ref examples);

	void retrieveNames(GAPIC_term_ref kind, GAPIC_term_ref names);

	void showExampleInfo(GAPIC_int idx);

	int tryAddNode(GAPIC_term_ref name) {

		return addTermNodeIndex(name);
	}
	int tryGetNode(GAPIC_term_ref name) {

		return getTermNodeIndex(name);
	}
	void showActiveNodes() {

		KbgActiveNodeWriter wrt(&cerr, this);
		m_NameHash.write(&wrt);
	}
	void showNodes() {

		 
		KbgActiveNodeWriter wrt(&cerr, this);
		m_NameHash.write(&wrt);
	}
	int getNbSets() {

		return m_Sets.size();
	}
	int getTotalExamples() {

		return m_Examples.size();
	}
	int getNbExamples() {

		return m_CrExamples.size();
	}
	int getTotalTheories() {

		return m_Nodes.size();
	}
	int getMaxMemory() {

		return (int)(m_MemMax/1024);
	}
	inline void showSingleNode(int node) {
		KbgSingleNodeWriter wrt(&cerr, this, node);
		m_NameHash.write(&wrt);
	}
	bool shouldGc();

	bool isReachable(GAPIC_term_ref name);

	void showNodeInfo(int nodeid);
	void addSet(int idx);
	void initParents(int nodeid, GAPIC_term_ref uses);
	void initParents(int nodeid, PTermList* uses);
	void selectTheoryAndCo(KbgNode* node);
	int checkFullyCompiled(KbgNode* node);
	int checkTheoryActivate(KbgNode* node, GAPIC_term_ref pp, int idx);
	int shouldConstructParents(KbgNode* node);
	void activateCompiledTheoryImpl(KbgNode* node, int nodeid);
	void constructCompiledTheory(KbgNode* node, int nodeid);
	void constructSimpleTheory(KbgNode* node, int nodeid, GAPIC_term_ref pp, GAPIC_term_ref theory);
	void constructTemplateTheory(KbgNode* node, GAPIC_term_ref pp, GAPIC_term_ref theory);
	void constructMetaTheory(KbgNode* node, int nodeid, GAPIC_term_ref theory);
	void updateReachableFlag(KbgNode* node);
	void updateMaxDepth(int idx);
	int updateParentsCompiled(int idx);
	int getNodeDepth(KbgNode* node);
	int getNodeRoot(KbgNode* node);
	int getNodeExampleID(KbgNode* node);
	int nextTheoryActive();
	int nextTheoryExpensive();
	int nextTheoryCheap();
	int nextTheoryTree();
	int nextTheory();
	void nextTheoryTreePop();
	void closeSetFiles();
	void setSemiLock(int nodeid, int semi);
	void unlockParents(KbgNode* node);
	int getTermNodeIndex(GAPIC_term_ref name);
	int addTermNodeIndex(GAPIC_term_ref name);
	void createBinFile(const char* fname, int reload);
	KbgNodeActive* activateNode(KbgNode* node, int id);
	void moveNodeFront(KbgNodeActive* active);
	void deactivateTheoryImpl(int nodeid, GAPIC_term_ref pp, GAPIC_term_ref* scr);
	void doMemoryManage(KbgNode* node, GAPIC_term_ref pp);
	KbgNode* addNode(PTerm* name);
	KbgNode* addNode(KbgVectorKey* key);
	int addNodeIndex(KbgVectorKey* key);
	int getNodeIndex(KbgVectorKey* key);
	int addNodeIndex(PTerm* name);
	ostream & Print(ostream & strm) const;
	FILE* getFile() { return m_BinFile; }
	inline void setCurrent(PTerm* term) { m_Current = term; }
	inline PTerm* getCurrent() { return m_Current.get(); }
	inline void addTemplate(long templ) {
		m_Templates.inc()->init(templ);
	}
	inline void addClause(long clause) { m_Clauses.push_back(clause); }
	inline PTermSymTable* getSymTable() { return m_PSymTab; }
	inline GapicSymTable* getGapicTable() { return &m_GSymTab; }
	inline void setFileName(const string& fname) { m_FileName = fname; }
	inline const string& getFileName() const { return m_FileName; }
	inline KbgNode* getNode(int index) { return m_Nodes.get(index); }
	inline const KbgNode* getNodeConst(int index) const { return m_Nodes.getConst(index); }
	inline KbgTemplateInfo* getTemplateInfo(int idx) { return m_Templates.get(idx); }
	inline KbgSet* getCurrentSet() { return m_CrSet; }
	inline void setCurrentSet(KbgSet* set) { m_CrSet = set; }
	inline KbgSet* getSet(int idx) { return m_Sets[idx]; }
	inline int getParentListSize() const { return m_Parents.size(); }
	inline void pushParentIndex(int idx) { m_Parents.push_back(idx); }
	inline void setParentCount(int idx, int count) { m_Parents[idx] = count; }
	inline int* getParents(int idx) { return &m_Parents[idx]; }
	inline int* getChilds(int idx) { return &m_ChildInfo[idx]; }
	inline unsigned long getMemory() { return m_MemSize; }
	inline void addMemory(int add) { m_MemSize += add; }
	inline void delMemory(int del) { m_MemSize -= del; }
	inline KbgNodeActive* getCacheTail() { return m_CacheTail; }
	inline KbgNodeActive* getCacheHead() { return m_CacheHead; }
	inline KbgNodeActive* getLRU() { return m_CacheTail->getPrev(); }
	inline KbgNodeActive* getMRU() { return m_CacheHead->getNext(); }
};

typedef GapicNonExMutCollectable<Kbg, &KbgType> KbgCollectable;

#endif
