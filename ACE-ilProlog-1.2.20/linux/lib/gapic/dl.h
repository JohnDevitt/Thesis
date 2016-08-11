#ifdef  __cplusplus
extern "C" {
#endif
#ifdef NEVER
} // to get rid of indenting
#endif

#define DONT_NEED_OPCODES

#include <stdlib.h>
#include <string.h>

#include "toinclude.h"
#include "token.h"
#include "memory.h"

typedef struct machine ILP_engine;
typedef dlong ILP_symbol_ref;
typedef dlong module_index_type;
typedef dlong ILP_term_ref;

#define args(i) (machine->Areg[i])
#define bh (machine->BH)
#define trreg (machine->TR)
#define tr_limit (machine->limit_trail)
#define hreg  (machine->H)
#define breg (machine->B)
#define ereg (machine->E)
#define contp (machine->CONTP)
#define tos (machine ->TOS)
#define i_terms(i) (machine->c_interface_terms[i])
#define i_symbols(i) (machine->c_interface_symbols[i])

extern int term_size(dlong* term);

#ifdef USEDLL
	SODEF dlong nil_atom;
#endif

inline static ILP_term_ref ILP_new_term_ref(ILP_engine *machine)
{
  if (machine->c_interface_max_ref_used >=
      machine->c_interface_ref_allocated) {
    machine->c_interface_ref_allocated =machine->c_interface_ref_allocated *2;
    machine->c_interface_terms =
      (dlong **) realloc(machine->c_interface_terms,
			 sizeof(dlong **)*machine->c_interface_ref_allocated);
  }
  machine->c_interface_terms[machine->c_interface_max_ref_used]=(dlong*)nil_atom;
  machine->c_interface_max_ref_used++;
  return (machine->c_interface_max_ref_used-1);
}

static inline ILP_symbol_ref ILP_new_symbol_ref(ILP_engine *machine)
{
  int to_be_used;
  if (machine->c_interface_symbols_unused == -1)
    expand_symbol_refs(machine);
  to_be_used = machine->c_interface_symbols_unused;
  machine->c_interface_symbols_unused =
                                   machine->c_interface_symbols[to_be_used];
  machine->c_interface_symbols[to_be_used]=nil_atom;
  return (to_be_used);
}

static inline void ILP_delete_symbol_ref(ILP_engine *machine,
					 ILP_symbol_ref symbol)
{
  machine->c_interface_symbols[symbol] = machine->c_interface_symbols_unused;
  machine->c_interface_symbols_unused = symbol;
}

static inline int ILP_number_of_symbols(ILP_engine *machine){
  int i = 0;
  int p = machine -> c_interface_symbols_unused;
  while ( p != -1) {
    i++;
    p = machine->c_interface_symbols[p];
  }
  return(machine->c_interface_symbols_allocated -i);
}

static inline void ILP_copy_symbol(ILP_engine *machine,
				   ILP_symbol_ref from, ILP_symbol_ref to) {
  i_symbols(to) = i_symbols(from);
}

static inline int ILP_order_symbol(ILP_engine *machine,
				     ILP_symbol_ref symb1,
				     ILP_symbol_ref symb2) {
  if (i_symbols(symb1) == i_symbols(symb2)) return (0);
  else if (is_atom(i_symbols(symb1))) {
    if (is_atom(i_symbols(symb1))) {
      return(strcmp(get_atom_name(i_symbols(symb1)),
		    get_atom_name(i_symbols(symb2))));
    } else return(-1);
  } else {
    if (is_atom(i_symbols(symb1))) return (1);
    else {
      int i = get_funct_arity(i_symbols(symb1));
      int j = get_funct_arity(i_symbols(symb2));
      if (i < j) return (-1);
      if (j < i) return (1);
      else return(strcmp(get_funct_name(i_symbols(symb1)),
			 get_funct_name(i_symbols(symb2))));
    }
  }
}

static inline char *name_of_symbol(ILP_engine *machine,
				   ILP_symbol_ref symbol)
{
  if (is_atom(i_symbols(symbol)))
    return(get_atom_name(i_symbols(symbol)));
  else
    return(get_funct_name(i_symbols(symbol)));
}

