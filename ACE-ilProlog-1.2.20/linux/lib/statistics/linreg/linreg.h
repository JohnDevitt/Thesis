


#ifndef LINREG_INC
#define LINREG_INC

extern gapic_type LinRegType;

#define LIN_REG_VARS 2

class LinReg {
protected:
	GAPIC_float m_Sum[LIN_REG_VARS];
	GAPIC_float m_SumSq[LIN_REG_VARS];
	GAPIC_float m_SumXY, m_Count;
public:
	LinReg();
	void clear();

	void update(GAPIC_float x, GAPIC_float y);

	double covariance() const;

	double variance(GAPIC_int which) const;

	double mean(GAPIC_int which) const;

	double intercept() const;

	double slope() const;

	double count() const;	

	double r() const;

	double resvar() const;

	ostream & Print(ostream & strm) const;
};



typedef GapicNonExMutCollectable<LinReg, &LinRegType> LinRegCollectable;

#endif
