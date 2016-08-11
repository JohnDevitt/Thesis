
        


#ifndef C_PROLOG_H_INC
#define C_PROLOG_H_INC

#include <iostream>
#include "except.h"

using namespace std;

#define YAP_NEW_UNIFY












                
    #define G_SO
    #define SOPROVIDE
    #define CLASS_SO


   

	#define GAPIC_IS_NAN(a) isnan(a)






 

 
       

        

#if (defined(COMPILER_ILP) + defined(COMPILER_YAP) + defined(COMPILER_SICSTUS) + defined(COMPILER_MASTER) + defined(COMPILER_TERMPL)) < 1
#error a COMPILER_xxx macro should be defined
#endif
#if (defined(COMPILER_ILP) + defined(COMPILER_YAP) + defined(COMPILER_SICSTUS) + defined(COMPILER_MASTER) + defined(COMPILER_TERMPL)) > 1
#error multiple COMPILER_xxx macros were defined
#endif

 

     

#ifdef COMPILER_SICSTUS
 
    
#include <sicstus/sicstus.h>
#endif

#ifdef COMPILER_ILP
#define DONT_NEED_OPCODES
#ifdef ILPROLOG_DL_HAS_A_BUG
#include "better_ilprolog_dl.h"
#else
#undef _LARGEFILE_SOURCE
#include <dl.h>
#endif

#define allocate allocate
#define deallocate deallocate
#define fail fail
#define negate negate

#include <malloc.h>



#endif

#ifdef COMPILER_YAP

extern "C" {
	#include YAP_INCLUDE_PATH

	          
#ifdef YAP_NEW_UNIFY
	extern Term YapMkVarTerm(void);
	extern Term YapMkPairTerm(Term a, Term b);
#endif
#ifndef YAP_NEW_UNIFY
	extern Term C_MkVarTerm(void);
	extern Term C_MkPairTerm(Term a, Term b);
#endif
}

	#include <stdio.h>
	#include <malloc.h>
	#include <math.h>

	int MY_YAP_make_term_ref();
	Term* MY_YAP_get_term(int ref);
	void MY_YAP_free_terms();
#endif

#ifdef COMPILER_MASTER
#error Sorry, MASTER is not (yet) supported in GAPIOC
#endif

#ifdef COMPILER_TERMPL
	#include <termpl.h>
	#include <stdio.h>
	#include <malloc.h>
#endif

 

  

#ifndef GAPIC_MODULE
#error GAPIC_MODULE should be defined.
#endif

#ifdef COMPILER_SICSTUS
#endif

#ifdef COMPILER_ILP
G_SO extern ILP_engine *gapic_global_machine;
            
#endif

 

   

#ifdef COMPILER_SICSTUS
typedef unsigned int GAPIC_size;
typedef SP_term_ref GAPIC_term_ref;
typedef long GAPIC_int;
typedef double GAPIC_float;
typedef unsigned long GAPIC_c_atom;
typedef struct {
  GAPIC_atom name;
  int arity;
} GAPIC_c_functor;
typedef int GAPIC_result;
#endif

#ifdef COMPILER_ILP
typedef unsigned int GAPIC_size;
typedef ILP_term_ref GAPIC_term_ref;
typedef int GAPIC_int;
typedef double GAPIC_float;
typedef ILP_symbol_ref GAPIC_c_atom;
typedef ILP_symbol_ref GAPIC_c_functor;
typedef GAPIC_int GAPIC_result;
#endif

#ifdef COMPILER_TERMPL
typedef unsigned int GAPIC_size;
typedef int GAPIC_int;
typedef double GAPIC_float;
typedef TERMPL_symbol_ref GAPIC_c_atom;
typedef TERMPL_symbol_ref GAPIC_c_functor;
typedef TERMPL_term_ref GAPIC_term_ref;
typedef GAPIC_int GAPIC_result;
#endif

#ifdef COMPILER_YAP
typedef Int GAPIC_size;
typedef int GAPIC_int;
typedef flt GAPIC_float;
typedef Atom GAPIC_c_atom;
typedef Functor GAPIC_c_functor;
typedef long GAPIC_term_ref;
typedef Int GAPIC_result;
#endif

   

#ifdef COMPILER_SICSTUS
#define GAPIC_SUCCESS SP_SUCCESS
#define GAPIC_FAIL SP_FAILURE
#define GAPIC_succeed return GAPIC_SUCCESS;
#define GAPIC_fail return GAPIC_FAIL;
#define GAPIC_invalid_atom 0
#define GAPIC_invalid_functor {GAPIC_invalid_atom,-1}
#endif

#ifdef COMPILER_ILP
#define GAPIC_SUCCESS 1
#define GAPIC_FAIL 0
#define GAPIC_succeed goto ilp_gapic_end_label
#define GAPIC_fail return GAPIC_FAIL;
#define GAPIC_invalid_atom (-1)
#define GAPIC_invalid_functor GAPIC_invalid_atom;
#endif

#ifdef COMPILER_TERMPL
#define GAPIC_SUCCESS 1
#define GAPIC_FAIL 0
#define GAPIC_invalid_atom (-1)
#define GAPIC_invalid_functor GAPIC_invalid_atom;
#endif

#ifdef COMPILER_YAP
#define GAPIC_SUCCESS 1
#define GAPIC_FAIL 0
#define GAPIC_invalid_atom (-1)
#define GAPIC_invalid_functor (-2);
#define GAPIC_succeed goto yap_gapic_yes_label;
#define GAPIC_fail { MY_YAP_free_terms(); return GAPIC_FAIL; }
#endif

     

#ifdef COMPILER_SICSTUS
static inline void *GAPIC_malloc(GAPIC_size size) {return SP_malloc(size);}
static inline void GAPIC_free(void *ptr) {SP_free(ptr);}
static inline void *GAPIC_realloc(void *ptr, GAPIC_size size)
	{return SP_realloc(ptr,size);}
#endif

#if (defined(COMPILER_ILP) + defined(COMPILER_TERMPL) + defined(COMPILER_YAP)) >= 1
static inline void *GAPIC_malloc(GAPIC_size size) {return malloc(size);}
static inline void GAPIC_free(void *ptr) {free(ptr);}
static inline void *GAPIC_realloc(void *ptr, GAPIC_size size)
	{return realloc(ptr,size);}
#endif

   

#ifdef COMPILER_SICSTUS
static inline GAPIC_term_ref GAPIC_new_term_ref() {return SP_new_term_ref();}
static inline GAPIC_c_atom GAPIC_new_c_atom() {
  return 0;}
static inline GAPIC_c_functor GAPIC_new_c_functor() {
  return {0,-1};}
