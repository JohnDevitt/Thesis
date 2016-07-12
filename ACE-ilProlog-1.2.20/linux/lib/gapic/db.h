#ifndef GAPIC_DB_INC
#define GAPIC_DB_INC

#include <string>
#include <map>
#include <iostream>
#include "c_prolog.h"
#include "gapicgc.h"

class GapicDbTypeDef {
 private:
  int memsize;
  string name;
 protected:
  void SetMemSize(int _memsize) {memsize=_memsize;}
 public:
  const string& GetName() const {return name;}
  inline int GetMemSize() const {return memsize;}
  virtual int IsComplete() const=0;
  virtual int IsSystem() const=0;
  virtual void clear(void *data) const=0;
  virtual ostream& Print(ostream& os) const=0;
  virtual ostream& PrintObject(ostream& os,void *val) const=0;
  virtual int TermToVal(GAPIC_term_ref src,void *val)=0;
  GapicDbTypeDef(int _memsize,const string& _name) 
    : memsize(_memsize),name(_name) {}
  virtual ~GapicDbTypeDef();
};
 
typedef const GAPIC_functor* GapicDbTypeClassKey;
typedef GapicDbTypeDef *(* GapicDbTypeClassConstructor)(GAPIC_term_ref src, const string& name);
struct GapicDbTypeClassKeyCompare {
  bool operator()(GapicDbTypeClassKey s1, 
		  GapicDbTypeClassKey s2) const {
              
          
    return strcmp(s1->GetString(),s2->GetString())<0;
  }
};

typedef map<GapicDbTypeClassKey,
  GapicDbTypeClassConstructor,
  GapicDbTypeClassKeyCompare> GapicDbTypeClassIndex;

class GapicDbTypeClassTable {
 private:
  GapicDbTypeClassIndex idx;
 public:
  typedef GapicDbTypeClassIndex::const_iterator const_iterator;
  GapicDbTypeClassIndex::const_iterator begin() const {
    return idx.begin();}
  GapicDbTypeClassIndex::const_iterator end() const {
    return idx.end();}
  GapicDbTypeClassConstructor Find(GapicDbTypeClassKey key) const {
    GapicDbTypeClassIndex::const_iterator iter=idx.find(key);
    if(iter==idx.end()) return NULL;
    return iter->second;
  }
  ostream& Print(ostream& os) const {
    os << "#=" << idx.size() << endl;
    for(const_iterator iter=begin();
	iter!=end();
	iter++) {
    os << *(iter->first) << endl;
    return os;
    }
  }
  int Add(GapicDbTypeClassKey key,GapicDbTypeClassConstructor constr) {
    pair<GapicDbTypeClassKey,GapicDbTypeClassConstructor> data;
    pair<GapicDbTypeClassIndex::iterator,bool> ret_val;
    data.first=key;data.second=constr;
    ret_val=idx.insert(data);



    return ret_val.second;
  }
  GapicDbTypeClassTable();
};

extern GapicDbTypeClassTable *gapic_db_typeclass_index;


typedef GapicDbTypeDef *GapicDbTypeDefPtr;

class GapicDbType : public RefCountObject {
 private:
  GapicDbTypeDefPtr def;
  string name;
 public:
  inline const string& GetName() const {return name;}
  ostream& Print(ostream& os) const {
    def->Print(os);os << endl;return os;}
  ostream& PrintObject(ostream& os,void *val) const {
    def->PrintObject(os,val);os << endl;return os;}
  inline void clear(void *data) const {def->clear(data);}
  inline int TermToVal(GAPIC_term_ref src,void *val) const {
    return def->TermToVal(src,val);}
  inline int GetMemSize() const {return def->GetMemSize();}
  inline int IsDefined() const {return def!=NULL;}
  inline int IsComplete() const {return def->IsComplete();}
  GapicDbTypeDefPtr Get() const {return def;}
  GapicDbType(GAPIC_term_ref src)
    throw(GapicObjectCreationError);
  ~GapicDbType();
};

extern gapic_type gapic_db_type;

