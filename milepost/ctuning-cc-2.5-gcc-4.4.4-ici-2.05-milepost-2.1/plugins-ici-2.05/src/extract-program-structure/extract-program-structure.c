/*
 * ICI Plugin: Extract program structure
 *
 * Copyright (C) 2010 by Grigori Fursin
 * University of Versailles at Saint-Quentin-en-Yvelines, France
 *
 * http://fursin.net/research
 */

/* Include the plugin interface files */
#include "ic-legacy.h" /* convert ICI 1.0 to ICI 2.0 */
#include "gcc/ic-interface.h"
#include "ic-defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE *file = NULL;
static FILE *file1 = NULL;
static char fname[1024]="";

void executed_pass (void)
{
  char *pass_name;
  char *func_name;
  char *varx;
  int vary;
  char output_file[256];
  static char *prev_func_name = NULL;
  static int line=0;

  /* Iterate through passes (obtained as features)
   * to save original GCC order */

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

      if ((strlen(fname)>0) && ((file1=fopen(fname, "a"))!=NULL))
      {
        //see http://cTuning.org/wiki/index.php/CTools:ICI:List_of_features

        fprintf(file1, "   FUNCTION=%s\n", func_name);
        if ((varx = (char *) ici_get_feature("function_decl_filename"))!=NULL)
          fprintf(file1, "    FUNCTION_DECLARATION_FILENAME=%s\n", varx);
        if ((vary = (int) ici_get_feature("function_decl_line"))!=0)
          fprintf(file1, "    FUNCTION_DECLARATION_LINE=%u\n", vary);
        if ((varx = (char *) ici_get_feature("function_filename"))!=NULL)
          fprintf(file1, "    FUNCTION_FILENAME=%s\n", varx);
        if ((vary = (int) ici_get_feature("function_start_line"))!=0)
          fprintf(file1, "    FUNCTION_START_LINE=%u\n", vary);
        if ((vary = (int) ici_get_feature("function_end_line"))!=0)
          fprintf(file1, "    FUNCTION_END_LINE=%u\n", vary);
        fprintf(file1, "    ICI_FILE_WITH_PASSES=%s\n", output_file);
        fclose(file1);
      }
    }

//old    pass_name = (char *) ici_get_parameter("pass_name");
    pass_name = (char *) get_feature("pass_name");
    fprintf(file, "%s\n", pass_name);

    line++;
  }
}

char start (void)
{
  char *var;

  if (getenv(ICI_PLUGIN_VERBOSE)!=NULL)
    fprintf(ICI_VERBOSE_OUT, "ICI: start plugin ...\n");

  if ((var=getenv("CTUNING_EXTRACT_STRUCTURE"))!=NULL)
    strcpy(fname, var);

  if ((var=getenv("CTUNING_EXTRACT_FEATURES"))!=NULL)
    strcpy(fname, var);

  ici_register_event ("pass_execution", &executed_pass);
}

char stop (void)
{
  if (file != NULL)
    fclose(file);
  
  if (getenv(ICI_PLUGIN_VERBOSE)!=NULL)
    fprintf(ICI_VERBOSE_OUT, "ICI: stop plugin ...\n");
}