static inline void GAPIC_put_term(GAPIC_term_ref dst,GAPIC_term_ref src) {
	SP_put_term(dst,src);}
#endif

#ifdef COMPILER_ILP
static inline GAPIC_term_ref GAPIC_new_term_ref() {
  return ILP_new_term_ref(gapic_global_machine);}
static inline GAPIC_c_atom GAPIC_new_c_atom() {
  return ILP_new_symbol_ref(gapic_global_machine);}
static inline GAPIC_c_functor GAPIC_new_c_functor() {
  return ILP_new_symbol_ref(gapic_global_machine);}
static inline void GAPIC_put_term(GAPIC_term_ref dst,GAPIC_term_ref src) {
  ILP_from_term_ref_to_term_ref(gapic_global_machine,src,dst);}
static inline ILP_term_ref ILP_term_get_arg(ILP_engine *machine, int from) {
  ILP_term_ref ref;
  ref=ILP_new_term_ref(machine);
  ILP_from_register_to_term_ref(machine,from,ref);
  return ref;
}
#endif

#ifdef COMPILER_TERMPL
static inline GAPIC_term_ref GAPIC_new_term_ref() {
  return TERMPL_new_term_ref();
}
static inline GAPIC_c_atom GAPIC_new_c_atom() {
  return TERMPL_new_symbol_ref();
}
static inline GAPIC_c_functor GAPIC_new_c_functor() {
  return TERMPL_new_symbol_ref();
}
static inline void GAPIC_put_term(GAPIC_term_ref dst, GAPIC_term_ref src) {
  return TERMPL_put_term(dst, src);
}
#endif

#ifdef COMPILER_YAP
static inline GAPIC_term_ref GAPIC_new_term_ref() {
  return MY_YAP_make_term_ref();
}
static inline GAPIC_c_atom GAPIC_new_c_atom() {
  return 0;
}
static inline GAPIC_c_functor GAPIC_new_c_functor() {
  return 0;
}
static inline void GAPIC_put_term(GAPIC_term_ref dst, GAPIC_term_ref src) {
	*MY_YAP_get_term(dst) = *MY_YAP_get_term(src);
}
extern Term yap_var_terms[256];
#endif

   

#ifdef COMPILER_SICSTUS
static inline int GAPIC_is_var(GAPIC_term_ref term) {
  return SP_is_variable(term);
}
static inline int GAPIC_is_integer(GAPIC_term_ref term) {
  return SP_is_integer(term);
}
static inline GAPIC_result GAPIC_get_integer(GAPIC_term_ref term,
					     GAPIC_int *i) {
  return SP_get_integer(term,i);
}
static inline GAPIC_result GAPIC_put_float(GAPIC_term_ref term,
					   GAPIC_int i) {
  return SP_put_integer(term,i);
}
static inline GAPIC_result GAPIC_unify_float(GAPIC_term_ref term,
					     GAPIC_int i) {
  return SP_unify_integer(term,i);
}
static inline int GAPIC_is_float(GAPIC_term_ref term) {
  return SP_is_float(term);
}
static inline GAPIC_result GAPIC_get_float(GAPIC_term_ref term,
					   GAPIC_float *i) {
  return SP_get_float(term,i);
}
static inline GAPIC_result GAPIC_put_float(GAPIC_term_ref term,
					   GAPIC_float i) {
  return SP_put_float(term,i);
}
static inline GAPIC_result GAPIC_unify_float(GAPIC_term_ref term,
					   GAPIC_float i) {
  return SP_unify_float(term,i);
}
static inline int GAPIC_is_atom(GAPIC_term_ref term) {
  return SP_is_atom(term);
}
static inline GAPIC_result GAPIC_get_c_atom(GAPIC_term_ref term,
					    GAPIC_c_atom *i) {
  return SP_get_atom(term,i);
}
static inline GAPIC_result GAPIC_put_c_atom(GAPIC_term_ref term,
					    GAPIC_c_atom i) {
  return SP_put_atom(term,i);
}
static inline GAPIC_result GAPIC_unify_c_atom(GAPIC_term_ref term,
					      GAPIC_c_atom i) {
  return SP_unify_atom(term,i);
}

static inline int GAPIC_is_list(GAPIC_term_ref term) {
  return SP_is_list(term);
}
static inline GAPIC_result GAPIC_get_list(GAPIC_term_ref list,
					  GAPIC_term_ref head,
					  GAPIC_term_ref tail) {
  return SP_get_list(list,head,tail);
}
static inline GAPIC_result GAPIC_put_list(GAPIC_term_ref term,
					  GAPIC_term_ref head,
					  GAPIC_term_ref tail) {
  return SP_put_functor(term,head,tail);
}
static inline int GAPIC_is_functor(GAPIC_term_ref term) {
  return SP_is_compound(term);
}
static inline GAPIC_result GAPIC_get_c_functor(GAPIC_term_ref term,
					       GAPIC_c_functor *functor) {
  return SP_get_functor(term, &(functor->name), &(functor->arity));
}
static inline GAPIC_result GAPIC_put_c_functor(GAPIC_term_ref term,
					    const GAPIC_c_functor& f) {
  return SP_put_functor(term,f.name,f.arity);
}
static inline int GAPIC_arity_of_c_functor(GAPIC_c_functor functor) {
  return functor.arity;
}
static inline int GAPIC_cmp_c_functor(GAPIC_c_functor f1,GAPIC_c_functor f2) {
  return (f1.name==f2.name)&&(f1.arity==f2.arity);
}
static inline int GAPIC_cmp_c_atom(GAPIC_c_atom a1,GAPIC_c_atom a2) {
  return (a1==a2);
}
static inline void GAPIC_copy_c_atom(GAPIC_c_atom* Dst,
				       const GAPIC_c_atom Src) {
  *Dst=Src;
}
static inline void GAPIC_copy_c_functor(GAPIC_c_functor* Dst,
				       const GAPIC_c_functor Src) {
  *Dst=Src;
}
static inline int GAPIC_is_invalid_c_functor(GAPIC_c_functor f) {
  return (f.arity==(-1));
}
static inline void GAPIC_c_atom_from_string(const char *s,GAPIC_c_atom *atom) {
  *atom=SP_atom_from_string(s);      
}
static inline char *GAPIC_string_from_c_atom(GAPIC_c_atom atom) {
  return SP_string_from_atom(atom);
}
static inline char *GAPIC_string_from_c_functor(GAPIC_c_functor f) {
  return SP_string_from_atom(f.name);
}
static inline void GAPIC_c_functor_from_string(const char *s,int arity,
					       GAPIC_c_functor *f) {
  f->name=SP_atom_from_string(s);
  f->arity=arity;
}
static inline GAPIC_result GAPIC_get_arg(int i,
					 GAPIC_term_ref term,
					 GAPIC_term_ref arg) {
  return SP_get_arg(i,term,arg);
}
inline GAPIC_result GAPIC_compare_c_functor_to_term(const GAPIC_c_functor
						    *functor,
						    GAPIC_term_ref term) {
  GAPIC_c_functor f;
  GAPIC_get_c_functor(term,&f);
  int result=GAPIC_cmp_c_functor(*functor,f);
  return result;
}
inline GAPIC_result GAPIC_compare_c_atom_to_term(const GAPIC_c_atom *atom,
						 GAPIC_term_ref term) {
  GAPIC_c_atom a;
  GAPIC_get_c_atom(term,&a);
  int result=GAPIC_cmp_c_atom(*atom,a);
  return result;
}
inline int GAPIC_compare(const GAPIC_term_ref a,
				  const GAPIC_term_ref b) {
  return SP_compare(a,b);
}
#endif

