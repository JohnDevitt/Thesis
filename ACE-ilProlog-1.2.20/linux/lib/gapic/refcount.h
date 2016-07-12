
#ifndef REF_COUNT
#define REF_COUNT


#include <vector>

using namespace std;

class RefCountObject {
  // an RefCountObject is an object with info for sharing,
  // that is not protected against misuse.

 private:
  int owner_count; // the number of owners of the object
 protected:
  int GetOwnerCount() const { return owner_count;}

 public:
  // modification
  void Use() { // called by all objects that want to be owner
    owner_count++;}
  int Release() { // release ownership
    return (--owner_count)==0;}

  // query
  int Unused() const {  // true if owned by nobody
    return (owner_count==0);}
  int IsExclusive() const { // true if only owned by one owner
    return (owner_count==1);}

  // construction
  RefCountObject() {
    owner_count=0;}
};

template <class T> class RefCountPtr {
  // a safe RefCount object is a pointer to a RefCount object
  // that implements safe behaviour for use by classes that
  // only see the public section
 private:
  T* unsafe_ptr;
  void _SetPtr(T *_ptr) { // set the pointer and use the object
    unsafe_ptr=_ptr;if(unsafe_ptr!=NULL)unsafe_ptr->Use();}
 protected:
  void InitPtr() { // initialises the pointer to nil
    unsafe_ptr=NULL;}
  const T* unsafePtr() const {
    return unsafe_ptr;}
  T* GetPtr() const { // get the pointer
    return unsafe_ptr;}
 public:
  inline int IsNull() const {
    return (unsafe_ptr==NULL);}
  inline int isNull() const {
    return (unsafe_ptr==NULL);}
  inline int notNull() const {
    return (unsafe_ptr!=NULL);}
  // modification
  void ClearPtr() { // clear pointer and set to nil
    if(unsafe_ptr==NULL)return;
    unsafe_ptr->Release();
    if(unsafe_ptr->Unused()){delete unsafe_ptr;};
    unsafe_ptr=NULL;}
  inline RefCountPtr<T>& operator =(RefCountPtr<T> src) { // set pointer
     SetPtr(src.unsafe_ptr); return *this; }
  inline RefCountPtr<T>& operator =(T *src) { // set pointer
     SetPtr(src); return *this; }
  void Create() { // create a new object
    ClearPtr();
    unsafe_ptr=new T;}
  void SetPtr(RefCountPtr<T>& src) { // set pointer
    SetPtr(src.unsafe_ptr);}
  void SetPtr(T *ptr) { // sets pointer releasing ownership of previously
                        // ozner object
        /*
         * first Use() the new pointer, then delete unsafe_ptr if necessary
         * order is important if ptr and unsafe_ptr point to the same object
         */
        if (ptr != NULL) ptr->Use();
        if (unsafe_ptr != NULL && unsafe_ptr->Release()) {
          delete unsafe_ptr;
	}
        unsafe_ptr = ptr;
    }
  void ensure_exclusive() { // ensure exclusive use of object
                           // by copying if necessary
    if(!unsafe_ptr->IsExclusive())
      DoMakeExclusive();
  }
  void DoMakeExclusive() {
    unsafe_ptr->Release();
    unsafe_ptr=new T(*unsafe_ptr);
    unsafe_ptr->Use();
  }

  // dereference
  inline const T& operator*() const { return *unsafe_ptr; }
  inline const T* operator->() const { return unsafe_ptr; }
  inline const T* Get() const {return unsafe_ptr;}
  inline const T* get() const {return unsafe_ptr;}

  inline RefCountPtr() {InitPtr();};
  inline RefCountPtr(const RefCountPtr<T>& src) {_SetPtr(src.unsafe_ptr);};
  inline RefCountPtr(T* src) {_SetPtr(src);};
  inline ~RefCountPtr() {ClearPtr();};
};

template <class T> class MutableRefCountPtr : public RefCountPtr<T> {
 public:
  inline T* GetPtr() const {return RefCountPtr<T>::GetPtr();}
  inline MutableRefCountPtr() {}
  inline MutableRefCountPtr(const RefCountPtr<T>& src)
    : RefCountPtr<T>(src) {}
  inline MutableRefCountPtr(T* src) : RefCountPtr<T>(src) {}

  inline T& operator*() const { return *RefCountPtr<T>::GetPtr(); }
  inline T* operator->() const { return RefCountPtr<T>::GetPtr(); }
  inline T* get() const { return RefCountPtr<T>::GetPtr(); }
};


template <class ElemType>
  class RefCountVector : public vector<RefCountPtr<ElemType> > {};


template <class ElemType>
  class MutableRefCountVector : public vector<MutableRefCountPtr<ElemType> > {};





//// from here old




class MutableRefCountObject : public RefCountObject {
 private:
  int ConstCount; // number of owners requiring constantness of object
 public:
  void ForbidMutation() {ConstCount++;}
  int AllowMutation() {return --ConstCount;}
  int MutationAllowed() const {return ConstCount==0;}
  MutableRefCountObject() : ConstCount(0) {}
};


template <class T> class ExclMutableRefCountPtr
: public RefCountPtr<T> {
  // a safe mutable RefCount object is a pointer to a mutable RefCount object
  // that implements safe behaviour for use by classes that
  // only see the public section
 public:
  int CanMutate() const {
    return this->unsafePtr()->IsExclusive()||this->unsafePtr()->MutationAllowed();}
  void ensure_mutable() { // ensure exclusive use of object
                           // by copying if necessary
    if(!CanMutate())
      this->DoMakeExclusive();
  }

  T* GetPtr() const {return RefCountPtr<T>::GetPtr();}

  ExclMutableRefCountPtr() {};
  ExclMutableRefCountPtr(const ExclMutableRefCountPtr<T>& src)
    : RefCountPtr<T>(src) {};
};

#endif
