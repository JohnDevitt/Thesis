


#ifndef STD_COLL_DIST_INC
#define STD_COLL_DIST_INC

#include "scalar_dist.h"
#include "prolog_dist.h"
#include "coll_dist.h"
#include "edit_dist.h"
#include "std_scal_dist.h"
#include "../gapic/float_array.h"
#include "../gapic/int_array.h"
#include "../gapic/struct/c_struct.h"




      
   

template <class container_coll_class, class base_dist_coll_class,
  class coll_dist_class>
class PrologRealDistanceCollTemplate :
public PrologRealDistanceTemplate<container_coll_class> {
 private:
  typedef typename base_dist_coll_class::basic_type base_dist_class;
  typedef typename container_coll_class::basic_type container_class;
  RefCountPtr<base_dist_class> cbasedist;
  RefCountPtr<coll_dist_class> cdist;
  typedef const char* constcharptr;
  static const constcharptr constructor_name=
    "PrologRealDistanceTemplate::construct";
 public:
  RefCountPtr<coll_dist_class> GetCollectionDistance() const {
    return cdist;}
  virtual ostream& Print(ostream& os) const {
    os << "[real_distance_coll_template(";cdist->Print(os);os << ")]";
    return os;
  }
  virtual GAPIC_result compute(const container_class *a, 
			      const container_class *b,
			       GAPIC_float* dist) const;
  PrologRealDistanceCollTemplate() {

    cbasedist=new StdFloatDistance();

       
     
        
    
    cdist=new coll_dist_class(cbasedist.Get());

  }
  PrologRealDistanceCollTemplate(RefCountPtr<base_dist_class> base_src) {
    cbasedist=base_src;
    cdist=new coll_dist_class(cbasedist.Get());
  }
  PrologRealDistanceCollTemplate(GAPIC_term_ref src) 
    throw(GapicObjectCreationError) {
    cbasedist=HeapGetRefCountPtrFromTermForConstruct<base_dist_coll_class>
      ("PrologRealDistanceCollTemplate:construct:arg 1(1)",
       GetTermArgForConstruct(constructor_name,src,
				      get_GAPIC_functor_conj_2(),1));
    cdist=new coll_dist_class(cbasedist.Get(),
			     GetTermArgForConstruct(constructor_name,
							     src,
				      get_GAPIC_functor_conj_2(),2));
  }
};


template <class coll_dist>
class CollectionDistanceNoInit 
: public coll_dist { 
 public:
  typedef typename coll_dist::base_distance base_distance;
  CollectionDistanceNoInit(const base_distance* _base)
    : coll_dist(_base) {}
  CollectionDistanceNoInit(const base_distance* _base,
				 GAPIC_term_ref src) 
    throw(GapicObjectCreationError)
    : coll_dist(_base) {
    throw GapicObjectCreationError("No init constructor");}
};


template <class base_dist_coll_class, class coll_dist_class>
class PrologRealFloatArrayDistanceTemplate 
: public PrologRealDistanceCollTemplate<gapic_float_array_collectable,
  base_dist_coll_class,coll_dist_class> {
 public:
  PrologRealFloatArrayDistanceTemplate() 
    : PrologRealDistanceCollTemplate<gapic_float_array_collectable,
    base_dist_coll_class,coll_dist_class>() {

  }
  PrologRealFloatArrayDistanceTemplate(RefCountPtr<base_dist_class> base_src) 
    : PrologRealDistanceCollTemplate<gapic_float_array_collectable,
    base_dist_coll_class,coll_dist_class>(base_src) {}
  PrologRealFloatArrayDistanceTemplate(GAPIC_term_ref src) 
    throw(GapicObjectCreationError) 
    : PrologRealDistanceCollTemplate<gapic_float_array_collectable,
    base_dist_coll_class,coll_dist_class>(src) {}
}; 

