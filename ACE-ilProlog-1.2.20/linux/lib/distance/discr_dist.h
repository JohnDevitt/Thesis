
#ifndef DISCR_DIST_INC
#define DISCR_DIST_INC

#include "base_dist.h"
#include "std_coll_dist.h"

class ReadfrommatrixDistance : public Distance {
 private:
  float_matrix *matrix;
  int mymatrix;
 public:
  typedef GAPIC_float domain_type;
  typedef GAPIC_int element_type;
  void Put(element_type i,element_type j,GAPIC_float d) {
    matrix->Put(i,j,d);}
  GAPIC_float Get(element_type i,element_type j) const {
    return matrix->Get(i,j);}
  GAPIC_float compute_distance(element_type i,element_type j) const {
    return Get(i,j);}
  GAPIC_float compute_sqr_distance(element_type i,element_type j) const {
    return Get(i,j)*Get(i,j);}
  int GetDimX() const {
    return matrix->GetRows();}
  int GetDimY() const {
    return matrix->GetCols();}
 
 
  ReadfrommatrixDistance(int _dimx,int _dimy) 
    : mymatrix(1),matrix(new float_matrix(_dimx,_dimy)),
    Distance("ReadfrommatrixDistance") {}
    
  ReadfrommatrixDistance(float_matrix *_matrix) 
    : mymatrix(0),matrix(_matrix), Distance("ReadfrommatrixDistance")
    {}
  ReadfrommatrixDistance(const ReadfrommatrixDistance& src) 
    : mymatrix(0),matrix(src.matrix),
    Distance("ReadfrommatrixDistance") {}
  ~ReadfrommatrixDistance(); 
};

extern gapic_type ReadfrommatrixDistanceType;
typedef GapicCollectable<ReadfrommatrixDistance,&ReadfrommatrixDistanceType>
ReadfrommatrixDistanceCollectable;

class PrologRealReadfrommatrixDistance 
: public PrologRealDistance {
 private:
  ReadfrommatrixDistance dist;
  RefCountPtr<ReadfrommatrixDistance> distptr;
public:
  void init() {distptr.SetPtr(&dist);}
  
  RefCountPtr<ReadfrommatrixDistance> GetDist() const {return distptr;}
  typedef GAPIC_int element_type;
  typedef GAPIC_float domain_type;
  virtual ostream& Print(ostream& os) const;
  virtual GAPIC_result td_compute(GAPIC_term_ref a,GAPIC_term_ref b,
				  GAPIC_float* _dist) const;
  virtual GAPIC_result id_compute(cheap_id a, cheap_id b, 
				  GAPIC_float* _dist) const;  

  PrologRealReadfrommatrixDistance(GAPIC_term_ref src) 
    throw(GapicObjectCreationError) 
    : dist(HeapGetPtrFromTermForConstruct<gapic_float_matrix_collectable>
     ("ReadfrommatrixDistance::construct - arg 1",src))
    {init();}
  PrologRealReadfrommatrixDistance(const ReadfrommatrixDistance *_dist)
    : dist(*_dist) {init();}
  virtual ~PrologRealReadfrommatrixDistance();
};


extern gapic_type PrologRealReadfrommatrixDistanceType;
typedef GapicSubCollectable<PrologRealReadfrommatrixDistance,
  &PrologRealReadfrommatrixDistanceType,
  PrologRealDistanceCollectable> PrologRealReadfrommatrixDistanceCollectable;


typedef Ptset_Vector_Distance<ReadfrommatrixDistanceCollectable> 
 Ptset_Vector_Readfrommatrix_Distance;

typedef PrologRealIntArrayDistanceTemplate<
  ReadfrommatrixDistanceCollectable, 
  Ptset_Vector_Readfrommatrix_Distance> 
 _PrologRealIntArrayPtsetReadfrommatrixDistance;

class PrologRealIntArrayPtsetReadfrommatrixDistance
: public _PrologRealIntArrayPtsetReadfrommatrixDistance {
 public:
  PrologRealIntArrayPtsetReadfrommatrixDistance(GAPIC_term_ref src) 
    : _PrologRealIntArrayPtsetReadfrommatrixDistance
    (
     HeapGetDataFromTermForConstruct
     <PrologRealReadfrommatrixDistanceCollectable>
     ("PrologRealIntArrayPtsetReadfrommatrixDistance construct - arg 1",src)
     ->GetDist()) {}
};


extern gapic_type PrologRealIntArrayPtsetReadfrommatrixDistanceType;
typedef GapicSubCollectable<
  PrologRealIntArrayPtsetReadfrommatrixDistance,
  &PrologRealIntArrayPtsetReadfrommatrixDistanceType,
  PrologRealIntArrayDistanceCollectable> 
PrologRealIntArrayPtsetReadfrommatrixDistanceCollectable;

#endif