#ifdef COMPILER_ILP
static inline int GAPIC_is_var(GAPIC_term_ref term) {
  return ILP_is_var_term(gapic_global_machine,term);
}
static inline void GAPIC_put_var(GAPIC_term_ref term) {
  ILP_put_variable_term(gapic_global_machine,term);
}
static inline int GAPIC_is_integer(GAPIC_term_ref term) {
  return ILP_is_integer_term(gapic_global_machine,term);
}
static inline GAPIC_result GAPIC_get_integer(GAPIC_term_ref term,
					     GAPIC_int *i) {
  if(ILP_is_integer_term(gapic_global_machine,term)) {
    *i = ILP_get_integer_term(gapic_global_machine,term);
    return GAPIC_SUCCESS;
  }
  return GAPIC_FAIL;
}
static inline GAPIC_result GAPIC_put_integer(GAPIC_term_ref term,
					     GAPIC_int i) {
  ILP_put_integer_term(gapic_global_machine,term,i);
  return GAPIC_SUCCESS;
}
static inline GAPIC_result GAPIC_unify_integer(GAPIC_term_ref term,
					     GAPIC_int i) {
  ILP_unify_integer_term(gapic_global_machine,term,i);
  return GAPIC_SUCCESS;
}

static inline int GAPIC_is_float(GAPIC_term_ref term) {
  return ILP_is_double_term(gapic_global_machine,term);
}
static inline GAPIC_result GAPIC_get_float(GAPIC_term_ref term,
					   GAPIC_float *i) {
  if(ILP_is_double_term(gapic_global_machine,term)) {
    *i = ILP_get_double_term(gapic_global_machine,term);
    return GAPIC_SUCCESS;
  }
  return GAPIC_FAIL;
}
static inline GAPIC_result GAPIC_put_float(GAPIC_term_ref term,
					     GAPIC_float f) {
  ILP_put_double_term(gapic_global_machine,term,f);
  return GAPIC_SUCCESS;
}
static inline GAPIC_result GAPIC_unify_float(GAPIC_term_ref term,
					     GAPIC_float f) {
  ILP_unify_double_term(gapic_global_machine,term,f);
  return GAPIC_SUCCESS;
}
static inline int GAPIC_is_atom(GAPIC_term_ref term) {
  return ILP_is_atom_term(gapic_global_machine,term);
}
static inline GAPIC_result GAPIC_get_c_atom(GAPIC_term_ref term,
					    GAPIC_c_atom *i) {
  if(ILP_is_atom_term(gapic_global_machine,term)) {
    ILP_get_atom_term(gapic_global_machine,term,*i);
    return GAPIC_SUCCESS;
  }
  return GAPIC_FAIL;
}
static inline GAPIC_result GAPIC_put_c_atom(GAPIC_term_ref term,
					     GAPIC_c_atom a) {
  ILP_put_atom_term(gapic_global_machine,term,a);
  return GAPIC_SUCCESS;
}
static inline GAPIC_result GAPIC_unify_c_atom(GAPIC_term_ref term,
					     GAPIC_c_atom a) {
  ILP_unify_atom_term(gapic_global_machine,term,a);
  return GAPIC_SUCCESS;
}
static inline GAPIC_result GAPIC_unify(GAPIC_term_ref a,
				       GAPIC_term_ref b) {
  ILP_unify_term_term(gapic_global_machine,a,b);
  return GAPIC_SUCCESS;
}

static inline GAPIC_result GAPIC_is_unifiable(GAPIC_term_ref a,
				              GAPIC_term_ref b) {
  return ILP_unifiable_term_term(gapic_global_machine,a,b);
}

static inline int GAPIC_is_list(GAPIC_term_ref term) {
  return ILP_is_list_term(gapic_global_machine,term);
}
static inline GAPIC_result GAPIC_get_list(GAPIC_term_ref list,
					  GAPIC_term_ref head,
					  GAPIC_term_ref tail) {
  return ILP_get_arguments_list(gapic_global_machine,list,head,tail);
}
static inline GAPIC_result GAPIC_put_list(GAPIC_term_ref list,
					  GAPIC_term_ref head,
					  GAPIC_term_ref tail) {
  ILP_put_list_term(gapic_global_machine,list,head,tail);
  return GAPIC_SUCCESS;
}

static inline unsigned int GAPIC_hash_of_term(GAPIC_term_ref term) {
  return (unsigned int)ILP_hash_of_term(gapic_global_machine, term);
}

static inline unsigned int GAPIC_size_of_term(GAPIC_term_ref term) {
  return (unsigned int)ILP_size_of_term(gapic_global_machine, term);
}

static inline dlong GAPIC_symbol_id(ILP_symbol_ref symbol) {
  return ILP_symbol_id(gapic_global_machine, symbol);
}

static inline void GAPIC_add_list(GAPIC_term_ref head, GAPIC_term_ref tail) {
  ILP_add_list_term(gapic_global_machine, head, tail);
}

inline int require_example_sets() {
  cerr << "Need Example Sets!\n";
  exit(3);
  return GAPIC_SUCCESS;
}

#ifdef HAVE_EXAMPLE_SETS
#define if_example_sets(x) x
#else
#define if_example_sets(x) require_example_sets()
#endif

static inline int GAPIC_use_example(int set, int example) {
  return if_example_sets(ILP_use_example(gapic_global_machine, set, example));
}

static inline int GAPIC_delete_example(int set, int example) {
  return if_example_sets(ILP_delete_example(gapic_global_machine, set, example));
}

static inline int GAPIC_example_sets_gc() {
  return if_example_sets(ILP_example_sets_gc(gapic_global_machine));
}