template <class base_dist_coll_class, class coll_dist_class>
class PrologRealIntArrayDistanceTemplate 
: public PrologRealDistanceCollTemplate<gapic_int_array_collectable,
  base_dist_coll_class,coll_dist_class> {
 public:
  PrologRealIntArrayDistanceTemplate() 
    : PrologRealDistanceCollTemplate<gapic_int_array_collectable,
    base_dist_coll_class,coll_dist_class>() {

  }
  PrologRealIntArrayDistanceTemplate(RefCountPtr<base_dist_class> base_src) 
    : PrologRealDistanceCollTemplate<gapic_int_array_collectable,
    base_dist_coll_class,coll_dist_class>(base_src) {}
  PrologRealIntArrayDistanceTemplate(GAPIC_term_ref src) 
    throw(GapicObjectCreationError) 
    : PrologRealDistanceCollTemplate<gapic_int_array_collectable,
    base_dist_coll_class,coll_dist_class>(src) {}
}; 

   

typedef PrologRealDistanceTemplate<gapic_float_array_collectable>
PrologRealFloatArrayDistance;

extern gapic_type PrologRealFloatArrayDistanceType;
typedef GapicSubCollectable<
  PrologRealFloatArrayDistance,
  &PrologRealFloatArrayDistanceType,
  PrologRealDistanceCollectable> 
PrologRealFloatArrayDistanceCollectable;

typedef PrologRealDistanceTemplate<gapic_int_array_collectable>
PrologRealIntArrayDistance;

extern gapic_type PrologRealIntArrayDistanceType;
typedef GapicSubCollectable<
  PrologRealIntArrayDistance,
  &PrologRealIntArrayDistanceType,
  PrologRealDistanceCollectable> 
PrologRealIntArrayDistanceCollectable;

typedef PrologRealFloatArrayDistanceTemplate<
  StdFloatDistanceCollectable, 
  CollectionDistanceNoInit<
    Euclid_Vector_Distance<StdFloatDistanceCollectable> > > 
PrologRealFloatArrayStdEuclidDistance;

extern gapic_type PrologRealFloatArrayStdEuclidDistanceType;
typedef GapicSubCollectable<
  PrologRealFloatArrayStdEuclidDistance,
  &PrologRealFloatArrayStdEuclidDistanceType,
  PrologRealFloatArrayDistanceCollectable> 
PrologRealFloatArrayStdEuclidDistanceCollectable;

typedef PrologRealFloatArrayDistanceTemplate<
  StdFloatDistanceCollectable, 
  CollectionDistanceNoInit<
    Manhatten_Vector_Distance<StdFloatDistanceCollectable> > >
PrologRealFloatArrayStdManhattenDistance;

extern gapic_type PrologRealFloatArrayStdManhattenDistanceType;
typedef GapicSubCollectable<
  PrologRealFloatArrayStdManhattenDistance,
  &PrologRealFloatArrayStdManhattenDistanceType,
  PrologRealFloatArrayDistanceCollectable> 
PrologRealFloatArrayStdManhattenDistanceCollectable;

typedef PrologRealDistanceTemplate<RefCountCollectableReferenceCollectable>
PrologRealReferenceDistance;
extern gapic_type PrologRealReferenceDistanceType;
typedef GapicSubCollectable<
  PrologRealReferenceDistance,
  &PrologRealReferenceDistanceType,
  PrologRealDistanceCollectable> 
PrologRealReferenceDistanceCollectable;

class PrologStdFloatReferenceDistance : public PrologRealReferenceDistance {
 private:
  static StdFloatDistance std;
 public:
  GAPIC_result compute(const element_type *a,const element_type *b,
		       domain_type *dist) const;
  PrologStdFloatReferenceDistance() {}
  ~PrologStdFloatReferenceDistance() {}
};
extern gapic_type PrologStdFloatReferenceDistanceType;
typedef GapicSubCollectable<
  PrologStdFloatReferenceDistance,
  &PrologStdFloatReferenceDistanceType,
  PrologRealReferenceDistanceCollectable> 
PrologStdFloatReferenceDistanceCollectable;

     

  
             
                              
                              
            
                                     
                                 
            
                                      
                                  

