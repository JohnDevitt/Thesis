
#ifndef GAPICGC_INC
#define GAPICGC_INC

#include <stdio.h>

#include "c_prolog.h"
#include <vector>
#include <iostream>
#include <string.h>
    
 
    
typedef std::vector<bool, std::allocator<bool> > bit_vector;
 
#include "refcount.h"
#include "except.h"




  

 
class gapic_type;
class gapic_type_registry;

    
class Collectable;
class CRefCount_Collectable;
template <class type, const gapic_type *gt> class GapicCollectable;
template <class type, const gapic_type *gt> class GapicMutableCollectable;
template <class type, const gapic_type *gt> class BasicCollectable;

  
typedef GAPIC_int cheap_id;
class Gapic_Heap;

 
G_SO GAPIC_term_ref IntToPrologID(GAPIC_int i);
G_SO void IntToPrologID(GAPIC_int i,GAPIC_term_ref dst);
G_SO void IntToPrologID(GAPIC_int i, GAPIC_term_ref PrologID, GAPIC_term_ref tmp);
G_SO GAPIC_result PrologIDToInt(GAPIC_term_ref term,GAPIC_int *i);

template <class collectable_class>
collectable_class *HeapGet(cheap_id id);
template <class collectable_class>
G_SO void CreateCollectable(Collectable *dst,
		       typename collectable_class::basic_type *obj);
template <class collectable_class>
GAPIC_result DefaultCreateCollectable(Collectable *dst);
template <class collectable_class>
GAPIC_result InitCreateCollectable(Collectable *dst,GAPIC_term_ref src);
template <class collectable_class, class object_class>
GAPIC_result VirtualDefaultCreateCollectable(Collectable *dst);
template <class collectable_class, class object_class>
GAPIC_result VirtualInitCreateCollectable(Collectable *dst,GAPIC_term_ref src);


   

G_SO extern Gapic_Heap the_gapic_heap;
G_SO extern gapic_type_registry the_gapic_type_registry;

  

 

class CLASS_SO Collectable {
 public:
  typedef void basic_type;
  void *data;
  G_SO virtual int NeededByC() const;
  G_SO virtual int is_type(const gapic_type *x) const;
  G_SO virtual const char* type_name() const;
  G_SO virtual int type_id() const;
  G_SO virtual void do_free();
  G_SO void Clear();
  virtual int IncRefCount() {return 0;}
  virtual int DecRefCount() {return 0;}
  G_SO virtual ostream& Print(ostream& os) const;
  G_SO virtual GAPIC_result Copy(cheap_id id);
  G_SO virtual ostream& Write(ostream& os) const;
  G_SO virtual istream& Read(istream& os);
  G_SO bool hasData() const;
  Collectable(void *_data) {data=_data;}
  Collectable() {data=NULL;}
  virtual ~Collectable() {Clear();}
};

class Gapic_Heap_Data {
 private:
  typedef vector<cheap_id> cheapidvector;
  Collectable *data;
  cheapidvector freelist;
  int count;
  int capacity;
  void Resize(int newsize);
  void Expand();
  void Clear();
 public:
  cheap_id Add();
  G_SO void Del(cheap_id id);
  int memsize() const {return capacity;}
  int size() const {return count;}
  int freesize() const {return freelist.size();}
        
  Collectable& operator[](cheap_id i) const {return data[i];}
  Gapic_Heap_Data() : data(NULL),count(0),capacity(0) {};
  ~Gapic_Heap_Data() {Clear();}
};

class Gapic_Heap {
 private:
  Gapic_Heap_Data data;
  bit_vector *marked;
  inline void DoRelease(cheap_id i) {
    data.Del(i);
  }
  inline int TryRelease(cheap_id i) {
    if(data[i].NeededByC()) return 0;
    DoRelease(i);return 1;
  }
 public:
   
  void Dump();
  void Clear();

     
  G_SO int Register(Collectable **item);
  void Unregister(cheap_id id) {DoRelease(id);};
  GAPIC_term_ref PrologRegister(Collectable **item) {return 0;};
  void PrologUnregister() {};

   
  int valid_id(cheap_id i) const {
    return ((i>=0)&&(i<data.size()))?(data[i].data!=NULL):0;
  }
  int check_type(cheap_id i,const gapic_type *t) const {
    return valid_id(i)?(Get(i).is_type(t)):0;
  }
  int size() const {return data.size();};
  const Collectable& Get(cheap_id i) const {return data[i];}
  Collectable* GetPtr(cheap_id i) {return &(data[i]);}

     
  void Start_Collection(int talklevel);
  void Mark_Collected(cheap_id tobemarked);
  int End_Collection(int talklevel);      

   
  void init();
  Gapic_Heap() {init();}
};


