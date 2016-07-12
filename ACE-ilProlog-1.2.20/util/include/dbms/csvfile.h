
class CSVFile {
private:
	DBMSInFile* m_File;
	istream* m_Stream;
	streampos m_Start;
	int m_NbCols;
	int m_CrRecord;
	size_t m_BufferPos;
	size_t m_BufferLen;
	char m_EndOfFile, m_PushBack;
	char m_PushBackChar;
	char* m_Buffer;
	char* m_Record;
	unsigned long m_RecordPos;
	unsigned long m_RecordLen;
	unsigned long* m_RecordI1;
	unsigned long* m_RecordI2;
public:
	CSVFile();
	~CSVFile();
	void open(DBMSInFile* file);
	void init();
	char isEOF();
	void getNextRecord();
	int getNextField();
	void skipSpaces();
	char isLastRecord();
	int getNextRecordChar();
	char getNextBufferChar();
	void pushBackBufferChar(char pushback);
	int countCols();
	int indexCols();
	int setNbCols(int cols);
	void printRecord();
	void dumpRecord();
	long countRows();
	void reset();
	char isMissing(int idx);
	long getLongInt(int idx);
	void toRecord(TableRecord* record);
	void toRecordSub(int i, TableCell* cell);
	double getDouble(int idx);
	string getString(int idx);
	string getQuotedString(int idx);
	unsigned int getUnsInt(int idx);
	inline int getNbCols() { return m_NbCols; };
	inline int getColWidth(int i) { return (m_RecordI2[i]-m_RecordI1[i]+1); };
	inline const char* getCol(int i) { return m_Record+m_RecordI1[i]; };
	inline int getCrRecord() { return m_CrRecord; };
	inline int getStartPos() { return m_Start; };
};

class CSVHeader {
private:
	int m_NbCols;
	long m_NbRows;
	char** m_Names;
	unsigned char* m_Types;
	long double* m_DMin;
	long double* m_DMax;
	long* m_IMin;
	long* m_IMax;
	long* m_NbMissing;
public:
	CSVHeader(CSVFile* file);
	unsigned char findType(const char* attr, int len);
	void updateTypes(CSVFile* file);
	void printHeader();
	void printType(int i);
	void printRecord(CSVFile* file);
	inline int getNbCols() { return m_NbCols; };
	inline const char* getName(int i) { return m_Names[i]; };
};

