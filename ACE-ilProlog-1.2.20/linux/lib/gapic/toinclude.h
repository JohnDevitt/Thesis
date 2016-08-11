/**
 * This is the toplevel file for defining general purpose constants,
 * types, ...
 * Some constants are defined by the configure script; these can
 * be found, together with their documentation, in 'config.h'
 */

#ifndef __TOINCLUDE 
#define __TOINCLUDE 1

#define HIPP_VERSION "0.1.14"

#define _FILE_OFFSET_BITS 64
#include <stdio.h>

// #define _ISOC99_SOURCE
#include "config.h"
#define NEWTAGGING
 

/* OS-specific flags. 
 * FIXME: These should all be removed and handled by autoconf in some way.
 * I'm waiting for this because the nuances are not always clear (RT) */

#if defined(cygwin) || defined(MICROSOFT)
#define WINDOWS 1
#endif


/* Dynamic Linking defines.
 * TODO: This could be handled by 'libtool' in the future */
#ifdef USESO
	#ifdef WINDOWS
		#define SODEF __declspec(dllimport)
        #define SOPROVIDE __declspec(dllexport)
	#else
		#define SODEF
        #define SOPROVIDE	
	#endif
#else
    #ifdef BUILDSO
	#define SODEF __declspec(dllexport)
    #else
	#define SODEF
	#define MAIN_FUNCTION main
    #endif
#endif


#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif


/****************************************************************************** 
 * Primitive types 
 ******************************************************************************/

#define dlong long
/* #define dfloat float */
#define dfloat double

#ifndef HAVE_BYTE
typedef unsigned char byte;
#endif

typedef byte *codep;
typedef dlong boolean;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


/****************************************************************************** 
 * Alignment of doubles.
 * On Sparc, doubles should be aligned in a special way.
 ******************************************************************************/

#ifdef SPARC
#define ALIGNED_DOUBLE
#endif
#ifdef ALPHA
// #define ALIGNED_DOUBLE
#endif

#ifndef ALIGNED_DOUBLE
#define store_double_arg(where,ff) \
			*(dfloat *)(where) = (dfloat) ff
#define get_double_arg(ff, where) \
			ff = *(dfloat *)(where)
#else
typedef union {
  struct two_ints{ dlong i1,i2; } two_ints;
  dfloat f;
} conversion_type;

#define store_double_arg(where, ff) { \
			conversion_type ct; \
			ct.f = ff; \
			*(dlong *) (where) = ct.two_ints.i1; \
			*(((dlong *) (where)) +1) = ct.two_ints.i2; \
		}
#define get_double_arg(ff, where) { \
			conversion_type ct; \
			ct.two_ints.i1 = *(dlong*)(where); \
			ct.two_ints.i2 = *(((dlong*)(where))+1); \
			ff = ct.f; \
		}
#endif



/****************************************************************************** 
 * Files & I/O
 ******************************************************************************/

#ifdef MICROSOFT
#define STDIN 0
/* 
#define STDIN_TYPE FILE *
#define STDIN_NOFILE NULL
*/
#else
#define STDIN STDIN_FILENO
#define STDOUT STDOUT_FILENO
#endif

#define STDIN_TYPE int
#define STDIN_NOFILE -1

#define STDERR stderr

#define ILP_STDOUT 0
#define ILP_STDERR 1
#define ILP_STDIN 2

enum open_file_type {NOT_USED, FILE_WRITE, FILE_READ, USER_DEFINED}; 

typedef struct file_struct
{
  enum open_file_type status;
  FILE *fp;
  int fd;
  dlong f_symbol;   /* index in atomtable */
  unsigned char *ptr_in, *end_in, *buffer_in;  
  unsigned char *ptr_out, *buffer_out;
  unsigned int buffer_size;
  int (*fill_buffer)(struct file_struct *);
  int (*empty_buffer)(struct file_struct *);
  int (*close_file)(struct file_struct *);
  int linenumber;
} file_struct;

#define MAX_OPEN_FILES 256

#ifdef HAVE_LIBREADLINE 
#include <readline/readline.h>
#include <readline/history.h>
#endif



#ifdef WINDOWS
	#define ILPROLOG_PATH_SEP '\\'
	#define ILPROLOG_DIR_SEP ';' 
	
	char* wingetenv(const char * name);
#else
	#define ILPROLOG_PATH_SEP '/'
	#define ILPROLOG_DIR_SEP ':'
#endif

// Do not remove: needed for creating Windows versions of ACE!
#ifdef AUTO_INSTALL_DIR
#ifndef MAIN_C
	extern char* auto_install_dir;
#endif
    void make_auto_install_dir(const char* mainarg, const char* ilphome, const char* initialf);
#endif

#define IS_ABS_PATH(s) (s[0] == '/' || (s[0] != 0 && s[1] == ':'))


