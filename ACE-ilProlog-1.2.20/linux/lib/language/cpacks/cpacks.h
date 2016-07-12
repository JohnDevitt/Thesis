
#ifndef CPACKS_INC
#define CPACKS_INC



extern gapic_type CPackMetaImplType;
extern gapic_type CPackMetaStatType;
extern gapic_type CPackMetaArrayType;

#define CPACK_NODE_SUBSET 0
#define CPACK_QUERY_NODE_SUBSET 1

#define CPACK_QUERY_ID_ATTR 0
#define CPACK_QUERY_PARENT_ATTR 1

#define CPACK_OPTION_EXECUTABLE_ROOT  1
#define CPACK_OPTION_ADD_UPDATE_STAT  2
#define CPACK_DOLLAR_RENAME_PREDS     4
#define CPACK_OPTION_ADD_ARRAY        8
#define CPACK_OPTION_ADD_TRUE         16
#define CPACK_OPTION_ADD_LEAF_ID      32
#define CPACK_OPTION_ADD_QUERY_ID     64

#define CPACK_PREFIX_MODE_BROOM  0
#define CPACK_PREFIX_MODE_WARMR  1

  

class CPackImpl;

typedef struct {
	NodeID m_NodeID;
		    
	int m_TotalChildren;
		     
	NodeID  m_FirstChild;
		     
	NodeID* m_ChildRecord;
		     
	NodeID m_ID;
	int m_ChildCount;
	unsigned char m_MySuccess;
	unsigned char m_SubSuccess;
} CPackMetaNodeData;

extern gapic_type CPackWeightStatType;

class CPackWeightStat : public ExdistrStatistic {
protected:
	GAPIC_int  m_ID;
	CPackImpl* m_Pack;
	CTreeNodeSubset* m_Subset;
public:
	CPackWeightStat();
	CPackWeightStat(GAPIC_term_ref ref);
	virtual ~CPackWeightStat();
	virtual void resize(GAPIC_int n, GAPIC_int s);
	virtual void reset();
	virtual void add(const ExdistrStatistic* s);
	virtual void scale(GAPIC_float s);
	virtual void update(GAPIC_int query, GAPIC_float weight);
	virtual void to_prolog(GAPIC_int idx, GAPIC_int query, GAPIC_term_ref prolog);
	virtual ExdistrStatistic* clone() const;
	virtual ostream& PrintStat(ostream& os) const;
};

extern gapic_type CPackCovlistStatType;

class CPackCovlistStat : public CPackWeightStat {
private:
	GAPIC_int m_List;
	TargetValue* m_Target;
public:
	CPackCovlistStat();
	CPackCovlistStat(GAPIC_term_ref ref);
	virtual ~CPackCovlistStat();
	virtual void update(GAPIC_int query, GAPIC_float weight);
};

class CPackPackBuilder {
protected:
	CPackImpl* m_Pack;
	int m_Options;
	GAPIC_term_ref m_PTerms;
	GAPIC_term_ref m_Scratch;
	GAPIC_term_ref m_Head, m_Tail;
	PrefixRenamer* m_Renamer;
public:
	CPackPackBuilder(CPackImpl* pack, GAPIC_term_ref pp, int opts);
	virtual ~CPackPackBuilder();
	void createPrologPackRecursive(NodeID node, GAPIC_term_ref child);
	void createPrologPack(GAPIC_term_ref result);
	void copyConjunctionFromTail(GAPIC_term_ref crpack);
	void copyConjunctionNoLeaves(GAPIC_term_ref crpack);
	void addChildToPack(GAPIC_term_ref crpack, GAPIC_term_ref child);
	NodeID getNextChild(CTreeNode* data, int* childpos);
	void getMainTermInTail(CTreeNode* data);
	int getNbChildren(CTreeNode* data);
	void deleteRenamer();
	int needsExtraLeaf(CTreeNode* node);
	virtual void createExtraLeaf(NodeID node, GAPIC_term_ref crpack);
	virtual int addInternalLeaves();
	virtual int addFinalLeaves();
	virtual void initializeHead(GAPIC_term_ref crpack, GAPIC_term_ref result);
};

class CPPBuilderUpdateStats : public CPackPackBuilder {
protected:
	GAPIC_term_ref m_StatVar, m_WeightVar;
	GAPIC_functor  m_UpdateStat, m_PackCall;
public:
	CPPBuilderUpdateStats(CPackImpl* pack, GAPIC_term_ref pp, int opts);
	virtual ~CPPBuilderUpdateStats();
	virtual void createExtraLeaf(NodeID node, GAPIC_term_ref crpack);
	virtual int addInternalLeaves();
	virtual int addFinalLeaves();
	virtual void initializeHead(GAPIC_term_ref crpack, GAPIC_term_ref result);
};