template <class typed_dist_class, class first_typed_dist_coll, 
  class second_typed_dist_coll>
class PrologPairDistanceTemplate 
: public typed_dist_class {
 public:
  typedef typename typed_dist_class::domain_type domain_type;
  typedef typename first_typed_dist_coll::basic_type first_dist_type;
  typedef typename second_typed_dist_coll::basic_type second_dist_type;
  typedef RefCountPtr<first_dist_type> first_dist_ptr;
  typedef RefCountPtr<second_dist_type> second_dist_ptr;
 private:
  typedef const char* constcharptr;
  static const constcharptr constructor_name=
    "PrologPairDistanceTemplateConstruct";
  first_dist_ptr first;
  second_dist_ptr second;
 public:
  const first_dist_type* GetFirst() const {return first.Get();} 
  const second_dist_type* GetSecond() const {return second.Get();} 
  virtual ostream& Print(ostream& os) const;
  PrologPairDistanceTemplate(const first_dist_ptr& _first,
			     const second_dist_ptr& _second) 
    : first(_first),second(_second) {}
  PrologPairDistanceTemplate(GAPIC_term_ref src) 
    throw(GapicObjectCreationError)
    : first(HeapGetRefCountPtrFromTermForConstruct<first_typed_dist_coll>
	    (constructor_name,GetTermArgForConstruct
	     (constructor_name,src,get_GAPIC_functor_conj_2(),1))),
    second(HeapGetRefCountPtrFromTermForConstruct<second_typed_dist_coll>
	    (constructor_name,GetTermArgForConstruct
	     (constructor_name,src,get_GAPIC_functor_conj_2(),2))) {}
  virtual ~PrologPairDistanceTemplate() {}
};

template <class typed_dist_class, class first_typed_dist, 
 class second_typed_dist>
ostream& PrologPairDistanceTemplate<typed_dist_class,
  first_typed_dist,second_typed_dist>::
Print(ostream& os) const {
  os<<"pair_dist("; first->Print(os);os<<",";second->Print(os);os<<")";
}

        

typedef PrologRealDistanceTemplate<RefCountCollRefDoubleCollectable>
PrologRealDoubleDistance;
extern gapic_type PrologRealDoubleDistanceType;
typedef GapicSubCollectable<
  PrologRealDoubleDistance,
  &PrologRealDoubleDistanceType,
  PrologRealDistanceCollectable> 
PrologRealDoubleDistanceCollectable;

     
           
 

class PrologRealReferencePairDistance 
: public PrologPairDistanceTemplate<PrologRealDoubleDistance,
  PrologRealDistanceCollectable,
  PrologRealDistanceCollectable> {
 private:
 public:
  PrologRealReferencePairDistance(const first_dist_ptr& _first,
				  const second_dist_ptr& _second) 
    : PrologPairDistanceTemplate<PrologRealDoubleDistance,
    PrologRealDistanceCollectable,
    PrologRealDistanceCollectable>(_first,_second) {}
  PrologRealReferencePairDistance(GAPIC_term_ref src)
    throw(GapicObjectCreationError)
    : PrologPairDistanceTemplate<PrologRealDoubleDistance,
    PrologRealDistanceCollectable,
    PrologRealDistanceCollectable>(src) {}
  ~PrologRealReferencePairDistance() {}
};

extern gapic_type PrologRealReferencePairDistanceType;
typedef GapicSubCollectable<
  PrologRealReferencePairDistance,
  &PrologRealReferencePairDistanceType,
  PrologRealDoubleDistanceCollectable> 
PrologRealReferencePairDistanceCollectable;

class PrologRealPairManhattenDistance
: public PrologRealReferencePairDistance {
 public:
  GAPIC_result compute(const element_type *a,const element_type *b,
		       domain_type *dist) const;
  PrologRealPairManhattenDistance(const first_dist_ptr& _first,
				  const second_dist_ptr& _second) 
    : PrologRealReferencePairDistance(_first,_second) {}
  PrologRealPairManhattenDistance(GAPIC_term_ref src) 
    throw(GapicObjectCreationError)
    : PrologRealReferencePairDistance(src) {}
  ~PrologRealPairManhattenDistance() {}
};