/* Label & offset macros.
 * The version for position independent code (PIC) is actually never used.
 * Bart knows more about PIC */

// #define PIC_hipP
#ifdef PIC_hipP
#define set_memlabel_or_offset(p,l) (dlong)(memlabeltabel[l]-p)
#define set_label_or_offset(p,l) (dlong)(labeltabel[l]-p)
#define get_label_or_offset0(p,l) ((l) ? ((codep)(p+(dlong)l)) : 0)
#define get_label_or_offset(p,l) ((codep)(p+(dlong)l))
#define set_fail(p) 0
#else
#define set_memlabel_or_offset(p,l) (dlong)memlabeltabel[l]
#define set_label_or_offset(p,l) (dlong)labeltabel[l]
#define get_label_or_offset0(p,l) (codep)l
#define get_label_or_offset(p,l) (codep)l
#define set_fail(p) 0
#endif

#define USER_MOD 0


/* for the following, the unit is (dlong *) */

#define SPARE_HEAP           2000
#define SPARE_TRAIL            10
#define SPARE_LS              300
#define SPARE_CP              310
#define MALLOC_SPARE           16
#define MAXARG                256
#define C_INTERFACE_REF_SIZE   60
#define C_INTERFACE_SYMBOLS_SIZE   60

#define SLACK 64
#define OUTPUTBUFFERSIZE  4096
#define INPUTBUFFERSIZE  4096

/* constants specifying what to do on undefined predicates */
#define UNDEF_WARN   0
#define UNDEF_FAIL   1
#define UNDEF_TRACE  3
#define UNDEF_ERROR  4

#define ARGS 250

extern dlong global_set_protected_pred;
extern int nr_internal_code;
extern char* internal_code[];


// extern char debugbuffer[100000];
// extern int debugbufferptr;



/****************************************************************************** 
 * Hash tables
 ******************************************************************************/ 
#define HASH_MOD 0
// #define PROF_ATOMHASH 1
// #define PROF_FUNCHASH 1
#define HASH_LIST 23

typedef struct record_key_type {
  unsigned dlong key1;
  unsigned dlong key2;
} record_key_type;

typedef struct permanent_hash_tableentry {
  record_key_type key;
  dlong *value;
} permanent_hash_tableentry;

typedef struct hash_pocket_type {
  dlong key;
  dlong value;
} hash_pocket_type;

typedef struct hash_bucket_type {
  hash_pocket_type pocket;
  dlong next;
} hash_bucket_type;

typedef struct hash_table {
  dlong mask;
  dlong next_free;
  hash_bucket_type *entrys;
} hash_table;

typedef struct index_new_type {
  int index;
  boolean isnew;
} index_new_type;

typedef struct Hash_table_iterator {
  int index;
  int next;
} Hash_table_iterator;


/****************************************************************************** 
 * Query packs 
 ******************************************************************************/

#define FLOCK_INDEX_INITIAL_SIZE 20
#define FLOCK_TABLES_INITIAL_SIZE 1000

/*#define FLOCK_INDEX_INITIAL_SIZE 2
#define FLOCK_TABLES_INITIAL_SIZE 0*/

typedef struct flock_table
{
  dlong opcode;
  dlong n;
  codep alts[2];    /* do not change the '2'. Functions using 
                       sizeof(struct flock_table) depend on it */
} flock_table ;

struct flock_index
{
    codep flock_start_opcode; 
    dlong *flock_tables, *flock_tables_static;
    dlong *flock_ptr_static;
    size_t tables_size;
    dlong max_tables /* The amount of tables in the table[] array. This does not
						include the first imaginary table containing the amount
						of tables */;
    flock_table* table[1]; 
    /* table[0] == nb_tables. Should be a struct field, but then all
     *                        indexes should be changed into index-1 etc. */
};

#define FT_P struct flock_table *
extern struct flock_index *flock_index;
extern dlong flock_b;

// TODO: Actually use this macro in the code (a.o. in symtab.c)
#define get_static_flocktable(fi,i) \
	((flock_table*) ((dlong) fi->table[i] \
		+ ((dlong) fi->flock_tables_static \
			- (dlong) fi->flock_tables)))
	

/****************************************************************************** 
 * ADPacks
 ******************************************************************************/

// branches is obsolete with ADPACK_FAST_INIT
// adpack_alttable_ptr is obsolete ?
#define ADPACK_FAST_INIT
//#define ADPACK_DEBUG

typedef struct adpack_alttable {
    dlong parent_branch;
    dlong branches;
    dlong openns;
    dlong tosucceed;
    dlong openBranches[1]; //FIXME: should be dynamic 
} adpack_alttable;

