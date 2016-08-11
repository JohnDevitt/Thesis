#ifndef PTSET_BASE_INC
#define PTSET_BASE_INC

typedef enum {
  PTSET_DIST_METHOD_HAUSDORFF,
  PTSET_DIST_METHOD_LINK,
  PTSET_DIST_METHOD_SURJECTION,
  PTSET_DIST_METHOD_FAIR_SURJECTION,
  PTSET_DIST_METHOD_MATCHING
} Ptset_distance_method;

int GetPtsetMethodFromTerm(GAPIC_term_ref src,Ptset_distance_method* method);

Ptset_distance_method GetPtsetMethodFromTermForCreation(GAPIC_term_ref src)
     throw(GapicObjectCreationError);

#endif
