
#ifndef AVL2C_INC
#define AVL2C_INC

#include <map>
#include "nlp_main.h"

typedef int NodeIndexData;

typedef GAPIC_term_ref NodeIndexKey;

struct NodeIndexCompare {
  bool operator()(const NodeIndexKey s1, 
		  const NodeIndexKey s2) const {
    return GAPIC_compare(s1,s2);
  }
};

typedef map<NodeIndexKey,NodeIndexData,NodeIndexCompare> NodeIndex;

typedef pair<int,int> IDLink;
typedef vector<IDLink> IDLinkVector;

class nlp_net_creator {
 public:
   
  NlpPlace place;
  NlpWeights *weight_ptr;
  const NlpPredDef* pred_def;

   
  int varstruct;
  GAPIC_term_ref interm;     
  GAPIC_term_ref outterm;    
  GAPIC_term_ref fullterm;      
  inline GAPIC_term_ref MakeFullTerm();
  inline int ProcessFullTerm(); 

   
  NodeIndex node_idx;
  GroundNodePtrVector *node_ptr;
  int this_id;
  int this_node;
  int *id2node;
  int *first_node_varstruct;
  IDLinkVector links_to_add;
  int id2node_size;
  ostream& PrintIdx(ostream& os) const;
  nlp_net_creator(int _id2node_size,GroundNodePtrVector *_node_ptr) :
    id2node_size(_id2node_size),
    node_ptr(_node_ptr) {
    node_ptr->resize(_id2node_size,NULL);
    interm=GAPIC_new_term_ref();
    outterm=GAPIC_new_term_ref();
    id2node=new int[_id2node_size];
    first_node_varstruct=new int[_id2node_size];
    for(int i=0;i<_id2node_size;i++)
      id2node[i]=first_node_varstruct[i]=-1;
  }
  ~nlp_net_creator() {
          
    delete id2node;
    delete first_node_varstruct;
    
  }
};

#endif