extern gapic_type PrologRealPairManhattenDistanceType;
typedef GapicSubCollectable<
  PrologRealPairManhattenDistance,
  &PrologRealPairManhattenDistanceType,
  PrologRealReferencePairDistanceCollectable> 
PrologRealPairManhattenDistanceCollectable;

class PrologRealPairEuclidDistance
: public PrologRealReferencePairDistance {
 public:
  GAPIC_result compute(const element_type *a,const element_type *b,
		       domain_type *dist) const;
  PrologRealPairEuclidDistance(const first_dist_ptr& _first,
				  const second_dist_ptr& _second) 
    : PrologRealReferencePairDistance(_first,_second) {}
  PrologRealPairEuclidDistance(GAPIC_term_ref src) 
    throw(GapicObjectCreationError)
    : PrologRealReferencePairDistance(src) {}
  ~PrologRealPairEuclidDistance() {}
};

extern gapic_type PrologRealPairEuclidDistanceType;
typedef GapicSubCollectable<
  PrologRealPairEuclidDistance,
  &PrologRealPairEuclidDistanceType,
  PrologRealReferencePairDistanceCollectable> 
PrologRealPairEuclidDistanceCollectable;

extern GAPIC_functor tuple3_distance_functor_3;

template <class typed_dist_class, class first_typed_dist_coll, 
  class second_typed_dist_coll, class third_typed_dist_coll>
class PrologTuple3DistanceTemplate 
: public typed_dist_class {
 public:
  typedef typename typed_dist_class::domain_type domain_type;
  typedef typename first_typed_dist_coll::basic_type first_dist_type;
  typedef typename second_typed_dist_coll::basic_type second_dist_type;
  typedef typename third_typed_dist_coll::basic_type third_dist_type;
  typedef RefCountPtr<first_dist_type> first_dist_ptr;
  typedef RefCountPtr<second_dist_type> second_dist_ptr;
  typedef RefCountPtr<third_dist_type> third_dist_ptr;
 private:
  typedef const char* constcharptr;
  static const constcharptr constructor_name=
    "PrologTuple3DistanceTemplateConstruct";
  first_dist_ptr first;
  second_dist_ptr second;
  third_dist_ptr third;
 public:
  const first_dist_type* GetFirst() const {return first.Get();} 
  const second_dist_type* GetSecond() const {return second.Get();} 
  const third_dist_type* GetThird() const {return third.Get();} 
  virtual ostream& Print(ostream& os) const;
  PrologTuple3DistanceTemplate(const first_dist_ptr& _first,
			     const second_dist_ptr& _second,
			     const third_dist_ptr& _third) 
    : first(_first),second(_second),third(_third) {}
  PrologTuple3DistanceTemplate(GAPIC_term_ref src) 
    throw(GapicObjectCreationError)
    : first(HeapGetRefCountPtrFromTermForConstruct<first_typed_dist_coll>
	    (constructor_name,GetTermArgForConstruct
	     (constructor_name,src,tuple3_distance_functor_3,1))),
    second(HeapGetRefCountPtrFromTermForConstruct<second_typed_dist_coll>
	    (constructor_name,GetTermArgForConstruct
	     (constructor_name,src,tuple3_distance_functor_3,2))),
    third(HeapGetRefCountPtrFromTermForConstruct<third_typed_dist_coll>
	   (constructor_name,GetTermArgForConstruct
	     (constructor_name,src,tuple3_distance_functor_3,3))) {}
  virtual ~PrologTuple3DistanceTemplate() {}
};

template <class typed_dist_class, class first_typed_dist, 
 class second_typed_dist, class third_typed_dist>
ostream& PrologTuple3DistanceTemplate<typed_dist_class,
  first_typed_dist,second_typed_dist,third_typed_dist>::
