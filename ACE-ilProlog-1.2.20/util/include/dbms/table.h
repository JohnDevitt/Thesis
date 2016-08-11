
#include <vector>
#include <string>

using namespace std;

typedef enum {
  CELLTYPE_EMPTY,
  CELLTYPE_NOMINAL,
  CELLTYPE_DOUBLE,
  CELLTYPE_FLOAT,
  CELLTYPE_INT,
  CELLTYPE_BYTE,
  CELLTYPE_BIT,
  CELLTYPE_POINTER,
  CELLTYPE_STRING,
  CELLTYPE_UNKNOWN
} CellType;

#define DBMS_ATTRTYPE_UNKNOWN	0
#define DBMS_ATTRTYPE_TERM	1

#define DBMS_INT_BYTE		0
#define DBMS_INT_SHORT		1
#define DBMS_INT_INT		2
#define DBMS_INT_BIT		3

#define VECTOR_TYPE_INT		0

typedef unsigned int AttrID;
typedef unsigned char uchar;

class TableCell;
class NominalTableCell;
class RealTableCell;
class StringTableCell;

class AttributeType : public RefCountObject {
private:
	string m_Name;
	int m_Bytes;
public:
	AttributeType() {}
	AttributeType(const string& name, int bytes)
	  : m_Name(name), m_Bytes(bytes) {}
	virtual ~AttributeType();
	inline void setName(string& name) { m_Name = name; };
	inline void setName(const char* name) { m_Name = name; }
	inline const string& getName() const { return m_Name; };
	inline void setBytes(int bytes) { m_Bytes = bytes; };
	inline int getBytes() const { return m_Bytes; };
	virtual CellType getCellType() const;
	virtual int getAttrType() const;
	virtual TableCell* createCell();
	virtual void initMemory(uchar* data) const;
	virtual void deleteMemory(uchar* data) const;
	virtual void initValue(uchar* data) const;
	virtual void getStringValue(const uchar* data, string& strg) const=0;
	virtual void getString(string& strg) const;
	G_SO void print(ostream& out);
	G_SO void print(ostream& out, int tab1);
	G_SO void print(ostream& out, int tab1, int tab2);
	G_SO void printTab(ostream& out, const string& strg, int tab);
};

class DummyAttributeType : public AttributeType {
public:
  DummyAttributeType();
  virtual ~DummyAttributeType();
  virtual void getStringValue(const uchar* data, string& strg) const;
};

class NominalAttributeType : public AttributeType {
 private:
  vector<string> m_Values;
  DBMSValueHash m_Hash;
 public:
  NominalAttributeType(string& name);
  virtual ~NominalAttributeType();
  void addValue(string& value);
  inline int getIndex(string& value) { return m_Hash.valueIndex(value); };
  inline int getNbValues() const { return m_Values.size(); };
  inline const string& getValue(int i) const { return m_Values[i]; };
  void print_hash(ostream& out);
  virtual void getString(string& strg) const;
  virtual TableCell* createCell();
  virtual CellType getCellType() const;
  virtual void initMemory(uchar* data) const;
  inline DBMSValueHash* getHash() { return &m_Hash; };
  virtual void getStringValue(const uchar* data, string& strg) const;
  inline void putNominalValue(uchar *dst, const NominalTableCell *src);
};

class VectorAttributeType : public AttributeType {
public:
	VectorAttributeType(string& name);
	VectorAttributeType(const char *name);
	virtual ~VectorAttributeType();
	virtual void getString(string& strg) const;
	virtual TableCell* createCell();
	virtual CellType getCellType() const;
	virtual void initMemory(uchar* data) const;
	virtual void deleteMemory(uchar* data) const;
	virtual void initValue(uchar* data) const;
	virtual void getStringValue(const uchar* data, string& strg) const;
	void addIntValue(uchar* data, int value);
};

class RealAttributeType : public  AttributeType {
private:
	double m_Min, m_Max;
public:
	RealAttributeType(string& name);
	RealAttributeType(const char *name);
	virtual ~RealAttributeType();
	virtual TableCell* createCell();
	virtual void putRealValue(uchar * data, const RealTableCell* src)=0;
};

class DoubleAttributeType : public RealAttributeType {
public:
  DoubleAttributeType(string& name) : RealAttributeType(name) {setBytes(sizeof(double));}
  DoubleAttributeType(const char *name);
  virtual ~DoubleAttributeType();
  virtual void getString(string& strg) const;
  virtual CellType getCellType() const;
  virtual void initMemory(uchar* data) const;
  virtual void getStringValue(const uchar* data, string& strg) const;
  inline void putDoubleValue(uchar *data, double value) const {*((double*)data)=value;}
  virtual void putRealValue(uchar * data, const RealTableCell* src);
};

class FloatAttributeType : public RealAttributeType {
public:
  FloatAttributeType(string& name);
  FloatAttributeType(const char* name);
  virtual ~FloatAttributeType();
  virtual void getString(string& strg) const;
  virtual CellType getCellType() const;
  virtual void initMemory(uchar* data) const;
  virtual void getStringValue(const uchar* data, string& strg) const;
  inline void putFloatValue(uchar *data, float value) const {*((float*)data)=value;}
  virtual void putRealValue(uchar * data, const RealTableCell* src);
};

class TermAttributeType : public AttributeType {
public:
	TermAttributeType(string& name);
	TermAttributeType(const char* name);
	virtual ~TermAttributeType();
	virtual void getString(string& strg) const;
	virtual TableCell* createCell();
	virtual CellType getCellType() const;
	virtual int getAttrType() const;
	virtual void initMemory(uchar* data) const;
	virtual void getStringValue(const uchar* data, string& strg) const;
};

