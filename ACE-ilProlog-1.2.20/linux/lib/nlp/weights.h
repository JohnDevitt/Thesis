#ifndef WEIGHTS_INC
#define WEIGHTS_INC


#include <string> 
#include <stdlib.h>
#include "../../gapic/c_prolog.h"
#include "../../gapic/gapicgc.h"
#include "classes.h"
#include "Network.h"

class NlpSimpleWeight {
 private:
  double current;
  double adjustment;
 public:
  NlpSimpleWeight() : current(0),adjustment(0) {}
  NlpSimpleWeight(double init) : current(init), adjustment(0) {}
#ifdef WINDOWS
  void UniformRandom() {current=0.0;} 
#else
  void UniformRandom() {current=(random()&0xffff)*1.0/0x10000L;}
#endif
  double Get() const {return current;}
  void Adjust(double adj) {adjustment+=adj;}
  void Update() {current+=adjustment;adjustment=0;}
};


class NlpWeight {
 public:
  virtual void UniformRandom()=0;
  virtual const NlpNode *GetNode() const=0;
  virtual ostream& Print(ostream& os) const=0;
  virtual void Update()=0;
  virtual const char*GetTypeCode() const=0;
  NlpWeight() {}
  virtual ~NlpWeight();
};

class InputNlpWeight : public NlpWeight {
 private:
  const NlpInputNode *skel_node;
 public:
  virtual void UniformRandom(); 
  virtual const NlpInputNode *GetNode() const;
  virtual ostream& Print(ostream& os) const;
  virtual void Update();
  virtual const char* GetTypeCode() const;
  InputNlpWeight(const NlpInputNode *_node) 
    : skel_node(_node) {}
  virtual ~InputNlpWeight();
};
 
class ConNlpWeight : public NlpWeight {
 private:
  const NlpConNode *skel_node;
 public:
  virtual void UniformRandom();
  virtual void Update();
  virtual const NlpConNode *GetNode() const;
  virtual ostream& Print(ostream& os) const;
  virtual const char*GetTypeCode() const;
  ConNlpWeight(const NlpConNode *_node) 
    : skel_node(_node) {}
  virtual ~ConNlpWeight();
};

class DisNlpWeight : public NlpWeight {
 private:
  NlpSimpleWeight *weights;
  const NlpDisNode *skel_node;
 public:
  virtual void UniformRandom();
  virtual void Update();
  const NlpSimpleWeight *GetWeightArray() const {return weights;}
  void Adjust(int i, double adj) {
    weights[i].Adjust(adj);
  }
  virtual const char*GetTypeCode() const;
  virtual const NlpDisNode *GetNode() const;
  virtual ostream& Print(ostream& os) const;
  DisNlpWeight(const NlpDisNode *_node) 
    : skel_node(_node),
    weights(new NlpSimpleWeight[_node->GetLinks().size()]) {}
  virtual ~DisNlpWeight();
};

#endif
