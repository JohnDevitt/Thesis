
class PrologList {
private:
	FILE* m_Handle;
	long m_Index;
public:
	PrologList(const char* file);
	void close();
	long getNextInt();
};