#define ADPACK_SUCCESS  0x1
#define ADPACK_CLOSED     0x2
#define ADPACK_LEAF     0x1
typedef struct adpack_branch {
    codep code; /* Also contains success & open flags */
    adpack_alttable* alttable; /* Also contains leaf flag */
} adpack_branch;

typedef struct adpack_actstack_el {
    struct adpack_actstack_el *prev;
    dlong b; 
} adpack_actstack_el;

#define ADPACK_DEACTIVATED 0x1
typedef struct adpack_act {
    dlong branch;
    dlong* path; /* Also contains deactivated flag */
} adpack_act;

struct adpack_data {
    adpack_branch* branchtable;
	byte* branchstatus;
    adpack_actstack_el* actstack;
    adpack_act* acttable;
    dlong* retry_instr;
    adpack_alttable** alttables; 
	void* alttables_data;
	void* static_alttables_data;
    dlong* pathtable;
    void *alttable_ptr, *static_alttable_ptr, *pathtable_ptr;
};


/****************************************************************************** 
 * Codezone + Code GC
 ******************************************************************************/

typedef struct code_gc_struct {
  dlong tim1;
  dlong tofexptrs;
  int code_inc_opt;
} code_gc_struct;

typedef struct cexptrs{
  codep caddr;         /* address in the codezone */
  codep *peref;        /* address of the external reference */
} cexptrs;

typedef struct csegment_entry{
  codep begin;
  codep end;
  struct csegment_entry *next;
  struct csegment_entry *prev;
} csegment_entry;

extern csegment_entry *lcodezone;
extern codep codezone;
extern codep topofcode;

SODEF extern codep *entries;
#define main_entrypoint(i)  entries[i]


#define freeze_heap() \
	if (locmach.FH > ((CP_P)breg)->h) ((CP_P)breg)->h = locmach.FH
#define adapt_freeze_hreg(plocmach) \
	{(plocmach)->BH = (plocmach)->FH = (plocmach)->H; \
     ((CP_P)(plocmach)->B)->h = (plocmach)->H;}


/****************************************************************************** 
 * Convenience macros.
 ******************************************************************************/

#define CP_P struct choicepoint *
#define CP_FIX_LEN (sizeof(struct choicepoint)/sizeof(dlong *))


	 
/****************************************************************************** 
 * Machine
 ******************************************************************************/

typedef struct machine {
  dlong *H;
  dlong *TR;
  dlong *E;
  dlong *B;
  dlong *BH;
  codep CONTP;
  dlong *TOS;
  dlong Areg[ARGS];
  dlong *FH;
  codep P;
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
  hash_table permanent_hash;
  FILE *errout;
  STDIN_TYPE file_in;
  int file_out;
  struct file_struct openfiles[MAX_OPEN_FILES];
  dlong nr_of_heapgc;
  dlong nr_of_major_heapgc;
  dlong time_of_heapgc;
  dlong bytes_of_heap_gc1;
  dlong bytes_of_heap_gc2;
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
  dlong c_interface_symbols_unused;
  dlong c_interface_symbols_allocated;

  /* Flocks */
  codep flock_retry_opcode;

  /* ADPacks */
  adpack_branch* adpack_branchtable;
  byte* adpack_branchstatus;
  adpack_actstack_el* adpack_actstack;
  adpack_act* adpack_acttable;
  dlong* adpack_retry_instr;
  adpack_alttable** adpack_alttables;
  dlong adpack_b;

  /* JIT */
  codep cf_target;
} machine;

struct used_consts_type {
  dlong yes_0, no_0;
};

struct used_functors_tagged_type {
  dlong dollar_dollar_cf_code_1;
};

struct used_functors_type {
  dlong deal_with_alarm_0;
};

struct used_atoms_type {
  dlong debug_call_1, dynamic_execute_1, mcall_1; 
  dlong plus_2, sub_2, band_2, bor_2, mult_2, div_2, idiv_2;
  dlong mod_2, lgshiftl_2, lgshiftr_2,max_2,min_2, power_2;
  dlong plus_1, sub_1, neg_1, log_1,abs_1,exp_1,round_1,sqrt_1, ceiling_1;
  dlong cos_1, sin_1, tan_1;
  dlong fx, fy, xf, yf, xfx, xfy, yfx;
  dlong dot_2, dot_0, user_0, nil_0;
  dlong warn_0, fail_0, cutfail_0, halt_0, trace_0, error_0, yes_0, no_0;
  dlong deal_with_sigint_1;
  dlong lib_0, flock_0, user_query_1, dollarVAR_1, gen_cp_0, dollar_default_0;
  dlong colon_2, semicolon_2;
  dlong ask_0, exception_0, overwrite_0;
  dlong dollar_adpack_disable_0;
  
  /* JIT compilation */
  dlong jit_compile_1;
};

