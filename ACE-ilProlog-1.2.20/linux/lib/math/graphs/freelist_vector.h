#ifndef FREELIST_VECTOR_INC
#define FREELIST_VECTOR_INC

#include <vector>
#include <iostream>
using namespace std;

const int FREELIST_VECTOR_DEBUG_NONE=0;
const int FREELIST_VECTOR_DEBUG_STD=1;
const int FREELIST_VECTOR_DEBUG_INTENSIVE=2;

class freelist_vector_item {
  public:
  int is_deleted() const {return 0;}
  int is_not_deleted() const {return 0;}
  void set_deleted(int i) {}
  static const int delete_marking = 0; 
  static const int freelist_vector_debug = FREELIST_VECTOR_DEBUG_STD;
};

template <class itemtype>
class freelist_vector {
 public:
  typedef itemtype element_type;
  static const int safe_code=1;
 private:
  typedef std::vector<element_type> vector_type;
  typedef std::vector<unsigned> freelist_type;
  vector_type m_data;    
  freelist_type m_freelist;
  size_t m_cnt;
  static int _segfault() {
    if(safe_code>=5)
      *((int *)NULL)=0;
    return 0;
  }
 public:
  int check_nodel_id(const char *func, unsigned id) const {
    if(element_type::freelist_vector_debug == FREELIST_VECTOR_DEBUG_NONE)
      return 1;
    if(id<0) {
      cerr << func << " : id (=" << id << "<0\n";
      return _segfault();
    }
    if(id>=m_data.size()-1) {
      cerr << func << " : id (=" << id 
	   << " > size() (=" << m_data.size() << ")\n";
      return _segfault();
    }
    if(m_data[id].is_deleted()) {
      cerr << func << " : id (=" << id 
	   << " refers to deleted item\n";
      return _segfault();
    }
    if(element_type::freelist_vector_debug >= FREELIST_VECTOR_DEBUG_INTENSIVE){
      for(unsigned fli=0;fli<m_freelist.size();fli++)
	if(m_freelist[fli]==id) {
	  cerr << func << " : id (=" << id 
	       << " belongs to freelist\n";
	  return _segfault();
	}
    }
    return 1;
  }
  inline size_t size() const {return m_cnt;}
  inline const element_type& operator[](unsigned i) const {
    if(safe_code) 
      if(!check_nodel_id("freelist_vector::const operator[]",i))
	return *((element_type*)NULL);
    return m_data[i];
  }
  inline element_type& operator[](unsigned i) {
    if(safe_code) 
      if(!check_nodel_id("freelist_vector::operator[]",i))
	return *((element_type*)NULL);
    return m_data[i];
  }
  inline unsigned add() {
    unsigned id;
    if(m_freelist.empty()) {
      id=m_data.size()-1;
      m_data.push_back(element_type());    
    }
    else {
      id=m_freelist[m_freelist.size()-1];
      m_freelist.pop_back();
      m_data[id].set_deleted(0);
    }
    m_cnt++;
    return id;
  }
  inline void del(unsigned id) {
    if(safe_code) 
      if(!check_nodel_id("freelist_vector::del(id)",id)) 
	return;
    m_data[id].set_deleted(1);
    m_cnt--;
    m_freelist.push_back(id);
  }
  int check_consistency(int talking=1) const;
           
                   
               
 
  class iterator {
  private:
    typename vector_type::iterator m_item;
    void inc(int i) {
      while(i>0) {dec();i--;}
      while(i<0) {inc();i++;}
    }
  public:
    int iterator_id(typename vector_type::const_iterator begin) const {
      return m_item-begin;}
    void inc() {do {m_item++;} while(m_item->is_deleted());}
    void dec() {do {m_item--;} while(m_item->is_deleted());}
    iterator& operator++() {inc(); return *this;}
    iterator operator++(int) {
      iterator __tmp = *this; inc(); return __tmp;}
    iterator& operator--() {dec(); return *this;}
    iterator operator--(int) {
      iterator __tmp = *this; dec(); return __tmp;}
    iterator& operator+=(int i) {inc(i);return *this;}
    iterator& operator-=(int i) {inc(-i);return *this;}
    int operator==(const iterator& cmp) const {
      return m_item==cmp.m_item;}
    int operator!=(const iterator& cmp) const {
      return m_item!=cmp.m_item;}
    element_type* operator->() const {
      return &(*m_item);}
    element_type& operator*() const {
      return *m_item;}    
    iterator& operator=(const iterator& src) {
      m_item = src.m_item;}
    iterator(const typename vector_type::iterator& src)
      : m_item(src) {}
    iterator(const iterator& src) 
      : m_item(src.m_item) {}
  };
  unsigned iterator_id(const iterator& iter) const {
    return iter.iterator_id(m_data.begin());}
  iterator begin() {
    iterator i(m_data.begin()-1);i++;return i;}
  iterator end() {
    return iterator(m_data.end()-1);}   
 