static inline int arity_of_symbol(ILP_engine *machine,
				  ILP_symbol_ref symbol)
{
  if (is_atom(i_symbols(symbol)))
    return(0);
  else
    return(get_funct_arity(i_symbols(symbol)));
}


static inline dlong ILP_symbol_id(ILP_engine *machine, ILP_symbol_ref symbol) {
  return i_symbols(symbol);
}

static inline dlong ILP_hash_of_term(ILP_engine *machine, ILP_term_ref t)
{
  dlong *p, *q;
  p =  machine->c_interface_terms[t]; deref_val_refval(p,q);
  if (is_atom(p)) {
  	return (dlong)p;
  } else if (is_struct(p)) {
	return (dlong)get_struct_pointer_new(p,q);
  } else if (is_integer(p)) {
  	return get_integer(p);
  } else if (is_real(p)) {
	dfloat res = get_real(p,q);
	void* pfloat = (void*)&res;
	return *((dlong*)pfloat);
  } else {
  	return 0;
  }
}

static inline int ILP_size_of_term(ILP_engine *machine, ILP_term_ref t) {
  dlong *p =  machine->c_interface_terms[t]; deref_to_cell(p);
  return term_size(p);
}

static inline char *ILP_name_of_atom_term(ILP_engine *machine,
				          ILP_term_ref t) {
  dlong *p =  machine->c_interface_terms[t]; simple_deref(p);
  return get_atom_name((dlong)p);
}

static inline char *ILP_name_arity_of_functor_term(ILP_engine *machine,
					           ILP_term_ref t,
					           int* arity) {
  dlong *p, ref;
  p = machine->c_interface_terms[t]; simple_deref(p);
  ref = get_struct_value(p);
  *arity = get_funct_arity(ref);
  return get_funct_name(ref);
}

static inline module_index_type module_of_symbol(ILP_engine *machine,
						 ILP_symbol_ref symbol)
{
  if (is_atom(i_symbols(symbol)))
    return(get_atom_module(i_symbols(symbol)));
  else
    return(get_funct_module(i_symbols(symbol)));
}

static inline void lookup_symbol(ILP_engine *machine,
				 const char *name, int arity,
				 ILP_symbol_ref symbol)
{
  if (arity == 0)
    i_symbols(symbol) = make_atom(atom_lookup(USER_MOD, name));
  else
    i_symbols(symbol) = make_funct(func_lookup(USER_MOD, arity, name));
}

static inline void lookup_symbol_with_module(ILP_engine *machine,
					     const char *name, int arity,
					     module_index_type module_index,
					     ILP_symbol_ref symbol)
{
  if (arity == 0)
    i_symbols(symbol) = make_atom(atom_lookup(module_index, name));
  else
    i_symbols(symbol) = make_funct(func_lookup(module_index, arity, name));
}


static inline boolean ILP_compare_symbol(ILP_engine *machine,
					 ILP_symbol_ref symbol1,
					 ILP_symbol_ref symbol2)
{
  return (i_symbols(symbol1) == i_symbols(symbol2));
}

static inline void symbol_of_module_index(ILP_engine *machine,
					  module_index_type nr,
					  ILP_symbol_ref symbol)
{
  i_symbols(symbol) = make_atom(modtab[nr]);
}

static inline module_index_type lookup_module_index(ILP_engine *machine,
						    ILP_symbol_ref symbol)
{
  if (!is_atom(symbol)) return (0);
  else return(mod_lookup(get_atom_index(i_symbols(symbol))));
}

static inline boolean ILP_is_integer(ILP_engine *machine, int i) {
  dlong *p = (dlong *)args(i); simple_deref(p);
  if (is_integer(p)) return (1); else return (0);
}

static inline boolean ILP_is_atom(ILP_engine *machine, int i) {
  dlong *p = (dlong *)args(i); simple_deref(p);
  return (is_atom(p));
}

static inline boolean ILP_is_double(ILP_engine *machine, int i) {
  dlong *p = (dlong *)args(i); simple_deref(p);
  return (is_real(p));
}