static inline dlong GAPIC_example_sets_size() {
  return if_example_sets(ILP_example_sets_size(gapic_global_machine));
}

static inline int GAPIC_heap_gc() {
  ILP_heap_gc(gapic_global_machine);
}

static inline int GAPIC_is_functor(GAPIC_term_ref term) {
  return ILP_is_functor_term(gapic_global_machine,term);
}
static inline GAPIC_result GAPIC_put_c_functor(GAPIC_term_ref term,
					       GAPIC_c_functor a) {
  ILP_put_functor_term(gapic_global_machine,term,a);
  return GAPIC_SUCCESS;
}

static inline GAPIC_result
GAPIC_get_c_functor(GAPIC_term_ref term,GAPIC_c_functor *functor) {
  ILP_get_functor_term(gapic_global_machine,term,*functor);
  return GAPIC_SUCCESS;
}
static inline int GAPIC_arity_of_c_functor(GAPIC_c_functor functor) {
  return arity_of_symbol(gapic_global_machine,functor);
}
static inline int GAPIC_cmp_c_atom(GAPIC_c_atom a1,GAPIC_c_atom a2) {
  return ILP_compare_symbol(gapic_global_machine,a1,a2);
}
static inline int GAPIC_order_c_atom(GAPIC_c_atom a1,GAPIC_c_atom a2) {
  return ILP_order_symbol(gapic_global_machine,a1,a2);
}
static inline int GAPIC_cmp_c_functor(GAPIC_c_functor f1,GAPIC_c_functor f2) {
  return ILP_compare_symbol(gapic_global_machine,f1,f2);
}
static inline void GAPIC_copy_c_atom(GAPIC_c_atom* Dst,
				       const GAPIC_c_atom Src) {
  ILP_copy_symbol(gapic_global_machine,Src,*Dst);
}
static inline void GAPIC_copy_c_functor(GAPIC_c_functor* Dst,
				       const GAPIC_c_functor Src) {
  ILP_copy_symbol(gapic_global_machine,Src,*Dst);
}
static inline int GAPIC_is_invalid_c_functor(GAPIC_c_functor f) {
  return f==GAPIC_invalid_functor;
}
static inline void GAPIC_c_atom_from_string(const char *name,
					  GAPIC_c_atom* atom) {
  lookup_symbol(gapic_global_machine,name,0,*atom);
}
static inline char *GAPIC_string_from_c_atom(GAPIC_c_atom atom) {
  return name_of_symbol(gapic_global_machine,atom);
}
static inline char *GAPIC_string_from_c_functor(GAPIC_c_functor f) {
  return name_of_symbol(gapic_global_machine,f);
}

static inline void GAPIC_c_functor_from_string(const char *name,
					       int arity,
					       GAPIC_c_functor *f) {
  lookup_symbol(gapic_global_machine,name,arity,*f);
}
static inline void GAPIC_get_arg(int i,
				 GAPIC_term_ref term,
				 GAPIC_term_ref arg) {
  ILP_get_argument_functor(gapic_global_machine,term,i,arg);
}
static inline void GAPIC_unify_arg(int i,
				 GAPIC_term_ref term,
				 GAPIC_term_ref arg) {
  ILP_unify_arg_term(gapic_global_machine,term,i,arg);
}
static inline void GAPIC_set_arg(int i,
				 GAPIC_term_ref term,
				 GAPIC_term_ref arg) {
  ILP_set_arg_term(gapic_global_machine,term,i,arg);
}
inline GAPIC_result GAPIC_compare_c_atom_to_term(const GAPIC_c_atom *atom,
						 GAPIC_term_ref term) {
      
         
  return ILP_is_atom_term(gapic_global_machine,term) &&
         ILP_compare_symbol_to_term(gapic_global_machine,term,*atom);
}
inline GAPIC_result GAPIC_compare_c_functor_to_term(const GAPIC_c_functor *f,
						 GAPIC_term_ref term) {
  return ILP_compare_symbol_to_term(gapic_global_machine,term,*f);
}

inline int GAPIC_compare(const GAPIC_term_ref a,
			 const GAPIC_term_ref b) {
  return ILP_compare_term_term(gapic_global_machine,a,b);
}

inline char *GAPIC_name_of_atom_term(GAPIC_term_ref a) {
  return ILP_name_of_atom_term(gapic_global_machine, a);
}

inline char *GAPIC_name_arity_of_functor_term(GAPIC_term_ref a, int* arity) {
  return ILP_name_arity_of_functor_term(gapic_global_machine, a, arity);
}

inline int GAPIC_arity_of_functor_term(GAPIC_term_ref a) {
	int arity;
	ILP_name_arity_of_functor_term(gapic_global_machine, a, &arity);
	return arity;
}

inline int GAPIC_get_file_number(GAPIC_term_ref file) {
  return ILP_get_file_number(gapic_global_machine, file);
}

inline unsigned long GAPIC_get_gc_time() {
	return (unsigned long)gapic_global_machine->time_of_heapgc;
}
#endif

#ifdef COMPILER_TERMPL
static inline int GAPIC_is_var(GAPIC_term_ref term) {
  return TERMPL_is_var_term(term);
}
static inline int GAPIC_is_integer(GAPIC_term_ref term) {
  return TERMPL_is_integer_term(term);
}
static inline GAPIC_result GAPIC_get_integer(GAPIC_term_ref term, GAPIC_int *i) {
  if(TERMPL_is_integer_term(term)) {
    *i = TERMPL_get_integer_term(term);
    return GAPIC_SUCCESS;
  }
  return GAPIC_FAIL;
}
static inline GAPIC_result GAPIC_put_integer(GAPIC_term_ref term, GAPIC_int i) {
  TERMPL_put_integer_term(term,i);
  return GAPIC_SUCCESS;
}
static inline GAPIC_result GAPIC_unify_integer(GAPIC_term_ref term, GAPIC_int i) {
  TERMPL_unify_integer_term(term,i);
  return GAPIC_SUCCESS;
}
static inline int GAPIC_is_float(GAPIC_term_ref term) {
  return TERMPL_is_double_term(term);
}
static inline GAPIC_result GAPIC_get_float(GAPIC_term_ref term, GAPIC_float *i) {
  if(TERMPL_is_double_term(term)) {
    *i = TERMPL_get_double_term(term);
    return GAPIC_SUCCESS;
  }
  return GAPIC_FAIL;
}
static inline GAPIC_result GAPIC_put_float(GAPIC_term_ref term, GAPIC_float f) {
  TERMPL_put_double_term(term,f);
  return GAPIC_SUCCESS;
}
static inline GAPIC_result GAPIC_unify_float(GAPIC_term_ref term, GAPIC_float f) {
  TERMPL_unify_double_term(term,f);
  return GAPIC_SUCCESS;
}
static inline int GAPIC_is_atom(GAPIC_term_ref term) {
  return TERMPL_is_atom_term(term);
}
static inline GAPIC_result GAPIC_get_c_atom(GAPIC_term_ref term, GAPIC_c_atom *i) {
  if(TERMPL_is_atom_term(term)) {
    TERMPL_get_atom_term(term,*i);
    return GAPIC_SUCCESS;
  }
  return GAPIC_FAIL;
}
static inline GAPIC_result GAPIC_put_c_atom(GAPIC_term_ref term, GAPIC_c_atom a) {
  TERMPL_put_atom_term(term,a);
  return GAPIC_SUCCESS;
}
static inline GAPIC_result GAPIC_unify_c_atom(GAPIC_term_ref term, GAPIC_c_atom a) {
  TERMPL_unify_atom_term(term,a);
  return GAPIC_SUCCESS;
}

