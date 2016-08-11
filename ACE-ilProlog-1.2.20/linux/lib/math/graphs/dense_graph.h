#ufndef DENSE_GRAPH_INC
#define DENSE_GRAPH_INC

#include "bitmatrix.h"
#include "graph_structure.h"

class DenseGraph : public Graph {
 public:
  const static int safe_code=1;
 private:
  BitMatrix m_adj;
  unsigned m_edgecount;
  unsigned m_nodecount;
  class node_type : public freelist_vector_item {};
  typedef freelist_vector<node_type> node_vector;
  node_vector m_nodes;
  int check_nodel_node_id(const char *func, graph_node_id id) const {
    return m_nodes.check_nodel_id(func,id);}
  inline unsigned edge2from(graph_edge_id id) const {
    return id >> 16;}
  inline unsigned edge2to(graph_edge_id id) const {
    return id&0xffff;}
  inline unsigned edge_id(unsigned from,unsigned to) const {
    return (from << 16) + to;} 
  int check_nodel_edge_id(const char *func, graph_edge_id id) const {
    unsigned from=edge2from(id);
    if(!check_nodel_node_id("DenseGraph::check_nodel_edge_id: from",from)) {
      cerr << "DenseGraph::check_nodel_edge_id: from node " << from
	   << "does not exist\n";
      cerr << func << " : invalid edge\n";
      return 0;
    }
    unsigned to=edge2to(id);
    if(!check_nodel_node_id("DenseGraph::check_nodel_edge_id: to",to)) {
      cerr << "DenseGraph::check_nodel_edge_id: to node " << to 
	   << " does not exist\n";
      cerr << func << " : invalid edge\n";
      return 0;
    }
    if(!m_adj(from,to)) {
      cerr << func << " : DenseGraph::check_nodel_edge_id: "
	   << "no edge from " << from << " to " << to << endl;
      return 0;
    }
    return 1;
  }
 public:
  unsigned edge_count() const {return m_edgecount;}
  unsigned node_count() const {return m_nodecount;}
  graph_node_id add_node() {
    if(safe_code) if(m_nodecount>0xfff0) {
      cerr << "DenseGraph::add_node : too many nodes\n";return (unsigned)(-1);}
    return m_nodes.add();}
  void delete_node(graph_node_id node) {
    m_nodes.del(node);
    for(int i=0;i<node_count();i++) m_adj(i,node)=m_adj(node,i)=0;
  }
  graph_edge_id add_edge(graph_node_id from, graph_node_id to) {
    if(safe_code) {
      if((!check_nodel_node_id("DenseGraph::add_edge, from node",from))
	 ||(!check_nodel_node_id("DenseGraph::add_edge, to node",to)))
	return (unsigned)(-1);
      if(m_adj(from,to)) {
	cerr<<"denseGraph::add_edge: already an edge ("<<from<<","<<")\n";
	return (unsigned)(-1);
      }
    }
    m_adj(from,to)=1;
    return edge_id(from,to);
  }
  void delete_edge(graph_edge_id edge) {
    if(safe_code) 
      if(!check_nodel_edge_id("DenseGraph::delete_edge",edge)) return;
    m_adj(edge2from(edge),edge2to(edge))=0;		     
  }
  int is_adj(graph_node_id from, graph_node_id to) const {
    return m_adj(from,to);}
 private:
  class base_edge_iterator {
  protected:
    const DenseGraph* m_graph;
    graph_node_id m_node;
    graph_node_id m_other;
    inline int check_comparable(const char *func, 
			 const base_edge_iterator& cmp) const {
      if(safe_code) {
	if(m_graph!=cmp.m_graph) {
	  cerr << "DenseGraph::" << func 
	       << " : iterators are incomparable (different graphs)\n"; 
	  return 0;
	}
	if(m_node!=cmp.m_node) {
	  cerr << "DenseGraph::" << func 
	       << "iterators are incomparable (different in-nodes)\n"; 
	  return 0;
	}
      }
      return 1;
    }
    base_edge_iterator(const DenseGraph* graph, 
		       graph_node_id node, graph_node_id other)
      : m_graph(graph), m_node(node), m_other(other) {}
    base_edge_iterator(const base_edge_iterator& src)
      : m_graph(src.m_graph), m_node(src.m_node), m_other(src.m_other) {}
  };
 public:
  class out_edge_iterator : private base_edge_iterator {
  private:
    void inc(int i) {
      while(i>0) {this->dec();i--;}
      while(i<0) {this->inc();i++;}
    }
  public:
    void inc() {
      int ok=0;
      while(!ok) {
	m_other++;
	if(m_other>=m_graph->node_count())ok=1;
	else if(m_graph->is_adj(m_node,m_other)) ok=1;
      }
    }
    void dec() {int ok=0;while(!ok) {m_other--;if(m_other<0)ok=1;
    else if(m_graph->is_adj(m_node,m_other)) ok=1;}}
    out_edge_iterator& operator++() {inc(); return *this;}
    out_edge_iterator operator++(int) {
      out_edge_iterator __tmp = *this; inc(); return __tmp;}
    out_edge_iterator& operator--() {dec(); return *this;}
    out_edge_iterator operator--(int) {
      out_edge_iterator __tmp = *this; dec(); return __tmp;}
    out_edge_iterator& operator+=(int i) {inc(i);return *this;}
    out_edge_iterator& operator-=(int i) {inc(-i);return *this;}
    int operator==(const out_edge_iterator& cmp) const {
      if(!check_comparable("out_edge_iterator::operator==()", cmp)) return 0;
      return m_other==cmp.m_other;
    }
    int operator!=(const out_edge_iterator& cmp) const {
      if(!check_comparable("out_edge_iterator::operator!=()", cmp)) return 0;
      return m_other!=cmp.m_other;
    }
    graph_node_id operator*() const {
      return m_other;}   
    out_edge_iterator& operator=(const out_edge_iterator& src) {
      m_node = src.m_node; m_other=src.m_other; m_graph=src.m_graph;}
    out_edge_iterator(const DenseGraph* graph, 
		      graph_node_id node, graph_node_id other)
      : base_edge_iterator(graph,node,other) {
      if(m_other==(unsigned)(-1)) inc();}
    out_edge_iterator(const out_edge_iterator& src) 
      : base_edge_iterator(src) {}
  };

