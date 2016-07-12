


#ifndef MULTILINREG_INC
#define MULTILINREG_INC
 
extern gapic_type MultiLinRegType;

#include <math/matrix/matrix.C>
#include <statistics/exdistr_statistic/target_value.h>
#include <statistics/exdistr_statistic/stored_attribute.h>
#include <dbms/dbms.h>
#include <kb/dbms/cdbms.h>
#include <cmath>

class MultiLinReg {
protected:
	GAPIC_int m_NbExamples;
	GAPIC_int m_NbVars;

	Matrix* m_X;
	Matrix* m_Y;
	Matrix* m_Coeffs;
	 
	StoredRealAttribute* m_Target;
public:
	MultiLinReg(GAPIC_term_ref init);
	MultiLinReg() {};
	~MultiLinReg();
	ostream& Print(ostream& os) const {return os;}
	void fillY(GAPIC_term_ref ex_list);
	void fillX(GAPIC_term_ref xvalue_list);
	void coeffs(GAPIC_term_ref coef);    

	double resvar();      

};

typedef GapicNonExMutCollectable<MultiLinReg, &MultiLinRegType> MultiLinRegCollectable;

#endif
