
#ifndef CITESEER_WORD_INC
#define CITESEER_WORD_INC

 
#include "StringTable.h"
#include "../../../../../Util/preproc/c/Tokenizer.h"

  
   
    
  


  
  
  

     
   

    
   



class WordId {
 private:
  unsigned short int low;
  unsigned char high;
 public:
  inline ostream& write(ostream& os) const {
    os.write((const char *)&low,sizeof(low));
    return os.write((const char*)&high,sizeof(high));}
  inline istream& read(istream& is) {
    is.read((char *)&low,sizeof(low));
    return is.read((char *)&high,sizeof(high));}
  inline short int getLow() const {return low;}
  inline char getHigh() const {return high;}
  inline WordId& operator=(const WordId& src) {
    low=src.low;high=src.high;return *this;}
  inline WordId() : low(0),high(0) {}
  inline WordId(int id) : low(id&0xffff),high(id>>16) {}
  inline WordId(const WordId& src) : low(src.low),high(src.high) {}
  inline WordId(istream& is) {read(is);}
  inline int get() const {return (high<<16)+low;}
};

inline bool operator==(const WordId& a,const WordId& b) {
  return (a.getHigh()==b.getHigh())&&(a.getLow()==b.getLow());
}


class WordPreproc {
 private:
  class WordLanguage : public TokenizerLanguage {
    char spaces[255];
  public:
    WordLanguage();
  };

  class WordTokenizer : public StreamTokenizer {
  private:
    WordLanguage m_word_language;
  public:
    WordTokenizer(istream& is) 
      : StreamTokenizer(&is,&m_word_language) {
      select_language(0);}
  };
  
  typedef vector<const char*> charptrvector;
  charptrvector m_txt_files;

  void preprocFile(StringTable* words,
		   ofstream& wordfile,
		   ofstream& wordidx,
		   const char *fname);
 public:
  void preproc(const string& indir,const string& outdir);
  WordPreproc(); 
};

typedef struct {
  unsigned int did;
  unsigned int wordfile_pos;
} DocumentWordInfo;


typedef pair<int,int> IDPosPair;

class WordData {
 private:
  CompactedStringTable *m_words;
  vector<DocumentWordInfo> m_documents;
  char *m_raw_abstracts;
  int m_abstractfile;
 public:
       
  const char* id2string(int i) const {
    return m_words->id2string(i);}
  unsigned int docCount() const {return m_documents.size()-1;}
  unsigned int wordCount() const {return m_words->count();}
  int wordSize() const {return m_words->size();}
  const char* getAbstractWord(int i) const {
    return m_words->id2string(getAbstract(i));}
  int getAbstract(int i) const {
    return ((WordId*)(m_raw_abstracts+(i*3)))->get();}
  const DocumentWordInfo& getDoc(int i) const {return m_documents[i];}
  WordData(const string& dir);
  ~WordData();
};



#endif