static inline GAPIC_result GAPIC_unify(GAPIC_term_ref a, GAPIC_term_ref b) {
  TERMPL_unify_term_term(a,b);
  return GAPIC_SUCCESS;
}
static inline int GAPIC_is_list(GAPIC_term_ref term) {
  return TERMPL_is_list_term(term);
}
static inline GAPIC_result GAPIC_get_list(GAPIC_term_ref list, GAPIC_term_ref head, GAPIC_term_ref tail) {
  return TERMPL_get_arguments_list(list,head,tail);
}
static inline GAPIC_result GAPIC_put_list(GAPIC_term_ref list, GAPIC_term_ref head, GAPIC_term_ref tail) {
  TERMPL_put_list_term(list,head,tail);
  return GAPIC_SUCCESS;
}
static inline int GAPIC_is_functor(GAPIC_term_ref term) {
  return TERMPL_is_functor_term(term);
}
static inline GAPIC_result GAPIC_put_c_functor(GAPIC_term_ref term, GAPIC_c_functor a) {
  TERMPL_put_functor_term(term,a);
  return GAPIC_SUCCESS;
}
static inline GAPIC_result GAPIC_get_c_functor(GAPIC_term_ref term,GAPIC_c_functor *functor) {
  TERMPL_get_functor_term(term,*functor);
  return GAPIC_SUCCESS;
}
static inline int GAPIC_arity_of_c_functor(GAPIC_c_functor functor) {
  return TERMPL_arity_of_symbol(functor);
}
static inline int GAPIC_cmp_c_atom(GAPIC_c_atom a1,GAPIC_c_atom a2) {
  return TERMPL_compare_symbol(a1,a2);
}
static inline int GAPIC_order_c_atom(GAPIC_c_atom a1,GAPIC_c_atom a2) {
  return TERMPL_order_symbol(a1,a2);
}
static inline int GAPIC_cmp_c_functor(GAPIC_c_functor f1,GAPIC_c_functor f2) {
  return TERMPL_compare_symbol(f1,f2);
}
static inline void GAPIC_copy_c_atom(GAPIC_c_atom* Dst, const GAPIC_c_atom Src) {
  TERMPL_copy_symbol(Src,*Dst);
}
static inline void GAPIC_copy_c_functor(GAPIC_c_functor* Dst, const GAPIC_c_functor Src) {
  TERMPL_copy_symbol(Src,*Dst);
}
static inline int GAPIC_is_invalid_c_functor(GAPIC_c_functor f) {
  return f==GAPIC_invalid_functor;
}
static inline void GAPIC_c_atom_from_string(const char *name, GAPIC_c_atom* atom) {
  TERMPL_lookup_symbol(name,0,*atom);
}
static inline char *GAPIC_string_from_c_atom(GAPIC_c_atom atom) {
  return TERMPL_name_of_symbol(atom);
}
static inline char *GAPIC_string_from_c_functor(GAPIC_c_functor f) {
  return TERMPL_name_of_symbol(f);
}
static inline void GAPIC_c_functor_from_string(const char *name, int arity, GAPIC_c_functor *f) {
  TERMPL_lookup_symbol(name,arity,*f);
}
static inline void GAPIC_get_arg(int i, GAPIC_term_ref term, GAPIC_term_ref arg) {
  TERMPL_get_argument_functor(term,i,arg);
}
inline GAPIC_result GAPIC_compare_c_atom_to_term(const GAPIC_c_atom *atom, GAPIC_term_ref term) {
  return TERMPL_compare_symbol_to_term(term,*atom);
}
inline GAPIC_result GAPIC_compare_c_functor_to_term(const GAPIC_c_functor *f, GAPIC_term_ref term) {
  return TERMPL_compare_symbol_to_term(term,*f);
}
#endif