/* debugging.c */
int debug_2_builtin(struct machine *plocmach);

/* main.c */
SODEF extern dlong compare_terms(dlong *,dlong *);
extern char *expand_file_name(char *);
extern dlong current_time(void);
dlong unify_terms(dlong *, dlong *, struct machine *);
SODEF dlong unify_terms_always_trail(dlong *, dlong *, struct machine *);
void allocatefilebuffer(struct file_struct *, STDIN_TYPE);
void returnfilebuffer(struct file_struct*);
dlong char_list_to_string(dlong *, char *, dlong/*, dlong **/);
dlong list_to_string(dlong *, char *, dlong/*, dlong **/);
SODEF void expand_symbol_refs(struct machine *machine);
SODEF int execute_machine(struct machine * mach_do_not_use, dlong first_time);
void exit_mess_t1(char *mess, struct machine *plocmach);


SODEF extern struct used_atoms_type used_atoms;
SODEF extern struct used_functors_type used_functors;
SODEF extern struct used_functors_tagged_type used_functors_tagged;
extern struct used_consts_type used_consts;
extern long overestimate_amount_gc;
extern char *instr_name[];
/* extern byte codezone[]; */
SODEF extern dlong nil_atom;


/* Structure containing all the run-time options */
enum overwrite_system_predicate_action_type {ASK, OVERWRITE, EXCEPTION};

typedef struct options_bag_type {
	boolean do_code_gc; 
	boolean silent;
	boolean show_prompt;
	boolean show_banner;
	boolean save_history;
	enum overwrite_system_predicate_action_type 
		overwrite_system_predicate_action;
	boolean use_readline;
} options_bag_type;

extern options_bag_type options_bag;


typedef struct atominfo
{ dlong modnr;
  char *name;
} atominfo;
#define AI_P struct atominfo *
extern dlong numb_atoms;

typedef struct funcinfo
{ dlong arity;
  dlong modnr;
  char *name;
  char debug_info;
} funcinfo;
SODEF extern struct funcinfo *functab;
extern dlong numb_funcs;

#define SPIED_BIT 0x1
#define PROTECTED_BIT 0x2

#define is_spied(index)     (functab[index].debug_info & SPIED_BIT)
#define is_not_spied(index) (~(functab[index].debug_info & SPIED_BIT))

#define set_spied(index)     functab[index].debug_info |= SPIED_BIT
#define set_not_spied(index) functab[index].debug_info &= ~SPIED_BIT

#define is_protected(index)      (functab[index].debug_info & PROTECTED_BIT)
#define set_protected(index)     functab[index].debug_info |= PROTECTED_BIT
#define set_unprotected(index)   functab[index].debug_info &= ~PROTECTED_BIT


extern dlong *modtab;
extern dlong current_version;

#ifdef PROFILING
extern dlong *call_count;
#define profile_call(i) call_count[i]++
#else
#define profile_call(i)
#endif

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

#define atom_to_func(ai)  func_lookup(atomtab_modnr(ai),0,atomtab_name(ai))  
#define func_to_atom(fi) atom_lookup(functab[fi].modnr,functab[fi].name)

#define arg_place(where,N,I) (where + I##_##N)


