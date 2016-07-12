#ifndef NODENLINK
#define NODENLINK

#include <vector.h>

#include "classes.h"
#include "CombFunction.h"
#include "Network.h"

class GroundLink {
 private:
  GroundNode *src_node;
 public:
  inline GroundNode* GetPtr() const {return src_node;}
  inline GroundNode& operator*() const {return *src_node;}
  inline GroundNode* operator->() const {return src_node;}
  GroundLink(GroundNode *src) 
    : src_node(src) {}
};

class GroundNodeStruct {
 private:
  GroundLinkList ground_links;
     
 public:
        
  inline GroundLinkList& GetGroundLinks() {return ground_links;}
  inline void AddLink(GroundNode *src_node) {
    ground_links.push_back(GroundLink(src_node));}
      
      
};

class GroundNodeData {
 public:
  double yield;
  double delta;
};

class GroundNode {
 private:
  NlpWeight *weight;
  GroundNodeStruct structure;
  GroundNodeData data;
 protected:
 public:
  inline void AddDelta(double adj) {data.delta+=adj;}
  CombFunctionPtr GetCombFunction() const {
    return weight->GetNode()->GetCombFunction();}
  inline double GetYield() const {return data.yield;}
  inline double GetDelta() const {return data.delta;}
  inline GroundLinkList& GetLinks() {return structure.GetGroundLinks();}
  inline void AddLink(GroundNode *src_node) {
    structure.AddLink(src_node);}
  NlpWeight *GetWeight() const {return weight;}
  virtual double calcYield() = NULL;
  virtual void backProp(double learningRate) = NULL; 
  GroundNode(NlpWeight *_weight) : weight(_weight) {}
};

class GroundInputNode : public GroundNode {
 public:
  GroundInputNode(InputNlpWeight *_weight) 
    : GroundNode(_weight) {}

  virtual double calcYield() {}
  virtual void backProp(double learningRate) {}
};  

class BodyIDGroundNode : public GroundNode {
 private:
  int bodynr;
 public:
  inline int GetBodyNr() const {return bodynr;}
  BodyIDGroundNode(NlpWeight *_weight,int _bodynr) 
    : GroundNode(_weight),bodynr(_bodynr) {}
};

class GroundConNode : public BodyIDGroundNode {
 public:
  GroundConNode(ConNlpWeight *_weight,int _bodynr)
    : BodyIDGroundNode(_weight,_bodynr) {}
  virtual double calcYield();
  virtual void backProp(double learningRate);
};  

class GroundDisNode: public GroundNode {
 private:
 public:
  GroundDisNode(DisNlpWeight *_weight)
    : GroundNode(_weight) {}
  virtual double calcYield();
  virtual void backProp(double learningRate);
};  


    
 
       
   
   
     
     
  




#endif
