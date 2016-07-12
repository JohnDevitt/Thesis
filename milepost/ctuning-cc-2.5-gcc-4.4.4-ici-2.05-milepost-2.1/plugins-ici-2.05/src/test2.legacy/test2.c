/* ICI-specific includes */
#include "ic-legacy.h"
#include "ic-defs.h"
#include "gcc/ic-interface.h"

/* System includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NEW_MAX_INLINE_INSNS_RECURSIVE 600L

static FILE *file = NULL;

void executed_pass (void)
{
  const char *pass_name;
  char *func_name, *func_fname, *decl_fname;
  const char **names;
  long int func_start, func_end, decl_line;
  int i;
  char output_file[256];
  static char *prev_func_name = NULL;
  static int line = 0;
  const int *all_passes;

  /* Iterate through passes (obtained as features)
   * to save original GCC order */

  /* Are we processing pass chain "all_passes"?  */
  all_passes = ici_get_parameter ("all_passes");

  /* Get names of current pass and current function.  */
  pass_name = (const char *) ici_get_feature ("pass_name");      
  func_name = (char *) ici_get_feature ("function_name");

  if (getenv (ICI_PLUGIN_VERBOSE) != NULL)
    {
      fprintf (ICI_VERBOSE_OUT,
	       "Running callback 'executed_pass' ("
	       "pass name: '%s', in chain 'all_passes': %s)\n",
	       (pass_name ? pass_name : "<unknown>"),
	       ((!all_passes || *all_passes != 1) ? "NO" : "YES"));
      /* print the list of registered events */
      fprintf (ICI_VERBOSE_OUT, "Registered events: ");
      names = ici_list_events ();
      if (*names == NULL)
	fprintf (stderr, "<none>\n");
      else
	{
	  for (i=0; *(names+1); i++, names++)
	    fprintf (ICI_VERBOSE_OUT, "%s, ", *names);
	  fprintf (ICI_VERBOSE_OUT, "%s\n", *names);
	}
      free (names);		/* this instance will not be used anymore */ 
    }

  /* if we're processing pass chain "all_passes", report each occurrence of
     a new function */
  if ((all_passes != NULL) && (*all_passes == 1))
    {      
      if (!prev_func_name || strcmp (func_name, prev_func_name))
	{
	  /* new function */
	  func_fname = (char *) ici_get_feature ("function_filename");
	  func_start = (long int) ici_get_feature ("function_start_line");
	  func_end = (long int) ici_get_feature ("function_end_line");
	  decl_fname = (char *) ici_get_feature ("function_decl_filename");
	  decl_line = (long int) ici_get_feature ("function_decl_line");
	  fprintf (ICI_VERBOSE_OUT,
		   "===> now processing function '%s' (declared at '%s:%ld',"
		   " body at '%s:%ld..%ld')\n",
		   func_name ? func_name : "<unknown>",
		   decl_fname, decl_line,
		   func_fname ? func_fname : "<unknown>",
		   func_start, func_end);
	}

      if (file == NULL ||
	  (prev_func_name != NULL && strcmp(func_name, prev_func_name)))
	{
	  if (prev_func_name != NULL)
	    free(prev_func_name);

	  prev_func_name =
	    (char *) malloc(sizeof(char) * (strlen(func_name) + 1));
	  strcpy(prev_func_name, func_name);
	  
	  if (file != NULL)
	    fclose(file);
	  
	  sprintf (output_file, "ici_passes_function.%s.txt", func_name);
	  file = fopen (output_file, "w");
	  line = 0;
	}
      
      line++;
      fprintf (file, "%s\n", pass_name ? pass_name : "<unknown>");
    }
}


/* plugin initialization function */
void start (void)
{
  const char *option_value, **option_names, **param_names,
    **pass_names, **actual_passes, **list_of_features, **next_feature;
  int option_count, param_count, pass_count, i, value;

  /* Register the event - use old ICI function name with GCC API.  */
  ici_register_event ("pass_execution", &executed_pass);

  if (getenv (ICI_PLUGIN_VERBOSE))
    {
      /* List all registered passes.  */
      for (actual_passes = ici_list_passes(), i = 0;
	   *actual_passes;
	   actual_passes++, i++)
	fprintf (ICI_VERBOSE_OUT,
		 "The pass at position %3d is '%s'\n", i, *actual_passes);
      
      /* List all available compiler features.  */
      list_of_features = ici_list_features ();
      
      for (i=0, next_feature = list_of_features;
	   *next_feature;
	   next_feature++, i++)
	fprintf (ICI_VERBOSE_OUT, "Feature #%d: '%s'\n", i, *next_feature);
      
      /* Print target configuration flags.  */
      option_value = 
	(const char *) ici_get_subfeature ("compiler_flags", "march=");
      fprintf (ICI_VERBOSE_OUT, "Option '-march=': value = '%s'\n",
	       (option_value ? option_value : "<unset>"));
      
      option_value =
	(const char *) ici_get_subfeature ("compiler_flags", "mcpu=");
      fprintf (ICI_VERBOSE_OUT, "Option '-mcpu=': value = '%s'\n",
	       (option_value ? option_value : "<unset>"));
      
      option_value =
	(const char *) ici_get_subfeature ("compiler_flags", "mtune=");
      fprintf (ICI_VERBOSE_OUT, "Option '-mtune=': value = '%s'\n",
	       (option_value ? option_value : "<unset>"));
      
      /* List all known compiler flags.  */
      option_names = (const char **) ici_get_feature ("compiler_flags");
      option_count =
	ici_get_feature_size ("compiler_flags") / sizeof (const char *);
      
      for (i = 0; i < option_count; i++)
	fprintf (ICI_VERBOSE_OUT, "Flag[%3d] is '%s'\n", i, option_names[i]);
      
      /* List all compiler parameters with their current values. */
      param_names = (const char **) ici_get_feature ("compiler_params");
      param_count =
	ici_get_feature_size ("compiler_params") / sizeof (const char *) ;
      
      for (i = 0; i < param_count; i++)
	fprintf (ICI_VERBOSE_OUT, "Parameter[%3d] is '%s', value = %ld\n",
		 i, param_names[i],
		 (long int) ici_get_subfeature ("compiler_params", param_names[i]));
      
      /* get list of all compiler passes */
      pass_names = (const char **) ici_get_feature ("named_passes");
      pass_count = ici_get_feature_size ("named_passes") / sizeof (const char *); 
      for (i = 0; i < pass_count; i++)
	fprintf (ICI_VERBOSE_OUT, "Pass[%3d] is '%s'\n", i, pass_names[i]);
      
    }

  /* set parameter, check the outcome */
  value = (int) (long int)
    ici_get_subfeature ("compiler_params", "max-inline-insns-recursive");
  
  fprintf (ICI_VERBOSE_OUT,
	   "\nChanging the value of parameter 'max-inline-insns-recursive'"
	   " (old value = %d)...\n",
	   value);
  
  ici_set_subfeature ("compiler_params", "max-inline-insns-recursive",
		      (void *) NEW_MAX_INLINE_INSNS_RECURSIVE);
  value = (int) (long int)
    ici_get_subfeature ("compiler_params", "max-inline-insns-recursive");
  
  assert (value == NEW_MAX_INLINE_INSNS_RECURSIVE);
  fprintf (ICI_VERBOSE_OUT,
	   "Parameter 'max-inline-insns-recursive' set correctly to value %d\n\n",
	   value);
  
  fprintf (ICI_VERBOSE_OUT, "Plugin initialization done.\n\n");
}

void stop (void)
{
  if (file != NULL)
    fclose(file);
}
