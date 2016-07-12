/*
 * ICI Plugin: Substitute GCC original passes with the ones from file or environment variable (per function)
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

void controller (void)
{
  const char **list;
  FILE *file = NULL;
  const char *fpasses="ici_passes_all.txt";
  const char *var_passes_all=NULL;
  char pass_name[128];

  if ((var_passes_all=getenv(ICI_PASSES_ALL))!=NULL)
  {
    if (getenv(ICI_PLUGIN_VERBOSE)!=NULL)
    {
      fprintf(ICI_VERBOSE_OUT, "ICI: Use passes from the ICI_PASSES_ALL variable for all functions ...\n");
      fprintf(ICI_VERBOSE_OUT, "ICI: PASS sequence: %s\n", var_passes_all);
    }

    int i=0;
    int j=0;
    
    strcpy(pass_name, "");
    while (j<strlen(var_passes_all))
    {
      if (var_passes_all[j]!=',')
        pass_name[i++]=var_passes_all[j++];
      else
      {
        j++;
        pass_name[i]=0;
        ici_run_pass (pass_name);
        strcpy(pass_name, "");
	i=0;
      }
    }
    pass_name[i]=0;
    ici_run_pass (pass_name);
    strcpy(pass_name, "");
  }
  else
  {
    char *func_name = (char *) ici_get_feature("function_name");

    if (func_name)
    {
      char file_func_name[1024];
 
      if (getenv(ICI_PLUGIN_VERBOSE)!=NULL)
        fprintf(ICI_VERBOSE_OUT, "ICI: function = %s\n", func_name);

      sprintf(file_func_name, "ici_passes_function.%s.txt", func_name);
      file = fopen(file_func_name, "r");
      if (file != NULL)
        if (getenv(ICI_PLUGIN_VERBOSE)!=NULL)
          fprintf(ICI_VERBOSE_OUT, "ICI: file with passes (%s) found !..\n", file_func_name);
 
      if (file == NULL)
      {
        file = fopen (fpasses, "r");
        if (file != NULL)
          if (getenv(ICI_PLUGIN_VERBOSE)!=NULL)
            fprintf(ICI_VERBOSE_OUT, "ICI: file with passes (%s) found !..\n", fpasses);
      }

      if (file != NULL)
      {
        int i=0;
        while(!feof(file))
        {
          fscanf(file, "%s\n", pass_name);
          ici_run_pass (pass_name);
        }
        fclose(file);
      }
      else
      {
        if (getenv(ICI_PLUGIN_VERBOSE)!=NULL)
          fprintf(ICI_VERBOSE_OUT, "ICI warning: file with passes not found ...\n");
      }
    }
    else
    {
      if (getenv(ICI_PLUGIN_VERBOSE)!=NULL)
        fprintf(ICI_VERBOSE_OUT, "ICI warning: function name is NULL ...\n");
    }
  }
}

void avoid_gate()
{
  int *all_passes=ici_get_parameter("all_passes");
  if ((all_passes!=NULL) && (*all_passes==1))
  {
    unsigned char *gate = ici_get_parameter("gate_status");
    if (gate!=NULL)
      *gate = 1;
    else
    {
      /* This shouldn't happen */
      fprintf(stderr, "ICI error: avoid_gate function is called but gate_status parameter is not defined ...\n");
      exit(1);
    }    
  }
}

char start (void)
{
  if (getenv(ICI_PLUGIN_VERBOSE)!=NULL)
    fprintf(ICI_VERBOSE_OUT, "ICI: start plugin ...\n");

  ici_register_event ("all_passes_manager", &controller);
  ici_register_event ("avoid_gate", &avoid_gate);
}

char stop (void)
{
  if (getenv(ICI_PLUGIN_VERBOSE)!=NULL)
    fprintf(ICI_VERBOSE_OUT, "ICI: stop plugin ...\n");
}
