/*
 * ICI Plugin: Save executed passes into files (per function)
 *
 * Copyright (C) 2006-2010 by Grigori Fursin
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

static char *fml_feat="features.P";
static char *fml_feat_proc="ml-feat-proc";

static char* var_ici_pass;
char* var_ml_feat_proc;

char* ici_work_dir="";

void run_ml_pass(char* func_name, char* pass_name);

void controller (void)
{
  const char **list;
  FILE *file = NULL;
  FILE *file1 = NULL;
  const char *fpasses="ici_passes_all.txt";
  const char *var_passes_all=NULL;
  char pass_name[128];
  char func_names[1024];
  int process=0;

  char *func_name = (char *) ici_get_feature("function_name");

  fprintf(ICI_VERBOSE_OUT, "ICI: work dir %s ...\n", ici_work_dir);

  if (func_name)
  {
    //check file with selected functions
    process=1;
    if (getenv(ICI_FILE_SELECT_FUNCTIONS)!=NULL)
    {
      file1=fopen(getenv(ICI_FILE_SELECT_FUNCTIONS), "r");
      if (file1 != NULL)
      {
        process=0;
        while(!feof(file1))
        {
          fgets(func_names, 1022, file1);
          if (strlen(func_names)>0 && func_names[strlen(func_names)-1]=='\n') func_names[strlen(func_names)-1]=0;
          if (strcmp(func_name, func_names)==0)
          {
            process=1;
            break;
          }
        }
        fclose(file1);
      }
    }

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
          /* Check pass when to extract program features */
          if ((process==1) && (strcmp(var_ici_pass, pass_name)==0))
            run_ml_pass(func_name, pass_name);          
          strcpy(pass_name, "");
          i=0;
        }
      }
      pass_name[i]=0;
      ici_run_pass (pass_name);
      if ((process==1) && (strcmp(var_ici_pass, pass_name)==0))
        run_ml_pass(func_name, pass_name);          
      strcpy(pass_name, "");
    }
    else
    {
      char file_func_name[1024];

      if (getenv(ICI_PLUGIN_VERBOSE)!=NULL)
        fprintf(ICI_VERBOSE_OUT, "ICI: function = %s\n", func_name);

      sprintf(file_func_name, "%sici_passes_function.%s.txt", ici_work_dir, func_name);
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
          if ((process==1) && (strcmp(var_ici_pass, pass_name)==0))
            run_ml_pass(func_name, pass_name);
        }
        fclose(file);
      }
      else
      {
        if (getenv(ICI_PLUGIN_VERBOSE)!=NULL)
          fprintf(ICI_VERBOSE_OUT, "ICI warning: file with passes not found ...\n");
      }
    }
  }
  else
  {
    if (getenv(ICI_PLUGIN_VERBOSE)!=NULL)
      fprintf(ICI_VERBOSE_OUT, "ICI warning: function name is NULL ...\n");
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

  if (getenv("ICI_WORK_DIR")!=NULL)
    ici_work_dir=getenv("ICI_WORK_DIR");

  ici_register_event ("all_passes_manager", &controller);
  ici_register_event ("avoid_gate", &avoid_gate);

  /* Check environment variable after which pass to extract features */
  if ((var_ici_pass = getenv(ICI_PROG_FEAT_PASS)) == NULL)
  {
    printf("Error: Environment variable " ICI_PROG_FEAT_PASS " is not defined!\n");
    exit(1);
  }
}

char stop (void)
{
  if (getenv(ICI_PLUGIN_VERBOSE)!=NULL)
    fprintf(ICI_VERBOSE_OUT, "ICI: stop plugin ...\n");
}

void run_ml_pass(char* func_name, char* pass_name)
{
  FILE* file; 

  if (getenv(ICI_PLUGIN_VERBOSE)!=NULL)
    fprintf(ICI_VERBOSE_OUT, "Executing ML feature extracting pass ...\n");

  remove(fml_feat);

  /***************** EXECUTE PASS WITH FEATURE EXTRACTION *****************/
  ici_run_pass ("ml-feat");

  file = fopen(fml_feat, "r");
  if (file != NULL)
  {
    fclose(file);
    char filename[1024];

    if (getenv(ICI_PLUGIN_VERBOSE)!=NULL)
      fprintf(ICI_VERBOSE_OUT, "ICI: file %s exists!\n", fml_feat);

    /***************** CALL EXTERNAL TOOL TO COMMUNICATE WITH PROLOG *****************/
    system(fml_feat_proc);

    sprintf(filename, "ici_features_function.%s.%s.ft", func_name, pass_name);

    rename("features.FT", filename);
  }
}
