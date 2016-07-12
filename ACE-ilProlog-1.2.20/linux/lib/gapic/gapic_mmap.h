


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

	#include <sys/mman.h>

#include "gapicgc.h"

class GapicMmap {
 private:
  void *m_data;
  size_t m_length;
  int m_file_desc;
  int open_my_file(const char *name, int flags) {
    if(m_file_desc!=-1) 
      close(m_file_desc);
    m_file_desc=open(name,flags);
    return (m_file_desc!=-1);
  }
  int do_map(size_t len, int prot, int flags, int fd, off_t offset) {

    	m_data=mmap(NULL, len, prot, flags, fd, offset);



    m_length=len;
    return m_data!=NULL;
  }    
  int do_read_map(size_t len, int fd, off_t offset) {

    return do_map(len, PROT_READ, MAP_SHARED, fd, offset);



  }
  int do_full_read_map(int fd) {
    size_t file_length=lseek(fd,0,SEEK_END);
    return do_read_map(file_length, fd, 0);
  } 
  int do_full_read_map(const char *name) {
    if(!open_my_file(name,O_RDONLY)) return 0;
    size_t file_length=lseek(m_file_desc,0,SEEK_END);
    return do_read_map(file_length, m_file_desc, 0);
  } 
 public:
  void clear() { 
    if(m_data!=NULL) {

      	munmap(m_data,m_length);

      m_data=NULL;
      m_length=0;
    }
    if(m_file_desc!=-1) {
      close(m_file_desc);
      m_file_desc=-1;
    }
  }
  void Print() const {Print(cout);}
  ostream& Print(ostream& os) const {
    os << "GapicMmap(data=" << m_data << ",len="<< m_length 
       << ",fd=" << m_file_desc << ")";
  }
  size_t size() const {return m_length;}
  const void *data() const {return m_data;}
  char operator[](int i) const {return ((char *)m_data)[i];}
	void GetLength(GAPIC_int* len) const {*len=m_length;}

  GapicMmap() : m_data(NULL), m_length(0), m_file_desc(-1) {}
  GapicMmap(GAPIC_term_ref name) throw(GapicObjectCreationError);
  ~GapicMmap() {clear();}
};

extern gapic_type GapicMmapType;
typedef GapicCollectable<GapicMmap,&GapicMmapType> 
	GapicMmapCollectable;