#define fill_try_retry_trust_code(where,what,arity,label,len)  \
          {  if (where >= lcodezone-> end ) return(0);\
	    *where = what;                                    \
             *(arity_type *)(where + what##_1) = (arity_type)arity; \
             *(label_type *)(where + what##_2) = (label_type)label;   \
             where += len;                                     \
          }


#if defined(THREADED)
#define fill_opcode(p,opc) *(void **)p = instr_labels[opc]
#define compare_opcode(p, opc) *(void**)p == instr_labels[opc]
#define goto_more_exec      goto  **(void **)locpc
extern void **instr_labels;
#define points_to_debug_entry(pc)  ((pc != 0) && (*(void **)pc == instr_labels[debug_entry]))
#define points_to_or_retry_trust(pc)   ((pc != 0) && ((*(void **)pc == instr_labels[retrymeorelse]) || (*(void **)pc == instr_labels[trustmeorelsefail])))

#elif defined(SWITCH)
#define fill_opcode(p,opc) *(switch_opcode_type*)p = opc
#define compare_opcode(p, opc) *(switch_opcode_type*)p == opc
#define goto_more_exec goto more_exec
#define points_to_debug_entry(pc) (*(switch_opcode_type*)pc == debug_entry)
#define points_to_or_retry_trust(pc)   ((*((switch_opcode_type *)(pc)) == retrymeorelse) || (*((switch_opcode_type *)(pc)) == trustmeorelsefail))
#endif


// FIXME: Zouden opeenvolgend in de functortabel moeten gestockeerd worden. Dan
//        een switch doen op het nummer van de functor. (evaluate_expression
//        in builtins.c
// Moet allemaal naar expression.[ch]
enum operation {SUM, SUB, AND, OR, MULT, DIV, IDIV, MOD, LOGSHIFTL, LOGSHIFTR, 
                MAX, MIN, POWER, SUM_1, SUB_1, NEGATE, LOG,
                ABS, EXP, ROUND, CEILING, SQRT, BIT_AND, BIT_OR,
				COS, SIN, TAN
               /* and many others*/};  
enum tagexpr { ERROREXPR, INT_EXPR, DFLOAT };


/**
 * Atom tables.
 *
 * atomi1tab is een tabel van pointers naar blokken van elk 2^19 atomen.
 * Van de atoomindex zijn de eerste 10 bits de offset in atomi1tab, en
 * de laatste 19 bits de offset in het blok.
 *
 * NOTE:
 * - Er wordt verondersteld dat een tag 3 bits is.
 */

#define get_ati1(i)  (dlong)((unsigned int)i>>19)  // take bits 29-20 and >> 19
#define get_ati2(i)  (dlong)( i & 0x0007ffff)      // keep rightmost 19 bits
#define make_atindex(i1,i2)   (dlong)((i1<<19) + i2)

SODEF extern AI_P  atomi1tab[];

static inline AI_P atomtab(dlong i)   
{
  return((atomi1tab[get_ati1(i)] + get_ati2(i)));// is a AI_P
}

static inline char* atomtab_name(dlong i)  
{
  return((atomi1tab[get_ati1(i)] + get_ati2(i))->name);
}
static inline dlong atomtab_modnr(dlong i)   
{
  return((atomi1tab[get_ati1(i)] + get_ati2(i))->modnr);
}

SODEF extern dlong atom_lookup(dlong, const char *);
SODEF extern dlong func_lookup(dlong, dlong, const char *);
extern dlong mod_lookup(dlong);
extern codep sym_codep(dlong, dlong, char *);
extern void exit_mess(char *, dlong);
extern void exit_mess_allocate(char *, dlong, dlong);


// FIXME: Moet naar expression.h
typedef union {
  dlong ival;
  dfloat dflval;
}  valexpr;

typedef struct {
  enum tagexpr tag;
  valexpr val;
} taggedvalexpr;

extern taggedvalexpr evaluate_expression_gen(dlong *p, struct machine *mach);

#ifdef NEWTAGGING

#define REF0       0   // 0000
#define REF1       4   // 0100
#define REF2       8   // 1000
#define REF3      12   // 1100 0xc

#define case_ref  case REF0: case REF1: case REF2: case REF3

#define LIST      2   // 0010
#define LIST1      6   // 0110
#define LIST2     10   // 1010 0xa
#define LIST3     14   // 1110 0xe

#define case_list  case LIST: case LIST1: case LIST2: case LIST3

#define ATOM_TAG   1   // 0001
#define INT_TAG    5   // 0101

#define STRUCT     3   // 0011 tag on data 
#define REAL       7   // 0111 tag on data 
#define DELAY1    11   // 1011 tag on data 
#define REST_TAG  15   // 1111 tag on data 

#define tag(p)                     (((dlong)p) & 0xf)
#define minitag(p)                 (((dlong)p) & 0x3)
// tagcell(p) returns the tag of a value, in case p is a ref to a 
// datatype which has tag on data, then the tag of the data referred 
// is returned
#define tag_cell(p)                tag_cell_func((dlong)p)

static inline dlong tag_cell_func(dlong p) {
  if (minitag(p) == REF0) {
    dlong q = *((dlong*) p);
    if (minitag(q) == STRUCT) return (tag(q));
    else return(REF0);
  } else return (tag(p));
}

#define ref_or_value(p,q)          ((minitag(p)==0x3) ? q : p)
#define struct_reference(p,q)      q

#define is_ref(t)                   (minitag(t) == REF0)
#define is_ref_cell(t)              ((minitag(t) == REF0) && \
		                        ((dlong)t == *(dlong*)t) )
#define is_atomic(t)                ((minitag(t) == ATOM_TAG) || (tag(t)==REAL))
#define is_atom_or_int(t)           ((minitag(t) == ATOM_TAG))
#define is_list(t)                  (minitag(t) == LIST)
#define is_struct(t)                (tag(t) == STRUCT)
#define is_compound(t)              ((tag(t) == STRUCT) || minitag(t) == LIST)
#define is_integer(t)               (tag(t) == INT_TAG)
#define is_real(t)                  (tag(t) == REAL)
#define is_atom(t)                  (tag(t) == ATOM_TAG)
#define is_number(t)                ((tag(t) == INT_TAG) || (tag(t) == REAL))
#define is_delayed(t)               (tag(t) == DELAY1)
#define is_var(t)                   (is_ref(t) || is_delayed(t))

#define get_real_pointer_new(p,q)   ( (dlong*)q )
#define get_real_pointer_cell(p)    ( (dlong*)p )
#define get_real_from_pointer(p)    ( *(dfloat*)((dlong*)p+1))
#define get_real(p,q)               get_real_from_pointer(q)
#define get_real_cell(p)            get_real_from_pointer(p)

#define get_struct_value(p)         ((dlong)p)
#define get_struct_pointer(p)       ((dlong*)NULL)
#define get_struct_pointer_new(p,q) ((dlong *)q)
#define get_struct_pointer_cell(p) ((dlong *)p)
#define get_list_pointer(p)         ( (dlong *) (((unsigned dlong)p) & ~0x2) )
#define get_delay_pointer(p)        ( ((dlong *)p) +1)
                                                                                 
#define make_list(x)                ((dlong)(x) | 0x2)

#define get_atom_index(p)           ((dlong)p >> 4)
#define get_atom_name(p)            atomtab_name(get_atom_index(p))
#define get_atom_module(p)          atomtab_modnr(get_atom_index(p))
                                                                                
#define make_atom(at)               ((((dlong)at) << 4) | ATOM_TAG)

static dlong inline make_funct(dlong at)
{
   if (functab[at].arity > 127)
	      return(((((dlong)at) << 11) |  STRUCT));
   return(((((dlong)at) << 11) | (functab[at].arity << 4) | STRUCT));
} /* make_funct */

static dlong inline get_funct_arity(dlong i)
{
   dlong j = (i & 0x7ff) >> 4;
   if (j) return(j);
   return(functab[i>>11].arity);
} /* get_funct_arity */

#define get_funct_symb(i)           (i >> 11)
#define get_funct_symb_from_struct(p) ((dlong)p >> 11)


#define get_funct_name(i)           functab[get_funct_symb(i)].name
#define get_funct_module(i)         functab[get_funct_symb(i)].modnr

#define make_struct_p(p)            ((dlong)(p))

#define make_delay_p(p)             ((dlong)(p))

#define SIZE_PROLOG_INTEGER         1
#define get_integer(p)              (((dlong)p) >> 4)
#define make_tagged_int(i)          (((i) << 4) | INT_TAG)
#define build_integer(p,i)          {*p = make_tagged_int(i); }
                                                                                
#define make_real_p(p)              ((dlong)p)
#define SIZE_OF_FLOAT               3 
#define push_real(h,f)      *h = REAL;(h)++;*(dfloat *)(h) = (dfloat)(f);(h)+=2
#define store_real(p, f)            *p= REAL; *(dfloat*)(p+1)=(dfloat)(f); 

// deref_val_refval takes as input val, a heap-value.
// It returns in val: deref-ed value.
//            refval: in case val has tag-on-data, a ref to this data
//                    otherwise equal to val.
#define deref_val_refval(val, refval) { \
	refval = val; \
	if (is_ref(val)) {\
	   val = (dlong*)(*val); \
           while ((val != refval) && is_ref(val)) { \
	      refval = val; val = (dlong*)(*val); }\
        } }
		
