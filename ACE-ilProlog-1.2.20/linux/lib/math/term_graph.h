#ifndef TERM_GRAPH_INC
#define TERM_GRAPH_INC



#include <gapic/struct/c_prolog_array.h>
#include <gapic/struct/c_avl.h>
#include "graphs/sparse_graph.h"

class TermGraphOptions {
 private:
  static int s_default_unique_nodelabels;
  static int s_default_unique_edgelabels;
  static int s_default_empty_nodelabels;
  static int s_default_empty_edgelabels;
  static GAPIC_functor s_unique_nodelabels_1;
  static GAPIC_functor s_unique_edgelabels_1;
  static GAPIC_functor s_empty_nodelabels_1;
  static GAPIC_functor s_empty_edgelabels_1;
  static GAPIC_functor s_prolog_data_1;
  static int get_arg_int(GAPIC_term_ref arg,int i);
 public:
  int m_unique_nodelabels:1;
  int m_unique_edgelabels:1;
  int m_empty_nodelabels:1;
  int m_empty_edgelabels:1;
  TermGraphOptions& operator=(const TermGraphOptions& src) { 
    m_unique_nodelabels=src.m_unique_nodelabels;
    m_unique_edgelabels=src.m_unique_edgelabels;
    m_empty_nodelabels=src.m_empty_nodelabels;
    m_empty_edgelabels=src.m_empty_edgelabels;
  }
  TermGraphOptions(GAPIC_term_ref opt, GAPIC_term_ref pd) 
    throw(GapicObjectCreationError);
  TermGraphOptions(const TermGraphOptions& src) 
    : m_unique_nodelabels(src.m_unique_nodelabels),
    m_unique_edgelabels(src.m_unique_edgelabels), 
    m_empty_nodelabels(src.m_empty_nodelabels),
    m_empty_edgelabels(src.m_empty_edgelabels) {}
  TermGraphOptions() 
    : m_unique_nodelabels(s_default_unique_nodelabels), 
    m_unique_edgelabels(s_default_unique_edgelabels),
    m_empty_nodelabels(s_default_empty_nodelabels), 
    m_empty_edgelabels(s_default_empty_edgelabels) {}
};

class TermGraph {
 private:
  typedef BiDirSparseGraph graph_structure;
  static const GAPIC_functor s_pd_functor;
  static const GAPIC_atom s_no_label;
  static const int s_safe_code; 
  graph_structure m_graph;
  CPrologVector *m_node2label;
  CPrologVector *m_edge2label;
  const TermGraphOptions m_options;
  inline GAPIC_term_ref pd_get_arg(GAPIC_term_ref pd, int i) const;
 public:
  inline GAPIC_term_ref pd_create();
  inline GAPIC_term_ref pd_get_n2lab(GAPIC_term_ref pd) const;
  inline GAPIC_term_ref pd_get_e2lab(GAPIC_term_ref pd) const;
  inline GAPIC_term_ref pd_get_lab2n(GAPIC_term_ref pd) const;
  inline GAPIC_term_ref pd_get_lab2e(GAPIC_term_ref pd) const;
  inline void pd_set_n2lab(GAPIC_term_ref pd, GAPIC_term_ref n2lab);
  inline void pd_set_e2lab(GAPIC_term_ref pd, GAPIC_term_ref e2lab);
  inline void pd_set_lab2n(GAPIC_term_ref pd, GAPIC_term_ref lab2n);
  inline void pd_set_lab2e(GAPIC_term_ref pd, GAPIC_term_ref lab2e);
 public:
  inline int unique_nodelabels() const {return m_options.m_unique_nodelabels;}
  inline int unique_edgelabels() const {return m_options.m_unique_edgelabels;}
  inline int empty_nodelabels() const {return m_options.m_empty_nodelabels;}
  inline int empty_edgelabels() const {return m_options.m_empty_edgelabels;}
 public:
  unsigned node_count() const {return m_graph.node_count();}
  unsigned edge_count() const {return m_graph.edge_count();}
	void node_count(GAPIC_term_ref pd, GAPIC_int* count) const {

    *count=node_count();}
	void edge_count(GAPIC_term_ref pd, GAPIC_int* count) const {

    *count=edge_count();}
	void add_node(GAPIC_term_ref pd, GAPIC_int* node_id);

	void add_labeled_node(GAPIC_term_ref pd, GAPIC_term_ref lab, GAPIC_int* node_id);

	void del_node(GAPIC_term_ref pd, GAPIC_int node_id);

	void set_node_label(GAPIC_term_ref pd, GAPIC_int node_id, GAPIC_term_ref lab);

  GAPIC_term_ref get_node_label(GAPIC_term_ref pd,int node_id) const;
	void get_node_label(GAPIC_term_ref pd, GAPIC_int node_id, GAPIC_term_ref lab) const {

    GAPIC_unify(lab,get_node_label(pd,node_id));}
	GAPIC_result find_node(GAPIC_term_ref pd, GAPIC_term_ref lab, GAPIC_int* node_id) const;

	void add_edge(GAPIC_term_ref pd, GAPIC_int n1, GAPIC_int n2, GAPIC_int* edge);

	void add_labeled_edge(GAPIC_term_ref pd, GAPIC_term_ref lab, GAPIC_int n1, GAPIC_int n2, GAPIC_int* edge_id);

	void del_edge(GAPIC_term_ref pd, GAPIC_int node_id);

	void set_edge_label(GAPIC_term_ref pd, GAPIC_int node_id, GAPIC_term_ref lab);

  GAPIC_term_ref get_edge_label(GAPIC_term_ref pd,int node_id) const;
	void get_edge_label(GAPIC_term_ref pd, GAPIC_int node_id, GAPIC_term_ref lab) const {

    GAPIC_unify(lab,get_edge_label(pd,node_id));}
	GAPIC_result find_edge(GAPIC_term_ref pd, GAPIC_term_ref lab, GAPIC_int* node_id) const;

	void edge_from_to(GAPIC_term_ref pd, GAPIC_int edge, GAPIC_int* n1, GAPIC_int* n2) const {

    graph_node_id fr,to;m_graph.edge_from_to(edge,&fr,&to);*n1=fr;*n2=to;}
	void edge_from(GAPIC_term_ref pd, GAPIC_int edge, GAPIC_int* n1) const {

    *n1=m_graph.edge_from(edge);}
	void edge_to(GAPIC_term_ref pd, GAPIC_int edge, GAPIC_int* n1) const {

    *n1=m_graph.edge_to(edge);}
  ostream& Print(ostream& os) const {
    return os<<"[TermGraph]";}
  ostream& write(GAPIC_term_ref pd, ostream& os) const;
	void print(GAPIC_term_ref pd) const {

    write(pd,cout);}  
	GAPIC_result node_nolabel(GAPIC_term_ref pd, GAPIC_int node_id) const {

    return s_no_label.CmpTerm(get_node_label(pd,node_id));}
  TermGraph& operator=(const TermGraph& src) throw(GapicObjectCreationError) {
    throw GapicObjectCreationError("TermGraph:: object can not be assigned "
				   "without copying prolog data");}
 public:
  TermGraph(const TermGraph& src) throw(GapicObjectCreationError) {
    throw GapicObjectCreationError("TermGraph:: object can not be copied "
				   "without copying prolog data");}
  TermGraph(GAPIC_term_ref opt);
  TermGraph(TermGraphOptions opt, GAPIC_term_ref pd);
};

extern gapic_type GapicTermGraphType;
typedef GapicNonExMutCollectable<TermGraph,&GapicTermGraphType> 
	TermGraphCollectable;

#endif