#ifdef COMPILER_YAP
static inline int GAPIC_is_var(GAPIC_term_ref term) {
	return IsVarTerm(*MY_YAP_get_term(term));
}
static inline int GAPIC_is_integer(GAPIC_term_ref term) {
	Term mt = *MY_YAP_get_term(term);
	return !IsVarTerm(mt) && IsIntTerm(mt);
}
static inline GAPIC_result GAPIC_get_integer(GAPIC_term_ref term, GAPIC_int *i) {
	*i = IntOfTerm(*MY_YAP_get_term(term));
	return GAPIC_SUCCESS;
}
static inline GAPIC_result GAPIC_put_integer(GAPIC_term_ref term, GAPIC_int i) {
	*MY_YAP_get_term(term) = MkIntTerm(i);
	return GAPIC_SUCCESS;
}
static inline GAPIC_result GAPIC_unify_integer(GAPIC_term_ref term, GAPIC_int i) {
	Term tm = MkIntTerm(i);
#ifdef YAP_NEW_UNIFY
	return unify(*MY_YAP_get_term(term), tm);
#endif
#ifndef YAP_NEW_UNIFY
	return unify(MY_YAP_get_term(term), &tm);
#endif
}
static inline int GAPIC_is_float(GAPIC_term_ref term) {
	Term mt = *MY_YAP_get_term(term);
	return !IsVarTerm(mt) && IsFloatTerm(mt);
}
static inline GAPIC_result GAPIC_get_float(GAPIC_term_ref term, GAPIC_float *f) {
	*f = FloatOfTerm(*MY_YAP_get_term(term));
	return GAPIC_SUCCESS;
}
static inline GAPIC_result GAPIC_put_float(GAPIC_term_ref term, GAPIC_float f) {
	*MY_YAP_get_term(term) = MkFloatTerm(f);
	return GAPIC_SUCCESS;
}
static inline GAPIC_result GAPIC_unify_float(GAPIC_term_ref term, GAPIC_float f) {
	Term tm = MkFloatTerm(f);
#ifdef YAP_NEW_UNIFY
	return unify(*MY_YAP_get_term(term), tm);
#endif
#ifndef YAP_NEW_UNIFY
	return unify(MY_YAP_get_term(term), &tm);
#endif
}
static inline int GAPIC_is_atom(GAPIC_term_ref term) {
	Term mt = *MY_YAP_get_term(term);
	return !IsVarTerm(mt) && IsAtomTerm(mt);
}
static inline GAPIC_result GAPIC_get_c_atom(GAPIC_term_ref term, GAPIC_c_atom *i) {
	*i = AtomOfTerm(*MY_YAP_get_term(term));
	return GAPIC_SUCCESS;
}
static inline GAPIC_result GAPIC_put_c_atom(GAPIC_term_ref term, GAPIC_c_atom a) {
	*MY_YAP_get_term(term) = MkAtomTerm(a);
	return GAPIC_SUCCESS;
}
static inline GAPIC_result GAPIC_unify_c_atom(GAPIC_term_ref term, GAPIC_c_atom a) {
	Term tm = MkAtomTerm(a);
#ifdef YAP_NEW_UNIFY
	return unify(*MY_YAP_get_term(term), tm);
#endif
#ifndef YAP_NEW_UNIFY
	return unify(MY_YAP_get_term(term), &tm);
#endif
}
static inline GAPIC_result GAPIC_unify(GAPIC_term_ref a, GAPIC_term_ref b) {
#ifdef YAP_NEW_UNIFY
	return unify(*MY_YAP_get_term(a), *MY_YAP_get_term(b));
#endif
#ifndef YAP_NEW_UNIFY
	return unify(MY_YAP_get_term(a), MY_YAP_get_term(b));
#endif
}
static inline int GAPIC_is_list(GAPIC_term_ref term) {
	Term mt = *MY_YAP_get_term(term);
	return !IsVarTerm(mt) && IsPairTerm(mt);
}
static inline GAPIC_result GAPIC_get_list(GAPIC_term_ref list, GAPIC_term_ref head, GAPIC_term_ref tail) {
	Term mt = *MY_YAP_get_term(list);
	*MY_YAP_get_term(head) = Deref(HeadOfTerm(mt));
	*MY_YAP_get_term(tail) = Deref(TailOfTerm(mt));
	return GAPIC_SUCCESS;
}
static inline GAPIC_result GAPIC_put_list(GAPIC_term_ref list, GAPIC_term_ref head, GAPIC_term_ref tail) {
	*MY_YAP_get_term(list) = MkPairTerm(*MY_YAP_get_term(head), *MY_YAP_get_term(tail));
	return GAPIC_SUCCESS;
}
static inline int GAPIC_is_functor(GAPIC_term_ref term) {
	Term mt = *MY_YAP_get_term(term);
	return !IsVarTerm(mt) && IsApplTerm(mt);
}
static inline GAPIC_result GAPIC_put_c_functor(GAPIC_term_ref term, GAPIC_c_functor a) {
	int arity = ArityOfFunctor(a);
	for (int j = 0; j < arity; j++) {
		yap_var_terms[j] = MkVarTerm();
	}
	*MY_YAP_get_term(term) = MkApplTerm(a, arity, yap_var_terms);
	return GAPIC_SUCCESS;
}
static inline GAPIC_result GAPIC_get_c_functor(GAPIC_term_ref term,GAPIC_c_functor *functor) {
	*functor = FunctorOfTerm(*MY_YAP_get_term(term));
	return GAPIC_SUCCESS;
}
static inline int GAPIC_arity_of_c_functor(GAPIC_c_functor functor) {
	return ArityOfFunctor(functor);
}
static inline int GAPIC_cmp_c_atom(GAPIC_c_atom a1,GAPIC_c_atom a2) {
	return (a1 == a2);
}
static inline int GAPIC_order_c_atom(GAPIC_c_atom a1,GAPIC_c_atom a2) {
	if (a1 == a2) return 0;
	if (a1 < a2) return -1;
	return 1;
}
static inline int GAPIC_cmp_c_functor(GAPIC_c_functor f1,GAPIC_c_functor f2) {
	return NameOfFunctor(f1) == NameOfFunctor(f2) && ArityOfFunctor(f1) == ArityOfFunctor(f2);
}
static inline void GAPIC_copy_c_atom(GAPIC_c_atom* Dst, const GAPIC_c_atom Src) {
	*Dst = Src;
}
static inline void GAPIC_copy_c_functor(GAPIC_c_functor* Dst, const GAPIC_c_functor Src) {
	*Dst = Src;
}
static inline int GAPIC_is_invalid_c_functor(GAPIC_c_functor f) {
	return f==GAPIC_invalid_functor;
}
static inline void GAPIC_c_atom_from_string(const char *name, GAPIC_c_atom* atom) {
	*atom = LookupAtom((char*)name);
}
static inline char *GAPIC_string_from_c_atom(GAPIC_c_atom atom) {
	return AtomName(atom);
}
static inline char *GAPIC_string_from_c_functor(GAPIC_c_functor f) {
	return AtomName(NameOfFunctor(f));
}
static inline void GAPIC_c_functor_from_string(const char *name, int arity, GAPIC_c_functor *f) {
	*f = MkFunctor(LookupAtom((char*)name), arity);
}
static inline void GAPIC_get_arg(int i, GAPIC_term_ref term, GAPIC_term_ref arg) {
	*MY_YAP_get_term(arg) = Deref(ArgOfTerm(i, *MY_YAP_get_term(term)));
}
inline GAPIC_result GAPIC_compare_c_atom_to_term(const GAPIC_c_atom *atom, GAPIC_term_ref term) {
	Term mt = *MY_YAP_get_term(term);
	if (!IsVarTerm(mt) && IsAtomTerm(mt)) {
		return *atom == AtomOfTerm(mt);
	} else {
		return 0;
	}
}

 
 
 

inline GAPIC_result GAPIC_compare_c_functor_to_term(const GAPIC_c_functor *f1, GAPIC_term_ref term) {
	Term mt = *MY_YAP_get_term(term);
	if (!IsVarTerm(mt) && IsApplTerm(mt)) {
		GAPIC_c_functor f2 = FunctorOfTerm(mt);
		return GAPIC_cmp_c_functor(*f1, f2);
	} else {
		return 0;
	}
}
#endif


