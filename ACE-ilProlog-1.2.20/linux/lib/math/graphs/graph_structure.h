#ifndef GRAPH_STRUCTURE
#define GRAPH_STRUCTURE

#include <vector>
#include <iostream>
using namespace std;
typedef unsigned graph_node_id;
typedef unsigned graph_edge_id;

class GraphIterator {
 public:
  virtual void inc()=0;
  virtual void dec()=0;
  virtual int eq(const GraphIterator* cmp) const=0;
  virtual ~GraphIterator();
};

template <class iter_class, class iter_super> 
class VGraphIterator : public iter_super {
 private: 
  iter_class m_iter;
 public:
  virtual void inc();
  virtual void dec();
  virtual int eq(const GraphIterator* cmp) const;
  const iter_class& get_iter() const {return m_iter;}
  VGraphIterator(const iter_class& src) : m_iter(src) {}
  ~VGraphIterator();
};

template <class iter_class, class iter_super> 
void VGraphIterator<iter_class,iter_super>::inc() {
  ++m_iter;}
template <class iter_class, class iter_super> 
void VGraphIterator<iter_class,iter_super>::dec() {
  --m_iter;}
template <class iter_class, class iter_super> 
int VGraphIterator<iter_class,iter_super>::eq(const GraphIterator* cmp) const {
  return m_iter==((dynamic_cast<const VGraphIterator<iter_class,
		    iter_super>*>(cmp))->m_iter);}

template <class iter_class, class iter_super> 
VGraphIterator<iter_class,iter_super>::~VGraphIterator(){}

class GraphOutEdgeIterator : public GraphIterator {
 public:
  virtual unsigned get() const=0;
  virtual ~GraphOutEdgeIterator();
};
class GraphInEdgeIterator : public GraphIterator {
 public:
  virtual unsigned get() const=0;
  virtual ~GraphInEdgeIterator();
};
class GraphEdgeIterator : public GraphIterator {
 public:
  virtual ~GraphEdgeIterator();
};
class GraphNodeIterator : public GraphIterator {
 public:
  virtual ~GraphNodeIterator();
};

template <class iter_class>
class VGraphOutEdgeIterator:public VGraphIterator<iter_class,GraphOutEdgeIterator> {
 public:
  virtual unsigned get() const;
  virtual VGraphOutEdgeIterator<iter_class>* copy() const {
    return new VGraphOutEdgeIterator<iter_class>(this->get_iter());}
  VGraphOutEdgeIterator(const iter_class& src) : VGraphIterator<iter_class,GraphOutEdgeIterator>(src) {}
  ~VGraphOutEdgeIterator();
};
template <class iter_class>
unsigned VGraphOutEdgeIterator<iter_class>::get() const {
  return *this->get_iter();}
template <class iter_class>
VGraphOutEdgeIterator<iter_class>::~VGraphOutEdgeIterator() {}

template <class iter_class>
class VGraphInEdgeIterator:public VGraphIterator<iter_class,GraphInEdgeIterator> {
 public:
  virtual unsigned get() const;
  virtual VGraphInEdgeIterator<iter_class>* copy() const {
    return new VGraphInEdgeIterator<iter_class>(this->get_iter());}
  VGraphInEdgeIterator(const iter_class& src) : VGraphIterator<iter_class,GraphInEdgeIterator>(src) {}
  ~VGraphInEdgeIterator();
};
template <class iter_class>
unsigned VGraphInEdgeIterator<iter_class>::get() const {
  return *this->get_iter();}
template <class iter_class>
VGraphInEdgeIterator<iter_class>::~VGraphInEdgeIterator() {}

template <class iter_class>
class VGraphEdgeIterator:public VGraphIterator<iter_class,GraphEdgeIterator> {
 public:
  virtual VGraphEdgeIterator<iter_class>* copy() const {
    return new VGraphEdgeIterator<iter_class>(this->get_iter());}
  VGraphEdgeIterator(const iter_class& src) : VGraphIterator<iter_class,GraphEdgeIterator>(src) {}
  ~VGraphEdgeIterator();
};

template <class iter_class>
VGraphEdgeIterator<iter_class>::~VGraphEdgeIterator() {}

template <class iter_class>
class VGraphNodeIterator:public VGraphIterator<iter_class,GraphNodeIterator> {
 public:
  virtual VGraphNodeIterator<iter_class>* copy() const {
    return new VGraphNodeIterator<iter_class>(this->get_iter());}
  VGraphNodeIterator(const iter_class& src) : VGraphIterator<iter_class,GraphNodeIterator>(src) {}
  ~VGraphNodeIterator();
};
template <class iter_class>
VGraphNodeIterator<iter_class>::~VGraphNodeIterator() {}


class Graph {
 public:
  virtual unsigned v_node_count() const=0;
  virtual unsigned v_edge_count() const=0;
  virtual graph_node_id v_add_node()=0;
  virtual void v_delete_node(graph_node_id node)=0;
  virtual graph_edge_id v_add_edge(graph_node_id from, graph_node_id to)=0;
  virtual void v_delete_edge(graph_edge_id edge)=0;
  virtual GraphOutEdgeIterator* v_node_out_begin(int node) const=0;
  virtual GraphOutEdgeIterator* v_node_out_end(int node) const=0;
  virtual GraphInEdgeIterator* v_node_in_begin(int node) const=0;
  virtual GraphInEdgeIterator* v_node_in_end(int node) const=0;
  virtual GraphNodeIterator* v_node_begin() const=0;
  virtual GraphNodeIterator* v_node_end() const=0;
  virtual graph_node_id v_node_iterator_id(const GraphNodeIterator* n) const=0;
  virtual GraphEdgeIterator* v_edge_begin() const=0;
  virtual GraphEdgeIterator* v_edge_end() const=0;
  virtual graph_edge_id v_edge_iterator_id(const GraphEdgeIterator* n) const=0;
  virtual ostream& write_edges(ostream& os) const=0;
  virtual ostream& write_nodes(ostream& os) const=0;
  virtual ostream& write(ostream& os) const=0;
  virtual int check_consistency(int talking) const=0;
  virtual ~Graph();
};

#endif