  out_edge_iterator node_out_begin(int node) {
    if(safe_code) 
      if(!check_nodel_node_id("node_out_begin",node)) throw;
    return out_edge_iterator(this,node,(unsigned)(-1));
  }
  out_edge_iterator node_out_end(int node) {
    if(safe_code) if(!check_nodel_node_id("node_out_begin",node)) throw;
    return out_edge_iterator(this,node,node_count());
  }
  class in_edge_iterator : private base_edge_iterator {
  private:
    void inc(int i) {
      while(i>0) {dec();i--;}
      while(i<0) {inc();i++;}
    }
  public:
    void inc() {int ok=0;while(!ok) {m_other++;if(m_other>=m_graph->node_count())ok=1;
    else if(m_graph->is_adj(m_other,m_node)) ok=1;}}
    void dec() {int ok=0;while(!ok) {m_other--;if(m_other<0)ok=1;
    else if(m_graph->is_adj(m_other,m_node)) ok=1;}}
    in_edge_iterator& operator++() {inc(); return *this;}
    in_edge_iterator operator++(int) {
      in_edge_iterator __tmp = *this; inc(); return __tmp;}
    in_edge_iterator& operator--() {dec(); return *this;}
    in_edge_iterator operator--(int) {
      in_edge_iterator __tmp = *this; dec(); return __tmp;}
    in_edge_iterator& operator+=(int i) {inc(i);return *this;}
    in_edge_iterator& operator-=(int i) {inc(-i);return *this;}
    int operator==(const in_edge_iterator& cmp) const {
      if(!check_comparable("in_edge_iterator::operator==()", cmp)) return 0;
      return m_other==cmp.m_other;
    }
    int operator!=(const in_edge_iterator& cmp) const {
      if(!check_comparable("in_edge_iterator::operator!=()", cmp)) return 0;
      return m_other!=cmp.m_other;
    }
    graph_node_id operator*() const {
      return m_other;}   
    in_edge_iterator& operator=(const in_edge_iterator& src) {
      m_node = src.m_node; m_other=src.m_other; m_graph=src.m_graph;}
   in_edge_iterator(const DenseGraph* graph, 
		      graph_node_id node, graph_node_id other)
      : base_edge_iterator(graph,node,other) {
      if(m_other==(unsigned)(-1)) inc();}
   in_edge_iterator(const in_edge_iterator& src) 
      : base_edge_iterator(src) {}
  };
  in_edge_iterator node_in_begin(int node) {
    if(safe_code) if(!check_nodel_node_id("node_in_begin",node)) throw;
    return in_edge_iterator(this,node,(unsigned)(-1));
  }
  in_edge_iterator node_in_end(int node) {
    if(safe_code) if(!check_nodel_node_id("node_in_begin",node)) throw;
    return in_edge_iterator(this,node,node_count());
  }
  class edge_iterator : base_edge_iterator {
  private:
    void inc(int i) {
      while(i>0) {dec();i--;}
      while(i<0) {inc();i++;}
    }
  public:
    void inc() {
      int ok=0;
      while(!ok) {
	m_other++;
	if(m_other>=m_graph->node_count()) {
	  m_node++;
	  m_other=0;
	  if(m_node>=m_graph->node_count())
	    ok=1;
	}
	if(!ok) if(m_graph->is_adj(m_node,m_other)) ok=1;
      }
    }
    void dec() {
      int ok=0;
      while(!ok) {
	m_other--;
	if(m_other<0) {
	  m_other=m_graph->node_count()-1;
	  m_node--;
	  if(m_node<0) ok=1;
	}
	if(!ok) if(m_graph->is_adj(m_node,m_other)) ok=1;
      }
    }
    edge_iterator& operator++() {inc(); return *this;}
    edge_iterator operator++(int) {
      edge_iterator __tmp = *this; inc(); return __tmp;}
    edge_iterator& operator--() {dec(); return *this;}
    edge_iterator operator--(int) {
      edge_iterator __tmp = *this; dec(); return __tmp;}
    edge_iterator& operator+=(int i) {inc(i);return *this;}
    edge_iterator& operator-=(int i) {inc(-i);return *this;}
    int operator==(const edge_iterator& cmp) const {
      if(!check_comparable("edge_iterator::operator==()", cmp)) return 0;
      return m_other==cmp.m_other;
    }
    int operator!=(const edge_iterator& cmp) const {
      if(!check_comparable("edge_iterator::operator!=()", cmp)) return 0;
      return m_other!=cmp.m_other;
    }
    graph_node_id operator*() const {
      return m_other;}   
    edge_iterator& operator=(const edge_iterator& src) {
      m_node = src.m_node; m_other=src.m_other; m_graph=src.m_graph;}
    unsigned from() const {return m_node;}
    unsigned to() const {return m_other;}
    edge_iterator(const DenseGraph* graph, 
		  graph_node_id node, graph_node_id other)
      : base_edge_iterator(graph,node,other) {
      if(m_other==(unsigned)(-1)) inc();}
    edge_iterator(const edge_iterator& src) 
      : base_edge_iterator(src) {}
 };
  int edge_iterator_id(const edge_iterator& n) const {
    return edge_id(n.from(),n.to());}
  edge_iterator edge_begin() const {
    return edge_iterator(this,0,(unsigned)(-1));}
  edge_iterator edge_end() const {
    return edge_iterator(this,node_count(),0);}

  typedef node_vector::const_iterator node_iterator;
  int node_iterator_id(const node_iterator& n) const {
    return m_nodes.iterator_id(n);}
  node_iterator node_begin() const {
    return m_nodes.begin();}
  node_iterator node_end() const {
    return m_nodes.end();}

  virtual ostream& write_edges(ostream& os) const;
  virtual ostream& write_nodes(ostream& os) const;
  virtual ostream& write(ostream& os) const;
  virtual int check_consistency(int talking) const;
  virtual ~DenseGraph();
};

#endif