Print(ostream& os) const {
  os<<"tuple3_distance("; first->Print(os);os<<",";second->Print(os);
  os<<",";third->Print(os);os<<")";
}

typedef PrologRealDistanceTemplate<RefCountCollRefTuple3Collectable>
PrologRealTuple3Distance;
extern gapic_type PrologRealTuple3DistanceType;
typedef GapicSubCollectable<
  PrologRealTuple3Distance,
  &PrologRealTuple3DistanceType,
  PrologRealDistanceCollectable> 
PrologRealTuple3DistanceCollectable;

class PrologRealReferenceTuple3Distance 
: public PrologTuple3DistanceTemplate<PrologRealTuple3Distance,
  PrologRealDistanceCollectable,
  PrologRealDistanceCollectable,
  PrologRealDistanceCollectable> {
 private:
 public:
  PrologRealReferenceTuple3Distance(const first_dist_ptr& _first,
				    const second_dist_ptr& _second,
				    const third_dist_ptr& _third) 
    : PrologTuple3DistanceTemplate<PrologRealTuple3Distance,
    PrologRealDistanceCollectable,PrologRealDistanceCollectable,
    PrologRealDistanceCollectable>(_first,_second,_third) {}
  PrologRealReferenceTuple3Distance(GAPIC_term_ref src)
    throw(GapicObjectCreationError)
    : PrologTuple3DistanceTemplate<PrologRealTuple3Distance,
    PrologRealDistanceCollectable,PrologRealDistanceCollectable,
    PrologRealDistanceCollectable>(src) {}
  ~PrologRealReferenceTuple3Distance() {}
};

extern gapic_type PrologRealReferenceTuple3DistanceType;
typedef GapicSubCollectable<
  PrologRealReferenceTuple3Distance,
  &PrologRealReferenceTuple3DistanceType,
  PrologRealTuple3DistanceCollectable> 
PrologRealReferenceTuple3DistanceCollectable;

class PrologRealTuple3ManhattenDistance
: public PrologRealReferenceTuple3Distance {
 public:
  GAPIC_result compute(const element_type *a,const element_type *b,
		       domain_type *dist) const;
  PrologRealTuple3ManhattenDistance(const first_dist_ptr& _first,
				  const second_dist_ptr& _second,
				  const third_dist_ptr& _third) 
    : PrologRealReferenceTuple3Distance(_first,_second,_third) {}
  PrologRealTuple3ManhattenDistance(GAPIC_term_ref src) 
    throw(GapicObjectCreationError)
    : PrologRealReferenceTuple3Distance(src) {}
  ~PrologRealTuple3ManhattenDistance() {}
};

extern gapic_type PrologRealTuple3ManhattenDistanceType;
typedef GapicSubCollectable<
  PrologRealTuple3ManhattenDistance,
  &PrologRealTuple3ManhattenDistanceType,
  PrologRealReferenceTuple3DistanceCollectable> 
PrologRealTuple3ManhattenDistanceCollectable;

class PrologRealTuple3EuclidDistance
: public PrologRealReferenceTuple3Distance {
 public:
  GAPIC_result compute(const element_type *a,const element_type *b,
		       domain_type *dist) const;
  PrologRealTuple3EuclidDistance(const first_dist_ptr& _first,
				 const second_dist_ptr& _second,
				 const third_dist_ptr& _third) 
    : PrologRealReferenceTuple3Distance(_first,_second,_third) {}
  PrologRealTuple3EuclidDistance(GAPIC_term_ref src) 
    throw(GapicObjectCreationError)
    : PrologRealReferenceTuple3Distance(src) {}
  ~PrologRealTuple3EuclidDistance() {}
};

extern gapic_type PrologRealTuple3EuclidDistanceType;
typedef GapicSubCollectable<
  PrologRealTuple3EuclidDistance,
  &PrologRealTuple3EuclidDistanceType,
  PrologRealReferenceTuple3DistanceCollectable> 
PrologRealTuple3EuclidDistanceCollectable;