static inline boolean ILP_is_list(ILP_engine *machine, int i) {
  dlong *p = (dlong *)args(i); simple_deref(p);
  return (is_list(p));
}

static inline boolean ILP_is_functor(ILP_engine *machine, int i) {
  dlong *p = (dlong *)args(i); simple_deref(p);
  return (is_struct(p));
}

static inline boolean ILP_is_var(ILP_engine *machine, int i) {
  dlong *p = (dlong *)args(i); simple_deref(p);
  if (is_ref(p)) return (1);
  else return (0);
}

static inline boolean ILP_is_integer_term(ILP_engine *machine, ILP_term_ref t) {
  dlong *p =  machine->c_interface_terms[t]; simple_deref(p);
  if (is_integer(p)) return (1); else return (0);
}

static inline boolean ILP_is_atom_term(ILP_engine *machine, ILP_term_ref t) {
  dlong *p =  machine->c_interface_terms[t]; simple_deref(p);
  return (is_atom(p));
}

static inline boolean ILP_is_double_term(ILP_engine *machine, ILP_term_ref t) {
  dlong *p = machine->c_interface_terms[t]; simple_deref(p);
  return (is_real(p));
}

static inline boolean ILP_is_list_term(ILP_engine *machine, ILP_term_ref t) {
  dlong *p = machine->c_interface_terms[t]; simple_deref(p);
  return (is_list(p));
}

static inline boolean ILP_is_functor_term(ILP_engine *machine, ILP_term_ref t) {
  dlong *p = machine->c_interface_terms[t]; simple_deref(p);
  return (is_struct(p));
}

static inline boolean ILP_is_var_term(ILP_engine *machine, ILP_term_ref t) {
  dlong *p = machine->c_interface_terms[t]; simple_deref(p);
  if (is_ref(p)) return (1);
  else return (0);
}


static inline boolean ILP_compare_symbol_to_arg(ILP_engine *machine,
						int i,
						ILP_symbol_ref symbol) {
  dlong *p;
  p =  (dlong *)args(i); simple_deref(p);
  if (is_atom(p)) return (i_symbols(symbol) == (dlong)p);
  else if (is_struct(p)) return (i_symbols(symbol) == get_struct_value(p));
  else fprintf(STDERR, "Term is not an atom nor a structure\n");
  return (0);
}

static inline boolean ILP_compare_symbol_to_term(ILP_engine *machine,
						 ILP_term_ref t,
						 ILP_symbol_ref symbol){

  dlong *p;
  p =  machine->c_interface_terms[t]; simple_deref(p);
  if (is_atom(p)) return (i_symbols(symbol) == (dlong)p);
  else if (is_struct(p)) return (i_symbols(symbol) == get_struct_value(p));
  else fprintf(STDERR, "Term is not an atom nor a structure\n");
  return (0);
}


#ifdef ILP_C_INT_NO_CHECK
#define check_dlong(How, What, OnError)
#else
#define check_dlong(How, What, OnError) \
      if (!How(What)) {fprintf(STDERR, "Term has invalid type!\n"); OnError;}
#endif

static inline int ILP_get_integer(ILP_engine *machine, int i) {
  dlong *p;
  dlong res;
  p =  (dlong *)args(i); simple_deref(p);
  check_dlong(is_integer, p, return(-1))
  res = get_integer(p);
  return (res);
}

static inline void ILP_get_atom(ILP_engine *machine,
				int i,
				ILP_symbol_ref symbol) {
  dlong *p = (dlong *)args(i); simple_deref(p);
  check_dlong(is_atom, p, return)
  i_symbols(symbol) = (dlong)p;
}

static inline double ILP_get_double(ILP_engine *machine, int i) {
  dlong *p, *q;
  dfloat res;
  p =  (dlong *)args(i); deref_val_refval(p,q);
  check_dlong(is_real, p, return (-1));
  res = get_real(p,q);
  return (res);
}

static inline void ILP_from_register_to_term_ref(ILP_engine *machine,
						 int from, ILP_term_ref to) {
  dlong *p = (dlong *)args(from); deref_to_cell(p);
  machine->c_interface_terms[to] = p;
}

