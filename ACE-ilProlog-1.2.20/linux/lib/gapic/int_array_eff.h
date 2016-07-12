


#ifndef EFFMATRIX_INC
#define EFFMATRIX_INC

extern gapic_type IntMatrixEffType;

class CLASS_SO IntMatrixEff : public GapicIDObject {
protected:
	GAPIC_int m_Rows, m_Cols, m_Bits;
	GAPIC_int m_BitsBase, m_CellAddrShift, m_CellAddrMask, m_BitCellMask;
	unsigned long* m_Data;
public:
	IntMatrixEff(GAPIC_term_ref init);
	int getArray(GAPIC_int index) const;

	void putArray(GAPIC_int index, GAPIC_int value);

	G_SO int get(GAPIC_int row, GAPIC_int col) const;

	void put(GAPIC_int row, GAPIC_int col, GAPIC_int value);

	void add(GAPIC_int row, GAPIC_int col, GAPIC_int value);	

	int getSize();		

	ostream& Print(ostream& os) const;
	inline int getNbRows() { return m_Rows; }
	inline int getNbCols() { return m_Cols; }	
};

typedef GapicNonExMutCollectable<IntMatrixEff, &IntMatrixEffType> IntMatrixEffCollectable;

#endif
