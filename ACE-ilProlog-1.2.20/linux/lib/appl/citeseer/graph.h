
#ifndef CITESEER_GRAPH_INC
#define CITESEER_GRAPH_INC

#include <hash_map.h>
#include <string>
#include "StringTable.h"
#include "../../../../../Util/preproc/c/Tokenizer.h"
#include "word.h"

typedef string AuthorName;
typedef int AuthorID;
typedef int CiteseerGID;
typedef int CiteseerDID;

  


     
   

     
   


typedef struct {
  CiteseerGID gid;
  CiteseerDID did;
} CitationInfo;

#define CITESEER_GRAPH_TEXT 1

class CiteseerGraphData {
 private:
  CiteseerDID m_did;
  string m_key;
  int m_year;
  vector<CitationInfo> m_citations;
  vector<WordId> m_title;
     
  AuthorID *m_authors;
  short int m_author_count;
 public:
  ostream& write(ostream& os) const;
  CiteseerDID getDid() const {return m_did;}
  const string& getKey() const {return m_key;}
  int getYear() const {return m_year;}
  int authorCount() const {return m_author_count;}
  AuthorID getAuthor(int i) const {return m_authors[i];}
  int getTitleLength() const {return m_title.size();}
  WordId getTitleWord(int i) const {return m_title[i];}
  int getCitationCount() const {return m_citations.size();}
  CiteseerDID getCitationDid(int i) const {return m_citations[i].did;}
  CiteseerGID getCitationGid(int i) const {return m_citations[i].gid;}
       
  CiteseerGraphData(StringTable *word_table, 
		    StringTable *author_table, 
		    Tokenizer& tok);
  CiteseerGraphData(istream& is);
  ~CiteseerGraphData() {
    delete m_authors;}
};

void preprocGraph(const string& indir,
		  const string& outdir,
		  StringTable *word_table);

typedef CiteseerGraphData* CiteseerGraphDataPtr;

class CiteseerGraph : public vector<CiteseerGraphDataPtr> {
 private:
  CompactedStringTable *m_author_table;
 public:
  int authorCount() const {return m_author_table->count();}
  const CompactedStringTable* getAuthors() const {return m_author_table;}
  int findDid(int did) const;
  ostream& write(ostream& os) const;
  CiteseerGraph(istream& is);
};

#endif

