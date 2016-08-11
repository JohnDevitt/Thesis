#ifndef COMBFUNCTION
#define COMBFUNCTION

#include "classes.h"
#include "Ground.h"

extern CombFunction *std_add_func;
extern CombFunction *std_mul_func;

class CombFunction {
 public:
  virtual double combine(const NlpWeight* weight,
			const GroundLinkList *links) const = NULL;
  virtual double diffCombine(const NlpWeight* weight,
			    GroundLink diff,
			    const GroundLinkList *links) const = NULL;
};

class ClauseAdder : public CombFunction {
 public:
  virtual double combine(const NlpWeight* weight,
			const GroundLinkList *links) const;
  virtual double diffCombine(const NlpWeight* weight,
			    GroundLink diff,const GroundLinkList *link) const;
};
class UnweightedClauseAdder : public CombFunction {
 public:
  virtual double combine(const NlpWeight* weight,
			const GroundLinkList *links) const;
  virtual double diffCombine(const NlpWeight* weight,
			    GroundLink diff,const GroundLinkList *link) const;
};
class ClauseNoisyor : public CombFunction {
 public:
  virtual double combine(const NlpWeight* weight,
			const GroundLinkList *links) const;
  virtual double diffCombine(const NlpWeight* weight,
			    GroundLink diff,const GroundLinkList *link) const;
};

class Multiplier : public CombFunction {
 public:
  virtual double combine(const NlpWeight* weight,
			const GroundLinkList *links) const;
  virtual double diffCombine(const NlpWeight* weight,
			    GroundLink diff,const GroundLinkList *link) const;
};

class ConstCombFunction : public CombFunction {
 private:
  double value;
 public:
  virtual double combine(const NlpWeight* weight,
			const GroundLinkList *links) const;
  virtual double diffCombine(const NlpWeight* weight,
			    GroundLink diff,const GroundLinkList *link) const;
  ConstCombFunction(double _value) : value(_value) {}
  virtual ~ConstCombFunction();
};

class UnaryOpCombFunction : public CombFunction {
 private:
  CombFunction *cf1;
 public:
  const CombFunction *GetCf1() const {return cf1;}
  UnaryOpCombFunction(CombFunction *_cf1) 
    : cf1(_cf1) {}
  virtual ~UnaryOpCombFunction();
};

class ScaleCombFunction : public UnaryOpCombFunction {
 private:
  double factor;
 public:
  virtual double combine(const NlpWeight* weight,
			const GroundLinkList *links) const;
  virtual double diffCombine(const NlpWeight* weight,
			    GroundLink diff,const GroundLinkList *link) const;
  ScaleCombFunction(CombFunction *_cf1,double _factor) 
    : UnaryOpCombFunction(_cf1),factor(_factor) {}
  virtual ~ScaleCombFunction();
};

class TranslateCombFunction : public UnaryOpCombFunction {
 private:
  double displ;
 public:
  virtual double combine(const NlpWeight* weight,
			const GroundLinkList *links) const;
  virtual double diffCombine(const NlpWeight* weight,
			    GroundLink diff,const GroundLinkList *link) const;
  TranslateCombFunction(CombFunction *_cf1,double _displ) 
    : UnaryOpCombFunction(_cf1),displ(_displ) {}
  virtual ~TranslateCombFunction();
};

class SigmoidCombFunction : public UnaryOpCombFunction {
 public:
  virtual double combine(const NlpWeight* weight,
			const GroundLinkList *links) const;
  virtual double diffCombine(const NlpWeight* weight,
			    GroundLink diff,const GroundLinkList *link) const;
  SigmoidCombFunction(CombFunction *_cf1) 
    : UnaryOpCombFunction(_cf1) {}
  virtual ~SigmoidCombFunction();
};

#endif
