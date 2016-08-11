
#ifndef LBN_INC
#define LBN_INC




#include <math.h>
#include <vector>
#include "../gapic/gapicgc.h"
#include "../math/term_graph.h"


class c_lbn : public TermGraph {
 public:
	GAPIC_result find_node(GAPIC_term_ref pd, GAPIC_term_ref lab, GAPIC_int* node_id) const {

    return TermGraph::find_node(pd,lab,node_id);}
	void add_labeled_node(GAPIC_term_ref pd, GAPIC_term_ref lab, GAPIC_int* node_id) {

    TermGraph::add_labeled_node(pd,lab,node_id);}
	void add_labeled_edge(GAPIC_term_ref pd, GAPIC_term_ref lab, GAPIC_int n1, GAPIC_int n2, GAPIC_int* edge_id) {

    TermGraph::add_labeled_edge(pd,lab,n1,n2,edge_id);}
  c_lbn(GAPIC_term_ref opt) : TermGraph(opt) {}
};

extern gapic_type GapicLBNType;
typedef GapicNonExMutCollectable<c_lbn,&GapicLBNType> 
	c_lbnCollectable;


#endif
