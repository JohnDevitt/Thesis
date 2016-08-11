
#ifndef NLP_MAIN_INC
#define NLP_MAIN_INC



#include "../../gapic/c_prolog.h"
#include "classes.h"
#include "Ground.h"
#include "Network.h"
#include "CombFunction.h"
#include "avl2c.h"

class nlp_net_creator;

extern gapic_type GapicNlpNetType;
typedef GapicCollectable<nlp_c_net,&GapicNlpNetType> 
  NlpNetCollectable;

extern gapic_type GapicGroundNlpNetType;
typedef GapicNonExMutCollectable<nlp_c_ground_net,&GapicGroundNlpNetType> 
  GroundNlpNetCollectable;
typedef GroundNlpNetCollectable nlp_c_ground_netCollectable;

extern gapic_type GapicNlpWeightsType;
typedef GapicNonExMutCollectable<NlpWeights,&GapicNlpWeightsType> 
  NlpWeightsCollectable;

class nlp_c_ground_net {
 private:
  NlpOrder order;
  nlp_net_creator* create_vars;
     
  GroundNodePtrVector nodes;
  int process_init_avl(GAPIC_term_ref avl);
  int process_init_avl_2(GAPIC_term_ref avl);
  inline int process_init_sollist(GAPIC_term_ref solution_list);
  inline int process_init_solution(GAPIC_term_ref solution);
  inline int process_init_substerm(GAPIC_term_ref substerm);
  int process_init_pred(GAPIC_term_ref pred);
  void process_init_id2node();
 public:
	void get_activation(GAPIC_int id, GAPIC_float* act) const {

    *act=nodes[id]->GetYield();}
	void set_delta(GAPIC_int id, GAPIC_float delt) const {

    nodes[id]->SetDelta(delt);}
	void feedforward();

	void backprop(GAPIC_float learn_rate);

  ostream& Print(ostream& os) const;
  nlp_c_ground_net(GAPIC_term_ref src) 
    throw(GapicObjectCreationError);
  ~nlp_c_ground_net();
};

class nlp_c_net {
 private:
  NlpPredDefVector nlp;
  int id_count;
 public:
  ostream& Print(ostream& os) const;
  int GetIDCount() const {return id_count;}
  int GetPredCnt() const {return nlp.size();}
  NlpPredDefPtr GetPred(int i) const {return nlp[i];}
  NlpWeightPtr *NewWeightPtrs() const;
  NlpWeights* NewWeights() const;
  NlpPredDefPtr FindPredDef(GAPIC_term_ref call) const;
  nlp_c_net(GAPIC_term_ref src) 
    throw(GapicObjectCreationError);
};

class NlpWeights {
 private:
  const nlp_c_net *net;
  NlpWeightPtr *node_parameters;
 public:
  void UniformRandom();
  void Update();
	void uniform_random() {

    UniformRandom();}
	void update() {

    Update();}
  ostream& Print(ostream& os) const;
  NlpWeightPtr Get(int id) const {return node_parameters[id];}
  NlpWeights(const nlp_c_net *_net,NlpWeightPtr *_node_parameters)
    : net(_net),node_parameters(_node_parameters) {}
  NlpWeights(GAPIC_term_ref src)
    throw(GapicObjectCreationError);
  ~NlpWeights() {
    delete node_parameters;}
};

#endif