typedef GAPIC_result (*CollectableDefaultCreator)(Collectable *dst);
typedef GAPIC_result (*CollectableInitCreator)(Collectable *dst,
						    GAPIC_term_ref src);
typedef void (*CollectablePtrCreator)(Collectable *dst,void *newobj);


GAPIC_result CollectableFailDefaultCreator(Collectable *dst);
GAPIC_result CollectableFailInitCreator(Collectable *dst,
					GAPIC_term_ref src);

class CLASS_SO gapic_type {
 private:
  const char *name;              
  GAPIC_atom *prolog_name;        
  int id;                      
  int flags;                    
  CollectableInitCreator init_creator;          
  CollectableDefaultCreator default_creator;
  CollectablePtrCreator ptr_creator;
 public:
  inline GAPIC_result DefaultCreate(Collectable *dst) const {

    return (*default_creator)(dst);}
  inline GAPIC_result InitCreate(Collectable *dst,GAPIC_term_ref src) const {

    return (*init_creator)(dst,src);}
  inline void PtrCreate(Collectable *dst,void *src) const {

    (*ptr_creator)(dst,src);}
  GAPIC_result RegisterDefault(cheap_id *id) const;
  GAPIC_result RegisterInit(cheap_id *id,GAPIC_term_ref src) const;
  void RegisterPtr(cheap_id *id,void *newobj) const;
  void RegisterPrologName(GAPIC_atom* atom) {prolog_name=atom;}
  GAPIC_atom* GetPrologName() const {return prolog_name;}
  const char *GetName() const {return name;}
  int GetID() const {return id;}
  void RegisterYourself();
  gapic_type(const char *_name, CollectableDefaultCreator _default=NULL, CollectableInitCreator _init=NULL, int flags=0, CollectablePtrCreator _creator=NULL);
  ~gapic_type();
};

#define GAPIC_TYPE_MANUALREGISTER 1
#define GAPIC_TYPE_ABSTRACT 2

class gapic_type_registry {
 private:
  typedef vector<gapic_type *> gapic_type_vector;
  gapic_type_vector types;
 public:
  int Register(gapic_type *type);
  void RegisterWithProlog(gapic_type *type);
  void RegisterAllWithProlog();
  const gapic_type* find(GAPIC_atom &a) const;
  void list_registered_types() const;
  gapic_type_registry() {}
};


class CRefCount_Collectable : public Collectable {
 private:
  RefCountObject* GetCRefCount_data() const {
    return (RefCountObject *)data;}
 public:
  virtual int IncRefCount() {Use();return 1;}
  virtual int DecRefCount() {Release();return 1;}
  void Use() {GetCRefCount_data()->Use();}
  int Release() {return GetCRefCount_data()->Release();}
  int IsExclusive() const {return GetCRefCount_data()->IsExclusive();}
  int Unused() const {return GetCRefCount_data()->Unused();}
  virtual int NeededByC() const {return !Unused();}
  CRefCount_Collectable(void *src) : Collectable(src) {};
};


template <class type, const gapic_type *gt>
class GapicCollectable : public CRefCount_Collectable {
 public:
  typedef type basic_type;
  virtual int is_type(const gapic_type *x) const {
    return x->GetID()==gt->GetID();}
  virtual const char* type_name() const {return gt->GetName();}
  virtual int type_id() const {return gt->GetID();}
  static const gapic_type *GetType() {return gt;}
  const type* Get() const {return (type *)data;}
  RefCountPtr<type> GetRefCountPtr() {return RefCountPtr<type>((type *)data);}
  virtual void do_free();
  virtual ostream& Print(ostream& os) const {
    return Get()->Print(os);}
  GapicCollectable(const GapicCollectable<type,gt>& src)
    : CRefCount_Collectable(src.data) {}
  GapicCollectable(type* src)
    : CRefCount_Collectable(src) {}
  virtual ~GapicCollectable() {do_free();};
};