typedef GapicCollectable<GapicDbType,&gapic_db_type> GapicDbTypeCollectable;

typedef RefCountPtr<GapicDbType> GapicDbTypeRefCountPtr;

class GapicDbTypeDefScalar : public GapicDbTypeDef {
 public:
  virtual void clear(void *data) const;
  GapicDbTypeDefScalar(int _memsize,const string& _name) 
    : GapicDbTypeDef(_memsize,_name) {}
};

class GapicDbTypeDefSystemScalar : public GapicDbTypeDefScalar {
 public:
  virtual int IsComplete() const;
  virtual int IsSystem() const;
  GapicDbTypeDefSystemScalar(int _memsize,const string& _name) 
    : GapicDbTypeDefScalar(_memsize,_name) {}
  virtual ~GapicDbTypeDefSystemScalar();
};

class GapicDbTypeDefInt : public GapicDbTypeDefSystemScalar {
 public:
  virtual int TermToVal(GAPIC_term_ref src,void *val);
  virtual ostream& Print(ostream& os) const;
  virtual ostream& PrintObject(ostream& os,void *val) const;
  GapicDbTypeDefInt(const string& _name) 
    : GapicDbTypeDefSystemScalar(sizeof(GAPIC_int),_name) {}
};

class GapicDbTypeDefFloat : public GapicDbTypeDefSystemScalar {
 public:
  virtual int TermToVal(GAPIC_term_ref src,void *val);
  virtual ostream& Print(ostream& os) const;
  virtual ostream& PrintObject(ostream& os,void *val) const;
  GapicDbTypeDefFloat(const string& _name) 
    : GapicDbTypeDefSystemScalar(sizeof(GAPIC_float),_name) {}
};

class GapicDbTypeDefAtom : public GapicDbTypeDefSystemScalar {
 public:
  virtual int TermToVal(GAPIC_term_ref src,void *val);
  virtual ostream& Print(ostream& os) const;
  virtual ostream& PrintObject(ostream& os,void *val) const;
  GapicDbTypeDefAtom(const string& _name) 
    : GapicDbTypeDefSystemScalar(sizeof(GAPIC_atom),_name) {}
};

class GapicDbTypeDefRef : public GapicDbTypeDef {
 private:
  GapicDbTypeRefCountPtr base_type;
  int Init(GAPIC_term_ref src)
    throw(GapicObjectCreationError);  
 public:
  virtual void clear(void *data) const;
  virtual int IsComplete() const;
  virtual int IsSystem() const;
  virtual int TermToVal(GAPIC_term_ref src,void *val);
  virtual ostream& Print(ostream& os) const;
  virtual ostream& PrintObject(ostream& os,void *val) const;
  GapicDbTypeDefRef(GAPIC_term_ref src,const string& _name)
    throw(GapicObjectCreationError)
    : GapicDbTypeDef(0,_name) {SetMemSize(Init(src));}
  ~GapicDbTypeDefRef();
};

class GapicDbTypeDefStructField {
 private:
  string name;
  GapicDbTypeRefCountPtr type;
 public:
  int IsComplete() const {return type->IsComplete();}
  int GetMemSize() const {return type->GetMemSize();}
  int TermToVal(GAPIC_term_ref src,void *val) {
    type->TermToVal(src,val);}
  void clear(void *data) const {type->clear(data);}
  ostream& Print(ostream& os) const;
  ostream& PrintObject(ostream& os,void *val) const {
    os << name << "="; type->PrintObject(os,val);}
  const string& GetName() const {return name;}
  const GapicDbType* GetType() const {return type.Get();}
  GapicDbTypeDefStructField& operator=(const GapicDbTypeDefStructField& src) {
    name=src.name;type=src.type;return *this;
  }
  GapicDbTypeDefStructField(const GapicDbTypeDefStructField& src) 
    : name(src.name),type(src.type) {}
  GapicDbTypeDefStructField(string _name,
			    GapicDbTypeRefCountPtr _type)
    : name(_name),type(_type) {}
};

