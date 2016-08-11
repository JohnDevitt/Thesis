
#ifndef CITESEER_STRINGTABLE_INC
#define CITESEER_STRINGTABLE_INC

#include <string.h>
#include <hash_map.h>
#include "../../../../../Util/preproc/c/StringKeyHash.h"

#ifndef GAPIC_realloc
#include <stdlib.h>
#define GAPIC_realloc(x,y) realloc(x,y)
#endif

class StringTable {
 private:
  typedef StringKeyHash<int> StringKeyHashInt; 
  StringKeyHashInt m_hash;
  vector<StringKeyHashInt::value_type *> m_table;
 public:
  int String2ID(const string& str);
  const string& ID2String(int id) {
    return m_table[id]->first;}
  int count() const {
    return m_table.size();}
  typedef vector<StringKeyHashInt::value_type *>::iterator iterator;
  iterator begin() {return m_table.begin();}
  iterator end() {return m_table.end();}
  typedef vector<StringKeyHashInt::value_type *>::const_iterator const_iterator;
  const_iterator begin() const {return m_table.begin();}
  const_iterator end() const {return m_table.end();}
  StringTable() {}
};

class CompactedStringTable {
 private:
  typedef pair<int,int> IdPosPair;
  char *m_char_table;
  int m_char_table_size;
  int m_word_count; 
  IdPosPair* m_word_table_id_index;
  IdPosPair* m_word_table_alpha_index;
 public:
  void makeAlphaIndex();
  void write(ostream& os) const;
  int getCharTableSize() const {return m_char_table_size;}
  int getWordCount() const {return m_word_count;}
  const char* id2string(int i) const {
    return m_char_table+m_word_table_id_index[i].second;}
  int alphaId(int i) const {
    return m_word_table_alpha_index[i].first;}
  const char* alphaString(int i) const {
    return m_char_table+m_word_table_alpha_index[i].second;}
  int count() const {return m_word_count;}
  int size() const {return m_char_table_size;}
  CompactedStringTable(const StringTable& src);
  CompactedStringTable(istream& is);
  ~CompactedStringTable();
};

#endif