#ifdef __cplusplus
class GAPIC_atom {
 private:
  GAPIC_c_atom atom;
#ifdef COMPILER_ILP
  void init() {

    atom=ILP_new_symbol_ref(gapic_global_machine);};
  void destroy() {

    ILP_delete_symbol_ref(gapic_global_machine,atom);};
#endif
#if defined(COMPILER_SICSTUS) + defined(COMPILER_YAP) >= 1
  void init() {};
  void destroy() {};
#endif
#ifdef COMPILER_TERMPL
  void init() { atom=TERMPL_new_symbol_ref(); };
  void destroy() { TERMPL_delete_symbol_ref(atom); };
#endif
 public:
#ifdef COMPILER_ILP
  ILP_symbol_ref UnsafeGetSymbol() const {return atom;}
#endif
#ifdef COMPILER_ILP
  GAPIC_result ilp_unify_atom(ILP_engine *machine,int i) {
    return ILP_unify_atom(machine,i,atom);}
  GAPIC_atom(ILP_engine *machine,int i) {
    init();ILP_get_atom(machine,i,atom);}
#endif
  GAPIC_atom& operator=(const GAPIC_atom& Src) {

    GAPIC_copy_c_atom(&atom,Src.atom);
    return *this;}
  GAPIC_atom& operator=(const GAPIC_c_atom Src) {

    GAPIC_copy_c_atom(&atom,Src);return *this;}
  const GAPIC_c_atom& Get() const {return atom;};
  GAPIC_result PutTerm(GAPIC_term_ref term) {
    return GAPIC_get_c_atom(term,&atom);}
  GAPIC_result CmpTerm(GAPIC_term_ref term) const {
    return GAPIC_compare_c_atom_to_term(&atom,term);}
  GAPIC_result CmpGenTerm(GAPIC_term_ref term) const {
    return GAPIC_is_atom(term)?CmpTerm(term):GAPIC_FAIL;}
  void GAPIC_ILP_put_atom(int i) {
    ILP_get_atom(gapic_global_machine,i,atom);
  }
  void PutString(const char *name) {GAPIC_c_atom_from_string(name,&atom);}
  const char *GetString() const {return GAPIC_string_from_c_atom(atom);}
  ostream& Print(ostream& os) const {
    return (os << GetString());}
  GAPIC_atom(GAPIC_term_ref Src)
    throw(GapicObjectCreationError) {
    init();
    if(PutTerm(Src)!=GAPIC_SUCCESS)
      throw GapicObjectCreationError("GAPIC_atom:term is not an atom");
  }
  GAPIC_atom(const GAPIC_atom& Src) {

    init();GAPIC_copy_c_atom(&atom,Src.atom);}
  GAPIC_atom() {init();}
  GAPIC_atom(const char *name) {init();PutString(name);};
  ~GAPIC_atom() {destroy();}
};

class GAPIC_functor {
 private:
  GAPIC_c_functor functor;
 public:
#ifdef COMPILER_ILP
  void init() {

    functor=ILP_new_symbol_ref(gapic_global_machine);

  }
  void destroy() {

    ILP_delete_symbol_ref(gapic_global_machine,functor);

  }
#endif
#if defined(COMPILER_SICSTUS) + defined(COMPILER_YAP) >= 1
  void init() {};
  void destroy() {};
#endif
#ifdef COMPILER_TERMPL
  void init() {	functor=TERMPL_new_symbol_ref(); }
  void destroy() { TERMPL_delete_symbol_ref(functor); }
#endif
 public:
#ifdef COMPILER_ILP
  ILP_symbol_ref UnsafeGetSymbol() const {return functor;}
#endif
  GAPIC_functor& operator=(const GAPIC_functor& Src) {
    GAPIC_copy_c_functor(&functor,Src.functor);return *this;}
  GAPIC_functor& operator=(const GAPIC_c_functor& Src) {
    GAPIC_copy_c_functor(&functor,Src);return *this;}
  GAPIC_result PutTerm(GAPIC_term_ref term) {
    return GAPIC_get_c_functor(term,&functor);}
  GAPIC_result PutGenTerm(GAPIC_term_ref term) {
    if(GAPIC_is_atom(term))
      return GAPIC_get_c_atom(term,&functor);
    else
      return GAPIC_get_c_functor(term,&functor);}
  GAPIC_result CmpTerm(GAPIC_term_ref term) const {

    return GAPIC_compare_c_functor_to_term(&functor,term);}
  GAPIC_result CmpGenTerm(GAPIC_term_ref term) const {
    return GAPIC_is_functor(term)?CmpTerm(term):GAPIC_FAIL;}
  int GetArity() const {return GAPIC_arity_of_c_functor(functor);}
  int is_invalid() const {return GAPIC_is_invalid_c_functor(functor);}
  const char *GetString() const {return GAPIC_string_from_c_functor(functor);}
  void PutString(const char *name,int arity) {
    GAPIC_c_functor_from_string(name,arity,&functor);}
  const GAPIC_c_functor& Get()  const {return functor;};
  GAPIC_functor(const GAPIC_functor& Src) {

    init();GAPIC_copy_c_functor(&functor,Src.functor);}
  GAPIC_functor(bool doinit = true) { if (doinit) init(); }
  GAPIC_functor(const char *name,int arity) {init();PutString(name,arity);};
  GAPIC_functor(GAPIC_term_ref src) { init(); PutTerm(src); }
  ~GAPIC_functor() {destroy();}
};
static inline int operator==(const GAPIC_functor& a,const GAPIC_functor& b) {
   return GAPIC_cmp_c_functor(a.Get(),b.Get());
}
static inline int operator!=(const GAPIC_functor& a,const GAPIC_functor& b) {
  return !(a==b);
}
static inline int operator==(const GAPIC_atom& a,const GAPIC_atom& b) {
   return GAPIC_cmp_c_atom(a.Get(),b.Get());
}
static inline int operator!=(const GAPIC_atom& a,const GAPIC_atom& b) {
  return !(a==b);
}
static inline int operator<(const GAPIC_atom& a,const GAPIC_atom& b) {
  return (GAPIC_order_c_atom(a.Get(),b.Get())<0);
}
static inline int operator>(const GAPIC_atom& a,const GAPIC_atom& b) {
  return (GAPIC_order_c_atom(b.Get(),a.Get())<0);
}

