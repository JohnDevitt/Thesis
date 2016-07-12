

#include <iostream>
#include <vector>



class Graph {
 public:
  virtual std::ostream& write(std::ostream& os) const=0;
  virtual bool isEdge(unsigned i, unsigned j) const=0;
  virtual int resize(size_t vertex_cnt)=0;
  virtual ~Graph();
};

class LabeledGraph : public Graph {
 public:
  virtual std::ostream& write(std::ostream& os) const=0;
  virtual bool isEdge(unsigned i, unsigned j) const=0;
  virtual ~LabeledGraph();
};

class BitMatrix {
 private:
  static const int safe_code=1;
  std::vector<bool> m_matrix;
  size_t m_rows, m_cols;
 public:
  size_t cols() const {return m_cols;}
  size_t rows() const {return m_rows;}
  size_t size() const {return m_cols*m_rows;}
  inline std::vector<bool>::reference operator()(int i,int j);
  inline bool operator()(int i,int j) const;
  inline void set_rows(size_t r, bool b=0);
  inline void set_cols(size_t c, bool b=0);
  inline void resize(size_t c, size_t r, bool b=0);
  BitMatrix() : m_rows(0), m_cols(0) {}
};

class MatrixGraph : public Graph {
 private:
  BitMatrix m_adj;
  size_t m_vertex_cnt;
 public:
  virtual bool isEdge(unsigned i, unsigned j) const;
  virtual std::ostream& write(std::ostream& os) const;
  virtual ~MatrixGraph();
};


    

inline std::vector<bool>::reference BitMatrix::operator()(int i,int j) {
  if(safe_code) 
    if((i<0)||(j<0)||(i>=m_rows)||(j>=m_cols)) 
      std::cerr << "BitMatrix::operator[] : out of range\n";
  return m_matrix[i*m_cols+j];
}

inline bool BitMatrix::operator()(int i,int j) const {
  if(safe_code) 
    if((i<0)||(j<0)||(i>=m_rows)||(j>=m_cols)) 
      std::cerr << "BitMatrix::operator[] : out of range\n";
  return m_matrix[i*m_cols+j];
}

inline void BitMatrix::set_rows(size_t r, bool b) {
  if(r!=m_rows) {
    m_matrix.resize(m_cols*m_rows,b);
    m_rows=r;
  }
}

inline void BitMatrix::set_cols(size_t c, bool b) {
  if(c<m_cols) {
    for(int r=1;r<m_rows;r++)
      for(int ci=0;ci<c;ci++)
	m_matrix[r*c+ci]=m_matrix[r*m_cols+ci];
  } else if(c>m_cols) {
    for(int r=m_rows-1;r>0;r--) {
      for(int ci=c-1;ci>=m_cols;c--)
	m_matrix[r*c+ci]=b;
      for(int ci=m_cols-1;ci>=0;ci--)
	m_matrix[r*c+ci]=m_matrix[r*m_cols+ci];
    }
  } 
  m_cols=c;
}

inline void BitMatrix::resize(size_t c, size_t r, bool b) {
  if(r<m_rows) set_rows(r,b);
  set_cols(c,b);
  set_rows(r,b);
}  
 