typedef vector<GapicDbTypeDefStructField> GapicDbTypeDefStructFieldList;

class GapicDbTypeDefStruct : public GapicDbTypeDef {
 private:
  GapicDbTypeDefStructFieldList fields;
  GAPIC_functor data_functor;
  int Init(GAPIC_term_ref src)
    throw(GapicObjectCreationError);
 public:
  virtual void clear(void *data) const;
  virtual int IsComplete() const;
  virtual int IsSystem() const;
  virtual int TermToVal(GAPIC_term_ref src,void *val);
  virtual ostream& Print(ostream& os) const;
  virtual ostream& PrintObject(ostream& os,void *val) const;
  GapicDbTypeDefStruct(GAPIC_term_ref src,const string& _name)
    throw(GapicObjectCreationError)
    : GapicDbTypeDef(0,_name) {SetMemSize(Init(src));}
  ~GapicDbTypeDefStruct();
};

class GapicDbTypeDefContainer : public GapicDbTypeDef {
 protected:
  GapicDbTypeRefCountPtr base_type;
 public:
  virtual int IsComplete() const;
  virtual int IsSystem() const;
  GapicDbTypeDefContainer(int _memsize,const string& _name)
    throw(GapicObjectCreationError)
    : GapicDbTypeDef(_memsize,_name) {}
  ~GapicDbTypeDefContainer();
};

class GapicDbTypeDefArray : public GapicDbTypeDefContainer {
 private:
  int start,end,size;
  int Init(GAPIC_term_ref src)
    throw(GapicObjectCreationError);  
 public:
  virtual void clear(void *data) const;
  virtual int TermToVal(GAPIC_term_ref src,void *val);
  virtual ostream& Print(ostream& os) const;
  virtual ostream& PrintObject(ostream& os,void *val) const;
  GapicDbTypeDefArray(GAPIC_term_ref src,const string& _name)
    throw(GapicObjectCreationError)
    : GapicDbTypeDefContainer(0,_name) {SetMemSize(Init(src));}
  ~GapicDbTypeDefArray();
};

class GapicDbTypeDefExpandableContainer : public GapicDbTypeDefContainer {
 private:
  void Init(GAPIC_term_ref src) throw(GapicObjectCreationError);
 public:
  virtual int size(void *data)=0;
  virtual void push_back(void *data,GAPIC_term_ref src)=0;
  GapicDbTypeDefExpandableContainer(int _memsize, GAPIC_term_ref src,
				    const string& _name)
    throw(GapicObjectCreationError)
    : GapicDbTypeDefContainer(_memsize,_name) {Init(src);}
  ~GapicDbTypeDefExpandableContainer();
};

typedef vector<void*> GapicDbTypeDefVectorData;

class GapicDbTypeDefVector : public GapicDbTypeDefExpandableContainer {
 private:
 public:
  virtual void clear(void *data) const;
  virtual int size(void *data);
  virtual void push_back(void *data,GAPIC_term_ref src);
  virtual int TermToVal(GAPIC_term_ref src,void *val);
  virtual ostream& Print(ostream& os) const;
  virtual ostream& PrintObject(ostream& os,void *val) const;
  GapicDbTypeDefVector(GAPIC_term_ref src, const string& _name)
    throw(GapicObjectCreationError)
    : GapicDbTypeDefExpandableContainer(sizeof(GapicDbTypeDefVectorData),
					src,_name) {}
  ~GapicDbTypeDefVector();
};


  

class GapicDbObject : public RefCountObject {
 private:
  void *data;
  GapicDbTypeRefCountPtr type;
 public:
  ostream& Print(ostream& os) const {
    return type->PrintObject(os,data);}
  GapicDbObject(GAPIC_term_ref src) 
    throw(GapicObjectCreationError);
  ~GapicDbObject();
};

typedef RefCountPtr<GapicDbObject> GapicDbObjectRefCountPtr;

extern gapic_type gapic_db_object;

typedef GapicCollectable<GapicDbObject,
  &gapic_db_object> GapicDbObjectCollectable;


#endif
