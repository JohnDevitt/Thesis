#ifndef PPOLYNOME_INC
#define PPOLYNOME_INC



#include "func_approx/polynome.h"

class PpolynomeFit : public PolynomeFit {
 private:
  Polynomial m_poly;
 public:  
	void add_point(GAPIC_float x, GAPIC_float y, GAPIC_float weight) {

    addPoint(x,y,weight);}
	void compute_fit() {

    getCurve(m_poly);}
	void apply(GAPIC_float x, GAPIC_float* y) {

    *y=m_poly.apply(x);}
	void differentiate() {

    m_poly.differentiate();}
	void get_nth_diff_coef(GAPIC_int i, GAPIC_int n, GAPIC_float* coef) {

    *coef=m_poly.getNthDiffCoef(i,n);}
	void nth_diff_at_x(GAPIC_int n, GAPIC_float x, GAPIC_float* y){

    *y=m_poly.nthDiffAtX(n,x);}
	void get_coeff(GAPIC_int i, GAPIC_float* y) {

    *y=m_poly.getCoeff(i);}


  PpolynomeFit(GAPIC_term_ref term) 
    : PolynomeFit(degreeFromTerm(term)) {}
  int degreeFromTerm(GAPIC_term_ref term) {
    int degree;
    if(GAPIC_get_integer(term,&degree)!=GAPIC_SUCCESS)
      cerr << "Error: PpolynomeFit constructor arg should be int!\n";
    return degree;
  }
  ostream& Print(ostream& os) const {
    os << "[PpolynomeFit(";
    m_poly.write(os);
    os << endl;
    writeCoef(os);
    os << ")]\n";}
};

extern gapic_type GapicPolynomeFitType;
typedef GapicNonExMutCollectable<PpolynomeFit,&GapicPolynomeFitType> 
	PpolynomeFitCollectable;

#endif