static inline int ILP_get_integer_term(ILP_engine *machine, ILP_term_ref t) {
  dlong *p;
  dlong res;
  p =  machine->c_interface_terms[t]; simple_deref(p);
  check_dlong(is_integer, p, return (-1))
  res = get_integer(p);
  return (res);
}

static inline void ILP_get_atom_term(ILP_engine *machine,
				     ILP_term_ref t,
				     ILP_symbol_ref symbol) {
  dlong *p = machine->c_interface_terms[t]; simple_deref(p);
  check_dlong(is_atom, p, return)
  i_symbols(symbol) = (dlong)p;
}

static inline double ILP_get_double_term(ILP_engine *machine, ILP_term_ref t) {
  dlong *p, *q;
  dfloat res;
  p =  machine->c_interface_terms[t]; deref_val_refval(p,q);
  check_dlong(is_real, p, return (-1))
  res = get_real(p,q);
  return (res);
}

static inline void ILP_get_functor_term(ILP_engine *machine,
					ILP_term_ref t,
					ILP_symbol_ref symbol) {
  dlong *p = machine->c_interface_terms[t]; simple_deref(p);
  check_dlong(is_struct, p, return)
  i_symbols(symbol) = get_struct_value(p);
}

static inline void ILP_from_term_ref_to_term_ref(ILP_engine *machine,
						 ILP_term_ref from,
						 ILP_term_ref to) {
  machine->c_interface_terms[to] = machine->c_interface_terms[from];
}


static inline boolean ILP_get_arguments_list(ILP_engine *machine,
					     ILP_term_ref t,
					     ILP_term_ref head,
					     ILP_term_ref tail) {
  dlong *p = machine->c_interface_terms[t]; simple_deref(p);
  check_dlong(is_list, p, return(0));
  p = get_list_pointer(p);
  machine->c_interface_terms[head] = (dlong*)*p;
  machine->c_interface_terms[tail] = (dlong*)*(p+1);
  return (1);
}

static inline boolean ILP_get_argument_functor(ILP_engine *machine,
					       ILP_term_ref t,
					       int argnr, ILP_term_ref to) {
  dlong *p, *q, structv;

  p =  machine->c_interface_terms[t]; deref_val_refval(p,q);
  check_dlong(is_struct, p, return(0))
  structv = get_struct_value(p);
  p = get_struct_pointer_new(p,q);
  if (get_funct_arity(structv) < argnr) {
    fprintf(STDERR, "Not that many arguments in functor\n");
    return (0);
  }
  machine->c_interface_terms[to] = (dlong*) *(p+argnr);
  return (1);
}

static inline boolean ILP_unify_integer(ILP_engine *machine, int i, int value) {
  dlong *p = (dlong *)args(i); simple_deref(p);
  if (is_ref(p)) {
    trail_indep(p,bh,trreg,tr_limit, machine);
    build_integer(p,value);
    return (1);
  } else if (is_integer(p)){
    return(value == get_integer(p));
  } else return (0);
}

static inline boolean ILP_unify_atom(ILP_engine *machine, int i,
				     ILP_symbol_ref symbol) {
  dlong *p = (dlong *)args(i); simple_deref(p);
  if (is_ref(p)) {
    trail_indep(p,bh,trreg,tr_limit, machine);
    *p = i_symbols(symbol);
    return (1);
  } else if (is_atom(p)) {
    return(p == (dlong*)i_symbols(symbol));
  } else return (0);
}


static inline boolean ILP_unify_double(ILP_engine *machine, int i,
				       double value) {
  dlong *p, *q;
  p =  (dlong *)args(i); deref_val_refval(p,q);
  if (is_ref(p)) {
    trail_indep(p,bh,trreg,tr_limit, machine);
    build_real(p,value,hreg);
    return (1);
  } else if (is_real(p)) {
    return(value == get_real(p,q));
  } else return (0);
}

static inline boolean ILP_unify(ILP_engine *machine, int i, int j) {
  int res;
  int trail_len;

  trail_len = trreg - machine-> begin_trail;
  res = unify_terms_always_trail((dlong*)args(i), (dlong*)args(j), machine);
  if (!res) {
    dlong *orig = machine-> begin_trail + trail_len;
    untrail(trreg, orig);
  }
  return (res);
}

