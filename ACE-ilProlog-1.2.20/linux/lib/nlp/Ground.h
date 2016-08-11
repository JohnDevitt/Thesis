#ifndef NODENLINK
#define NODENLINK

#include <vector.h>

#include "classes.h"
#include "CombFunction.h"
#include "weights.h"
#include "Network.h"

#define GROUND_IDS

#ifdef GROUND_IDS
extern int last_ground_id;
#endif

class NlpOrder {
 public:
  int dis_count;
};

class GroundLink {
 private:
#ifdef GROUND_IDS
  int id;
#endif
  GroundNode *src_node;
 public:
#ifdef GROUND_IDS
  int GetID() const {return id;}
#endif
  inline GroundNode* GetPtr() const {return src_node;}
  inline GroundNode& operator*() const {return *src_node;}
  inline GroundNode* operator->() const {return src_node;}
  GroundLink(GroundNode *src) 
    : src_node(src) 
#ifdef GROUND_IDS
    , id(last_ground_id++)
#endif
    {}
};

class GroundNodeStruct {
 private:
  GroundLinkList ground_links;
     
 public:
        
  inline GroundLinkList& GetGroundLinks() {return ground_links;}
  inline const GroundLinkList& GetConstGroundLinks() const {
    return ground_links;}
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
#ifdef GROUND_IDS
  int id;
#endif
  NlpWeight *weight;
  GroundNodeStruct structure;
  GroundNodeData data;
 protected:
 public:
#ifdef GROUND_IDS
  int GetID() const {return id;}
#endif
  inline void DoBackprop(double learnrate) {backProp(learnrate);}
  inline double ComputeYield() {
    ResetDelta();
    data.yield=calcYield();
    return data.yield;}
  virtual ostream& Print(ostream& os) const;
  inline void ResetDelta() {data.delta=0;}
  inline void SetDelta(double _delta) {data.delta=_delta;}
  inline void AddDelta(double adj) {data.delta+=adj;}
  CombFunctionPtr GetCombFunction() const {
    return weight->GetNode()->GetCombFunction();}
  inline double GetYield() const {return data.yield;}
  inline double GetDelta() const {return data.delta;}
  inline GroundLinkList& GetLinks() {return structure.GetGroundLinks();}
  inline const GroundLinkList& GetConstLinks() const {
    return structure.GetConstGroundLinks();}
  inline void AddLink(GroundNode *src_node) {
    structure.AddLink(src_node);}
  NlpWeight *GetWeight() const {return weight;}
  virtual const char* GetTypeCode() const=0;
  virtual double calcYield() = NULL;
  virtual void backProp(double learningRate) = NULL; 
  GroundNode(NlpWeight *_weight) : weight(_weight) 
#ifdef GROUND_IDS
    , id(last_ground_id++)
#endif
    {}
};

class GroundInputNode : public GroundNode {
 public:
  GroundInputNode(InputNlpWeight *_weight) 
    : GroundNode(_weight) {}
  virtual const char* GetTypeCode() const=0;
  virtual void backProp(double learningRate) {}
};  

class GroundFloatInputNode : public GroundInputNode {
 private:
  GAPIC_float value;
 public:
  virtual double calcYield();
  virtual ostream& Print(ostream& os) const;
  GroundFloatInputNode(InputNlpWeight *_weight,GAPIC_float f) 
    : GroundInputNode(_weight),value(f) {}
  virtual const char* GetTypeCode() const;
};  

class BodyIDGroundNode : public GroundNode {
 private:
  int bodynr;
 public:
  virtual const char* GetTypeCode() const=0;
  inline int GetBodyNr() const {return bodynr;}
  BodyIDGroundNode(NlpWeight *_weight,int _bodynr) 
    : GroundNode(_weight),bodynr(_bodynr) {}
};

class GroundConNode : public BodyIDGroundNode {
 public:
  GroundConNode(ConNlpWeight *_weight,int _bodynr)
    : BodyIDGroundNode(_weight,_bodynr) {}
  virtual const char* GetTypeCode() const;
  virtual double calcYield();
  virtual void backProp(double learningRate);
};  

class GroundDisNode: public GroundNode {
 private:
 public:
  GroundDisNode(DisNlpWeight *_weight)
    : GroundNode(_weight) {}
  virtual const char* GetTypeCode() const;
  virtual double calcYield();
  virtual void backProp(double learningRate);
};  


    
 
       
   
   
     
     
  




#endif