static inline GAPIC_result GAPIC_put_atom(GAPIC_term_ref term,
					  const GAPIC_atom& atom) {
  return GAPIC_put_c_atom(term,atom.Get());
}
static inline GAPIC_result GAPIC_unify_atom(GAPIC_term_ref term,
					   const GAPIC_atom& atom) {
  return GAPIC_unify_c_atom(term,atom.Get());
}
static inline GAPIC_result GAPIC_get_atom(GAPIC_term_ref term,
					  GAPIC_atom *atom) {
  return atom->PutTerm(term);
}
static inline GAPIC_result GAPIC_put_functor(GAPIC_term_ref term,
					     const GAPIC_functor& functor) {
  return GAPIC_put_c_functor(term,functor.Get());
}
static inline GAPIC_result GAPIC_get_functor(GAPIC_term_ref term,
					     GAPIC_functor *functor) {
  return functor->PutTerm(term);
}
static inline int GAPIC_arity_of_functor(const GAPIC_functor& functor) {
  return functor.GetArity();
}
static inline int GAPIC_cmp_functor(const GAPIC_functor& f1,
				    const GAPIC_functor& f2) {
  return f1==f2;
}
static inline int GAPIC_cmp_atom(const GAPIC_atom& a1,
				 const GAPIC_atom& a2) {
  return a1==a2;
}
static inline int GAPIC_is_invalid_functor(GAPIC_functor f) {
  return f.is_invalid();
}
static inline void GAPIC_atom_from_string(char *s, GAPIC_atom *atom) {
  atom->PutString(s);
}
static inline const char *GAPIC_string_from_atom(const GAPIC_atom& atom) {
  return atom.GetString();
}
static inline const char *GAPIC_string_from_functor(GAPIC_functor f) {
  return f.GetString();
}


#endif

static inline void GAPIC_printf_c_atom(GAPIC_c_atom a) {
  printf("%s",GAPIC_string_from_c_atom(a));
}
#ifdef __cplusplus
static inline void GAPIC_printf_atom(const GAPIC_atom& a) {
  printf("%s",GAPIC_string_from_atom(a));
}
#endif
static inline void GAPIC_printf_int(GAPIC_int i) {
  printf("%d",i);
}
static inline void GAPIC_printf_float(GAPIC_float f) {
  printf("%f",(float)f);
}

#ifdef __cplusplus
static inline ostream& operator<<(ostream& os,const GAPIC_atom& a) {
  return(os<<GAPIC_string_from_atom(a));
}
static inline ostream& operator<<(ostream& os,const GAPIC_functor& a) {
  return(os<<a.GetString()<<"/"<<a.GetArity());
}
G_SO ostream& print_term(ostream& os,GAPIC_term_ref term);

G_SO extern GAPIC_atom *_gapic_atom_nil_0;
G_SO extern GAPIC_atom *_gapic_atom_true_0;
G_SO extern GAPIC_atom *_gapic_atom_fail_0;
G_SO extern GAPIC_functor *_gapic_functor_conj_2;
G_SO extern GAPIC_functor *_gapic_functor_disj_2;
G_SO extern GAPIC_functor *_gapic_functor_list_2;
G_SO extern GAPIC_functor *_gapic_functor_colon_2;
G_SO extern GAPIC_functor *_gapic_functor_colon_minus_2;
#define get_GAPIC_atom_nil_0() (*_gapic_atom_nil_0)
#define get_GAPIC_atom_true_0() (*_gapic_atom_true_0)
#define get_GAPIC_atom_fail_0() (*_gapic_atom_fail_0)
#define get_GAPIC_functor_conj_2() (*_gapic_functor_conj_2)
#define get_GAPIC_functor_disj_2() (*_gapic_functor_disj_2)
#define get_GAPIC_functor_list_2() (*_gapic_functor_list_2)
#define get_GAPIC_functor_colon_2() (*_gapic_functor_colon_2)
#define get_GAPIC_functor_colon_minus_2() (*_gapic_functor_colon_minus_2)

extern "C" {
void printf_functor(GAPIC_functor functor);
}

#endif   

extern "C" {
int list_length(GAPIC_term_ref list, int* length);
int GAPIC_termtype_check_fails(const char *what, const char* type);
void examine_term(GAPIC_term_ref term);
}


inline int GAPIC_check_is_functor(GAPIC_term_ref term,
			   const char* what) {
  if(GAPIC_is_functor(term)) return 1;
  return GAPIC_termtype_check_fails(what,"functor");
}
inline int GAPIC_check_is_atom(GAPIC_term_ref term,
			   const char* what) {
  if(GAPIC_is_atom(term)) return 1;
  return GAPIC_termtype_check_fails(what,"atom");
}
inline int GAPIC_check_is_float(GAPIC_term_ref term,
			   const char* what) {
  if(GAPIC_is_float(term)) return 1;
  return GAPIC_termtype_check_fails(what,"float");
}
inline int GAPIC_check_is_integer(GAPIC_term_ref term,
			   const char* what) {
  if(GAPIC_is_integer(term)) return 1;
  return GAPIC_termtype_check_fails(what,"integer");
}

inline int GAPIC_get_short_integer(GAPIC_term_ref term,short int *si) {
  GAPIC_int i;
  GAPIC_get_integer(term,&i);
  *si=(short int)i;
}
inline GAPIC_term_ref GAPIC_new_var() {
  GAPIC_term_ref v=GAPIC_new_term_ref();
  GAPIC_put_var(v);
  return v;
}

#ifdef DEBUG
#define CHECK(x) ((void)((x)||check_fail(#x,__FILE__,__LINE__)))
int check_fail(const char *msg, const char *file, int line);
#else
#define CHECK(x)
#endif

void random_file_name(char *buf,int len);

void GapicDoDelayedInitialization();

class GapicDelayedInitializer {
public:
	GapicDelayedInitializer();
	virtual ~GapicDelayedInitializer();
	virtual void doInitialize();
};

class CLASS_SO GAPIC_atom_static : public GapicDelayedInitializer {
private:
	string m_Name;
	GAPIC_atom* m_Atom;
public:
	GAPIC_atom_static(const char* name);
	virtual ~GAPIC_atom_static();
	G_SO virtual void doInitialize();
	inline GAPIC_atom* get() { return m_Atom; }
	inline GAPIC_result CmpGenTerm(GAPIC_term_ref term) const { return m_Atom->CmpGenTerm(term);}
};

class CLASS_SO GAPIC_functor_static : public GapicDelayedInitializer {
private:
	int m_Arity;
	string m_Name;
	GAPIC_functor* m_Functor;
public:
	GAPIC_functor_static(const char* name, int arity);
	virtual ~GAPIC_functor_static();
	G_SO virtual void doInitialize();
	inline GAPIC_functor* get() { return m_Functor; }
	inline const GAPIC_functor* getConst() const { return m_Functor; }
	inline GAPIC_result CmpGenTerm(GAPIC_term_ref term) const { return m_Functor->CmpGenTerm(term);}	
};

#endif   