static inline boolean ILP_unify_integer_term(ILP_engine *machine,
					     ILP_term_ref t, int value) {
  dlong *p = machine->c_interface_terms[t]; simple_deref(p);
  if (is_ref(p)) {
    trail_indep(p,bh,trreg,tr_limit, machine);
    build_integer(p,value);
    return (1);
  } else if (is_integer(p)){
    return(value == get_integer(p));
  } else return (0);
}

static inline boolean ILP_unify_atom_term(ILP_engine *machine,
					  ILP_term_ref t,
					  ILP_symbol_ref symbol) {
  dlong *p = machine->c_interface_terms[t]; simple_deref(p);
  if (is_ref(p)) {
    trail_indep(p,bh,trreg,tr_limit, machine);
    *p = i_symbols(symbol);
    return (1);
  } else if (is_atom(p)) {
    return(p == (dlong*)i_symbols(symbol));
  } else return (0);
}


static inline boolean ILP_unify_double_term(ILP_engine *machine,
					    ILP_term_ref t,
					    double value) {
  dlong *p, *q;

  p =  machine->c_interface_terms[t]; deref_val_refval(p,q);
  if (is_ref(p)) {
    trail_indep(p,bh,trreg,tr_limit, machine);
    build_real(p,value,hreg);
    return (1);
  } else if (is_real(p)) {
    return(value == get_real(p,q));
  } else return (0);
}

static inline boolean ILP_unify_arg_term(ILP_engine *machine,
					 ILP_term_ref t1,
					 int argnr,
					 ILP_term_ref t2) {
  dlong *p, *q;
  dlong symb;
  int arity, res, trail_len;

  p =  machine->c_interface_terms[t1]; deref_val_refval(p,q);
  check_dlong(is_struct, p, return(0));
  symb = get_struct_value(p);
  p = get_struct_pointer_new(p,q);
  arity = get_funct_arity(symb);
  if ((argnr < 1) || (argnr > arity)) {
     fprintf(stderr, "Unifying an non-existing argument of a functor ");
     fprintf(stderr, "arity=%d, argnr=%d, functor_name=%s\n",arity, argnr,
	                                                     get_funct_name(symb));
     return (0);
  }
  p = p + argnr;
  trail_len = trreg - machine-> begin_trail;
  res = unify_terms_always_trail(p, machine->c_interface_terms[t2], machine);
  if (!res) {
    dlong *orig = machine-> begin_trail + trail_len;
    untrail(trreg, orig);
  }
  return (res);
}

static inline boolean ILP_set_arg_term(ILP_engine *machine,
                                       ILP_term_ref t1,
                                       int argnr,
                                       ILP_term_ref t2) {
  dlong *p1, *p2, *q1, *q2;
  p1 = machine->c_interface_terms[t1]; deref_val_refval(p1, q1);
  p2 = machine->c_interface_terms[t2]; deref_val_refval(p2, q2);

  if (!is_atom_or_int(p2)) adapt_freeze_hreg(machine);

  if (is_struct(p1)) {
    q1 = get_struct_pointer_new(p1,q1) + argnr;
  } else if (is_list(p1)) {
    q1 = get_list_pointer(p1) + argnr-1;
  } else {
    fprintf(STDERR, "Illegal argument for setarg/3\n");
    return 0;
  }
  *q1 = (dlong) ref_or_value(p2,q2);
  return (1);
}

static inline boolean ILP_unify_term(ILP_engine *machine, int i,
				     ILP_term_ref t) {
  int res;
  int trail_len;

  trail_len = trreg - machine-> begin_trail;
  res = unify_terms_always_trail(machine->c_interface_terms[t], (dlong*)args(i), machine);
  if (!res) {
    dlong *orig = machine-> begin_trail + trail_len;
    untrail(trreg, orig);
  }
  return (res);
}

