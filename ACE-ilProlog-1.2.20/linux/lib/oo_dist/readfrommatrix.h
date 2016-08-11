
#ifndef READFROMMATRIX_OODIST_INC
#define READFROMMATRIX_OODIST_INC

#include "c_dist.h"

class ReadfromMatrixDistance : public IntDistance {
 private:
  const float_matrix *matrix;
 public:
  GAPIC_float Get(int i,int j) const {
    return matrix->Get(i,j);}
  virtual void compute_distance(int i,int j) const;
  int GetDimX() const {
    return matrix->GetRows();}
  int GetDimY() const {
    return matrix->GetCols();}
  ReadfromMatrixDistance(const float_matrix *_matrix) 
    : mymatrix(0),matrix(_matrix), Distance("ReadfrommatrixDistance")
    {}
};

#endif