extern GAPIC_functor tuple4_distance_functor_4;

template <class typed_dist_class, class first_typed_dist_coll, 
  class second_typed_dist_coll, class third_typed_dist_coll,
  class fourth_typed_dist_coll>
class PrologTuple4DistanceTemplate 
: public typed_dist_class {
 public:
  typedef typename typed_dist_class::domain_type domain_type;
  typedef typename first_typed_dist_coll::basic_type first_dist_type;
  typedef typename second_typed_dist_coll::basic_type second_dist_type;
  typedef typename third_typed_dist_coll::basic_type third_dist_type;
  typedef typename fourth_typed_dist_coll::basic_type fourth_dist_type;
  typedef RefCountPtr<first_dist_type> first_dist_ptr;
  typedef RefCountPtr<second_dist_type> second_dist_ptr;
  typedef RefCountPtr<third_dist_type> third_dist_ptr;
  typedef RefCountPtr<fourth_dist_type> fourth_dist_ptr;
 private:
  typedef const char* constcharptr;
  static const constcharptr constructor_name=
    "PrologTuple4DistanceTemplateConstruct";
  first_dist_ptr first;
  second_dist_ptr second;
  third_dist_ptr third;
  fourth_dist_ptr fourth;
 public:
  const first_dist_type* GetFirst() const {return first.Get();} 
  const second_dist_type* GetSecond() const {return second.Get();} 
  const third_dist_type* GetThird() const {return third.Get();} 
  const fourth_dist_type* GetFourth() const {return fourth.Get();} 
  virtual ostream& Print(ostream& os) const;
  PrologTuple4DistanceTemplate(const first_dist_ptr& _first,
			       const second_dist_ptr& _second,
			       const third_dist_ptr& _third,
			       const fourth_dist_ptr& _fourth) 
    : first(_first),second(_second),third(_third),fourth(_fourth) {}
  PrologTuple4DistanceTemplate(GAPIC_term_ref src) 
    throw(GapicObjectCreationError)
    : first(HeapGetRefCountPtrFromTermForConstruct<first_typed_dist_coll>
	    (constructor_name,GetTermArgForConstruct
	     (constructor_name,src,tuple4_distance_functor_4,1))),
    second(HeapGetRefCountPtrFromTermForConstruct<second_typed_dist_coll>
	    (constructor_name,GetTermArgForConstruct
	     (constructor_name,src,tuple4_distance_functor_4,2))),
    third(HeapGetRefCountPtrFromTermForConstruct<third_typed_dist_coll>
	   (constructor_name,GetTermArgForConstruct
	     (constructor_name,src,tuple4_distance_functor_4,3))),
    fourth(HeapGetRefCountPtrFromTermForConstruct<fourth_typed_dist_coll>
	   (constructor_name,GetTermArgForConstruct
	     (constructor_name,src,tuple4_distance_functor_4,4))) {}
  virtual ~PrologTuple4DistanceTemplate() {}
};

template <class typed_dist_class, class first_typed_dist, 
 class second_typed_dist, class third_typed_dist,class fourth_typed_dist>
ostream& PrologTuple4DistanceTemplate<typed_dist_class,
  first_typed_dist,second_typed_dist,third_typed_dist,fourth_typed_dist>::
Print(ostream& os) const {
  os<<"tuple4_distance("; first->Print(os);os<<",";second->Print(os);
  os<<",";third->Print(os);os<<",";fourth->Print(os);os<<")";
}

typedef PrologRealDistanceTemplate<RefCountCollRefTuple4Collectable>
PrologRealTuple4Distance;
extern gapic_type PrologRealTuple4DistanceType;
typedef GapicSubCollectable<
  PrologRealTuple4Distance,
  &PrologRealTuple4DistanceType,
  PrologRealDistanceCollectable> 
PrologRealTuple4DistanceCollectable;