#define deref_lab(val, refval, undef)  \
	if (is_ref(val)) {\
           do {\
	      refval = val; val = (dlong*)(*val); \
              if (val == refval) undef ; \
           } while (is_ref(val)); \
	} else {refval = val; }
		
#define simple_deref(val) { \
    while (is_ref(val)) { \
       dlong *qqqq = val; val = (dlong*)*val; \
       if (qqqq == val) break; \
    } }

// deref until not ref or points to data with tag on data 
#define deref_to_cell(val) { \
    dlong tag = minitag(val);\
    while (tag == REF0) {\
	dlong q = *val; \
	if ((dlong) val == q) break;\
	tag = minitag(q);\
	if (tag == STRUCT) break;\
	val = (dlong*) q;\
    }\
  }

#else // end NEWTAGGING

#ifndef ALPHA

#define REF0        0
#define REF1        4

#define case_ref    case REF0: case REF1

#define LIST        7

#define case_list  case LIST

#define STRUCT      6

#define ATOM_TAG        2
#define INT_TAG         3
#define REAL        1

#define DELAY1       5

#define tag(p)                     (((dlong)p) & 0x7)
#define minitag(p)                 (((dlong)p) & 0x3)
#define tag_cell(p)                 tag(p)

#define ref_or_value(p,q)           p
#define struct_reference(p,q)       p

