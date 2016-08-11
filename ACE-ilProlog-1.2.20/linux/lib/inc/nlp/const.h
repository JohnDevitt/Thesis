#ifndef NLP_CONST_INC
#define NLP_CONST_INC

#include "../../gapic/c_prolog.h"

typedef enum {NLP_HIDDEN_PLACE,NLP_INPUT_PLACE,NLP_OUTPUT_PLACE} NlpPlace;

extern GAPIC_atom nlp_hidden_atom;
extern GAPIC_atom nlp_input_atom;
extern GAPIC_atom nlp_output_atom;
extern GAPIC_atom nlp_unique_mode_atom;
extern GAPIC_functor nlp_combfunc_functor;

#define NLP_UNIQUE_MODE 1

#endif