static inline boolean ILP_unify_term_term(ILP_engine *machine,
					  ILP_term_ref t1, ILP_term_ref t2) {
  int res;
  int trail_len;
  trail_len = trreg - machine-> begin_trail;

  res = unify_terms_always_trail(machine->c_interface_terms[t1], machine->c_interface_terms[t2], machine);
  if (!res) {
    dlong *orig = machine-> begin_trail + trail_len;
    untrail(trreg, orig);
  }
  return (res);
}

static inline boolean ILP_unifiable_term_term(ILP_engine *machine,
					 ILP_term_ref t1, ILP_term_ref t2) {
  int res;
  int trail_len;

  trail_len = trreg - machine-> begin_trail;
  res = unify_terms_always_trail(machine->c_interface_terms[t1], machine->c_interface_terms[t2], machine);
  {
    dlong *orig = machine-> begin_trail + trail_len;
    untrail(trreg, orig);
  }
  return (res);
}

static inline int ILP_compare_term_term(ILP_engine *machine,
					ILP_term_ref t1, ILP_term_ref t2) {
  int res = compare_terms(machine->c_interface_terms[t1],
		          machine->c_interface_terms[t2]);
  return (res);
}


static inline void ILP_setarg(ILP_engine *machine, int i,
			      ILP_term_ref t1, ILP_term_ref t2) {
  dlong *p, *q;
  dlong structv;
  p = machine->c_interface_terms[t1]; deref_val_refval(p,q);
  check_dlong(is_struct, p, return)
  q = get_struct_pointer_new(p,q);
  structv = get_struct_value(p);
  if ((i < 1) || (i > get_funct_arity(structv))){
    fprintf(STDERR, "invalid call to ILP_setarg, invalid argument\n");
    return;
  }
  q = q + i;
  trail_indep(q, bh, trreg,tr_limit,machine);
  *q = (dlong) machine->c_interface_terms[t2];
}

static inline void ILP_put_integer(ILP_engine *machine, int reg, int value) {
  build_integer(&(args(reg)),value);
}

static inline void ILP_put_atom(ILP_engine *machine, int reg,
				ILP_symbol_ref symbol) {
  args(reg) = i_symbols(symbol);
}

static inline void ILP_put_double(ILP_engine *machine, int reg, double f) {
  build_real(&(args(reg)),f,hreg);
}

static inline void ILP_put_variable(ILP_engine *machine, int reg) {
  args(reg) = *hreg = (dlong)hreg;
  hreg++;
}

static inline void ILP_put_term(ILP_engine *machine, int reg, ILP_term_ref t) {
   args(reg) = (dlong)machine->c_interface_terms[t];
}

static inline void ILP_put_integer_term(ILP_engine *machine,
					ILP_term_ref t,
					int value) {
  build_integer((dlong*)&(machine->c_interface_terms[t]), value);
}

static inline void ILP_put_atom_term(ILP_engine *machine, ILP_term_ref t,
				     ILP_symbol_ref symbol) {
   machine->c_interface_terms[t] = (dlong*) i_symbols(symbol);
}

static inline void ILP_put_double_term(ILP_engine *machine,
				       ILP_term_ref t, double f) {
  build_real((dlong*)&(machine->c_interface_terms[t]),f,hreg);
}

static inline void ILP_put_list_term(ILP_engine *machine,
				     ILP_term_ref t,
				     ILP_term_ref head, ILP_term_ref tail) {
  *hreg = (dlong) machine->c_interface_terms[head];
  hreg++;
  *hreg = (dlong) machine->c_interface_terms[tail];
  hreg++;
  machine->c_interface_terms[t] = (dlong*)make_list(hreg-2);
}

static inline void ILP_add_list_term(ILP_engine *machine,
				     ILP_term_ref head, ILP_term_ref tail) {
  dlong *ntail, *list;
  *hreg = (dlong) machine->c_interface_terms[head];
  hreg++;
  ntail = hreg;
  *hreg = (dlong)hreg;
  hreg++;
  list = (dlong*)make_list(hreg-2);
  unify_terms_always_trail(machine->c_interface_terms[tail], list, machine);
  machine->c_interface_terms[tail] = ntail;
}

