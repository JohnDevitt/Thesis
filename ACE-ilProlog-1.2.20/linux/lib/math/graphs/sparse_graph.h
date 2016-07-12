#ifndef SPARSEGRAPH_INC
#define SPARSEGRAPH_INC


#include "freelist_vector.h"
#include "graph_structure.h"

class BiDirGraphNode : public freelist_vector_item {
 private:
  int m_deleted:1;
  typedef vector<graph_edge_id> in_edge_vector;
  typedef vector<graph_edge_id> out_edge_vector;
  in_edge_vector m_out_edges;
  out_edge_vector m_in_edges;
 public:  
  static const int delete_marking=1;
  void add_in_edge(graph_edge_id e) {m_in_edges.push_back(e);}
  void delete_in_edge(graph_edge_id e);
  void add_out_edge(graph_edge_id e) {m_out_edges.push_back(e);}
  void delete_out_edge(graph_edge_id e);
  inline int is_not_deleted() const {return !m_deleted;}
  inline int is_deleted() const {return m_deleted;}
  inline void set_deleted(bool i) {m_deleted=i;}
  typedef out_edge_vector::const_iterator out_edge_iterator;
  typedef in_edge_vector::const_iterator in_edge_iterator;
  inline out_edge_iterator out_begin() const {
    return m_out_edges.begin();}
  inline out_edge_iterator out_end() const {
    return m_out_edges.end();}
  inline in_edge_iterator in_begin() const {
    return m_in_edges.begin();}
  inline in_edge_iterator in_end() const {
    return m_in_edges.end();}
  BiDirGraphNode() : m_deleted(0) {}
};

class BiDirGraphEdge : public freelist_vector_item {
 private:
  graph_node_id m_from;
  graph_node_id m_to;
 public:
  static const int delete_marking=1;
  graph_node_id from_node() const {return m_from;}
  graph_node_id to_node() const {return m_to;}
  void set_from_node(graph_node_id n) {m_from=n;}
  void set_to_node(graph_node_id n) {m_to=n;}
  void set_deleted(int i) {if(i) m_from=(unsigned)(-1); else m_from=0;}
  int is_deleted() const { return (m_from == ((unsigned)(-1)));}
  int is_not_deleted() const { return (m_from != ((unsigned)(-1)));}
  BiDirGraphEdge() {}
};

class BiDirSparseGraph : public Graph {
 public:
  const static int safe_code=1;
 private:
  typedef freelist_vector<BiDirGraphNode> node_vector;
  typedef freelist_vector<BiDirGraphEdge> edge_vector;
  node_vector m_nodes; 
  edge_vector m_edges; 
  int check_nodel_node_id(const char *func, graph_node_id id) const {
    return m_nodes.check_nodel_id(func,id);}
  int check_nodel_edge_id(const char *func, graph_edge_id id) const {
    return m_nodes.check_nodel_id(func,id);}
  void _remove_edge(graph_edge_id edge);
 public:
  typedef BiDirGraphNode::out_edge_iterator out_edge_iterator;
  typedef BiDirGraphNode::in_edge_iterator in_edge_iterator;
  unsigned node_count() const {return m_nodes.size();}
  unsigned edge_count() const {return m_edges.size();}
  graph_node_id add_node() {return m_nodes.add();}
  void delete_node(graph_node_id node);
  graph_edge_id add_edge(graph_node_id from, graph_node_id to);
  void delete_edge(graph_edge_id edge);
  graph_node_id edge_from(graph_edge_id id) const {
    return m_edges[id].from_node();}
  graph_node_id edge_to(graph_edge_id id) const {
    return m_edges[id].to_node();}
  void edge_from_to(graph_edge_id id, 
		    graph_node_id* from, graph_node_id *to) const {
    *from=m_edges[id].from_node();*to=m_edges[id].to_node();}

  out_edge_iterator node_out_begin(int node) const {
    if(safe_code) if(!check_nodel_node_id("node_out_begin",node)) throw;
    return m_nodes[node].out_begin();
  }
  out_edge_iterator node_out_end(int node) const {
    if(safe_code) if(!check_nodel_node_id("node_out_begin",node)) throw;
    return m_nodes[node].out_end();
  }
  in_edge_iterator node_in_begin(int node) const {
    if(safe_code) if(!check_nodel_node_id("node_in_begin",node)) throw;
    return m_nodes[node].in_begin();
  }
  in_edge_iterator node_in_end(int node) const {
    if(safe_code) if(!check_nodel_node_id("node_in_begin",node)) throw;
    return m_nodes[node].in_end();
  }

  typedef node_vector::const_iterator node_iterator;
  int node_iterator_id(const node_iterator& n) const {
    return m_nodes.iterator_id(n);}
  node_iterator node_begin() const {
    return m_nodes.begin();}
  node_iterator node_end() const {
    return m_nodes.end();}

  typedef edge_vector::const_iterator edge_iterator;
  int edge_iterator_id(const edge_iterator& n) const {
    return m_edges.iterator_id(n);}
  edge_iterator edge_begin() const {
    return m_edges.begin();}
  edge_iterator edge_end() const {
    return m_edges.end();}

  typedef VGraphOutEdgeIterator<out_edge_iterator> v_out_edge_iterator;
  typedef VGraphInEdgeIterator<in_edge_iterator> v_in_edge_iterator;
  typedef VGraphEdgeIterator<edge_iterator> v_edge_iterator;
  typedef VGraphNodeIterator<node_iterator> v_node_iterator;

  virtual unsigned v_node_count() const;
  virtual unsigned v_edge_count() const;
  virtual graph_node_id v_add_node();
  virtual void v_delete_node(graph_node_id node);
  virtual graph_edge_id v_add_edge(graph_node_id from, graph_node_id to);
  virtual void v_delete_edge(graph_edge_id edge);
  virtual GraphOutEdgeIterator* v_node_out_begin(int node) const;
  virtual GraphOutEdgeIterator* v_node_out_end(int node) const;
  virtual GraphInEdgeIterator* v_node_in_begin(int node) const;
  virtual GraphInEdgeIterator* v_node_in_end(int node) const;
  virtual GraphNodeIterator* v_node_begin() const;
  virtual GraphNodeIterator* v_node_end() const;
  virtual unsigned v_node_iterator_id(const GraphNodeIterator* n) const;
  virtual GraphEdgeIterator* v_edge_begin() const;
  virtual GraphEdgeIterator* v_edge_end() const;
  virtual unsigned v_edge_iterator_id(const GraphEdgeIterator* n) const;

  virtual ostream& write_edges(ostream& os) const;
  virtual ostream& write_nodes(ostream& os) const;
  virtual ostream& write(ostream& os) const;
  virtual int check_consistency(int talking) const;
  BiDirSparseGraph() {}
  virtual ~BiDirSparseGraph();
};

#endif
