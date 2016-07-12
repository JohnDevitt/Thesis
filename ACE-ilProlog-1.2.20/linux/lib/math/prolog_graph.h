
#ifndef PROLOG_GRAPH_INC
#define PROLOG_GRAPH_INC



#include "graphs/graph_structure.h"
#include "graphs/sparse_graph.h"

class PSparseGraph {
 private:
  BiDirSparseGraph m_graph;
 public:
	void node_count(GAPIC_int* count) const {

    *count = m_graph.node_count();}
	void edge_count(GAPIC_int* count) const {

    *count = m_graph.edge_count();}
	void add_node(GAPIC_int* id) {

    *id=m_graph.add_node();}
	void delete_node(GAPIC_int id) {

    m_graph.delete_node(id);}
	void add_edge(GAPIC_int* id, GAPIC_int from, GAPIC_int to) {

    *id=m_graph.add_edge(from,to);}
	void delete_edge(GAPIC_int id) {

    m_graph.delete_edge(id);}
	void check_consistency(GAPIC_int talking, GAPIC_int* result) const {

    *result = m_graph.check_consistency(talking);}
  ostream& Print(ostream& os) const {
    return m_graph.write(os);}
  ~PSparseGraph();
};

extern gapic_type GapicSparseGraphType;
typedef GapicNonExMutCollectable<PSparseGraph,&GapicSparseGraphType> 
	PSparseGraphCollectable;

#endif
