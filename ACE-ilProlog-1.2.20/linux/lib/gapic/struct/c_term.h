
#ifndef C_TERM_H
#define C_TERM_H

class CTerm {
public:
	virtual void fromTerm(GAPIC_term_ref ref) = 0;
	virtual void toTerm(GAPIC_term_ref ref) const = 0;
	virtual void unifyTerm(GAPIC_term_ref ref) const = 0;
	virtual ostream& write(ostream& os) const = 0;
};

typedef CTerm* CTermPtr;

class CTermAtom : public CTerm {
private:
	GAPIC_atom m_Name;
public:
	CTermAtom(GAPIC_term_ref ref);
	virtual void fromTerm(GAPIC_term_ref ref);
	virtual void toTerm(GAPIC_term_ref ref) const;
	virtual void unifyTerm(GAPIC_term_ref ref) const;
	virtual ostream& write(ostream& os) const;
};

class CTermInt : public CTerm {
private:
	GAPIC_int m_Name;
public:
	CTermInt(GAPIC_term_ref ref);
	virtual void fromTerm(GAPIC_term_ref ref);
	virtual void toTerm(GAPIC_term_ref ref) const;
	virtual void unifyTerm(GAPIC_term_ref ref) const;
	virtual ostream& write(ostream& os) const;
};

CTerm* GAPIC_createCTerm(GAPIC_term_ref term);

#endif