class IntBaseAttributeType : public AttributeType {
protected:
	int m_Bits;
public:
	IntBaseAttributeType(string& name);
	IntBaseAttributeType(const char* name);
	virtual ~IntBaseAttributeType();
	virtual TableCell* createCell();
	virtual void getString(string& strg) const;
	inline void setBits(int bits) { m_Bits = bits; }
	inline int getBits() const { return m_Bits; }
};

class IntAttributeType : public IntBaseAttributeType {
public:
	IntAttributeType(string& name);
	IntAttributeType(const char* name);
	virtual ~IntAttributeType();
	virtual CellType getCellType() const;
	virtual void initMemory(uchar* data) const;
	virtual void getStringValue(const uchar* data, string& strg) const;
};

class ByteAttributeType : public IntBaseAttributeType {
public:
	ByteAttributeType(string& name);
	ByteAttributeType(const char* name);
	virtual ~ByteAttributeType();
	virtual CellType getCellType() const;
	virtual void initMemory(uchar* data) const;
	virtual void getStringValue(const uchar* data, string& strg) const;
};

class BitAttributeType : public IntBaseAttributeType {
public:
	BitAttributeType(string& name);
	BitAttributeType(const char* name);
	virtual ~BitAttributeType();
	virtual CellType getCellType() const;
	//	virtual void getStringValue(const uchar* data, string& strg) const;
};

class SmallIntAttributeType : public IntBaseAttributeType {
public:
	SmallIntAttributeType(string& name);
	SmallIntAttributeType(const char* name);
	virtual ~SmallIntAttributeType();
	virtual CellType getCellType() const;
	//virtual void getStringValue(const uchar* data, string& strg) const;
};


class StringAttributeType : public AttributeType {
public:
  StringAttributeType(const string& name)
    : AttributeType(name, sizeof(char *)) {}
  virtual CellType getCellType() const;
  virtual TableCell* createCell();
  virtual void initMemory(uchar* data) const;
  virtual void deleteMemory(uchar* data) const;
  virtual void getStringValue(const uchar* data, string& strg) const;
  virtual void getString(string& strg) const;
  void putStringValue(uchar* data, const string& src) const;
  void putStringValue(uchar* data, const char* src) const;
  inline void putStringValue(uchar* data, const StringTableCell* cell) const;
};



typedef AttributeType* PAttributeType;

class TableHeader : public RefCountObject {
private:
	string m_Name;
	MutableRefCountVector<AttributeType> m_Attributes;
public:
	TableHeader();
	~TableHeader();
	G_SO void defineAttribute(const char* name, const char* type, int len, AttrID id);
	G_SO void expandToAttribute(AttrID id);
	G_SO void showAttributes(ostream& out, int tab1, int tab2);
	G_SO void showAttributesRow(ostream& out, int tabsize);
	inline int getNbCols() const { return m_Attributes.size(); };
	inline PAttributeType getAttr(int i) { return m_Attributes[i].get(); };
	inline void addAttr(PAttributeType attr) { m_Attributes.push_back(MutableRefCountPtr<AttributeType>(attr)); };
	inline void addAttr(int i, PAttributeType attr) { m_Attributes[i] = attr; };
	inline void setName(const string& name) { m_Name = name; };
	inline void setName(const char* name) { m_Name = name; };
	inline const string& getName() const { return m_Name; };
};

typedef MutableRefCountPtr<TableHeader> TableRCPtr;

class TableCell : public RefCountObject {
public:
  CellType getCellType() const;
  virtual AttributeType* getType() const = 0;
  virtual void print(ostream& out) const = 0;
  virtual void storeValue(uchar *data) const = 0;
};

class TableRecord : public RefCountObject {
protected:
	MutableRefCountVector<TableCell> m_Cells;
	TableRCPtr m_Header;
public:
	void createFrom(TableHeader* header);
	void print(ostream& out) const;
	inline void addCell(TableCell* cell) { m_Cells.push_back(MutableRefCountPtr<TableCell>(cell)); };
	inline TableCell* getCell(int i) const { return m_Cells[i].get(); };
	inline int getNbCols() const { return m_Cells.size(); };
};

class NominalTableCell : public TableCell {
 protected:
  int m_Value;
  NominalAttributeType* m_Type;
 public:
  NominalTableCell(NominalAttributeType* type) : m_Type(type), m_Value(-1) {}
  virtual AttributeType* getType() const;
  virtual void print(ostream& out) const;
  void setValue(const char* strg, int len);
  inline int getValue() const { return m_Value; };
  virtual void storeValue(uchar *data) const;
};

class RealTableCell : public TableCell {
 protected:
  double m_Value;
  RealAttributeType* m_Type;
 public:
  RealTableCell(RealAttributeType* type) : m_Type(type), m_Value(0.0) {}
  virtual AttributeType* getType() const;
  virtual void print(ostream& out) const;
  inline void setValue(double value) { m_Value = value; };
  inline double getValue() const { return m_Value; };
  virtual void storeValue(uchar *data) const;
};

class StringTableCell : public TableCell {
 protected:
  string m_Value;
  StringAttributeType* m_Type;
 public:
  StringTableCell(StringAttributeType* type) : m_Type(type), m_Value("") {}
  virtual AttributeType* getType() const;
  virtual void print(ostream& out) const;
  void setValue(const string& str) {m_Value=str;}
  inline const string& getValue() const { return m_Value; };
  virtual void storeValue(uchar *data) const;
};

// --------------------------------------------------------------------
// implementation of some inline methods.


inline void NominalAttributeType::putNominalValue(uchar *dst, const NominalTableCell *src) {
  *((int*)dst)=src->getValue();
}

inline void StringAttributeType::putStringValue(uchar* data, const StringTableCell* cell) const {
  putStringValue(data,cell->getValue());
}
