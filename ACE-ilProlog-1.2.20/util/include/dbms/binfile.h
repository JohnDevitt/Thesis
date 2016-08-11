
class BINHeader : public RefCountObject {
protected:
	TableRCPtr m_Header;
	CellType* m_Types;
	int* m_Offsets;
	int m_Size;
public:
	BINHeader();
	~BINHeader();
	G_SO void createFromEmpty();
	G_SO void createFrom(TableHeader* header);
	void tryInitialize();
	inline void defineAttribute(const char* name, const char* type, int len, AttrID id) {
		m_Header->defineAttribute(name, type, len, id);
	}
	inline const string& getName() const { return m_Header->getName(); }
	inline void setName(const string& name) { m_Header->setName(name); }
	inline void setName(const char* name) { m_Header->setName(name); }
	inline int getSize() { return m_Size; };
	inline int getOffset(int i) { return m_Offsets[i]; };
	inline TableHeader* getHeader() { return m_Header.get(); };
	inline int getNbCols() { return getHeader()->getNbCols(); };
	inline void showAttributes(ostream& out, int tab1, int tab2) { m_Header->showAttributes(out, tab1, tab2); }
};

class CLASS_SO BINTable : public BINHeader {
protected:
	int m_NbRows;
	int m_DataSize;
	unsigned char* m_Data;
public:
	BINTable();
	~BINTable();
	G_SO void expandRows(int rows);
	inline void addRow() { expandRows(getNbRows()+1); }
	inline void addRows(int rows) { expandRows(getNbRows()+rows); }
	G_SO int getIntAttribute(int row, int col);
	G_SO void setIntAttribute(int row, int col, int value);
	G_SO unsigned char getByteAttribute(int row, int col);
	G_SO void addDoubleAttribute(int row, int col, double add);
	G_SO double getDoubleAttribute(int row, int col);
	G_SO void addIntVectorAttribute(int row, int col, int value);
	G_SO vector<int>* getIntVectorAttribute(int row, int col);
	G_SO void tryClearRow(int row);
	G_SO void deleteAttributes(int row);
	G_SO void init(int row, int col);
	void expandBinTable();
	G_SO void createTable(int nbrows);
	G_SO void fromRecord(int row, TableRecord* record);
	G_SO void getStringValue(int row, int col, string& strg);
	G_SO void print(int tabsize);
	inline int getNbRows() { return m_NbRows; };
	inline int getDataSize() { return m_DataSize; }
};