#define is_ref(t)                   (minitag(t) == REF0)
#define is_ref_cell(t)              (minitag(t) == REF0)
#define is_atomic(t)                ((tag(t) >= REAL) && (tag(t) <= INT_TAG))
#define is_atom_or_int(t)           ((tag(t) == ATOM_TAG) || (tag(t) == INT_TAG))
#define is_list(t)                  (tag(t) == LIST)
#define is_struct(t)                (tag(t) == STRUCT)
#define is_compound(t)              (tag(t) >= STRUCT)
#define is_integer(t)               (tag(t) == INT_TAG)
#define is_real(t)                  (tag(t) == REAL)
#define is_atom(t)                  (tag(t) == ATOM_TAG)
#define is_number(t)                ((tag(t) == INT_TAG) || (tag(t) == REAL))
#define is_delayed(t)               (tag(t) == DELAY1)
#define is_var(t)                   (is_ref(t) || is_delayed(t))

#define get_real_pointer(p)         ((dlong *)((unsigned dlong)(((dlong)p & ~0x7)) >> 1))
#define get_real_pointer_new(p,q)         ((dlong *)((unsigned dlong)(((dlong)p & ~0x7)) >> 1))
#define get_real_pointer_cell(p)    get_real_pointer(p)

#define get_real(p,q)               *(dfloat*)(get_real_pointer(p))
#define get_real_cell(p)            *(dfloat*)(get_real_pointer(p))


#define get_struct_pointer(p)       ((dlong *)((unsigned dlong)(((dlong)p & ~0x7)) >> 1))
#define get_struct_value(p)         (*get_struct_pointer(p))
#define get_struct_pointer_new(p,q) get_struct_pointer(p)
#define get_struct_pointer_cell(p)  get_struct_pointer(p)
#define get_list_pointer(p)         ((dlong *)((unsigned dlong)((~(dlong)p)) >> 1))
#define get_delay_pointer(p)        ((dlong *)((unsigned dlong)(((dlong)p & ~0x7)) >> 1))

#define make_list(x)                (~((dlong)(x) << 1))


#define get_atom_index(p)           ((dlong)p >> 3)

#define get_atom_name(p)            atomtab_name(get_atom_index(p))
#define get_atom_module(p)          atomtab_modnr(get_atom_index(p))

#define make_atom(at)               ((((dlong)at) << 3) | ATOM_TAG)
/*
#define make_funct(at)              ((((dlong)at) << 11) | (functab[at].arity << 3) | STRUCT)

#define get_funct_arity(i)          (i & 0x7ff) >> 3
*/
#define get_funct_symb(i)           (i >> 11)
#define get_funct_symb_from_struct(p) (*get_struct_pointer(p) >> 11)

inline static dlong make_funct(dlong at)
{
  if (functab[at].arity > 255)
    return(((((dlong)at) << 11) |  STRUCT));
  return(((((dlong)at) << 11) | (functab[at].arity << 3) | STRUCT));
} /* make_funct */

inline static dlong get_funct_arity(dlong i)
{
  dlong j = (i & 0x7ff) >> 3;
  if (j) return(j);
  return(functab[i>>11].arity);
} /* get_funct_arity */

#define get_funct_name(i)           functab[get_funct_symb(i)].name
#define get_funct_module(i)         functab[get_funct_symb(i)].modnr

#define make_struct_p(p)            (((dlong)(p) << 1) | STRUCT)
#define make_delay_p(p)             (((dlong)(p) << 1) | DELAY1)


#define SIZE_PROLOG_INTEGER         1
#define get_integer(p)              (((dlong)p) >> 3)
#define make_tagged_int(i)          (((i) << 3) | INT_TAG)
#define build_integer(p,i)          {*p = make_tagged_int(i); }

#define make_real_p(p)         (((dlong)(p) << 1) | REAL)
#define SIZE_OF_FLOAT sizeof(dfloat)/sizeof(dlong)
#define push_real(h,f)         *(dfloat *)(h) = (dfloat)(f); (h)+=SIZE_OF_FLOAT
#define store_real(p,f)        *(dfloat *)p = (dfloat)(f);

#define simple_deref(p) deref0(p);
#define deref_to_cell(p) deref0(p);
#define deref_val_refval(val, refval) deref(val, refval)
#define deref_lab(p,q,undef) deref2(p,q,undef)

#endif 

#endif 

#ifdef ALPHA

#define REF0        0
#define case_ref    case REF0

#define LIST        7

#define case_list  case LIST

#define STRUCT      6

#define ATOM_TAG        2
#define INT_TAG         3
#define REAL        1

#define DELAY1       5

#define tag(p)                     (((dlong)p) & 0x7)

#define is_ref(t)                   (tag(t) == REF0)
#define is_ref_cell(t)                   (tag(t) == REF0)
#define is_atomic(t)                ((tag(t) >= REAL) && (tag(t) <= INT_TAG))
#define is_list(t)                  (tag(t) == LIST)
#define is_struct(t)                (tag(t) == STRUCT)
#define is_compound(t)              (tag(t) >= STRUCT)
#define is_integer(t)               (tag(t) == INT_TAG)
#define is_real(t)                  (tag(t) == REAL)
#define is_atom(t)                  (tag(t) == ATOM_TAG)
#define is_number(t)                ((tag(t) == INT_TAG) || (tag(t) == REAL))
#define is_delayed(t)               (tag(t) == DELAY1)
#define is_var(t)                   (is_ref(t) || is_delayed(t))

