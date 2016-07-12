/*
 * ICI Plugin: Save executed passes with time into files (per function)
 *
 * Copyright (C) 2006-2009 by Grigori Fursin
 *
 * http://fursin.net/research
 *
 * UNIDAPT Group
 * http://unidapt.org
 */

/* Include the plugin interface files */
#include "ic-legacy.h" /* convert ICI 1.0 to ICI 2.0 */
#include "gcc/ic-interface.h"
#include "ic-defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

static FILE *file = NULL;

static long first=0;
static clock_t t_first;

void executed_pass (void)
{
  char *pass_name;
  char *func_name;
  char output_file[256];
  static char *prev_func_name = NULL;
  static int line=0;
  
  /* Iterate through passes (obtained as features)
   * to save original GCC order */

  if (first==0)
  {
    first++;
    t_first=clock();
  }
  else
  {
    first++;
    printf("%d,  %f sec\n", first, (((double)(clock()-t_first))/(CLOCKS_PER_SEC)));
  }
  
  int *all_passes=ici_get_parameter("all_passes");
  if ((all_passes!=NULL) && (*all_passes==1))
  {
    func_name = (char *) ici_get_feature("function_name");
    if (file == NULL || (prev_func_name != NULL && strcmp(func_name, prev_func_name)))
    {
      if (prev_func_name != NULL)
        free(prev_func_name);
      prev_func_name = (char *) malloc(sizeof(char) * (strlen(func_name) + 1));
      strcpy(prev_func_name, func_name);

      if (file != NULL)
        fclose(file);
      
      sprintf(output_file, "ici_passes_function.%s.txt", func_name);
      file = fopen(output_file, "w");
      line=0;
    }

//old    pass_name = (char *) ici_get_parameter("pass_name");
    pass_name = (char *) get_feature("pass_name");
    fprintf(file, "%s\n", pass_name);
    
    line++;
  }
}

char start (void)
{
  if (getenv(ICI_PLUGIN_VERBOSE)!=NULL)
    fprintf(ICI_VERBOSE_OUT, "ICI: start plugin ...\n");

  ici_register_event ("pass_execution", &executed_pass);
}

char stop (void)
{
  if (file != NULL)
    fclose(file);
  
  if (getenv(ICI_PLUGIN_VERBOSE)!=NULL)
    fprintf(ICI_VERBOSE_OUT, "ICI: stop plugin ...\n");
}
