
#ifndef __TOINCLUDE
#define __TOINCLUDE 1

#ifdef cygwin
#define WINDOWS 1
#elif MICROSOFT
#define WINDOWS 1
#elif sun
#define SOLARIS 1
#elif LINUX
#define LINUX 1
#elif linux 
#define LINUX 1
#endif

#ifdef sun
#define SPARC 1
#define UNIX 1 
#elif WINDOWS
#define PENTIUM 1
#elif LINUX
#define PENTIUM 1
#define UNIX 1
#define READLINE 1
#elif MICROSOFT
#define PENTIUM 1
#else
#endif

#define NOT_SPIED 0
#define SPIED 1

#define HASH_MOD 1

#define STDERR stderr

#include <math.h>
#include <fcntl.h>
#include <stdio.h>

#ifdef cygwin 
#include <time.h>
#define INLINE inline
#endif 

#ifdef UNIX
#include <sys/resource.h> 
#endif

#ifndef MICROSOFT
#include <sys/time.h>
#ifndef cygwin
#include <sys/param.h>
#endif
#include <unistd.h> 
#define INLINE inline
#endif

#ifdef MICROSOFT
#define INLINE
#define STDIN 0
 
   
  

#else
#define STDIN STDIN_FILENO
#endif
#define STDIN_TYPE int
#define STDIN_NOFILE -1

#ifdef READLINE 
#include <readline/readline.h>
#include <readline/history.h>
#endif

#include <sys/stat.h>
#include <stdlib.h>

#define USER_MOD 0


         

#define SPARE_HEAP           2000
#define SPARE_TRAIL            10
#define SPARE_LS              300
#define SPARE_CP              310
#define MALLOC_SPARE           16
#define MAXARG                256
#define C_INTERFACE_REF_SIZE   60
#define C_INTERFACE_SYMBOLS_SIZE   60

   
  


         
#define UNDEF_WARN   0
#define UNDEF_FAIL   1
#define UNDEF_TRACE  3
#define UNDEF_ERROR  4


#ifdef PENTIUM
#ifdef REGDECL
#define ASM_REG asm ("bx")
#else
#define ASM_REG
#endif
#elif SPARC
#define ASM_REG
#endif

  
#define CP_P struct choicepoint *
#define CP_FIX_LEN (sizeof(struct choicepoint)/sizeof(dlong *))

#ifndef DONT_NEED_OPCODES
#include "opcodes.h"
#endif

#define current_dbg stdout

#define dlong long

  
  


#define dfloat double
#define SIZE_FLOAT 2


typedef unsigned char byte;
typedef byte *codep;
typedef dlong boolean;

#define ARGS 250

typedef struct file_struct
{
  FILE *fp;
  dlong f_symbol;       
} file_struct;

struct permanent_hash_tableentry {
  dlong *key1;
  dlong *key2;
  dlong *value;
};

typedef struct hash_pocket {
  dlong key;
  dlong value;
  dlong next;
} hash_pocket;

typedef struct hash_table {
  dlong mask;
  dlong next_free;
  hash_pocket *entrys;
} hash_table;


  

typedef struct flock_table
{
  dlong opcode;
  dlong n;
  codep alts[2];          
} flock_table ;

#define FT_P struct flock_table *

extern dlong *flock_tables;
extern dlong *flock_ptr;

typedef struct machine {
  dlong *H;
  dlong *TR;
  dlong *E;
  dlong *B;
  dlong *BH;
  codep CONTP;
  dlong *TOS;
  dlong Areg[ARGS];
  codep P;
  dlong *PH;       
  dlong *begin_heap;
  dlong *end_heap;
  dlong *limit_heap;
  dlong *shadow_limit_heap;
  dlong *begin_trail;
  dlong *end_trail;
  dlong *limit_trail;
  dlong *begin_ls;
  dlong *end_ls;
  dlong *limit_ls;
  dlong *begin_cp;
  dlong *end_cp;
  dlong *limit_cp;
  dlong *begin_permanent_heap;
  dlong *end_permanent_heap;
  struct permanent_hash_tableentry *permanent_hash;
  dlong permanent_hash_size;
  dlong permanent_hash_limit;
  FILE *errout;
  STDIN_TYPE file_in;
  FILE *file_out;
  dlong nr_open_files;
  struct file_struct openfiles[256];
  dlong nr_of_heapgc;
  dlong time_of_heapgc;
  dlong bytes_of_heap_gc;
  dlong nr_of_heap_expansion;
  dlong time_of_heap_expansion;
  dlong nr_of_ls_expansion;
  dlong time_of_ls_expansion;
  dlong nr_of_cp_expansion;
  dlong time_of_cp_expansion;
  dlong nr_of_tr_expansion;
  dlong time_of_tr_expansion; 
  dlong on_undefined;
  CP_P  generation_cp;
  char *bitarray;
  dlong dim1, dim2, e;
  dlong **c_interface_terms;
  dlong c_interface_max_ref_used;
  dlong c_interface_ref_allocated;
  dlong *c_interface_symbols;
  dlong c_interface_max_symbols_used;
  dlong c_interface_symbols_allocated;
} machine;

