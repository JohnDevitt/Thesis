
#ifndef CTREE_INC
#define CTREE_INC

#define CTREE_NODE_SUBSET 0



G_SO extern gapic_type CTreeImplType;

class CTreeNode;
class CTreeNodeSubset;
class IntVoidPtrHash;

typedef unsigned int NodeID;
typedef unsigned int ChildID;
typedef unsigned int SubsetID;
typedef unsigned int MainTermID;
typedef unsigned char SelectedID;

class CPTreeInfo {
protected:
	GAPIC_term_ref m_Scratch1, m_Scratch2, m_Scratch3;
	GAPIC_term_ref m_Tree;
public:
	CPTreeInfo(GAPIC_term_ref tree);
	inline GAPIC_term_ref getTemp1() { return m_Scratch1; };
	inline GAPIC_term_ref getTemp2() { return m_Scratch2; };
	inline GAPIC_term_ref getTemp3() { return m_Scratch3; };
	inline GAPIC_term_ref getTree() { return m_Tree; };
};

class CTreeNode {
protected:
	ChildID m_Index;
	NodeID m_Parent;
	SelectedID m_Selected;
	MainTermID m_MainTermID;
	vector<NodeID> m_Children;
public:
	CTreeNode();
	void clearNode();
	inline int getNbChildren() { return m_Children.size(); };
	inline NodeID getChild(int i) { return m_Children[i]; };
	inline void setChild(int i, NodeID id) { m_Children[i] = id; }
	inline void addChild(NodeID id) { m_Children.push_back(id); };
	inline void removeAllChildren() { m_Children.clear(); }
	inline MainTermID getMainTermID() { return m_MainTermID; };
	inline void setMainTermID(MainTermID id) { m_MainTermID = id; };
	inline void setParent(NodeID id) { m_Parent = id; };
	inline NodeID getParent() { return m_Parent; };
	inline void setChildIndex(ChildID idx) { m_Index = idx; }
	inline ChildID getChildIndex() { return m_Index; }
	inline void decChildIndex() { m_Index--; }
	inline void delChild(int idx) { m_Children.erase(m_Children.begin()+idx); }
	inline vector<NodeID>::const_iterator firstChild() { return m_Children.begin(); }
	inline vector<NodeID>::const_iterator lastChild() { return m_Children.end(); }
	inline SelectedID getSelected() { return m_Selected; }
	inline void setSelected(SelectedID sel) { m_Selected = sel; }
};

class IntIntHash : public hash_map<int, int, hash_int_hash_key, eq_int_hash_key> {
public:
	int try_get(int key);
	void add_item(int key, int elem);
};

extern gapic_type CTreeSubsetIterType;

class CTreeSubsetIter {
protected:
	IntIntHash::const_iterator m_Current;
	IntIntHash::const_iterator m_End;
public:
	CTreeSubsetIter();
	CTreeSubsetIter(GAPIC_term_ref ref);
	bool isDone();

	int getNextNode();

	ostream& Print(ostream& os) const;
};

typedef GapicNonExMutCollectable<CTreeSubsetIter, &CTreeSubsetIterType> CTreeSubsetIterCollectable;

class CTreeImpl;