class CPPBuilderSuccessArray : public CPackPackBuilder {
protected:
	GAPIC_term_ref m_ArrayVar;
	GAPIC_functor  m_UpdateArray, m_PackCall;
public:
	CPPBuilderSuccessArray(CPackImpl* pack, GAPIC_term_ref pp, int opts);
	virtual ~CPPBuilderSuccessArray();
	virtual void createExtraLeaf(NodeID node, GAPIC_term_ref crpack);
	virtual int addInternalLeaves();
	virtual int addFinalLeaves();
	virtual void initializeHead(GAPIC_term_ref crpack, GAPIC_term_ref result);
};

class CPPBuilderAddTrue : public CPackPackBuilder {
public:
	CPPBuilderAddTrue(CPackImpl* pack, GAPIC_term_ref pp, int opts);
	virtual ~CPPBuilderAddTrue();
	virtual void createExtraLeaf(NodeID node, GAPIC_term_ref crpack);
	virtual int addInternalLeaves();
	virtual int addFinalLeaves();
};

class CPPBuilderLeafIDs : public CPackPackBuilder {
protected:
	GAPIC_functor m_IDFunctor;
public:
	CPPBuilderLeafIDs(CPackImpl* pack, GAPIC_term_ref pp, int opts);
	virtual ~CPPBuilderLeafIDs();
	virtual void createExtraLeaf(NodeID node, GAPIC_term_ref crpack);
	virtual int addFinalLeaves();
};

class CPPBuilderQueryIDs : public CPackPackBuilder {
protected:
	GAPIC_functor m_IDFunctor;
public:
	CPPBuilderQueryIDs(CPackImpl* pack, GAPIC_term_ref pp, int opts);
	virtual ~CPPBuilderQueryIDs();
	virtual void createExtraLeaf(NodeID node, GAPIC_term_ref crpack);
	virtual int addFinalLeaves();
};

extern gapic_type CPackImplType;


class CPackImpl : public CTreeImpl {
protected:
	vector<NodeID> m_Queries;
	IntVectorHash<NodeID> m_Hash;
	MainTermID m_ExecRoot;
	int m_BroomPrefix;
	int m_SmartMode;
	int m_CoverageLists;
	int m_QueryParent;
	int m_PrefixMode;
public:
	CPackImpl();
	CPackImpl(GAPIC_term_ref init);
	virtual ~CPackImpl();
	void clearQueries();

	void updateQueries();

	int setQuery(GAPIC_int node);

	void markQueryParent(GAPIC_int node);

	void setQueryParent(GAPIC_int node);

	void getQuery(GAPIC_term_ref pp, GAPIC_int qid, GAPIC_term_ref query);

	void getSmartQuery(GAPIC_term_ref pp, GAPIC_int qid, GAPIC_term_ref query);

	void getExecQuery(GAPIC_term_ref pp, GAPIC_int qid, GAPIC_term_ref query);

	void setBroomPrefix(GAPIC_term_ref pref);

	void setBroomPrefixNode(GAPIC_int nodeid, GAPIC_term_ref pref);

	void setSmartMode(GAPIC_int mode);

	void setPrefixMode(GAPIC_int mode);

	int getQueryEndNode(GAPIC_int query);

	void getNodeQuery(GAPIC_term_ref pp, GAPIC_int node, GAPIC_term_ref query);

	void getSmartNodeQuery(GAPIC_term_ref pp, GAPIC_int node, GAPIC_term_ref query);

	void getExecNodeQuery(GAPIC_term_ref pp, GAPIC_int qid, GAPIC_term_ref query);

	void setExecutableRoot(GAPIC_term_ref pp, GAPIC_term_ref root);

	void getExecutableRoot(GAPIC_term_ref pp, GAPIC_term_ref root);

	void createPrologPack(GAPIC_term_ref pp, GAPIC_int options, GAPIC_term_ref result);

	void setNodeHash(GAPIC_int node, GAPIC_int hash);

	void clearNodeHash();

	int selectFromHash(GAPIC_int hash);

	int selectFirstNodeFromHash(GAPIC_int hash);

	CTreeNode* getQueryNode(int qid);
	int getNbQueries() {

		return m_Queries.size();
	};
	void coverageLists(GAPIC_term_ref covlists);