struct used_atoms_type {
  dlong debug_call_1, dynamic_execute_1; 
  dlong plus_2, sub_2, band_2, bor_2, mult_2, div_2, idiv_2;
  dlong mod_2, lgshiftl_2, lgshiftr_2,max_2,min_2, power_2;
  dlong plus_1, sub_1, neg_1, log_1,abs_1,exp_1,round_1,sqrt_1;
  dlong fx, fy, xf, yf, xfx, xfy, yfx;
  dlong dot_2, dot_0, user_0, nil_0;
  dlong warn_0, fail_0, halt_0, trace_0, error_0, yes_0, no_0;
  dlong deal_with_sigint_1;
  dlong lib_0, flock_0, user_query_1, dollarVAR_1;
};


extern struct used_atoms_type used_atoms;
extern dlong amount_gc;
extern char *instr_name[];
    
extern dlong nil_atom;
extern boolean silent;
extern boolean load_compiled_file(char *, struct machine *, dlong, dlong *);
extern dlong compare_terms(dlong *,dlong *,dlong *);
extern void expand_file_name(char *, char *);
extern dlong number_of_atoms();
extern dlong number_of_funcs();
extern dlong code_size();
extern void deal_with_heap_overflow(struct machine *, dlong, dlong);
extern void deal_with_permanent_heap_overflow(struct machine *, dlong);
extern void deal_with_tr_overflow(struct machine *);
extern void deal_with_ls_overflow(struct machine *);
extern void deal_with_cp_overflow(struct machine *);
extern void set_gc_options(char *, dlong);
extern dlong current_time();

void code_gc_if_needed(struct machine* mach);
boolean brecord(dlong *key1, dlong *key2, dlong *term, struct machine* mach);
boolean brerecord(dlong *key1, dlong *key2, dlong *term,struct machine* mach);
boolean brecorded(dlong *key1, dlong *key2, dlong **term,struct machine* mach);
boolean brecord(dlong *key1, dlong *key2, dlong *term, struct machine* mach);
dlong *current_key_i(dlong *key1, dlong *key2, struct machine* mach);
boolean erase(dlong *key1, dlong *key2,struct machine* mach);
boolean normal_copy(struct machine *plocmach);
boolean numbervars(struct machine *plocmach);
boolean timelastmodified(struct machine *plocmach);
boolean dollar_current_predicate_module(struct machine *plocmach);
extern boolean  gvarlist(struct machine *plocmach); 
extern boolean  sort(struct machine *plocmach);
extern boolean  keysort(struct machine *plocmach);
int length_list(register dlong *p, struct machine *plocmach);
boolean load_compiled_piece(FILE *f, struct machine *mach);

void make_foreign_pred(dlong, dlong, dlong *, struct machine *);
void set_prompt(char *);
int lookahead_char2();
dlong list_to_string(dlong *, char *, dlong, dlong *);
dlong char_list_to_string(dlong *, char *, dlong, dlong *);
void returnfilebuffer(STDIN_TYPE);
dlong unify_terms(dlong *, dlong *, struct machine *);
void execute_machine(struct machine * mach_do_not_use, dlong first_time);

dlong get_time_of_file(char* file_name);

typedef struct atominfo
{ dlong modnr;
  char *name;
} atominfo;
extern struct atominfo atomtab[];
extern dlong numb_atoms;

