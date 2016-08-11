
#include <vector>
#include "gapic_alloc.h"

class gapic_table_base {
 protected:
  char *name;
 public:
  void invalid(GAPIC_int ID) const;  
};

template <class T> class gapic_table : public gapic_table_base {
 protected:
  vector<T> data;
  vector<GAPIC_int> free_list;
  int free_list_register();
 public:
  int is_valid(GAPIC_int ID) const {
    return ((ID<data.size())&&(ID>=0))?(data[ID]!=NULL):0;
  }
  GAPIC_int GetSize() const {return data.size();}
};


template <class T> int gapic_table<T>::free_list_register() {
  int ID;

  while(free_list.size()>0) {

    ID=free_list.back();

    free_list.pop_back();

    if(ID==data.size()-1)
      data.pop_back();
    else {

      return ID;
    }
  }
  return -1;
}

template <class T> class gapic_const_alloc_table : public gapic_table<T*> {
 public:
  const T* Get(GAPIC_int ID) const {

    return this->data[ID];
  }
  GAPIC_int GetNbElements() const {
    return this->data.size()-this->free_list.size();}  
  GAPIC_int GetSize() const {return this->data.size();}
  GAPIC_int Register(T *newitem);
  void Unregister(GAPIC_int ID);
  void Print(GAPIC_int ID) {
    this->data[ID]->Print();
  };
  void Copy(GAPIC_int Dst, GAPIC_int Src) {


    *(this->data[Dst])=*(this->data[Src]);
  };
  gapic_const_alloc_table(char *_name) {this->name=_name;};
};


template <class T>
GAPIC_int gapic_const_alloc_table<T>::Register(T *newitem) {
  int ID;
  ID=this->free_list_register();
  if(ID==-1) {

    this->data.push_back(newitem);

    return this->data.size()-1; 
  }
  this->data[ID]=newitem;
  return ID;
}

template <class T>
void gapic_const_alloc_table<T>::Unregister(GAPIC_int ID) {

  GAPIC_free(this->data[ID]);
  this->data[ID]=NULL;
  if(ID==this->data.size()) 
    this->data.pop_back();
  else
    this->free_list.push_back(ID);
}


template <class T> class gapic_alloc_table 
: public gapic_const_alloc_table<T> {
              
 public:
  T* GetPtr(GAPIC_int ID) const {
    return GetPointer(ID);}; 
  T* GetPointer(GAPIC_int ID) const {

    return this->data[ID];
  }
  gapic_alloc_table(char *_name) : gapic_const_alloc_table<T>(_name) {};
};



template <class T> class gapic_shared_const_alloc_table 
: public gapic_const_alloc_table<T> {
           
 public:
  void Use(GAPIC_int ID) {
    this->data[ID]->Use();}
  int Release(GAPIC_int ID) {
    return this->data[ID]->Release();}
  int Unused(GAPIC_int ID) const {      
    return this->data[ID]->Unused();}
               
       
  void Unregister(GAPIC_int ID) {
    if(Unused()) gapic_const_alloc_table<T>::Unregister(ID);
    else printf("%s:Failed to unregister item %d; "
		 "is used by at least one reference",this->name,ID);
  };
  GAPIC_int RegisterAndUse(T *newitem) {
    GAPIC_int ID=gapic_const_alloc_table<T>::Register(newitem);
    Use(ID);
    return ID;
  }
  int FullRelease(GAPIC_int ID) {
    int res=Release(ID);
    if(res) gapic_const_alloc_table<T>::Unregister(ID);
    return res;
  }
  gapic_shared_const_alloc_table(char *_name) 
    : gapic_const_alloc_table<T>(_name) {};
};



template <class T> class gapic_shared_alloc_table 
: public gapic_shared_const_alloc_table<T> {
        
 public:
  T* GetPtr(GAPIC_int ID) const {
    return GetPointer(ID);}; 
  T* GetPointer(GAPIC_int ID) const {

    return this->data[ID];
  }
  gapic_shared_alloc_table(char *_name) 
    : gapic_shared_const_alloc_table<T>(_name) {};
};


template <class T> class gapic_object_table : public gapic_table<T> {
 public:
  const T Get(GAPIC_int ID) const {

    return this->data[ID];
  }
  T* GetPtr(GAPIC_int ID) {
    return GetPointer(ID);}
  T* GetPointer(GAPIC_int ID) {

    return &(this->data[ID]);
  }  
  template <class TSrc> GAPIC_int Register(TSrc& newitem);
  void Unregister(GAPIC_int ID);
  void Print(GAPIC_int ID) {
    this->data[ID].Print();
  };
  void Copy(GAPIC_int Dst, GAPIC_int Src) {


    this->data[Dst]=this->data[Src];
  };
  gapic_object_table(char *_name) {this->name=_name;}
};

template <class T> 
template <class TSrc> 
GAPIC_int gapic_object_table<T>::Register(TSrc& newitem) {
  int ID;
  ID=this->free_list_register();
  if(ID==-1) {

    this->data.push_back(newitem);

    return this->data.size()-1; 
  }
  new(&(this->data[ID])) T(newitem);
  return ID;
}

template <class T>
void gapic_object_table<T>::Unregister(GAPIC_int ID) {

  if(ID==this->data.size()) 
    this->data.pop_back();
  else {
    this->data[ID].~T(); 
    this->free_list.push_back(ID);
  }
}