	inline int hasCoverageLists() {
		return m_CoverageLists;
	}
	inline vector<NodeID>* getHashNodes(int hash) {
		return m_Hash.try_get(hash);
	};	
	inline MainTermID getExecutableRootID() { return m_ExecRoot; }
	inline NodeID getQueryNodeID(int query) { return m_Queries[query]; }
	inline void addQuery(NodeID node) { m_Queries.push_back(node); };
	inline int getBroomPrefix() { return m_BroomPrefix; }
	inline void setBroomPrefix(int prefix) { m_BroomPrefix = prefix; }
	ostream& Print(ostream& os) const;
private:
	void setBroomPrefix();
	void getQuery(CTreeNode* pnode, GAPIC_term_ref pp, GAPIC_term_ref query);
	void getSmartQueryImpl(NodeID node, GAPIC_term_ref pp, GAPIC_term_ref query);
	void getExecQueryImpl(NodeID node, GAPIC_term_ref pp, GAPIC_term_ref query);
	void getSmartExecQuery(NodeID node, GAPIC_term_ref pp, GAPIC_term_ref query);
	void updateQueriesRecursive(NodeID node);
};

extern gapic_type CADPackImplType;

class CADPackNodeInfo {
protected:
	IntVectorHash<int> m_Hash;
	vector<int>* m_CoverageList;
	char m_ShouldDelete;
public:
	CADPackNodeInfo();
	~CADPackNodeInfo();
	void addNodeHash(unsigned int hash, int idx);
	inline vector<int>* getHashNodes(int hash) {
		return m_Hash.try_get(hash);
	};
};


class CADPackImpl : public CTreeImpl {
protected:
	vector<CADPackNodeInfo> m_NodeInfo;
public:
	CADPackImpl(GAPIC_term_ref ref);
	int merge(GAPIC_term_ref pp, GAPIC_term_ref query);

	void addCoverageList(CTreeImpl* pack, GAPIC_int query, GAPIC_int node);

	void handleCoverageLists();

	CADPackNodeInfo* createNodeInfo(NodeID node);
	inline CADPackNodeInfo* getNodeInfo(NodeID node) { return &m_NodeInfo[node]; }
	inline void addNodeHash(NodeID parent, unsigned int hash, int idx) {
		getNodeInfo(parent)->addNodeHash(hash, idx);
	}
};


class CPackMetaImpl {
protected:
	CPackImpl* m_Pack;
	unsigned int m_NbNodes;
	CPackMetaNodeData* m_NodeData;
	NodeID  m_CurrNodeID;
	NodeID* m_CurrChildData;
public:
	CPackMetaImpl();
	CPackMetaImpl(GAPIC_term_ref init);
	virtual ~CPackMetaImpl();
	void initMeta();
	void updatePack();

	void reset();

	void setNewPack(CTreeImpl* pack);

	virtual void initNode(NodeID node, CPackMetaNodeData* data) = 0;
	virtual void onSuccess(CPackMetaNodeData* data) = 0;
	ostream& Print(ostream& os) const;
	inline int getNbNodes() { return m_NbNodes; }
	inline CPackMetaNodeData* getRootData() { return m_NodeData; }
	inline CPackMetaNodeData* getNode(NodeID node) { return &m_NodeData[node]; }
	inline void getMainTerm(GAPIC_term_ref pp, NodeID node, GAPIC_term_ref conj) {
		m_Pack->getMainTerm(pp, node, conj);
	}
	inline void setPack(CPackImpl* pack) {
		m_Pack = pack;
	}
protected:
	inline NodeID allocNodeData() { return m_CurrNodeID++; }
	NodeID* allocChildren(unsigned int count);
	void initializeRecursive(NodeID node, NodeID target);
	void initializeNodeData();
};

typedef GapicNonExMutCollectable<CPackMetaImpl, &CPackMetaImplType> CPackMetaImplCollectable;


class CPackMetaStat : public CPackMetaImpl {
protected:
	ExdistrStatistic* m_Statistic;
public:
	CPackMetaStat();
	CPackMetaStat(GAPIC_term_ref init);
	virtual ~CPackMetaStat();
	void setPosStat(ExdistrStatistic* stat) {

		m_Statistic = stat;
	};
	inline ExdistrStatistic* getStatistic() { return m_Statistic; }
	virtual void initNode(NodeID node, CPackMetaNodeData* data);
	virtual void onSuccess(CPackMetaNodeData* data);
};


class CPackMetaArray : public CPackMetaImpl {
protected:
	int_array* m_Array;
public:
	CPackMetaArray();
	CPackMetaArray(GAPIC_term_ref init);
	virtual ~CPackMetaArray();
	void setArray(int_array* array) {

		m_Array = array;
	};
	inline int_array* getArray() { return m_Array; }
	virtual void initNode(NodeID node, CPackMetaNodeData* data);
	virtual void onSuccess(CPackMetaNodeData* data);
};



#endif