typedef struct funcinfo
{ dlong arity;
  dlong modnr;
  char *name;
  char debug_info;
} funcinfo;
extern struct funcinfo functab[];
extern dlong numb_funcs;

extern dlong *modtab;
extern dlong current_version;

extern codep entries[];

extern int leaping_on;
extern int skipping_on;
extern int debugging_on;
extern int zoeffing_on;
extern int tracing_on;
extern int load_debugging_on;
extern dlong arguments[MAXARG];
extern dlong argumentc;
extern dlong filenames[MAXARG];
extern dlong filenamec;


#define main_entrypoint(i)  entries[i]
#define atom_to_func(ai)  func_lookup(atomtab[ai].modnr,0,atomtab[ai].name)  
#define func_to_atom(fi) atom_lookup(functab[fi].modnr,functab[fi].name)


#define arg_place(where,N,I) (where + I##_##N)

#ifdef PENTIUM
#define store_double_arg(where,ff)\
         *(dfloat *)(where) = (dfloat) ff
#elif SPARC
typedef union {
  struct two_ints{ dlong i1,i2; } two_ints;
  dfloat f;
} conversion_type;

#define store_double_arg(where, ff)\
         { conversion_type ct;\
           ct.f = ff; \
           *(dlong *) (where) = ct.two_ints.i1;\
           *(((dlong *) (where)) +1) = ct.two_ints.i2;\
	 }
#endif

#ifdef PENTIUM
#define get_double_arg(ff, where) ff = *(dfloat *)(where)
#elif SPARC
#define get_double_arg(ff, where) \
         { conversion_type ct;\
           ct.two_ints.i1 = *(dlong*)(where);\
           ct.two_ints.i2 = *(((dlong*)(where))+1);\
           ff = ct.f; \
 	 }
#endif

