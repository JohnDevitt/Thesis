
#ifndef GAPIC_COLLECTIONS_INC
#define GAPIC_COLLECTIONS_INC
template <class element, int dimension>
class FixedArray {
 private:
  element data[dimension];
 public:
  element& operator[](int i) {return data[i];}
  int size() const {return dimension;}
};
#endif