  class const_iterator {
  private:
    typename vector_type::const_iterator m_item;
    void inc(int i) {
      while(i>0) {dec();i--;}
      while(i<0) {inc();i++;}
    }
  public:
    int iterator_id(typename vector_type::const_iterator begin) const {
      return m_item-begin;}
    void inc() {do {m_item++;} while(m_item->is_deleted());}
    void dec() {do {m_item--;} while(m_item->is_deleted());}
    const_iterator& operator++() {inc(); return *this;}
    const_iterator operator++(int) {
      const_iterator __tmp = *this; inc(); return __tmp;}
    const_iterator& operator--() {dec(); return *this;}
    const_iterator operator--(int) {
      const_iterator __tmp = *this; dec(); return __tmp;}
    const_iterator& operator+=(int i) {inc(i);return *this;}
    const_iterator& operator-=(int i) {inc(-i);return *this;}
    int operator==(const const_iterator& cmp) const {
      return m_item==cmp.m_item;}
    int operator!=(const const_iterator& cmp) const {
      return m_item!=cmp.m_item;}
    const element_type* operator->() const {
      return &(*m_item);}
    const element_type& operator*() const {
      return *m_item;}    
    const_iterator& operator=(const iterator& src) {
      m_item = src.m_item;}
    const_iterator& operator=(const const_iterator& src) {
      m_item = src.m_item;}
    const_iterator(const typename vector_type::iterator& src)
      : m_item(src) {}
    const_iterator(const typename vector_type::const_iterator& src)
      : m_item(src) {}
    const_iterator(const iterator& src)
      : m_item(src.m_item) {}
    const_iterator(const const_iterator& src) 
      : m_item(src.m_item) {}
  };
  unsigned iterator_id(const const_iterator& iter) const {
    return iter.iterator_id(m_data.begin());}
  const_iterator begin() const {
    const_iterator i(m_data.begin()-1);i++;return i;}
  const_iterator end() const {
    return const_iterator(m_data.end()-1);}   
  freelist_vector() {m_data.push_back(element_type());}
};

template <class itemtype>
int freelist_vector<itemtype>::check_consistency(int talking) const {
  const char *myname="freelist_vector::check_consistency : ";
  int ok=1;
  for(freelist_type::const_iterator i=m_freelist.begin();
      i!=m_freelist.end();i++)
    if(*i<0) {
      if(talking)
	cerr << myname << "freelist contains item("<<*i
	     << ") smaller than zero\n";
      if(talking<2) return 0; else ok=0;
    }
    else if(*i>=m_data.size()) {
      if(talking)
	cerr << myname << "freelist contains item("<<*i
	     << ") out of m_nodes range (size()=="<<m_data.size()<<")\n";
      if(talking<2) return 0; else ok=0;
    }
    else if(*i==m_data.size()-1) {
      if(talking)
	cerr << myname << "freelist contains item("<<*i
	     << ") which should be sentinel\n";
      if(talking<2) return 0; else ok=0;
    } 
    else 
      if(element_type::delete_marking)
	if(m_data[*i].is_not_deleted()) {
	  if(talking)
	    cerr << myname << "freelist contains item("<<*i
		 << ") which claims to be not deleted\n";
	  if(talking<2) return 0; else ok=0;
	} 
  for(unsigned e=0;e<m_data.size()-1;e++)
    if(m_data[e].is_deleted()) {
      int ok=0;
      for(freelist_type::const_iterator i=m_freelist.begin();
	  (i!=m_freelist.end())&(!ok);i++)
	if(*i==e) ok=1;
      if(!ok) {
	if(talking)
	  cerr << myname << "element with id=" << e 
	       << " is claiming to be deleted "
	       << "but can not be found in freelist\n";;
	if(talking<2) return 0; else ok=0;
      }
    }
  if(m_data.size()-1-m_freelist.size()!=m_cnt) {
    cerr << myname << "equation problem:\n"
	 << "m_data.size()(="<<m_data.size()<<") -1 "
	 << "-m_freelist.size()(="<<m_freelist.size()<<") != "
	 << "m_cnt (="<<m_cnt<<")\n";
    if(talking<2) return 0; else ok=0;
  }
  return ok;
}

#endif