#define fill_try_retry_trust_code(where,what,arity,label,len)  \
          {  if (where >= lcodezone-> end ) return(0);\
	    *where = what;                                    \
             *(arity_type *)(where + what##_1) = (arity_type)arity; \
             *(label_type *)(where + what##_2) = (label_type)label;   \
             where += len;                                     \
          }

#define HASH_LIST 19

#ifdef THREADED
#define fill_opcode(p,opc) *(void **)p = instr_labels[opc]
#define goto_more_exec      goto  **(void **)locpc
extern void **instr_labels;
#define points_to_debug_entry(pc)  ((pc != 0) && (*(void **)pc == instr_labels[debug_entry]))
#endif


   
   

#ifdef SWITCH
#define fill_opcode(p,opc) *(switch_opcode_type*)p = opc
#ifdef PLAIN_SWITCH
#define goto_more_exec goto more_exec
#endif
#ifdef JUMP_TABLE
#define goto_more_exec goto **(void **)(instr_labels+ *(switch_opcode_type*)locpc)
#endif
#define points_to_debug_entry(pc) (*(switch_opcode_type*)pc == debug_entry)
#endif


enum operation {SUM, SUB, AND, OR, MULT, DIV, IDIV, MOD, LOGSHIFTL, LOGSHIFTR, 
                MAX, MIN, POWER, SUM_1, SUB_1, NEGATE, LOG,
                ABS, EXP, ROUND, SQRT, BIT_AND, BIT_OR
                  };  
enum tagexpr { ERROREXPR, INT, DFLOAT };

extern dlong atom_lookup(dlong, const char *);
extern dlong func_lookup(dlong, dlong, const char *);
extern dlong mod_lookup(dlong);
extern codep sym_codep(dlong, dlong, char *);
extern void exit_mess(char *, dlong);

typedef union {
  dlong ival;
  dfloat dflval;
}  valexpr;

typedef struct {
  enum tagexpr tag;
  valexpr val;
} taggedvalexpr;

extern dlong compute(dlong *p1, dlong *p2, dlong *p3, 
              enum operation op, struct machine *locmach);
extern taggedvalexpr evaluate_expression(dlong *p, struct machine *mach);
extern taggedvalexpr evaluate_expression_gen(dlong *p, struct machine *mach);


         

            
           
           
                          
	             
          
                 
       
              
	          
	                          
			              
			              



  

#define REF0        0
#define REF1        4

#define LIST        7
#define STRUCT      6

#define ATOM        2
#define INT         3
#define REAL        1

#define DELAY       5

#define tag(p)                     (((dlong)p) & 0x7)
#define minitag(p)                 (((dlong)p) & 0x3)

#define is_ref(t)                   (minitag(t) == REF0)
#define is_atomic(t)                ((tag(t) >= REAL) && (tag(t) <= INT))
#define is_list(t)                  (tag(t) == LIST)
#define is_struct(t)                (tag(t) == STRUCT)
#define is_compound(t)              (tag(t) >= STRUCT)
#define is_integer(t)               (tag(t) == INT)
#define is_real(t)                  (tag(t) == REAL)
#define is_atom(t)                  (tag(t) == ATOM)
#define is_number(t)                ((tag(t) == INT) || (tag(t) == REAL))
#define is_delayed(t)               (tag(t) == DELAY)
#define is_var(t)                   (is_ref(t) || is_delayed(t))

#define get_real_pointer(p)         ((dlong *)((unsigned int)(((dlong)p & ~0x7)) >> 1))
#define get_struct_pointer(p)       ((dlong *)((unsigned int)(((dlong)p & ~0x7)) >> 1))
#define get_list_pointer(p)         ((dlong *)((unsigned int)((~(dlong)p)) >> 1))
#define get_delay_pointer(p)        ((dlong *)((unsigned int)(((dlong)p & ~0x7)) >> 1))

#define make_list(x)                (~((dlong)(x) << 1))


#define get_atom_index(p)           ((dlong)p >> 3)

#define get_atom_name(p)            atomtab[get_atom_index(p)].name
#define get_atom_module(p)          atomtab[get_atom_index(p)].modnr

#define make_atom(at)               ((((dlong)at) << 3) | ATOM)
#define make_funct(at)              ((((dlong)at) << 11) | (functab[at].arity << 3) | STRUCT)
#define get_funct_arity(i)          (i & 0x7ff) >> 3
#define get_funct_symb(i)           (i >> 11)
#define get_funct_name(i)           functab[get_funct_symb(i)].name
#define get_funct_module(i)         functab[get_funct_symb(i)].modnr

#define make_struct_p(p)            (((dlong)(p) << 1) | STRUCT)
#define make_delay_p(p)             (((dlong)(p) << 1) | DELAY)


#define SIZE_PROLOG_INTEGER         1
#define get_integer(p)              (((dlong)p) >> 3)
#define make_tagged_int(i)          (((i) << 3) | INT)
#define build_integer(p,i)          {*p = make_tagged_int(i); }


#define SIZE_OF_FLOAT sizeof(dfloat)/sizeof(dlong)

#ifdef SPARC
#define ALIGNED_DOUBLE
#endif

#ifdef ALIGNED_DOUBLE

INLINE dfloat get_aligned_real(dlong *);
#define get_real(p)            get_aligned_real(get_real_pointer(p))

INLINE void push_aligned_real(dlong *, dfloat);
#define push_real(h,f)         push_aligned_real(h,f); h += SIZE_OF_FLOAT

#else

#define get_real(p)            *(dfloat *)get_real_pointer(p)
#define push_real(h,f)         *(dfloat *)h = (dfloat)(f); h += SIZE_OF_FLOAT

#endif


#define build_real(p,f,h)      {*p = make_real_p(h); push_real(h,f); }
#define make_real_p(p)         (((dlong)(p) << 1) | REAL)


   




                



#define deref2(p,q,undef)  while (is_ref(p)) { q = (dlong *)*p ; if (q == p) undef;  p = q; }

#define deref0lab(p,undef) {dlong *q; deref2(p,q,undef); }

#define deref(p,q) deref2(p,q,break)

#define deref0(p) {dlong *q; deref(p,q); }

       
#define goto_fail_label { failure; goto_more_exec;}

#define failure locpc = (codep)(((CP_P) breg)->failurecont)

#define trail(p,bh,trreg,trlimit)                \
           if (p < bh)                           \
             {                                   \
	       if (trreg > trlimit)              \
		 deal_with_tr_overflow(plocmach); \
	       *trreg++ = (dlong)p;              \
	     }
#define trail_indep(p,bh,trreg,trlimit, plocmach)                \
           if (p < bh)                           \
             {                                   \
	       if (trreg > trlimit)              \
		 deal_with_tr_overflow(plocmach); \
	       *trreg++ = (dlong)p;              \
	     }

#define trail_unconditional(p,trreg,trlimit)     \
	       if (trreg > trlimit)              \
		 deal_with_tr_overflow(plocmach); \
	       *(trreg)++ = (dlong)p;              

#ifdef TRAIL_LS
#define trail_env(yvar) \
           if (((CP_P)breg)->tops < (dlong *)(ereg)) \
             {                                   \
	       if (trreg > tr_limit)              \
		 deal_with_tr_overflow(plocmach); \
	       *trreg++ = (dlong)(ereg+yvar);     \
	     }
#else
#define trail_env(yvar)
#endif



#define untrail1(p) { dlong *q; q = (dlong *)(*p); *q = (dlong)q; }
#define untrail(a,b) { while (a > b) {dlong *q; q = (dlong *)*(--a); *q = (dlong)q; } }

typedef struct choicepoint
{
  codep failurecont;
  dlong *h;
  dlong *tr;
  dlong *e;
  dlong *b;
  codep cp;
  dlong *tops;
} choicepoint;

#define reset_regs                                           \
{                                                            \
  hreg = ((CP_P) breg)->h;                    \
  ereg = ((CP_P) breg)->e;                    \
  contpreg = (codep)(((CP_P) breg)->cp);      \
  tos = (((CP_P) breg)->tops);                 \
}

#define push_choicepoint(alt,contp,nrargs)                      \
{                                                               \
  dlong *oldbreg;                                               \
                                                                \
  if (breg > cp_limit)                                          \
    deal_with_cp_overflow(&locmach);                            \
  oldbreg = breg;                                               \
  breg += nrargs + CP_FIX_LEN;\
  ((CP_P) breg)->failurecont = (codep)alt;       \
  ((CP_P) breg)->h = hreg;                       \
  ((CP_P) breg)->tr = trreg;                     \
  ((CP_P) breg)->e = ereg;                       \
  ((CP_P) breg)->b = oldbreg;                    \
  ((CP_P) breg)->cp = contp;                     \
  ((CP_P) breg)->tops = tos;                     \
}

#define cp_overflow(p,lim) if (p > cp_limit) deal_with_cp_overflow(&locmach);

#define push_choicepoint2(alt,contp)                             \
{                                                                \
  register dlong *p;                                             \
                                                                 \
  p = breg;                                                      \
  cp_overflow(p,cp_limit);                                       \
  p = breg;                                                      \
  p += CP_FIX_LEN;                                               \
  *(p++) = args(2);                                              \
  *(p++) = args(1);                                              \
  ((CP_P)p)->failurecont = (codep)alt;                           \
  ((CP_P)p)->h = hreg;                                           \
  ((CP_P)p)->tr = trreg;                                         \
  ((CP_P)p)->e = ereg;                                           \
  ((CP_P)p)->b = breg;                                           \
  ((CP_P)p)->cp = contp;                                         \
  ((CP_P)p)->tops = tos;                                         \
  breg = p;                                                      \
}

#define push_choicepoint3(alt,contp)                             \
{                                                                \
  register dlong *p;                                             \
                                                                 \
  p = breg;                                                      \
  cp_overflow(p,cp_limit);                                       \
  p = breg;                                                      \
  p += CP_FIX_LEN;                                                \
  *(p++) = args(3);                                              \
  *(p++) = args(2);                                              \
  *(p++) = args(1);                                              \
  ((CP_P)p)->failurecont = (codep)alt;                           \
  ((CP_P)p)->h = hreg;                                           \
  ((CP_P)p)->tr = trreg;                                         \
  ((CP_P)p)->e = ereg;                                           \
  ((CP_P)p)->b = breg;                                           \
  ((CP_P)p)->cp = contp;                                         \
  ((CP_P)p)->tops = tos;                                         \
  breg = p;                                                      \
}

#define push_choicepoint4(alt,contp)                             \
{                                                                \
  register dlong *p;                                             \
                                                                 \
  p = breg;                                                      \
  cp_overflow(p,cp_limit);                                       \
  p = breg;                                                      \
  p += CP_FIX_LEN;                                                \
  *(p++) = args(4);                                              \
  *(p++) = args(3);                                              \
  *(p++) = args(2);                                              \
  *(p++) = args(1);                                              \
  ((CP_P)p)->failurecont = (codep)alt;                           \
  ((CP_P)p)->h = hreg;                                           \
  ((CP_P)p)->tr = trreg;                                         \
  ((CP_P)p)->e = ereg;                                           \
  ((CP_P)p)->b = breg;                                           \
  ((CP_P)p)->cp = contp;                                         \
  ((CP_P)p)->tops = tos;                                         \
  breg = p;                                                      \
}

#define push_choicepoint5(alt,contp)                             \
{                                                                \
  register dlong *p;                                             \
                                                                 \
  p = breg;                                                      \
  cp_overflow(p,cp_limit);                                       \
  p = breg;                                                      \
  p += CP_FIX_LEN;                                                \
  *(p++) = args(5);                                              \
  *(p++) = args(4);                                              \
  *(p++) = args(3);                                              \
  *(p++) = args(2);                                              \
  *(p++) = args(1);                                              \
  ((CP_P)p)->failurecont = (codep)alt;                           \
  ((CP_P)p)->h = hreg;                                           \
  ((CP_P)p)->tr = trreg;                                         \
  ((CP_P)p)->e = ereg;                                           \
  ((CP_P)p)->b = breg;                                           \
  ((CP_P)p)->cp = contp;                                         \
  ((CP_P)p)->tops = tos;                                         \
  breg = p;                                                      \
}


#define push_arguments(arity)                \
{                                            \
  dlong i;                                     \
  dlong *p;                                    \
  p = breg+CP_FIX_LEN;  \
  for (i = arity; i; ) *p++ = args(i--);  \
}


#define pop_arguments(arity)      \
{                                 \
  dlong i = arity;                  \
  dlong *p;                         \
  p = breg - i;                   \
  while (i) args(i--) = *p++ ;    \
}


#define pop_arguments2(b)                                  \
{                                                                \
  register dlong *p;                                             \
  p = (dlong *)b -1;                                             \
  args(1) = *p ;                                                 \
  args(2) = *(p-1) ;                                             \
}

#define pop_arguments3(b)                                  \
{                                                                \
  register dlong *p;                                             \
  p = (dlong *)b -1;                                             \
  args(1) = *p ;                                                 \
  args(2) = *(p-1) ;                                             \
  args(3) = *(p-2) ;                                             \
}

#define pop_arguments4(b)                                  \
{                                                                \
  register dlong *p;                                             \
  p = (dlong *)b -1;                                             \
  args(1) = *p ;                                                 \
  args(2) = *(p-1) ;                                             \
  args(3) = *(p-2) ;                                             \
  args(4) = *(p-3) ;                                             \
}


#define pop_arguments5(b)                                  \
{                                                                \
  register dlong *p;                                             \
  p = (dlong *)b -1;                                             \
  args(1) = *p;                                                  \
  args(2) = *(p-1) ;                                             \
  args(3) = *(p-2) ;                                             \
  args(4) = *(p-3) ;                                             \
  args(5) = *(p-4) ;                                             \
}


                  

#define deal_with_loading_select_struct2(dlong0, dlong1, dlong3, topofcode) \
        if (!(0x1 & dlong0)) *(dlong *)(arg_place(locpc,1,select_functor2)) = make_atom(get_tempsymbtab(dlong1)); \
        if (!(0x2 & dlong0)) *(dlong *)(arg_place(locpc,3,select_functor2)) = make_atom(get_tempsymbtab(dlong3));

#define deal_with_loading_select_struct3(dlong0, dlong1, dlong3, dlong5, topofcode) \
        if (!(0x1 & dlong0)) *(dlong *)(arg_place(locpc,1,select_functor3)) = make_atom(get_tempsymbtab(dlong1)); \
        if (!(0x2 & dlong0)) *(dlong *)(arg_place(locpc,3,select_functor3)) = make_atom(get_tempsymbtab(dlong3)); \
        if (!(0x4 & dlong0)) *(dlong *)(arg_place(locpc,5,select_functor3)) = make_atom(get_tempsymbtab(dlong5));

#endif __TOINCLUDE