template <class type, const gapic_type *gt, class parent>
class GapicSubCollectable : public parent {
 public:
  typedef type basic_type;
  virtual int is_type(const gapic_type *x) const {
    return x->GetID()==gt->GetID();}
  virtual const char* type_name() const {return gt->GetName();}
  virtual int type_id() const {return gt->GetID();}
  static const gapic_type *GetType() {return gt;}
  const type* Get() const {return (type *)(this->data);}
  RefCountPtr<type> GetRefCountPtr() {return RefCountPtr<type>((type *)(this->data));}
  virtual ostream& Print(ostream& os) const {
    return Get()->Print(os);}
  GapicSubCollectable(const GapicSubCollectable<type,gt,parent>& src)
    : parent(src) {}
  GapicSubCollectable(type* src)
    : parent(src) {}
  virtual ~GapicSubCollectable() {}
};


template <class type, const gapic_type *gt>
class GapicMutableCollectable : public  GapicCollectable<type,gt> {
 protected:
  type* GetPtr() const {return (type *)(this->data);}
 public:
  virtual GAPIC_result Copy(cheap_id id);
  GapicMutableCollectable(const GapicMutableCollectable<type,gt>& src)
    : GapicCollectable<type,gt>(src) {}
  GapicMutableCollectable(type *src)
    : GapicCollectable<type,gt>(src) {};
};

template <class type, const gapic_type *gt>
class GapicNonExMutCollectable : public  GapicCollectable<type,gt> {
 public:
  type* GetPtr() const {return (type *)(this->data);}
  virtual GAPIC_result Copy(cheap_id id);
  GapicNonExMutCollectable(const GapicNonExMutCollectable<type,gt>& src)
    : GapicCollectable<type,gt>(src) {}
  GapicNonExMutCollectable(type *src)
    : GapicCollectable<type,gt>(src) {};
};

template <class type, const gapic_type *gt, class parent>
class GapicSubNonExMutCollectable
: public  GapicSubCollectable<type,gt,parent> {
 public:
  type* GetPtr() const {return (type *)(this->data);}
  virtual GAPIC_result Copy(cheap_id id);
  GapicSubNonExMutCollectable(const GapicSubNonExMutCollectable<type,gt,
			      parent>& src)
    : GapicSubCollectable<type,gt,parent>(src) {}
  GapicSubNonExMutCollectable(type *src)
    : GapicSubCollectable<type,gt,parent>(src) {};
};

template <class type, const gapic_type *gt>
class BasicCollectable : public Collectable {
 public:
  virtual int is_type(const gapic_type *x) const {
    return x->GetID()==gt->GetID();}
  virtual const char* type_name() const {return gt->GetName();}
  virtual int type_id() const {return gt->GetID();}
  type* Get() const {return (type *)(this->data);}
  virtual void do_free() {delete ((type *)(this->data));this->data=NULL;}
  BasicCollectable(type* src)
    : Collectable(src) {};
};

G_SO GAPIC_term_ref GetTermArgForConstruct(const char* where,GAPIC_term_ref src,
				      const GAPIC_functor& f,int arg)
     throw(GapicObjectCreationError);

  

template <class type, const gapic_type *gt>
GAPIC_result GapicMutableCollectable<type,gt>::Copy(cheap_id id) {
  if(!this->Get()->MutationAllowed()) return GAPIC_FAIL;
  GapicMutableCollectable<type,gt> *src;
  src=HeapGet<GapicMutableCollectable<type,gt> >(id);
  if(src==NULL) return GAPIC_FAIL;
  *(GetPtr())=*(src->Get());
  return GAPIC_SUCCESS;
}

template <class type, const gapic_type *gt>
GAPIC_result GapicNonExMutCollectable<type,gt>::Copy(cheap_id id) {
  GapicNonExMutCollectable<type,gt> *src;
  src=HeapGet<GapicNonExMutCollectable<type,gt> >(id);
  if(src==NULL) return GAPIC_FAIL;
  *(GetPtr())=*(src->Get());
  return GAPIC_SUCCESS;
}

template <class collectable_class>
collectable_class *HeapGet(cheap_id id) {
  if(the_gapic_heap.valid_id(id)) {
    Collectable *ptr=the_gapic_heap.GetPtr(id);
     
           
    return dynamic_cast<collectable_class *>(ptr);
  }
  else {
    cerr << "Invalid cheap id: " << id << endl;
    return NULL;
  }
}

template <class collectable_class>
collectable_class *HeapGetFromTerm(GAPIC_term_ref src) {
  cheap_id id;
  if(PrologIDToInt(src,&id)!=GAPIC_SUCCESS) {
    cerr << "Illegal prolog id: " << id << endl;
    return NULL;
  }
  return HeapGet<collectable_class>(id);
}