class PrologRealReferenceTuple4Distance 
: public PrologTuple4DistanceTemplate<PrologRealTuple4Distance,
  PrologRealDistanceCollectable,
  PrologRealDistanceCollectable,
  PrologRealDistanceCollectable,
  PrologRealDistanceCollectable> {
 private:
 public:
  PrologRealReferenceTuple4Distance(const first_dist_ptr& _first,
				    const second_dist_ptr& _second,
				    const third_dist_ptr& _third,
				    const fourth_dist_ptr& _fourth) 
    : PrologTuple4DistanceTemplate<PrologRealTuple4Distance,
    PrologRealDistanceCollectable,PrologRealDistanceCollectable,
    PrologRealDistanceCollectable,PrologRealDistanceCollectable>
    (_first,_second,_third,_fourth) {}
  PrologRealReferenceTuple4Distance(GAPIC_term_ref src)
    throw(GapicObjectCreationError)
    : PrologTuple4DistanceTemplate<PrologRealTuple4Distance,
    PrologRealDistanceCollectable,PrologRealDistanceCollectable,
    PrologRealDistanceCollectable,PrologRealDistanceCollectable>(src) {}
  ~PrologRealReferenceTuple4Distance() {}
};

extern gapic_type PrologRealReferenceTuple4DistanceType;
typedef GapicSubCollectable<
  PrologRealReferenceTuple4Distance,
  &PrologRealReferenceTuple4DistanceType,
  PrologRealTuple4DistanceCollectable> 
PrologRealReferenceTuple4DistanceCollectable;

class PrologRealTuple4ManhattenDistance
: public PrologRealReferenceTuple4Distance {
 public:
  GAPIC_result compute(const element_type *a,const element_type *b,
		       domain_type *dist) const;
  PrologRealTuple4ManhattenDistance(const first_dist_ptr& _first,
				  const second_dist_ptr& _second,
				  const third_dist_ptr& _third,
				  const fourth_dist_ptr& _fourth) 
    : PrologRealReferenceTuple4Distance(_first,_second,_third,_fourth) {}
  PrologRealTuple4ManhattenDistance(GAPIC_term_ref src) 
    throw(GapicObjectCreationError)
    : PrologRealReferenceTuple4Distance(src) {}
  ~PrologRealTuple4ManhattenDistance() {}
};

extern gapic_type PrologRealTuple4ManhattenDistanceType;
typedef GapicSubCollectable<
  PrologRealTuple4ManhattenDistance,
  &PrologRealTuple4ManhattenDistanceType,
  PrologRealReferenceTuple4DistanceCollectable> 
PrologRealTuple4ManhattenDistanceCollectable;

class PrologRealTuple4EuclidDistance
: public PrologRealReferenceTuple4Distance {
 public:
  GAPIC_result compute(const element_type *a,const element_type *b,
		       domain_type *dist) const;
  PrologRealTuple4EuclidDistance(const first_dist_ptr& _first,
				 const second_dist_ptr& _second,
				 const third_dist_ptr& _third,
				  const fourth_dist_ptr& _fourth) 
    : PrologRealReferenceTuple4Distance(_first,_second,_third,_fourth) {}
  PrologRealTuple4EuclidDistance(GAPIC_term_ref src) 
    throw(GapicObjectCreationError)
    : PrologRealReferenceTuple4Distance(src) {}
  ~PrologRealTuple4EuclidDistance() {}
};

extern gapic_type PrologRealTuple4EuclidDistanceType;
typedef GapicSubCollectable<
  PrologRealTuple4EuclidDistance,
  &PrologRealTuple4EuclidDistanceType,
  PrologRealReferenceTuple4DistanceCollectable> 
PrologRealTuple4EuclidDistanceCollectable;

  

template <class container_coll_class, class base_dist_coll_class,
  class coll_dist_class>
GAPIC_result PrologRealDistanceCollTemplate<container_coll_class,
base_dist_coll_class,coll_dist_class>::compute(const container_class *a, 
					       const container_class *b,
					       GAPIC_float* dist) const {


  *dist=cdist->compute_distance(*a,*b);
           
  return GAPIC_SUCCESS;
}

#endif
