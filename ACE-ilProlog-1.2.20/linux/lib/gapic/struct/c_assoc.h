#ifndef C_ASSOC_INC
#define C_ASSOC_INC

#include <gapic/c_prolog.h>

namespace CAssoc {
  int is_assoc(GAPIC_term_ref avl);
  int empty_assoc(GAPIC_term_ref avl);
  int get_assoc(GAPIC_term_ref key, GAPIC_term_ref avl, GAPIC_term_ref val);
  int del_assoc(GAPIC_term_ref key, GAPIC_term_ref old_avl, 
		GAPIC_term_ref val, GAPIC_term_ref new_avl);
  int del_max_assoc(GAPIC_term_ref Assoc0, GAPIC_term_ref Key, 
		    GAPIC_term_ref Val, GAPIC_term_ref Assoc);
  int put_assoc(GAPIC_term_ref Key, GAPIC_term_ref Assoc0, 
		GAPIC_term_ref Val, GAPIC_term_ref Assoc1);
  void init();
};

#endif
