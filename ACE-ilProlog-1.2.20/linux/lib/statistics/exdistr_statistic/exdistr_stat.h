
#ifndef EXDISTR_STAT_INC
#define EXDISTR_STAT_INC

#include "target_value.h"
#include "stored_attribute.h"
#include "statistic.h"
#include "stopcrit.h"
#include "heuristic.h"
#include "modelinfo.h"
#include "classification.h"
#include "regression.h"
#include "packsinfo.h"
#include "nsplit.h"

#define RUN_MODE_PLAIN	    0
#define RUN_MODE_EVALUATE   1
#define RUN_MODE_SPLIT      2

extern GAPIC_int g_run_mode;
extern unsigned long g_stat_update_count[];
extern unsigned long g_stat_add_count[];

G_SO extern GAPIC_atom *g_atom_none;
G_SO extern GAPIC_atom *g_global_atom;
G_SO extern GAPIC_functor *g_global_functor;
extern GAPIC_float g_heur_total;

int getModelIndex(GAPIC_term_ref model);

#endif
