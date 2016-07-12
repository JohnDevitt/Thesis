/* Include the plugin interface files */
#include "gcc/ic-interface.h"
#include "ic-defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NEW_MAX_INLINE_INSNS_RECURSIVE 600L

/* Callback to handle the "pass_execution" event.  */
static void
executed_pass (void)
{
  const char *pass_name = (const char *) get_feature ("pass_name");
  const char *func_name = (const char *) get_feature ("function_name");
  int *all_passes = (int *) get_event_parameter ("all_passes");

  fprintf (stderr, "Currently running pass '%s' on function '%s'"
	   " (in pass chain 'all_passes': %s)\n",
	   (pass_name ? pass_name : "<unknown>"),
	   (func_name ? func_name : "<unknown>"),
	   ((all_passes != NULL && *all_passes != 0) ? "YES" : "NO"));
}


/* Callback to handle the "gate_override" event.  */

static void
gate_override (void)
{
  int *gate_value = (int *) get_event_parameter ("gate_status");
  const char *pass = (const char *) get_feature ("current_pass_name");
  const char *function = (const char *) get_feature ("current_function_name");
  fprintf (stderr,
	   "Gate override: gate for pass '%s' on function '%s' is %s.\n",
	   pass, function ? function : "<no crnt function>",
	   *gate_value ? "OPEN" : "CLOSED");
}

/* Initialize the plugin: set up callbacks, report settings,
   alter compiler configuration...  */

void start (void)
{
  const char **list_of_features, **next_feature;
  const char *option_value;
  const char **event_names, **crnt_evt;
  int i, value;
  int max_inline_insns_recursive;

  /* Register the per-pass callback.  */
  register_plugin_event ("pass_execution", &executed_pass);

  /* Register a gate override callback.  */
  register_plugin_event ("gate_override", &gate_override);

  /* Print the names of all registered events.  */
  event_names = (const char **) list_plugin_events ();

  fprintf (ICI_VERBOSE_OUT, "Registered events: ");
  if (*event_names == NULL)
    fprintf (ICI_VERBOSE_OUT, "<none>.\n");
  else
    {
      /* Print a comma-separated list: all-but-last elt followed by a comma.  */
      for (crnt_evt = event_names; *(crnt_evt + 1); crnt_evt++)
	fprintf (ICI_VERBOSE_OUT, "%s, ", *crnt_evt);
      
      /* Last element: full-stop and newline.  */
      fprintf (ICI_VERBOSE_OUT, "%s.\n", *crnt_evt);
    }
  free (event_names);	 /* Cleanup...  This instance of the name list
			    will not be used anymore.  */ 

  /* List the names of all available features.  */
  list_of_features = list_features ();

  fprintf (ICI_VERBOSE_OUT, "Here's the list of currently registered features:\n");
  for (i = 0, next_feature = list_of_features;
       *next_feature;
       next_feature++, i++)
    fprintf (ICI_VERBOSE_OUT, "at position #%2d: '%s'\n", i, *next_feature);
  free (list_of_features);	/* clean up */

  /* Get some target-related options through the feature mechanism.  */
  option_value = (const char *) get_subfeature ("compiler_flags", "march=");
  fprintf (ICI_VERBOSE_OUT, "Option '-march=': value = '%s'\n",
	   (option_value ? option_value : "<unset>"));

  option_value = (const char *) get_subfeature ("compiler_flags", "mcpu=");
  fprintf (ICI_VERBOSE_OUT, "Option '-mcpu=': value = '%s'\n",
	   (option_value ? option_value : "<unset>"));

  option_value = (const char *) get_subfeature ("compiler_flags", "mtune=");
  fprintf (ICI_VERBOSE_OUT, "Option '-mtune=': value = '%s'\n",
	   (option_value ? option_value : "<unset>"));

  /* Override a compiler parameter, check the outcome.
     On 64-bit architectures pointers must be first converted to long int,
     then to int, so do a two-step conversion...  */
  max_inline_insns_recursive =
    (int) (long int) get_subfeature ("compiler_params",
				      "max-inline-insns-recursive");
  fprintf (ICI_VERBOSE_OUT,
	   "Original value of param 'max-inline-insns-recursive' = %d\n",
	   max_inline_insns_recursive);
  set_subfeature ("compiler_params", "max-inline-insns-recursive",
		  (void *) NEW_MAX_INLINE_INSNS_RECURSIVE);
  value = (int) (long int)
    get_subfeature ("compiler_params", "max-inline-insns-recursive");

  if (value != NEW_MAX_INLINE_INSNS_RECURSIVE) {
      fprintf (ICI_VERBOSE_OUT, "Dumb plugin: Did not manage to modify compiler "
	       "parameter 'max-inline-insns-recursive'");
      assert (value == NEW_MAX_INLINE_INSNS_RECURSIVE);
    }
  else
    fprintf (ICI_VERBOSE_OUT,
	     "Parameter 'max-inline-insns-recursive' set correctly to %d\n",
	     value);
}

void stop (void)
{
}
