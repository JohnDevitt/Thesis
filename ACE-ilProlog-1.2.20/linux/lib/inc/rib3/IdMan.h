
#include <vector>


class IdMan {
 private:
  vector<int> m_recycler;
  int m_last_id;
 public:
  inline int newId() {
    if(m_recycler.empty())
      return m_last_id++;
    else {
      int id=m_recycler[m_recycler.size()-1];
      m_recycler.pop_back();
      return id;
    }
  }
  inline void deleteId(int id) {
    m_recycler.push_back(id);}
  inline int get_last_id() const {
    return m_last_id;}
  inline vector<int> get_recycler() const {
    return m_recycler;}
  inline void init(int id, vector<int> recycler) {
    m_last_id=id; m_recycler=recycler;} 
  inline int idCount() const {
    return m_last_id-m_recycler.size();}
  inline IdMan() : m_last_id(0) {}
};
