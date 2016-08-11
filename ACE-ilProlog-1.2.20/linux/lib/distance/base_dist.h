
#ifndef GAPIC_DISTANCE_BASE_INC
#define GAPIC_DISTANCE_BASE_INC

#include <iostream.h>
#include "../gapic/refcount.h"

#ifndef C_PROLOG_H_INC
typedef double GAPIC_float;
typedef int GAPIC_int;
#endif

typedef enum {
  DISTANCE_NULL=0,
  DISTANCE_REFLEXIVE=1,
  DISTANCE_SYMMETRIC=2,
  DISTANCE_TRANSITIVE=4,
  DISTANCE_STRICT=8,
  DISTANCE_COST=5,
  DISTANCE_METRIC=7,
  DISTANCE_STRICTCOST=13,
  DISTANCE_STRICTMETRIC=15
} Distance_property_constant;

class distance_properties {
 private:
  int properties;
  bool has_property(int prop) const {return (properties&prop)==prop;}
 public:
  bool IsSymmetric() const {return has_property(DISTANCE_SYMMETRIC);}
  bool IsTransitive() const {return has_property(DISTANCE_TRANSITIVE);}
  bool IsStrict() const {return has_property(DISTANCE_STRICT);}
  bool IsMetric() const {return has_property(DISTANCE_METRIC);}
  bool IsStrictMetric() const {return has_property(DISTANCE_STRICTMETRIC);}
  bool IsCost() const {return has_property(DISTANCE_COST);}
  bool IsStrictCost() const {return has_property(DISTANCE_STRICTCOST);}

  distance_properties(int _prop) {properties=_prop;}
  distance_properties() {properties=0;}
};   


class Distance : public RefCountObject {
 protected:
  ostream& WarnNotSomeProperty(ostream& os,
			       const char *what, 
			       const char *property);
  void WarnNotPositive(const char *what) {
    WarnNotPositive(cout,what);}
  ostream& WarnNotPositive(ostream& os,const char *what);
  void WarnNotReflexive(const char *what) {
    WarnNotReflexive(cout,what);}
  ostream& WarnNotReflexive(ostream& os,const char *what);
  void WarnNotSymmetric(const char *what) {
    WarnNotSymmetric(cout,what);}
  ostream& WarnNotSymmetric(ostream& os,const char *what);
  void WarnNotTransitive(const char *what) {
    WarnNotTransitive(cout,what);}
  ostream& WarnNotTransitive(ostream& os,const char *what);
  void WarnNotStrict(const char *what) {
    WarnNotStrict(cout,what);}
  ostream& WarnNotStrict(ostream& os,const char *what);
  void WarnNotSimilarity(const char *what) {
    WarnNotSimilarity(cout,what);}
  ostream& WarnNotSimilarity(ostream& os,const char *what);
  void WarnNotCost(const char *what) {
    WarnNotCost(cout,what);}
  ostream& WarnNotCost(ostream& os,const char *what);
  void WarnNotMetric(const char *what) {
    WarnNotMetric(cout,what);}
  ostream& WarnNotMetric(ostream& os,const char *what);
  void WarnNotStrictMetric(const char *what) {
    WarnNotStrictMetric(cout,what);}
  ostream& WarnNotStrictMetric(ostream& os,const char *what);
  distance_properties properties;
  const char* name;
 protected:
  void SetProperties(distance_properties _properties) {
    properties=_properties;}
 public:
  const char* GetName() const {return name;}
  ostream& Print(ostream& os) const;
  ostream& PrintName(ostream& os) const;
  const distance_properties GetProperties() const {return properties;}
  Distance(Distance& src) 
    : name(src.name),properties(src.properties) {}
  Distance(const char *_name) : name(_name) {
     
  }
  Distance(const char *_name, const distance_properties _properties) 
    : name(_name), properties(_properties) {}
  Distance(const char *_name, Distance_property_constant _properties) 
    : name(_name), properties(_properties) {}
};

#endif
