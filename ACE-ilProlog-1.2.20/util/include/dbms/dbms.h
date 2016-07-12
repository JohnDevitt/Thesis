
// #define ALIGN_CONVERT_ENABLE

#ifndef DBMS_INC
#define DBMS_INC

#include <string>
#include <iostream>

#include <stdlib.h>
#include <stdio.h>

#define CHUNK_SIZE		60000
#define MAX_RECORD_SIZE		10000000
#define ATTR_NAME_LEN		30

typedef char* charptr;

#define TYPE_UNKNOWN	0
#define TYPE_INTEGER	1
#define TYPE_FLOAT	2
#define TYPE_STRING	3

#define RECORD_OPEN_WRITE	0
#define RECORD_OPEN_READ	1
#define RECORD_OPEN_RW		2

#define HAS_INFO(a,b)		((a->m_Info & b) != 0)
#define INFO_INDEX		1

class MVisitor;

class MVisitable {
protected:
	MVisitor* m_Visitor;
public:
	void setVisitor(MVisitor* vis);
	inline MVisitor* getVisitor() { return m_Visitor; };
};

class MVisitor {
public:
	virtual void initVisitor(MVisitable* vis) = 0;
	virtual MVisitor* cloneVisitor() = 0;
};

void printstrg(const char* strg, int spaces);
void printstrgafter(const char* strg, int spaces);
void printspc(int spaces);
void printnumber(int number, int spaces);
void printnstring(const char* strg, int len);

long atoin(const char* strg, int len);

#include <../preproc/c/Tokenizer.h>
#include <../preproc/c/RefCount.h>

struct dbms_eq_string {
	inline bool operator() (const string& s1, const string& s2) const {
	    return s1 == s2;
	}
};

struct dbms_hash_string {
	inline size_t operator() (const string& s) const {
		return hash<const char *>()(s.c_str());
	}
};

class DBMSValueHash : public hash_map<string, int, dbms_hash_string, dbms_eq_string> {
public:
	int valueIndex(string& key);
	void addValue(string& key, int index);
};

class DBMSInFile {
protected:
	filebuf* m_fb;
	istream* m_is;
	string m_name;
	streampos m_size;
public:
	DBMSInFile();
	~DBMSInFile();
	void open(string& name);
	void close();
	streampos tell();
	void seek(streampos pos);
	inline istream* getStream() { return m_is; };
	inline string& getName() { return m_name; };
	inline streampos getSize() { return m_size; };
};

#include <dbms/table.h>
#include <dbms/arff.h>
#include <dbms/csvfile.h>
#include <dbms/binfile.h>
#include <dbms/oldbinfile.h>
#include <dbms/prologlist.h>

#ifdef ALIGN_CONVERT_ENABLE
	#include <dbms/aligncnv.h>
#endif

#endif