class CLASS_SO CTreeNodeSubset {
protected:
	int m_Index;
	int m_NewKey;
	IntIntHash m_Nodes;
	GapicBINTable m_Attributes;
	vector<int> m_FreeList;
	CTreeImpl* m_Tree;
public:
	CTreeNodeSubset(int index, CTreeImpl* tree);
	void addToSubset(NodeID node);
	void removeFromSubset(NodeID node, CPrologArray* array);
	G_SO void removeAllElements(CPrologArray* array);
	int contains(NodeID node);
	void getAttribute(NodeID node, AttrID id, GAPIC_term_ref value);
	void setAttribute(NodeID node, AttrID id, GAPIC_term_ref value);
	int getIntAttribute(NodeID node, AttrID id);
	G_SO int getIntAttributeTest(NodeID node, AttrID id);
	G_SO void setIntAttribute(NodeID node, AttrID id, int value);
	G_SO void addDoubleAttribute(NodeID node, AttrID id, double add);
	void setAllAttribute(AttrID id, GAPIC_term_ref value);
	void initAllAttribute(CTreeNodeSubset* set, AttrID id);
	void initAttribute(NodeID node, AttrID id);
	G_SO void addDoubleAttributeExpand(int row, AttrID id, double add);
	G_SO void addIntVectorAttributeExpand(int row, AttrID id, int value);
	G_SO vector<int>* getIntVectorAttribute(int row, AttrID id);
	void setAttributeExpand(int row, AttrID id, GAPIC_term_ref value);
	G_SO int getByteAttributeExpand(int row, AttrID id);
	G_SO int getAttributeRow(NodeID node);
	int getAttributeRowExpand(NodeID node);
	G_SO void selectAllNodes();
	G_SO void selectNodesByID(AttrID id);
	void selectNodesByIDRec(NodeID node, AttrID id);
	void tryClearTermAttrs(int row, CPrologArray* array);
	void printSizeInfo(ostream& os) const;
	inline void defineAttribute(const char* name, const char* type, int len, AttrID id) {
		m_Attributes.defineAttribute(name, type, len, id);
	}
	inline void showAttributes(ostream& out, int tab1, int tab2) {
		m_Attributes.showAttributes(out, tab1, tab2);
	}
	inline const string& getName() const { return m_Attributes.getName(); }
	inline void setName(const char* name) { m_Attributes.setName(name); }
	inline int getIndex() { return m_Index; }
	inline void setIndex(int index) { m_Index = index; }
	inline int getNodeIndex(NodeID node) { return m_Nodes.try_get(node); }
	inline void expandRows(int rows) { m_Attributes.expandRows(rows); }
	inline int getNbRows() { m_Attributes.getNbRows(); }
	inline int getNbNodes() const { return m_Nodes.size(); }
	inline TableHeader* getHeader() { return m_Attributes.getHeader(); }
	inline int getIntAttributeImpl(int row, int col) { return m_Attributes.getIntAttribute(row, col); }
	inline void getAttributeString(int row, int col, string& strg) { m_Attributes.getStringValue(row, col, strg); }
	inline void tryClearRow(int row) { m_Attributes.tryClearRow(row); }
	inline GapicBINTable* getAttributes() { return &m_Attributes; }
	inline IntIntHash::const_iterator getStartNodeIter() { return m_Nodes.begin(); }
	inline IntIntHash::const_iterator getEndNodeIter() { return m_Nodes.end(); }
};

class CLASS_SO CTreeImpl : public GapicIDObject {
protected:
	CPrologArray m_PArray;
	vector<CTreeNode> m_Nodes;
	vector<CTreeNodeSubset*> m_Subsets;
	vector<NodeID> m_FreeList;
	int m_Temp;
public:
	CTreeImpl();
	CTreeImpl(GAPIC_term_ref init);
	virtual ~CTreeImpl();
	void setMainTerm(GAPIC_int node, GAPIC_term_ref conj, GAPIC_term_ref pp1, GAPIC_term_ref pp2);

	G_SO void getMainTerm(GAPIC_term_ref pp, GAPIC_int node, GAPIC_term_ref conj);

	G_SO int addChild(GAPIC_int parent);

	G_SO int getChild(GAPIC_int parent, GAPIC_int index);

	int getParent(GAPIC_int child);

	int getDepth(GAPIC_int node);

	G_SO int getNbChildren(GAPIC_int parent);

	void delNode(GAPIC_int node);

	void getNodePath(GAPIC_int node, GAPIC_term_ref path);

	void getNodePathID(GAPIC_int node, GAPIC_term_ref path);

	void showSchema();

	void showAttributes(GAPIC_int node, GAPIC_term_ref pp);

	void defineSubset(GAPIC_term_ref name, GAPIC_int subset);

	void clearSubset(GAPIC_int subset);

	void defineAttribute(GAPIC_term_ref name, GAPIC_term_ref type, GAPIC_int subset, GAPIC_int id);

	G_SO void addToSubset(GAPIC_int node, GAPIC_int subset);

