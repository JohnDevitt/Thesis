


#include <string>
#include <ctype.h>

class PrositeFieldID {
 private:
  char m_id[2];
  void set_char(int i, char x) {
    m_id[i]=toupper(x);}
 public:
  char& operator[](int i) {return m_id[i];}
  const char& operator[](int i) const {return m_id[i];}
  int operator==(const PrositeFieldID& b) const {
    return (m_id[0]==b.m_id[0]) && (m_id[1]==b.m_id[1]);}
  int operator<(const PrositeFieldID& b) const {
    return (m_id[0]<b.m_id[0])||((m_id[0]==b.m_id[0])&&(m_id[1]==b.m_id[1]));}
  void set(char x, char y) {set_char(0,x);set_char(1,y);}
  PrositeFieldID(char x, char y) {set(x,y);}
  PrositeFieldID(const char* x) {set(x[0],x[1]);}
};
inline ostream& operator<<(ostream& os, const PrositeFieldID& id) {
  return os << id[0] << id[1];
}

class PrositePatField {
 private:
  PrositeFieldID m_id;
 public:
  const PrositeFieldID& id() const {return m_id;}
  virtual ostream& write(ostream& os) const=0;
  PrositePatField(const char *id) : m_id(id) {} 
  virtual ~PrositePatField();
};
inline ostream& operator<<(ostream& os, const PrositePatField& fld) {
  return fld.write(os);
}


class PrositePatFieldUnknown : public PrositePatField {
 private:
  string m_txt;
 public:
  virtual ostream& write(ostream& os) const;
  PrositePatFieldUnknown(const char *id, const char *txt)
    : PrositePatField(id), m_txt(txt) {}
};

class PrositePattern {
 private:
  vector<PrositePatField*> m_fields;
  void read(istream& is);
 public:
  ostream& write(ostream& os) const;
  void Print() const {Print(cout);}
  ostream& Print(ostream& os) const {return write(os);}
  void clear();
  PrositePattern() {}
  ~PrositePattern() {clear();}
};


extern gapic_type GapicPrositePatternType;
typedef GapicNonExMutCollectable<PrositePattern,&GapicPrositePatternType> 
	PrositePatternCollectable;
