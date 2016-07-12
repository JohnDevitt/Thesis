


#include <string>
#include "dna.h"

class ProteinSeq {
 private:
  const char *m_data;
  string m_name;
  int m_length;
 public:
	void GetLength(GAPIC_int* len) const {*len=m_length;}

	void GetAcid(GAPIC_int pos, GAPIC_term_ref dst) const {  

    if((pos<1)||(pos>m_length)) {
      cerr << "ProteinSeq::GetAcid: pos (" << pos 
	   << ") out of range (1-"<<m_length<<")\n";
      return;
    }
    GAPIC_put_atom(dst,*(one_char_atom[m_data[pos-1]-32]));
  }
	void SetName(GAPIC_term_ref name) {

    GAPIC_atom name_atom(name);
    m_name=name_atom.GetString();
  }
	void GetName(GAPIC_term_ref name) const {

    GAPIC_atom name_atom;
    name_atom.PutString(m_name.c_str());
    GAPIC_put_atom(name,name_atom);
  }
  void Print() const {Print(cout);}
  ostream& Print(ostream& os) const;
  ProteinSeq() : m_data(NULL), m_length(0) {}
  ProteinSeq(const char*src,int len) 
    : m_name(""), m_length(len), m_data(src) {}
  ProteinSeq(const char*src,int len, string name) 
    : m_name(name), m_length(len), m_data(src) {}
};



extern gapic_type GapicProteinSeqType;
typedef GapicNonExMutCollectable<ProteinSeq,&GapicProteinSeqType> 
	ProteinSeqCollectable;