#define get_real_pointer_new(p,q)         ((dlong *)((dlong)p & ~0x7))
#define get_real_pointer_cell(p)    ((dlong *)((dlong)p & ~0x7))
#define get_real(p,q)               *(dfloat*)(get_real_pointer_new(p,q));
#define get_real_cell(p)            *(dfloat*)(get_real_pointer_cell(p));

#define get_struct_pointer(p)       ((dlong *)((dlong)p & ~0x7))
#define get_list_pointer(p)         ((dlong *)((dlong)p & ~0x7))
#define get_delay_pointer(p)        ((dlong *)((dlong)p & ~0x7))

#define make_list(x)                ((dlong)(x) | LIST)


#define get_atom_index(p)           ((dlong)p >> 3)

#define get_atom_name(p)            atomtab_name(get_atom_index(p))
#define get_atom_module(p)          atomtab_modnr(get_atom_index(p))

#define make_atom(at)               ((((dlong)at) << 3) | ATOM_TAG)
#define make_funct(at)              ((((dlong)at) << 11) | (functab[at].arity << 3) | STRUCT)
#define get_funct_arity(i)          (i & 0x7ff) >> 3
#define get_funct_symb(i)           (i >> 11)
#define get_funct_symb_from_struct(p) (*get_struct_pointer(p) >> 11)
#define get_funct_name(i)           functab[get_funct_symb(i)].name
#define get_funct_module(i)         functab[get_funct_symb(i)].modnr

#define make_struct_p(p)            ((dlong)(p) | STRUCT)
#define make_delay_p(p)             ((dlong)(p) | DELAY1)


#define SIZE_PROLOG_INTEGER         1
#define get_integer(p)              (((dlong)p) >> 3)
#define make_tagged_int(i)          (((i) << 3) | INT_TAG)
#define build_integer(p,i)          {*p = make_tagged_int(i); }

#define make_real_p(p)              ((dlong)(p) | REAL)

#define SIZE_OF_FLOAT sizeof(dfloat)/sizeof(dlong)
#define push_real(h,f)         *(dfloat *)(h) = (dfloat)(f); (h)+=SIZE_OF_FLOAT

#endif 


#ifdef ALIGNED_DOUBLE
static inline dfloat get_aligned_real(dlong *where) 
         { conversion_type ct;
           ct.two_ints.i1 = *(dlong*)(where);
           ct.two_ints.i2 = *(((dlong*)(where))+1);
           return(ct.f); 
         }
static inline void push_aligned_real(dlong *where, dfloat ff)
         { conversion_type ct;
           ct.f = ff; 
           *(dlong *) (where) = ct.two_ints.i1;
           *(((dlong *) (where)) +1) = ct.two_ints.i2;
         }

// #define get_real(p,q)            get_aligned_real(get_real_pointer_new(p,q))

#define push_real(h,f)         push_aligned_real(h,f); h += SIZE_OF_FLOAT
#define store_real(h,f)        push_aligned_real(h,f);
#define push_real_max(h,f)     push_aligned_real(h,f); h += 3

#else

// #define get_real(p,q)            *(dfloat *)get_real_pointer_new(p,q)
#define push_real_max(h,f)     push_real(h,f); h += 3-SIZE_OF_FLOAT

#endif


#define build_real(p,f,h)      {*p = make_real_p(h); push_real(h,f); }


/*
#define deref(p,q)    while (is_ref(p) && ((q = (dlong *)*p) != p)) p = q
*/


#define deref2(p,q,undef)  while (is_ref(p)) { q = (dlong *)*p ; if (q == p) undef;  p = q; }

#define deref(p,q) deref2(p,q,break)

#define deref0(p) {dlong *q; deref(p,q); }

/* #define goto_fail_label { goto fail_label; } */
#define goto_fail_label { failure; goto_more_exec;}

#define failure locpc = (codep)(((CP_P) breg)->failurecont)

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

#define push_choicepoint1(alt,contp)                             \
{                                                                \
  register dlong *p;                                             \
                                                                 \
  p = breg;                                                      \
  cp_overflow(p,cp_limit);                                       \
  p = breg;                                                      \
  p += CP_FIX_LEN;                                               \
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

#define pop_arguments1(b)                                  \
{                                                                \
  register dlong *p;                                             \
  p = (dlong *)b -1;                                             \
  args(1) = *p ;                                                 \
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


#endif 

