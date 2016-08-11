#ifndef VARSTRUCT_INC
#define VARSTRUCT_INC

#include "../../gapic/c_prolog.h"

#define MAX_NLP_ARITY 11
void init_varstruct();
int varstruct_split(GAPIC_term_ref term,
		    GAPIC_int *varstruct,
		    GAPIC_term_ref ground,
		    GAPIC_term_ref vars);
void varstruct_combine(int varstruct,
		       GAPIC_term_ref interm,
		       GAPIC_term_ref outterm,
		       GAPIC_term_ref fullterm);
#endif