G_SO extern const char *HeapGetErrInvID;
G_SO extern const char *HeapGetErrInvType;



template <class collectable_class>
collectable_class *HeapGetFromTermForConstruct(const char *where,
					       GAPIC_term_ref src)
     throw(GapicObjectCreationError) {
  cheap_id id;
  if(PrologIDToInt(src,&id)!=GAPIC_SUCCESS) {
 cerr << "bad term:"; print_term(cerr,src);
 cerr << endl;
    throw GapicObjectCreationError(where,HeapGetErrInvID);
  }
  collectable_class *ptr=HeapGet<collectable_class>(id);
  if(ptr==NULL) {
 cerr << "bad term-type:"; print_term(cerr,src);
 cerr << endl;
 cerr << "should be: " << collectable_class::GetType()->GetName() << endl;
    throw GapicObjectCreationError(where,HeapGetErrInvType);
  }
  return ptr;
}

template <class collectable_class>
inline collectable_class *UnsafeHeapGet(cheap_id id) {
  return (collectable_class *)(the_gapic_heap.GetPtr(id));
}

template <class collectable_class>
const typename collectable_class::basic_type *HeapGetData(cheap_id id) {
  collectable_class *ptr;
  ptr=HeapGet<collectable_class>(id);
  if(ptr==NULL)
    return NULL;
  return ptr->Get();
}

template <class collectable_class>
const typename collectable_class::basic_type *
HeapGetDataFromTerm(GAPIC_term_ref term) {
  collectable_class *ptr;
  ptr=HeapGetFromTerm<collectable_class>(term);
  if(ptr==NULL)
    return NULL;
  return ptr->Get();
}

template <class collectable_class>
inline const typename collectable_class::basic_type *
HeapGetDataFromTermForConstruct(const char* where, GAPIC_term_ref term)
     throw(GapicObjectCreationError) {
  return HeapGetFromTermForConstruct<collectable_class>(where,term)->Get();
}

template <class collectable_class>
inline const typename collectable_class::basic_type *
UnsafeHeapGetData(cheap_id id) {
  return UnsafeHeapGet<collectable_class>(id)->Get();
}

template <class collectable_class>
typename collectable_class::basic_type *HeapGetPtr(cheap_id id) {
  collectable_class *ptr;
  ptr=HeapGet<collectable_class>(id);
  if(ptr==NULL)
    return NULL;
  return ptr->GetPtr();
}

template <class collectable_class>
typename collectable_class::basic_type *
HeapGetPtrFromTerm(GAPIC_term_ref term) {
  collectable_class *ptr;
  ptr=HeapGetFromTerm<collectable_class>(term);
  if(ptr==NULL) {
    cerr << "HeapGetFromTerm returns 0" << endl;
    return NULL;
  }
  
           
  return ptr->GetPtr();
}



template <class collectable_class>
inline typename collectable_class::basic_type *
HeapGetPtrFromTermForConstruct(const char* where, GAPIC_term_ref term)
     throw(GapicObjectCreationError) {
  return HeapGetFromTermForConstruct<collectable_class>(where,term)->GetPtr();
}

template <class collectable_class>
inline typename collectable_class::basic_type *UnsafeHeapGetPtr(cheap_id id) {
  return HeapGet<collectable_class>(id)->GetPtr();
}


template <class collectable_class>
inline RefCountPtr<typename collectable_class::basic_type>
HeapGetRefCountPtr(cheap_id id) {
  collectable_class *ptr;
  ptr=HeapGet<collectable_class>(id);
  if(ptr==NULL)
    return NULL;
  return ptr->GetRefCountPtr();
}

template <class type, class collectable>
type* HeapGetPtrFromRef(GAPIC_term_ref ref) {
	  
		 
		 
		 
	
	 
	int id;
	PrologIDToInt(ref, &id);
	return HeapGetPtr<collectable>(id);
}

template <class collectable_class>
inline RefCountPtr<typename collectable_class::basic_type>
HeapGetRefCountPtrFromTerm(GAPIC_term_ref term) {
  collectable_class *ptr;
  ptr=HeapGetFromTerm<collectable_class>(term);
  if(ptr==NULL)
    return NULL;
  return ptr->GetRefCountPtr();
}

template <class collectable_class>
inline RefCountPtr<typename collectable_class::basic_type>
HeapGetRefCountPtrFromTermForConstruct(const char* where, GAPIC_term_ref term)
     throw(GapicObjectCreationError) {
  return HeapGetFromTermForConstruct<collectable_class>(where,term)
    ->GetRefCountPtr();
}

