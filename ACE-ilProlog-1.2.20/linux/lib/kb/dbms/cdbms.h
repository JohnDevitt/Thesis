
#ifndef KB_CDBMS_INC
#define KB_CDBMS_INC


class CLASS_SO GapicNominalType {
protected:
	GAPIC_atom* m_Values;
	int m_NbValues;
public:
	GapicNominalType();
	~GapicNominalType();
	void createFrom(NominalAttributeType* type);
	inline GAPIC_atom* getValue(int i) { return &m_Values[i]; };
	inline int getNbValues() { return m_NbValues; };
};

class CLASS_SO GapicBINTable : public BINTable {
protected:
	GapicNominalType* m_Nominal;
public:
	GapicBINTable();
	virtual ~GapicBINTable();
	void createFrom(TableHeader* header);
	G_SO void getSchema(GAPIC_int kind, GAPIC_term_ref ref);
	G_SO GAPIC_result get(GAPIC_int row, GAPIC_int col, GAPIC_term_ref ref);
	G_SO GAPIC_result set(GAPIC_int row, GAPIC_int col, GAPIC_term_ref ref);
	G_SO GAPIC_result getTupleProlog(GAPIC_int row, GAPIC_term_ref ref);
};

int dbms_load_arff_internal(const char* fname);

G_SO GapicBINTable* getBINTable(int id);

#endif

