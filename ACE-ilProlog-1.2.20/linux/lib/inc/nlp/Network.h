#ifndef NETWORK_INC
#define NETWORK_INC

#include <string> 
#include "../../gapic/c_prolog.h"
#include "../../gapic/gapicgc.h"
#include "classes.h"

extern int nlp_auto_sigmoids;


extern CombFunction *std_add_func;
extern CombFunction *std_sig_add_func;
extern CombFunction *std_mul_func;

class NlpNode {
 private:
  int id;
  string name;
  int private_comb_func;
  CombFunctionPtr comb_func;
  CombFunction *ParseCombFunc(GAPIC_term_ref src);
 protected:
  void SetCombFunc(CombFunctionPtr _c_f) {comb_func=_c_f;}
  void SetPrivateCombFunc(CombFunctionPtr _c_f) {
    SetCombFunc(_c_f);private_comb_func=1;}
  void TermToCombFunc(GAPIC_term_ref src) {
    SetPrivateCombFunc(ParseCombFunc(src));}
 public:
  virtual void InitWeights(NlpWeightPtr *parameters) const=0;
  virtual NlpPlace GetPlace() const;
  inline CombFunctionPtr GetCombFunction() const {return comb_func;}
  const string& GetName() const {return name;}
  int GetID() const {return id;}
  virtual ostream& Print(ostream& os) const=0;
  NlpNode(const string _name, int *id_count) 
    : private_comb_func(0),name(_name),id((*id_count)++),comb_func(NULL) {}
  virtual ~NlpNode();
};

class NlpLink {
 private:
  const NlpNode *src_node;
 public:
  inline const NlpNode* GetPtr() const {return src_node;}
  inline const NlpNode& operator*() const {return *src_node;}
  inline const NlpNode* operator->() const {return src_node;}
  NlpLink(NlpNode *src) 
    : src_node(src) {}
};

typedef vector<NlpLink> NlpLinkVector;

class NlpInputNode : public NlpNode {
 public:
  virtual ostream& Print(ostream& os) const;
  virtual void InitWeights(NlpWeightPtr *parameters) const;
  virtual NlpPlace GetPlace() const;
  NlpInputNode(const string _name, int *id_count)
    : NlpNode(_name,id_count) {}
};

class NlpNonInputNode : public NlpNode {
 private:
  NlpPlace place;
  NlpLinkVector links;
 protected:
  void InitLinkWeights(NlpWeightPtr *parameters) const;
  void AddLink(NlpLink link) {links.push_back(link);}
  void SetPlace(NlpPlace _place) {place=_place;}
 public:
  virtual ostream& Print(ostream& os) const;
  virtual void InitWeights(NlpWeightPtr *parameters) const;
  virtual NlpPlace GetPlace() const;
  const NlpLinkVector& GetLinks() const {return links;}
  NlpNonInputNode(const string _name, int *id_count)
    : NlpNode(_name,id_count),place(NLP_HIDDEN_PLACE) {}
  virtual ~NlpNonInputNode();
};

class NlpConNode : public NlpNonInputNode {
 public:
  virtual ostream& Print(ostream& os) const;
  virtual void InitWeights(NlpWeightPtr *parameters) const;
  NlpConNode(const string _name, int *id_count)
    : NlpNonInputNode(_name,id_count) {SetCombFunc(std_mul_func);}
  virtual ~NlpConNode();
};


class NlpClause : public NlpConNode {
 private:
  vector<NlpPredDefPtr> literals;
  void AddLiterals(const nlp_c_net* net, GAPIC_term_ref body);
 public:
  ostream& PrintIDs(ostream& os) const;
  virtual ostream& Print(ostream& os) const;
  NlpClause(string _name, GAPIC_term_ref clause, const nlp_c_net* net, int *id_count);
  virtual ~NlpClause();
};

class NlpDisNode : public NlpNonInputNode {
 public:
  virtual ostream& Print(ostream& os) const;
  NlpDisNode(string _name,int *id_count)
    : NlpNonInputNode(_name,id_count) {
    if(nlp_auto_sigmoids)SetCombFunc(std_sig_add_func);
    else SetCombFunc(std_add_func);}
  virtual ~NlpDisNode();
};

class NlpPredDef : public NlpDisNode {
 private:
  int flags;
  GAPIC_functor pred_name;
 public:
  int GetUniqueMode() const {return (flags&NLP_UNIQUE_MODE)!=0;}
  ostream& PrintIDs(ostream& os) const;
  virtual ostream& Print(ostream& os) const;
  int GetClauseCnt() const {return GetLinks().size();}
  virtual void InitWeights(NlpWeightPtr *parameters) const;
  int GetArity() const {return pred_name.GetArity();}
  int MatchesCall(GAPIC_term_ref call) const {
    return pred_name.CmpTerm(call)==GAPIC_SUCCESS;}
  void AddClauses(GAPIC_term_ref defs, 
		  const nlp_c_net* net,
		  int *id_count);
  NlpPredDef(string _name,
	     const GAPIC_functor& pred,
	     GAPIC_term_ref options,
	     int *id_count);
  virtual ~NlpPredDef();
};

#endif