	G_SO bool isInSubset(GAPIC_int node, GAPIC_int subset);

	void removeFromSubset(GAPIC_int node, GAPIC_int subset);

	bool getSubsetIndex(GAPIC_int node, GAPIC_int subset, GAPIC_int* row);

	int getSubsetSize(GAPIC_int subset);

	void setAttribute(GAPIC_int node, GAPIC_int subset, GAPIC_int id, GAPIC_term_ref value);

	void getAttribute(GAPIC_int node, GAPIC_int subset, GAPIC_int id, GAPIC_term_ref value);

	void setTermAttribute(GAPIC_int node, GAPIC_int subset, GAPIC_int id, GAPIC_term_ref pp1, GAPIC_term_ref value);

	void getTermAttribute(GAPIC_int node, GAPIC_int subset, GAPIC_int id, GAPIC_term_ref pp1, GAPIC_term_ref value);

	void setAllAttribute(GAPIC_int subset, GAPIC_int id, GAPIC_term_ref value);

	void initAttribute(GAPIC_int set1, GAPIC_int set2, GAPIC_int id);

	void selectFromArray(int_array* array);

	void selectFromSubset(GAPIC_int subset);

	void selectFromSubsetByAttr(GAPIC_int subset, GAPIC_int attr);

	void getSelectedNodes(GAPIC_term_ref nodes);

	void deleteUnselected();

	G_SO void unSelectAll();

	G_SO void selectAll();

	G_SO void selectPath(GAPIC_int node);

	G_SO void unselectNode(GAPIC_int node);

	void getSetInteger(GAPIC_term_ref value);

	int getChildIndex(GAPIC_int node) {

		return getNode(node)->getChildIndex();
	}
	NodeID getFreeNode();
	void G_SO setMainTerm(NodeID node, GAPIC_term_ref conj, GAPIC_term_ref pp);
	void delNodeFast(NodeID node);
	NodeID G_SO insertNode(NodeID parent);
	void selectSubTree(CTreeNode* node);
	void unSelectSubTree(CTreeNode* node);
	G_SO void selectRoot();
	void deleteUnselected(NodeID id);
	void countSelectedNodes(CTreeNode* node, unsigned int* count);
	G_SO unsigned int getNbSelectedNodes();
	void computeSelectedNodes(NodeID node, GapicListCons* list);
	void selectAllNodesRec(NodeID node);
	inline void removeFromSubsetImpl(NodeID node, SubsetID subset) {
		m_Subsets[subset]->removeFromSubset(node, &m_PArray);
	}
	inline void addToSubsetImpl(NodeID node, SubsetID subset) {
		m_Subsets[subset]->addToSubset(node);
	}
	inline bool isInSubsetImpl(NodeID node, SubsetID subset) {
		return m_Subsets[subset]->contains(node);
	}
	inline CTreeNode* getNode(int i) { return &m_Nodes[i]; };
	inline int getNbNodes() const { return m_Nodes.size(); }
	inline CTreeNodeSubset* getSubset(int i) { return m_Subsets[i]; }
	inline const CTreeNodeSubset* getSubsetPtr(int i) const { return m_Subsets[i]; }
	inline int getNbSubsets() const { return m_Subsets.size(); }
	inline int isValidNode(NodeID node) { return node >= 0 && node < getNbNodes(); }
	inline int isValidSubset(SubsetID subset) { return subset >= 0 && subset < m_Subsets.size() && m_Subsets[subset] != NULL; }
	inline void getMainTermImpl(MainTermID id, GAPIC_term_ref pp, GAPIC_term_ref result) {
		m_PArray.get(id, pp, result);
	}
	NodeID getParentImpl(NodeID child);
	G_SO int getDepthImpl(NodeID node);
	void expandSubsets(SubsetID subset);
	G_SO void defineSubset(const char* name, SubsetID subset);
	G_SO void defineAttribute(const char* name, const char* type, SubsetID subset, AttrID id);
	G_SO ostream& Print(ostream& os) const;
};

typedef GapicNonExMutCollectable<CTreeImpl, &CTreeImplType> CTreeImplCollectable;

#endif