static inline void ILP_put_functor_term(ILP_engine *machine,
					ILP_term_ref t,
					ILP_symbol_ref symbol) {
  int i;

  machine->c_interface_terms[t] = (dlong*) make_struct_p(hreg);
  *hreg = i_symbols(symbol);
  hreg++;
  i = get_funct_arity(i_symbols(symbol));
  while (i) {
    *hreg = (dlong)hreg;
    hreg++;
    i--;
  }
}


static inline void ILP_put_variable_term(ILP_engine *machine,
					 ILP_term_ref t) {
  (machine->c_interface_terms[t]) = *(dlong**)hreg = hreg;
  hreg++;
}

static inline boolean ILP_call_predicate(ILP_engine *machine,
					 ILP_symbol_ref symbol) {
  dlong *old_cp = breg;
  dlong predicate_index;

  if (is_atom(i_symbols(symbol)))
    predicate_index = atom_to_func(get_atom_index(i_symbols(symbol)));
  else
    predicate_index = get_funct_symb(i_symbols(symbol));

  machine->P = entries[predicate_index];
  /* make a dummy choicepoint:
     if the call fails, the choicepoint will be gone */
  breg += CP_FIX_LEN;
  ((CP_P) breg)->failurecont = (codep)entries[atom_to_func(used_atoms.halt_0)];
  ((CP_P) breg)->h = hreg;
  ((CP_P) breg)->tr = trreg;
  ((CP_P) breg)->e = ereg;
  ((CP_P) breg)->b = old_cp;
  ((CP_P) breg)->cp = contp;
  ((CP_P) breg)->tops = tos;

  /* to get back if the call succeeds: */

  contp = entries[atom_to_func(used_atoms.halt_0)];

  execute_machine(machine, 0);
  if (machine ->B != old_cp) {
    machine ->B = old_cp;
    machine->BH = ((CP_P) breg)->h;
    return(1);
  } else
    return (0);
}


static inline int ILP_get_file_number(ILP_engine *machine, ILP_term_ref t) {
  dlong *p, *q;
  dlong structv;
  p = machine->c_interface_terms[t]; deref_val_refval(p,q);
  if (!is_struct(p)) return (-1);
  structv = get_struct_value(p);
  p = get_struct_pointer_new(p,q);
  if (get_funct_arity(structv) != 1) return (-1);
  p = *(dlong**)(p+1); simple_deref(p);
  if (!is_integer(p)) return (-1);
  return (get_integer(p));
}

static inline char ILP_getc(ILP_engine *machine, ILP_term_ref t) {
  /* t  should be fd(i) */
  int i = ILP_get_file_number(machine, t);
  char c;
  if (i == -1 ) return (-1);
  c = advance_char_ext(&machine->openfiles[i]);
  if (c == 0) c = -1;
  return (c);
}

static inline char ILP_eof(ILP_engine *machine, ILP_term_ref t) {
  /* t  should be fd(i) */
  int i = ILP_get_file_number(machine, t);
  char c;
  if (i == -1 ) return (-1);
  c = lookahead_char2_ext(&machine->openfiles[i]);
  if (c == 0) return (1);
  else return (0);
}

static inline int ILP_putc(ILP_engine *machine, ILP_term_ref t, char c) {
  int i = ILP_get_file_number(machine, t);
  if (i == -1 ) return (-1);
  fputc(c,machine->openfiles[i].fp);
  return (1);
}

/*

typedef ILP_symbol_ref (*ilp_init_gc2C_type) (ILP_engine*, long major_gc);
typedef void (*ilp_gc2C_marked_type)(ILP_engine *machine,
                                     ILP_term_ref marked_functor);
typedef void (*ilp_gc2C_end_mark_type)();

*/

static inline void ilp_register_C2gc(ilp_init_gc2C_type init,
				     ilp_gc2C_marked_type mark,
				     ilp_gc2C_end_mark_type end){
  ilp_init_gc2C = init;
  ilp_gc2C_marked = mark;
  ilp_gc2C_end_mark = end;
}


static inline void ILP_heap_gc(ILP_engine* machine) {
  deal_with_heap_overflow(machine,2,0);
}

#include "extensions/dl.h"

#ifdef __cplusplus
}
#endif

