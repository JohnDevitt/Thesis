#ifdef  __cplusplus
extern "C" {
#endif
#ifdef NEVER
}     
#endif

#define DONT_NEED_OPCODES

#include "better_toinclude.h"

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

static inline ILP_term_ref ILP_new_term_ref(ILP_engine *machine) 
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
  if (machine->c_interface_max_symbols_used >= 
      machine->c_interface_symbols_allocated) {
    machine->c_interface_symbols_allocated =machine->c_interface_symbols_allocated *2; 
    machine->c_interface_symbols = 
      (dlong *) realloc(machine->c_interface_symbols,
			 sizeof(dlong *)*machine->c_interface_symbols_allocated);
  }
  machine->c_interface_terms[machine->c_interface_max_symbols_used]=(dlong*)nil_atom;
  machine->c_interface_max_symbols_used++;
  return (machine->c_interface_max_symbols_used-1);
}

static inline void ILP_delete_symbol_ref(ILP_engine *machine, ILP_symbol_ref symbol)
{
}

static inline void ILP_copy_symbol(ILP_engine *machine, 
				   ILP_symbol_ref from, ILP_symbol_ref to)
{
  i_symbols(to) = i_symbols(from); 
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
					     char *name, int arity, 
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

static inline boolean ILP_is_integer(ILP_engine *machine, int i)
{
  dlong *p, *q;
  p =  (dlong *)args(i); deref(p,q);
  if (is_integer(p)) return (1); else return (0); 
} 

static inline boolean ILP_is_atom(ILP_engine *machine, int i)
{
  dlong *p, *q;
  p =  (dlong *)args(i); deref(p,q);
  return (is_atom(p));
} 

static inline boolean ILP_is_double(ILP_engine *machine, int i)
{
  dlong *p, *q;
  p =  (dlong *)args(i); deref(p,q);
  return (is_real(p));
}
 
static inline boolean ILP_is_list(ILP_engine *machine, int i)
{
  dlong *p, *q;
  p =  (dlong *)args(i); deref(p,q);
  return (is_list(p));
} 

static inline boolean ILP_is_functor(ILP_engine *machine, int i)
{
  dlong *p, *q;
  p =  (dlong *)args(i); deref(p,q);
  return (is_struct(p));
} 

static inline boolean ILP_is_var(ILP_engine *machine, int i) 
{
  dlong *p, *q;
  p =  (dlong *)args(i); deref(p,q);
  if (is_ref(p)) return (1);
  else return (0);
}

static inline boolean ILP_is_integer_term(ILP_engine *machine, ILP_term_ref t)
{
  dlong *p, *q;
  p =  machine->c_interface_terms[t]; deref(p,q);
  if (is_integer(p)) return (1); else return (0); 
} 

static inline boolean ILP_is_atom_term(ILP_engine *machine, ILP_term_ref t)
{
  dlong *p, *q;
  p =  machine->c_interface_terms[t]; deref(p,q);
  return (is_atom(p));
} 

static inline boolean ILP_is_double_term(ILP_engine *machine, ILP_term_ref t)
{
  dlong *p, *q;
  p =  machine->c_interface_terms[t]; deref(p,q);
  return (is_real(p));
}
 
static inline boolean ILP_is_list_term(ILP_engine *machine, ILP_term_ref t)
{
  dlong *p, *q;
  p =  machine->c_interface_terms[t]; deref(p,q);
  return (is_list(p));
} 

static inline boolean ILP_is_functor_term(ILP_engine *machine, ILP_term_ref t)
{
  dlong *p, *q;
  p =  machine->c_interface_terms[t]; deref(p,q);
  return (is_struct(p));
} 

static inline boolean ILP_is_var_term(ILP_engine *machine, ILP_term_ref t) 
{
  dlong *p, *q;
  p =  machine->c_interface_terms[t]; deref(p,q);
  if (is_ref(p)) return (1);
  else return (0);
}

static inline int ILP_get_integer(ILP_engine *machine, int i)
{
  dlong *p, *q;
  dlong res;
  p =  (dlong *)args(i); deref(p,q);
  if (is_integer(p)) {
    res = get_integer(p);
    return (res); 
  } else {
      fprintf(STDERR, "Term is not an integer\n"); 
      return (-1);
  }
} 

static inline void ILP_get_atom(ILP_engine *machine, 
				int i,
				ILP_symbol_ref symbol)
{
  dlong *p, *q;
  p =  (dlong *)args(i); deref(p,q);
  if (is_atom(p)) 
    i_symbols(symbol) = (dlong)p;
  else 
    fprintf(STDERR, "Term is not an atom\n"); 
} 

static inline double ILP_get_double(ILP_engine *machine, int i)
{
  dlong *p, *q;
  dfloat res;
  p =  (dlong *)args(i); deref(p,q);
  if (is_real(p)) {
    res = get_real(p);
    return (res); 
  } else {
    fprintf(STDERR, "Term is not a double\n"); 
    return (-1);
  }
} 

static inline void ILP_from_register_to_term_ref(ILP_engine *machine, 
						 int from, ILP_term_ref to)
{
  dlong *p,*q;
  p =  (dlong *)args(from); deref(p,q);
  machine->c_interface_terms[to] = p;
}

static inline int ILP_get_integer_term(ILP_engine *machine, ILP_term_ref t)
{
  dlong *p, *q;
  dlong res;
  p =  machine->c_interface_terms[t]; deref(p,q);
  if (is_integer(p)) {
    res = get_integer(p);
    return (res); 
  } else {
      fprintf(STDERR, "Term is not an integer\n"); 
      return (-1);
  }
} 

static inline void ILP_get_atom_term(ILP_engine *machine, 
				     ILP_term_ref t,
				     ILP_symbol_ref symbol)
{
  dlong *p, *q;

  p =  machine->c_interface_terms[t]; deref(p,q);
  if (is_atom(p)) 
    i_symbols(symbol) = (dlong)p;
  else 
    fprintf(STDERR, "Term is not an atom\n"); 
} 

static inline double ILP_get_double_term(ILP_engine *machine, ILP_term_ref t)
{
  dlong *p, *q;
  dfloat res;

  p =  machine->c_interface_terms[t]; deref(p,q);
  if (is_real(p)) {
    res = get_real(p);
    return (res); 
  } else {
    fprintf(STDERR, "Term is not a double\n"); 
    return (-1);
  }
} 
static inline void ILP_get_functor_term(ILP_engine *machine, 
					ILP_term_ref t,
					ILP_symbol_ref symbol) {
  dlong *p, *q;
  char *name;
  
  p = machine->c_interface_terms[t]; deref(p,q);
  if (is_struct(p)) 
    i_symbols(symbol) = *(get_struct_pointer(p));
  else 
    fprintf(STDERR, "Term is not as structure\n"); 
}

static inline void ILP_from_term_ref_to_term_ref(ILP_engine *machine, 
						 ILP_term_ref from, 
						 ILP_term_ref to)
{
  machine->c_interface_terms[to] = machine->c_interface_terms[from]; 
}


static inline boolean ILP_get_arguments_list(ILP_engine *machine, 
					     ILP_term_ref t, 
					     ILP_term_ref head,
					     ILP_term_ref tail) 
{
  dlong *p, *q;
  char *name;

  p = machine->c_interface_terms[t]; deref(p,q);
  if (is_list(p)) {
    p = get_list_pointer(p);
    machine->c_interface_terms[head] = (dlong*)*p;
    machine->c_interface_terms[tail] = (dlong*)*(p+1); 
    return (1);
  } else {
    fprintf(STDERR, "Term is not a list\n"); 
    return (0);
  }
}

static inline boolean ILP_get_argument_functor(ILP_engine *machine, 
					       ILP_term_ref t, 
					       int argnr, ILP_term_ref to) 
{
  dlong *p, *q;
  char *name;

  p =  machine->c_interface_terms[t]; deref(p,q);
  if (is_struct(p)) {
    p = get_struct_pointer(p);
    if (get_funct_arity(*p) < argnr) { 
      fprintf(STDERR, "Not that many arguments in functor\n"); 
      return (0);
    }
    machine->c_interface_terms[to] = (dlong*) *(p+argnr);
    return (1);
  } else {
    fprintf(STDERR, "Term is not a functor\n"); 
    return (0);
  }
}

static inline boolean ILP_unify_integer(ILP_engine *machine, int i, int value)
{
  dlong *p, *q;

  p =  (dlong *)args(i); deref(p,q);
  if (is_ref(p)) {
    trail_indep(p,bh,trreg,tr_limit, machine);
    build_integer(p,value);
    return (1);
  } else if (is_integer(p)){
    return(value == get_integer(p));
  } else return (0);
} 

static inline boolean ILP_unify_atom(ILP_engine *machine, int i, 
				     ILP_symbol_ref symbol)
{
  dlong *p, *q;

  p =  (dlong *)args(i); deref(p,q);
  if (is_ref(p)) {
    trail_indep(p,bh,trreg,tr_limit, machine);
    *p = i_symbols(symbol);
  } else if (is_atom(p)) {
    return(p == (dlong*)i_symbols(symbol));
  } else return (0); 
} 


static inline boolean ILP_unify_double(ILP_engine *machine, int i, 
				       double value)
{
  dlong *p, *q;
  p =  (dlong *)args(i); deref(p,q);
  if (is_ref(p)) {
    trail_indep(p,bh,trreg,tr_limit, machine);
    build_real(p,value,hreg);
    return (1);
  } else if (is_real(p)) {
    return(value == get_real(p));
  } else return (0);
} 

static inline boolean ILP_unify(ILP_engine *machine, int i, int j) 
{
  int res; 
  int trail_len;

  trail_len = trreg - machine-> begin_trail;
  res = unify_terms((dlong*)args(i), (dlong*)args(j), machine);
  if (!res) {
    dlong *orig = machine-> begin_trail + trail_len; 
    untrail(trreg, orig);
  }
  return (res);
}

static inline boolean ILP_unify_integer_term(ILP_engine *machine, 
					     ILP_term_ref t, int value)
{
  dlong *p, *q;

  p =  machine->c_interface_terms[t]; deref(p,q);
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
					  ILP_symbol_ref symbol)
{
  dlong *p, *q;

  p =  machine->c_interface_terms[t]; deref(p,q);
  if (is_ref(p)) {
    trail_indep(p,bh,trreg,tr_limit, machine);
    *p = i_symbols(symbol);
  } else if (is_atom(p)) {
    return(p == (dlong*)i_symbols(symbol));
  } else return (0); 
} 


static inline boolean ILP_unify_double_term(ILP_engine *machine, 
					    ILP_term_ref t, 
					    double value)
{
  dlong *p, *q;

  p =  machine->c_interface_terms[t]; deref(p,q);
  if (is_ref(p)) {
    trail_indep(p,bh,trreg,tr_limit, machine);
    build_real(p,value,hreg);
    return (1);
  } else if (is_real(p)) {
    return(value == get_real(p));
  } else return (0);
} 

static inline boolean ILP_unify_arg_term(ILP_engine *machine, 
					 ILP_term_ref t1,
					 int argnr,
					 ILP_term_ref t2)
{
  dlong *p, *q;
  int arity, res, trail_len;

  p =  machine->c_interface_terms[t1]; deref(p,q);
  if (!is_struct(p)) {
    fprintf(stderr, "Unifying the argument of something not a functor\n");
    return (0);
  }
  p = get_struct_pointer(p);
  arity = get_funct_arity((dlong)p);
  if ((argnr < 1) || (argnr >= arity)) {
     fprintf(stderr, "Unifying an non-existing argument of a functor\n");
     return (0);
  }
  p = p + argnr;
  trail_len = trreg - machine-> begin_trail;
  res = unify_terms(p, machine->c_interface_terms[t2], machine);
  if (!res) {
    dlong *orig = machine-> begin_trail + trail_len; 
    untrail(trreg, orig);
  }
  return (res);
} 



static inline boolean ILP_unify_term(ILP_engine *machine, int i, 
				     ILP_term_ref t) 
{
  int res; 
  int trail_len;

  trail_len = trreg - machine-> begin_trail;
  res = unify_terms(machine->c_interface_terms[t], (dlong*)args(i), machine);
  if (!res) {
    dlong *orig = machine-> begin_trail + trail_len; 
    untrail(trreg, orig);
  }
  return (res);
}

static inline boolean ILP_unify_term_term(ILP_engine *machine,
					  ILP_term_ref t1, ILP_term_ref t2) 
{
  int res; 
  int trail_len;

  trail_len = trreg - machine-> begin_trail;
  res = unify_terms(machine->c_interface_terms[t1], machine->c_interface_terms[t1], machine);
  if (!res) {
    dlong *orig = machine-> begin_trail + trail_len; 
    untrail(trreg, orig);
  }
  return (res);
}

static inline void ILP_put_integer(ILP_engine *machine, int reg, int value)
{
  build_integer(&(args(reg)),value); 
}

static inline void ILP_put_atom(ILP_engine *machine, int reg, 
				ILP_symbol_ref symbol)
{
  args(reg) = i_symbols(symbol);
}

static inline void ILP_put_double(ILP_engine *machine, int reg, double f)
{
  build_real(&(args(reg)),f,hreg);
}

static inline void ILP_put_variable(ILP_engine *machine, int reg) 
{
  args(reg) = *hreg = (dlong)hreg;
  hreg++;
}

static inline void ILP_put_integer_term(ILP_engine *machine, 
					ILP_term_ref t, 
					int value)
{
  build_integer((dlong*)&(machine->c_interface_terms[t]), value); 
}

static inline void ILP_put_atom_term(ILP_engine *machine, ILP_term_ref t, 
				     ILP_symbol_ref symbol)
{
   machine->c_interface_terms[t] = (dlong*) i_symbols(symbol);
}

static inline void ILP_put_double_term(ILP_engine *machine, 
				       ILP_term_ref t, double f)
{
  build_real((dlong*)&(machine->c_interface_terms[t]),f,hreg);
}

static inline void ILP_put_list_term(ILP_engine *machine,
				     ILP_term_ref t, 
				     ILP_term_ref head, ILP_term_ref tail) 
{
  machine->c_interface_terms[t] = (dlong*)make_list(hreg);
  *hreg = (dlong) machine->c_interface_terms[head]; 
  hreg++;
  *hreg = (dlong) machine->c_interface_terms[tail];
  hreg++;
}

static inline void ILP_put_functor_term(ILP_engine *machine,
					ILP_term_ref t, 
					ILP_symbol_ref symbol) 
{
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
					 ILP_term_ref t) 
{
  (machine->c_interface_terms[t]) = *(dlong**)hreg = hreg;
  hreg++;
}

static inline boolean ILP_call_predicate(ILP_engine *machine, 
					 ILP_symbol_ref symbol) 
{
  dlong *old_cp = breg;
  dlong predicate_index;

  if (is_atom(i_symbols(symbol))) 
    predicate_index = atom_to_func(get_atom_index(i_symbols(symbol)));
  else 
    predicate_index = get_funct_symb(i_symbols(symbol));

  machine->P = entries[predicate_index];
      
              
  breg += CP_FIX_LEN;
  ((CP_P) breg)->failurecont = (codep)entries[atom_to_func(used_atoms.halt_0)];       
  ((CP_P) breg)->h = hreg;                       
  ((CP_P) breg)->tr = trreg;                     
  ((CP_P) breg)->e = ereg;                       
  ((CP_P) breg)->b = old_cp;                    
  ((CP_P) breg)->cp = contp;                     
  ((CP_P) breg)->tops = tos;                     

          

  contp = entries[atom_to_func(used_atoms.halt_0)];
          
  execute_machine(machine, 0);
  if (machine ->B != old_cp) {
    machine ->B = old_cp;
    machine->BH = ((CP_P) breg)->h;
    return(1);
  } else
    return (0);
}

#ifdef __cplusplus
}
#endif
