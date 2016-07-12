#ifndef C_AVL_INC
#define C_AVL_INC

#include <gapic/c_prolog.h>

namespace CAvl {
  int is_avl(GAPIC_term_ref avl);       
  int is_empty_avl(GAPIC_term_ref avl);    
  int chk_empty_avl(GAPIC_term_ref avl);      
  GAPIC_term_ref make_empty_avl();
  int get_avl(GAPIC_term_ref Key, GAPIC_term_ref AVL, GAPIC_term_ref Val);
  int set_avl(GAPIC_term_ref Key, GAPIC_term_ref AVL, 
	      GAPIC_term_ref Val, GAPIC_term_ref NewVal);
  int min_avl(GAPIC_term_ref AVL, GAPIC_term_ref Key, GAPIC_term_ref Val);
  int max_avl(GAPIC_term_ref AVL, GAPIC_term_ref Key, GAPIC_term_ref Val);
  int del_min_avl(GAPIC_term_ref AVL, GAPIC_term_ref Key, 
		  GAPIC_term_ref Val);
  int del_max_avl(GAPIC_term_ref AVL, GAPIC_term_ref Key, 
		  GAPIC_term_ref Val);
  int del_avl(GAPIC_term_ref Key, GAPIC_term_ref AVL, GAPIC_term_ref Val);
  int put_avl(GAPIC_term_ref Key, GAPIC_term_ref AVL, GAPIC_term_ref Val);
  int add_avl(GAPIC_term_ref Key, GAPIC_term_ref AVL, GAPIC_term_ref Val);
  void init();
};

#endif
