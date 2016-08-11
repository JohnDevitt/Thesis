
typedef struct {
	unsigned char m_Type;
	unsigned int m_Bits;
	unsigned int m_Offset;
	unsigned int m_Shift;
	unsigned int m_Mask;
	long m_Subtract;
	long m_Missing;
} OldBINAttrDesc;

class OldBINFile : public MVisitable {
protected:
	// Fixed
	FILE* m_Handle;
	long m_NbRecords;
	int m_NbCols;
	int m_RecordBytes;
	char m_SubCountBits;
	unsigned int m_NbSubRecs;
	unsigned char m_Info;
	// Variable
	OldBINAttrDesc* m_Attrs;
	OldBINFile* m_SubRecs;
public:
	OldBINFile();
	OldBINFile(int nbattr);
	void init(int nbattr);
	void addSubRecord(OldBINFile* file);
	void close();
	void optimize();
	void initarrays();
	void writeHeader();
	void readHeader();
	long getSize();
	long getPosition();
	int getLastIndex(unsigned char* bitsfree, int maxbytes);
	void printMapping(CSVHeader* header);
	void readBytes(void* buffer, size_t nbrd);
	void writeBytes(void* bytes, int nb);
	void skipBytes(int nb);
	void copyClone(OldBINFile* file);
	void copyNoClone(OldBINFile* file);
	void copyFixed(OldBINFile* file);
	void createVisitors();
	void printTuple(unsigned char* recbits);
	void setHandle(FILE* handle);
	long getIntAttribute(int idx, unsigned char* recbits);
	inline FILE* getHandle() { return m_Handle; };
	inline long getNbRecords() { return m_NbRecords; };
	inline long getNbSubRecs() { return m_NbSubRecs; };
	inline int getNbCols() { return m_NbCols; };
	inline int getNbCols(int sub) { return m_SubRecs[sub].m_NbCols; };
	inline int getRecordBytes() { return m_RecordBytes; };
	inline void setNbRecords(long nbrec) { m_NbRecords = nbrec; };
	inline OldBINFile* getSubRec(int i) { return &m_SubRecs[i]; };
	inline OldBINAttrDesc* getAttr(int i) { return &m_Attrs[i]; };
	inline unsigned char orInfo(unsigned char bit) { m_Info |= bit; return m_Info; };
};

class RecordOldBINFile : public MVisitor {
private:
	unsigned char* m_Bytes;
	unsigned int* m_SRCounts;
	OldBINFile* m_File;
public:
	RecordOldBINFile();
	RecordOldBINFile(int nbattr);
	void open(const char* fname, char read_write);
	void close();
	void convertRecord(CSVFile* file);
	void writeRecord();
	void initRead();
	void allocateBytes();
	void setHandles(FILE* handle);
	void copyClone(RecordOldBINFile* file);
	void storeNbRecords(long nbr);
	void storeInfo(unsigned char bits);
	void copySubRecCounts(RecordOldBINFile* file);
	void writeSubRecCount(unsigned int count);
	void copySubRecs(RecordOldBINFile* file);
	void skipSubRecs();
	void copyPlainRecord(RecordOldBINFile* file);
	void readSubRecCounts();
	void readPlainRecord();
	void skipPlainRecord();
	void printPlainRecord();
	void printSubRecs(int tab);

	virtual void initVisitor(MVisitable* vis);
	virtual MVisitor* cloneVisitor();
	inline OldBINFile* getFile() { return m_File; };
};

class TableOldBINFile : public OldBINFile {
private:
	unsigned char* m_Buffer;
	long* m_Index;
public:
	TableOldBINFile();
	void loadTable(const char* fname);
	long getAttribute(long row, int idx);
	long getNbSubRecords(int sub, long mrow);
	long getAttribute(int sub, long mrow, int srow, int col);
};

