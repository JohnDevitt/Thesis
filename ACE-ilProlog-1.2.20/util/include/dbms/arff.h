
class ARFFFile {
protected:
	DBMSInFile m_File;
public:
	ARFFFile();
	~ARFFFile();
	void open(string& name);
	void close();
	TableHeader* readHeader();
	void readRelation(Tokenizer* tokens, TableHeader* header);
	void readAttribute(Tokenizer* tokens, TableHeader* header);
	void initLanguage(Tokenizer* tokens);
	string getExpected(int i);
	inline DBMSInFile* getFile() { return &m_File; };
};