template <class collectable_class>
inline RefCountPtr<typename collectable_class::basic_type>
UnsafeHeapGetRefCountPtr(cheap_id id) {
  return HeapGet<collectable_class>(id)->GetRefCountPtr();
}


template <class type, const gapic_type *gt>
G_SO void GapicCollectable<type,gt>::do_free() {
  delete ((type *)data);data=NULL;
}

template <class collectable_class>
G_SO void CreateCollectable(Collectable *dst,
		       typename collectable_class::basic_type *obj) {
  new ((collectable_class *)(dst)) collectable_class(obj);
}

template <class collectable_class>
void PtrCreateCollectable(Collectable *dst,void *obj) {
  new ((collectable_class *)(dst))
    collectable_class((typename collectable_class::basic_type *)obj);
}

template <class collectable_class>
GAPIC_result DefaultCreateCollectable(Collectable *dst) {
  typedef typename collectable_class::basic_type type;
  type *newobj;

  try {
    newobj=new type();
  } catch(GapicObjectCreationError x) {
    cerr << x.GetMsg() << endl;
    return GAPIC_FAIL;
  } catch(...) {
    cerr << "catched exception in DefaultCreateCollectable<coll_class>/2\n";
    return GAPIC_FAIL;
  }
  CreateCollectable<collectable_class>(dst,newobj);
  return GAPIC_SUCCESS;
}

template <class collectable_class>
GAPIC_result InitCreateCollectable(Collectable *dst,GAPIC_term_ref src) {
  typedef typename collectable_class::basic_type type;
  type *newobj;

  try {
    newobj=new type(src);
  } catch(GapicObjectCreationError x) {
    cerr << x.GetMsg() << endl;
    return GAPIC_FAIL;
  } catch(...) {
    cerr << "catched exception in InitCreateCollectable<coll_class>/2\n";
    return GAPIC_FAIL;
  }
  CreateCollectable<collectable_class>(dst,newobj);
  return GAPIC_SUCCESS;
}


template <class collectable_class, class object_class>
GAPIC_result VirtualDefaultCreateCollectable(Collectable *dst) {

  try {
    object_class* newobj=new object_class();
    CreateCollectable<collectable_class>(dst,newobj);
    return GAPIC_SUCCESS;
  } catch(GapicObjectCreationError x) {
    cerr << x.GetMsg() << endl;
    return GAPIC_FAIL;
  } catch(...) {
    cerr << "catched exception in VirtualDefaultCreateCollectable<coll_class, object_class>/2\n";
    return GAPIC_FAIL;
  }
}

template <class collectable_class, class object_class>
GAPIC_result VirtualInitCreateCollectable(Collectable *dst,GAPIC_term_ref src) {

  try {
    object_class* newobj=new object_class(src);
    CreateCollectable<collectable_class>(dst,newobj);
    return GAPIC_SUCCESS;
  } catch(GapicObjectCreationError x) {
    cerr << x.GetMsg() << endl;
    return GAPIC_FAIL;
  } catch(...) {
    cerr << "catched exception in VirtualInitCreateCollectable<coll_class, object_class>/2\n";
    return GAPIC_FAIL;
  }
}



template <class collectable_class, class object_class> void GapicRegisterCreatedObject(GAPIC_term_ref termid, object_class *src) {
  Collectable *coll;
  cheap_id intid=the_gapic_heap.Register(&coll);
  CreateCollectable<collectable_class>(coll,src);
  GAPIC_put_term(termid,IntToPrologID(intid));
}



    
    
   
   
   
    
   
  
  

    
     
   
   
   
    
   
  
  


class GapicIDObject {
public:
	GAPIC_int m_Id;
public:
	GapicIDObject() { m_Id = -1; }
	   
	 
	int getID() { return m_Id; }
	void setID(int id) { m_Id = id; }
};

template <class type, class cls>
void update_gapic_id_obj(int* id, cls* ptr) {
	Collectable *col;
	ptr->setID(*id = the_gapic_heap.Register(&col));
	CreateCollectable<type>(col, ptr);
}

template <class object_class>
object_class* SimpleGetPtr(cheap_id i) {
	Collectable* coll = the_gapic_heap.GetPtr(i);
	return (object_class*)coll->data;
}

G_SO void GAPICRegisterAllTypes();

#endif
