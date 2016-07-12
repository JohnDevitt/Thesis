/*
 Author: Grigori Fursin (http://fursin.net/research)

 (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France
 (C) 2007-2010, UNIDAPT Group, INRIA, France

 Updates:
   2010.05.17 - Grigori Fursin rewrote most of the program 
                  and added many new features
   2010.05.02 - Abdul Memon fixed an issue with -D flags when recompiling
                  the whole Linux or a kernel

 TODO:
   Grigori wrote it very quickly without any reasonable style, variable declaration or comments.
   One day should be totally rewritten.

   Move cTuning environment variables and constants to .h file
   Clean up static/dynamic strings
   Encapsulate variables
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "ccc/ccc.h"
#include "ccc/stat.h"
#include "ccc/stat-comp.h"
#include "ccc/futils.h"
#include "ccc/fuuid.h"

static char ccc_opts[8192];
static char comp_a_name[128]; //analysis compiler name (such as MILEPOST GCC)
static char comp_u_name[128]; //user compiler (such as GCC, LLVM, XL, ICC, Open64, ROSE)
static char str1[8192];
static char str2[8192];
static char str3[8192];
static char str4[8192];
static char str5[8192];
static char str6[8192];
static char str7[1024];
static char str8[1024];
static char stropt[8192];
static char stroptmain[8192];
static char stroptadd[8192];
static char fobjmd5[128];
static char comp_id[128];
static char ip[128];
static char ici_passes[8192];
static char ici_features[8192];
static char func_name[1024];
static char var_ccc_comp_opts[8192];
static char var_ccc_plat_opts[8192];
static char opt_id[1024];

// should be moved to .h file
static char x_ctuning_opt[]="--ct-opt=";
static char x_ctuning_analysis_opt_level[]="CTUNING_ANALYSIS_OPT_LEVEL";

//if !="" then used as a file to aggregate info + transparent mode=2
static char x_ctuning_extract_structure[]="CTUNING_EXTRACT_STRUCTURE";

//if !="" then used as a file to aggregate info + transparent mode=3
static char x_ctuning_extract_features[]="CTUNING_EXTRACT_FEATURES"; 

//if !="" then used as a file to aggreated info + transparent mode=1
static char x_ctuning_predict_optimizations[]="CTUNING_PREDICT_OPTIMIZATIONS";

/*
 cc:
    CTUNING_ANALYSIS_CC (*)
    CTUNING_USER_CC (**)

 c++:
    CTUNING_ANALYSIS_CPP (*)
    CTUNING_USER_CPP (**)

 fortran:
    CTUNING_ANALYSIS_FORTRAN (*)
    CTUNING_USER_FORTRAN (**)

 (1) - compiler with plugins to extract features, etc (such as MILEPOST GCC)
 (2) - user compiler to invoke and pass predicted optimizations (if empty, invoke CTUNING_ANALYSIS_*)
*/

static char y_ctuning_cc[]="cc";
static char y_ctuning_a_cc[]="CTUNING_ANALYSIS_CC";
static char y_ctuning_u_cc[]="CTUNING_COMPILER_CC";

static char y_ctuning_cpp[]="c++";
static char y_ctuning_a_cpp[]="CTUNING_ANALYSIS_CPP";
static char y_ctuning_u_cpp[]="CTUNING_COMPILER_CPP";

static char y_ctuning_fortran[]="fortran";
static char y_ctuning_a_fortran[]="CTUNING_ANALYSIS_FORTRAN";
static char y_ctuning_u_fortran[]="CTUNING_COMPILER_FORTRAN";

static char* ref=NULL;

static char opts[16384];
static int iopts[8192];
static int iused[8192];
static int ioptsm=0;

/* define functions */
void glob_flags_read(char*);
void process_original_cmd(char*, char**, char*);
int find_flag_to_remove(char*);

/********************************************************************************/
int main(int argc, char* argv[])
{
  char* var_ccc_ici_use=NULL;
  char* var_ccc_ici_plugins=NULL;
  char* var_ccc_ici_passes_fn=NULL;
  char* var_ccc_ici_passes_ext=NULL;
  char* var_ccc_ici_passes_record_plugin=NULL;
  char* var_ccc_ici_passes_use_plugin=NULL;
  char* var_ccc_ici_ft_st_fn=NULL;
  char* var_ccc_ici_ft_st_ext=NULL;
  char* var_ccc_ici_ft_st_extract_plugin=NULL;
  char* var_ccc_opts=NULL;
  char* var_ici_prog_feat_pass=NULL;
  char* var_ici_verbose=NULL;
  char* var_ccc_cts_db=NULL;
  char* var_ccc_opt_arch_use=NULL;
  int ivar_ccc_opt_arch_use=0;
  char* var;

  char str_time_thr[1024]="";
  char str_notes[1024]="";
  char str_pg_use[1024]="";
  char str_output_cor[1024]="";
  char str_run_time[1024]="";
  char str_sort[1024]="";
  char str_dim[1024]="";
  char str_cut[1024]="";
  char str_opt_report[1024]="";
  char str_an_opt_lev[1024]="";
  char str_file_remove_opt[1024]="";
  char str_extract_structure[1024]="";

  char cur_dir[1024]="";

  char* var_plat_id=NULL;
  char* var_env_id=NULL;
  char* var_cmplr_id=NULL;
  char* var_cmplr_f_id=NULL;

  FILE* ff=NULL;
  FILE* ff1=NULL;
  FILE* ff2=NULL;
  int first=0;

  int x=0;
  int y=0;
  int mode=0;

  int iorig=0;

  char str_main[8192];
  char* str_main1[1024];

  //check for help
  for (x=1; x<argc; x++)
  {
    if      ((strcmp(argv[x], "--version")==0) ||
             (strcmp(argv[x], "--help")==0) ||
             (strcmp(argv[x], "-h")==0) ||
             (strcmp(argv[x], "-v")==0))
    {
      printf("cTuning Compiler Collection V2.5 (self-tuning, adaptive compiler)\n");
      printf("\n");
      printf("Wrapper around any user compiler to enable transparent architecture,\n");
      printf("code and dataset analysis, characterization and multi-objective\n");
      printf("optimization (currently execution time, code size and compilation time)\n");
      printf("based on empirical iterative feedback directed compilation, statistical analysis,\n");
      printf("collective optimization and machine learning (predictive modeling).\n");
      printf("\n");
      printf("cTuning-cc performs the following:\n");
      printf("* detects special flags or environment variables to invoke\n");
      printf("  analysis compilers that support Interactive Compilation Interface\n");
      printf("  (currently MILEPOST GCC - http://cTuning.org/milepost-gcc) to analyze\n");
      printf("  code structure, extract features, select and reorder optimizations, etc.\n");
      printf("\n");
      printf("* communicates with cTuning.org web-services and Collective Optimization\n");
      printf("  Database (http://cTuning.org/cdatabase) to suggest better optimizations\n");
      printf("  based on program, dataset and architecture features and machine learning\n");
      printf("  or to return an optimization referenced by the unique cTuning ID (useful\n");
      printf("  for manual sharing of optimization data, academic experiments\n");
      printf("  or bug reports, etc).\n");
      printf("\n");
      printf("* invokes any user compiler (for example, GCC, LLVM, ICC, Open64, Rose, XL, etc)\n");
      printf("  with the returned optimizations from cTuning.org.\n");
      printf("\n");
      printf("This is still an on-going collaborative effort toward self-tuning, adaptive\n");
      printf("computing system, so please be patient or help to extend it at cTuning.org ;) !\n");
      printf("\n");
      printf("Author:  Grigori Fursin (http://fursin.net/research)\n");
      printf("         (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France\n");
      printf("         (C) 2007-2010, UNIDAPT Group, INRIA, France\n");
      printf("License: GPL V2\n");
      printf("\n");
      printf("Some options:\n");
      printf("  -Oml  - substitute user optimizations and predict flags for a given program\n");
      printf("          to improve execution time, code size, compilation time\n");
      printf("          by correlating program features and optimizations\n");
      printf("          from the Collective Optimization Database at cTuning.org\n");
      printf("  --ct-test - Test cTuning web-service and database\n");
      printf("  --ct-opt=<OPT_ID> - Select optimization case from the Collective Optimization Database\n");
      printf("  --ct-extract-structure - Extract program structure\n");
      printf("  --ct-extract-features - Extract program features\n");
      printf("\n");
      printf("More information (R&D, extensions, docs): http://cTuning.org/ctuning-cc\n");
    }
  }

  //parse internal binary name and configure compilers
  strcpy(comp_a_name, "");
  strcpy(comp_u_name, "");

  for (x=0; x<strlen(argv[0]); x++)
  {
    if (argv[0][x]=='-')
    {
      if (strcmp(&argv[0][x+1], y_ctuning_cc)==0)
      {
        if ((ref=getenv(y_ctuning_a_cc)) == NULL)
        {
          printf("Error: Environment variable %s is not defined!\n", y_ctuning_a_cc);
          exit(1);
        }
        strcpy(comp_a_name, ref);
        if ((ref=getenv(y_ctuning_u_cc)) == NULL)
        {
          printf("Error: Environment variable %s is not defined!\n", y_ctuning_u_cc);
          exit(1);
        }
        strcpy(comp_u_name, ref);
      }
      else if (strcmp(&argv[0][x+1], y_ctuning_cpp)==0)
      {
        if ((ref=getenv(y_ctuning_a_cpp)) == NULL)
        {
          printf("Error: Environment variable %s is not defined!\n", y_ctuning_a_cpp);
          exit(1);
        }
        strcpy(comp_a_name, ref);
        if ((ref=getenv(y_ctuning_u_cpp)) == NULL)
        {
          printf("Error: Environment variable %s is not defined!\n", y_ctuning_u_cpp);
          exit(1);
        }
        strcpy(comp_u_name, ref);
      }
      else if (strcmp(&argv[0][x+1], y_ctuning_fortran)==0)
      {
        if ((ref=getenv(y_ctuning_a_fortran)) == NULL)
        {
          printf("Error: Environment variable %s is not defined!\n", y_ctuning_a_fortran);
          exit(1);
        }
        strcpy(comp_a_name, ref);
        if ((ref=getenv(y_ctuning_u_fortran)) == NULL)
        {
          printf("Error: Environment variable %s is not defined!\n", y_ctuning_u_fortran);
          exit(1);
        }
        strcpy(comp_u_name, ref);
      }

      break;
    }
  }

  if ((strcmp(comp_a_name, "")==0) || (strcmp(comp_u_name, "")==0))
  {
    printf("Error: can't understand the name of the compiler - it was likely renamed!\n");
    exit(1);
  }

  //parse flags, detect cTuning flags.
  //should move to header file one day...
  for (x=1; x<argc; x++)
  {
    if      (strcmp(argv[x], "-Oml")==0) mode=1;
    else if (strcmp(argv[x], "--version")==0) mode=-1;
    else if (strcmp(argv[x], "--help")==0) mode=-1;
    else if (strcmp(argv[x], "--ct-test")==0) mode=-2;
    else if (strcmp(argv[x], "--ct-extract-structure")==0) mode=2;
    else if (strcmp(argv[x], "--ct-extract-features")==0) mode=3;
    else if (strncmp(argv[x], x_ctuning_opt, strlen(x_ctuning_opt))==0)
    {
      strcpy(opt_id, argv[x]+strlen(x_ctuning_opt));
      mode=10;
    }
    else
    {
      str_main1[iorig++]=argv[x];
      str_main1[iorig]=NULL;
    }
  }

  //check some variables that influence cTuning modes in transparent optimization mode
  if ((var=getenv(x_ctuning_extract_structure)) !=NULL)
  {
    strcpy(str_extract_structure, var);
    mode=2;
  }
  if ((var=getenv(x_ctuning_extract_features)) !=NULL)
  {
    strcpy(str_extract_structure, var);
    mode=3;
  }
  if ((var=getenv(x_ctuning_predict_optimizations)) !=NULL)
  {
    strcpy(str_extract_structure, var);
    mode=1;
  }
  strcpy(ccc_opts, "");
  if ((var_ccc_opts=getenv(CCC_OPTS)) !=NULL)
  {
    strcpy(ccc_opts, var_ccc_opts);
    mode=4;
  }

  /* No cTuning modes: invoke original compiler and quit */
  if (mode==0)
  {
    process_original_cmd(str8, str_main1, NULL);
    sprintf(str1, "%s %s", comp_u_name, str8);
    exit(system(str1));
  }

  /* all other modes */
  fflush(stdout); fflush(stderr);

  //Checking various environment variables
  //add name to .h later ...

  //file to remove opt flags
  if ((var = getenv("CTUNING_FILE_REMOVE_OPT_FLAGS")) != NULL)
  {
    //process file with flags
     strcpy(str_file_remove_opt, var);
     glob_flags_read(str_file_remove_opt);
  }

  //mode=4 (just add flags from CCC_OPTS)
  if (mode==4)
  {
    printf("Adding flags transparently ...\n");

    process_original_cmd(str8, str_main1, str_file_remove_opt);
    sprintf(str2, "%s %s %s", comp_u_name, ccc_opts, str8);

    if ((strlen(str_extract_structure)>0) && ((ff1=fopen(str_extract_structure, "a"))!=NULL))
    {
      fprintf(ff1, " COMPILER_NAME=%s\n", comp_u_name);
      fprintf(ff1, "  COMMAND_LINE=%s %s\n", ccc_opts, str8);
      fclose(ff1);
    }

    fflush(stdout); fflush(stderr);

    exit(system(str2));
  }

  //continue environment variables
  //check which compilers to use for analysis and optimization

  if ((var_ccc_ici_use = getenv(CCC_ICI_USE)) == NULL)
  {
    printf("Error: Environment variable " CCC_ICI_USE " is not defined!\n");
    exit(1);
  }

  var_ici_verbose = getenv(ICI_VERBOSE);

  if ((var_ccc_ici_plugins = getenv(CCC_ICI_PLUGINS)) == NULL)
  {
    printf("Error: Environment variable " CCC_ICI_PLUGINS " is not defined!\n");
    exit(1);
  }

  if ((var_ccc_ici_passes_fn = getenv(CCC_ICI_PASSES_FN)) == NULL)
  {
    printf("Error: Environment variable " CCC_ICI_PASSES_FN " is not defined!\n");
    exit(1);
  }

  if ((var_ccc_ici_passes_ext = getenv(CCC_ICI_PASSES_EXT)) == NULL)
  {
    printf("Error: Environment variable " CCC_ICI_PASSES_EXT " is not defined!\n");
    exit(1);
  }

  if ((var_ccc_ici_passes_record_plugin = getenv(CCC_ICI_PASSES_RECORD_PLUGIN)) == NULL)
  {
    printf("Error: Environment variable " CCC_ICI_PASSES_RECORD_PLUGIN " is not defined!\n");
    exit(1);
  }

  if ((var_ccc_ici_ft_st_fn = getenv(CCC_ICI_FEATURES_ST_FN)) == NULL)
  {
    printf("Error: Environment variable " CCC_ICI_FEATURES_ST_FN " is not defined!\n");
    exit(1);
  }

  if ((var_ccc_ici_ft_st_ext = getenv(CCC_ICI_FEATURES_ST_EXT)) == NULL)
  {
    printf("Error: Environment variable " CCC_ICI_FEATURES_ST_EXT " is not defined!\n");
    exit(1);
  }

  if ((var_ccc_ici_ft_st_extract_plugin = getenv(CCC_ICI_FEATURES_ST_EXTRACT_PLUGIN)) == NULL)
  {
    printf("Error: Environment variable " CCC_ICI_FEATURES_ST_EXTRACT_PLUGIN " is not defined!\n");
    exit(1);
  }

  if ((var_ici_prog_feat_pass = getenv(ICI_PROG_FEAT_PASS)) == NULL)
  {
    printf("Error: Environment variable " ICI_PROG_FEAT_PASS " is not defined!\n");
    exit(1);
  }

  if ((var_plat_id = getenv(CCC_PLAT_ID)) == NULL)
  {
    printf("Error: Environment variable " CCC_PLAT_ID " is not defined!\n");
    exit(1);
  }

  if ((var_env_id = getenv(CCC_ENV_ID)) == NULL)
  {
    printf("Error: Environment variable " CCC_ENV_ID " is not defined!\n");
    exit(1);
  }

  if ((var_cmplr_id = getenv(CCC_COMPILER_ID)) == NULL)
  {
    printf("Error: Environment variable " CCC_COMPILER_ID " is not defined!\n");
    exit(1);
  }

  if ((var_cmplr_f_id = getenv(CCC_COMPILER_FEATURES_ID)) == NULL)
  {
    printf("Error: Environment variable " CCC_COMPILER_FEATURES_ID " is not defined!\n");
    exit(1);
  }

  if ((var_ccc_cts_db = getenv(CCC_CTS_DB)) == NULL)
  {
    printf("Error: Environment variable " CCC_CTS_DB " is not defined!\n");
    exit(1);
  }

  if ((var_ccc_opt_arch_use = getenv(CCC_OPT_ARCH_USE)) == NULL)
  {
    printf("Error: Environment variable " CCC_OPT_ARCH_USE " is not defined!\n");
    exit(1);
  }
  if (strcmp(var_ccc_opt_arch_use,"1")==0) ivar_ccc_opt_arch_use=1;

  if ((var=getenv(CCC_TIME_THRESHOLD)) != NULL)
    strcpy(str_time_thr, var);

  if ((var=getenv(CCC_NOTES)) != NULL)
    strcpy(str_notes, var);

  if ((var=getenv(CCC_PG_NOTE)) != NULL)
    strcpy(str_pg_use, var);

  if ((var=getenv(CCC_OUTPUT_CORRECT)) != NULL)
    strcpy(str_output_cor, var);

  if ((var=getenv(CCC_RUN_TIME)) != NULL)
    strcpy(str_run_time, var);

  if ((var=getenv(CCC_SORT)) != NULL)
    strcpy(str_sort, var);

  if ((var=getenv(CCC_DIR)) != NULL)
    strcpy(str_dim, var);

  if ((var=getenv(CCC_CUT)) != NULL)
    strcpy(str_cut, var);

  if ((var=getenv(CT_OPT_REPORT)) != NULL)
    strcpy(str_opt_report, var);

  if ((var=getenv(CCC_TIME_THRESHOLD)) != NULL)
    strcpy(str_time_thr, var);

  if ((var=getenv(x_ctuning_analysis_opt_level)) !=NULL)
    strcpy(str_an_opt_lev, var);

  fflush(stdout); fflush(stderr);

  //Testing cTuning web services
  if (mode==-2)
  {
    printf("Testing cTuning.org web-service ...\n");

    //prepare file to send to web-service
    if (get_filename_uuid(str3, _tmp1 ".%s.tmp"))
    {
      printf("\nError: Can't generate TMP file using UUID!\n");
      exit(1);
    }

    //prepare file for response
    if (get_filename_uuid(str4, _tmp1 ".%s.tmp"))
    {
      printf("\nError: Can't generate TMP file using UUID!\n");
      exit(1);
    }

    ff1 = fopen (str3, "w");
    fprintf(ff1, "TEST=ctuning_test\n");
    fclose(ff1);

    setenv("CCC_FILE", str3, 1); 
    setenv("CCC_FILE_OUT", str4, 1); 
    setenv("CCC_WEB_SERVICE", "ctuning_web_service_test", 1); 
    setenv("CCC_CTS_DB", var_ccc_cts_db, 1);

    fflush(stdout); fflush(stderr);

    system("web-service-cod > /dev/null");

    ff1 = fopen (str4, "r");
    if (ff1!=NULL)
    {
      fgets(str2, 8190, ff1);
      fclose(ff1);
    }

    printf("\nResponse from web-service:\n");
    printf("%s\n", str2);
    exit(0);
  }

  //Obtaining cTuning optimization case by ID
  if (mode==10)
  {
    printf("Quering cTuning.org web-service to obtain optimizations for opt_id=%s ...\n", opt_id);

    //prepare file to send to web-service
    if (get_filename_uuid(str3, _tmp1 ".%s.tmp"))
    {
      printf("\nError: Can't generate TMP file using UUID!\n");
      exit(1);
    }

    //prepare file for response
    if (get_filename_uuid(str4, _tmp1 ".%s.tmp"))
    {
      printf("\nError: Can't generate TMP file using UUID!\n");
      exit(1);
    }

    ff1 = fopen (str3, "w");
    fprintf(ff1, "OPT_ID=%s\n", opt_id);
    fclose(ff1);

    setenv("CCC_FILE", str3, 1); 
    setenv("CCC_FILE_OUT", str4, 1); 
    setenv("CCC_WEB_SERVICE", "get_ctuning_opt_case", 1); 
    setenv("CCC_CTS_DB", var_ccc_cts_db, 1);

    fflush(stdout); fflush(stderr);

    system("web-service-cod > /dev/null");

    if ((ff1=fopen (str4, "r"))!=NULL)
    {
      strcpy(str2,"");
      strcpy(str5,"");

      if (!feof(ff1)) fgets(str2, 8190, ff1);
      if (!feof(ff1)) fgets(str5, 8190, ff1);

      fclose(ff1);
    }   
    else
    {
      printf("Error: Can't find response file!\n");
    }

    strcpy(str1, "");
    if (strncmp(str2, "OPT_FLAGS=",10)==0)
    {
      strcpy(str1, &str2[10]);
      fparse1(str1);
    }
    else
    {
      printf("\nResponse from cTuning web-service:\n%s", str2);
    }

    strcpy(str6, "");
    if (strncmp(str5, "OPT_FLAGS_ARCH=",15)==0)
    {
      strcpy(str6, &str5[15]);
      fparse1(str6);
    }

    printf("\n");
    if (strcmp(str1, "")==0)
    {
      printf("Couldn't find better optimizations based on program features - using default optimizations ...\n");
      process_original_cmd(str8, str_main1, NULL);
      sprintf(str2, "%s %s", comp_u_name, str8);
    }
    else
    {
      //Select optimization
      printf("Returned associated flags from cTuning:\n");
      printf(" %s\n", str1);
      if (ivar_ccc_opt_arch_use==0) strcpy(str6, "");

      process_original_cmd(str8, str_main1, str_file_remove_opt);
      sprintf(str2, "%s %s %s %s", comp_u_name, str1, str6, str8);
    }

    printf("\nInvoking command:\n%s\n", str2);

    fflush(stdout); fflush(stderr);

    setenv(var_ccc_ici_use, "0", 1); 
    exit(system(str2));
  }

  //Continue ICI modes (mode==1 or 3)
  if ((mode==1) || (mode==2) || (mode==3))
  {
    //Extract program structure
    printf("Extracting program structure ...\n");

    //if aggregating
    if (strlen(str_extract_structure)>0)
    {
      //Get current dir
      if (get_filename_uuid(str3, _tmp1 ".%s.tmp"))
      {
        printf("\nError: Can't generate TMP file using UUID!\n");
        exit(1);
      }

      sprintf(str2, "rm -f %s", str3);
      system(str2);

      sprintf(str2, "pwd > %s", str3);
      system(str2);

      if ((ff=fopen(str3, "r"))!=NULL)
      {
        if (fgets(str1, 8191, ff)!=NULL)
        {
          strcpy(cur_dir, str1);
          fparse1(str1);

          if ((strlen(str_extract_structure)>0) && ((ff1=fopen(str_extract_structure, "a"))!=NULL))
          {
            fprintf(ff1, "CURRENT_DIRECTORY=%s\n", str1);
            fclose(ff1);
          }
        }
        fclose(ff);
      }
      else
      {
        printf("\nWarning: Can't find file %s with current directory!\n", str3);
        exit(1);
      }

      sprintf(str2, "rm -f %s", str3);
      system(str2);
    }

    setenv(var_ccc_ici_use, "1", 1); 
    setenv(var_ccc_ici_plugins, var_ccc_ici_passes_record_plugin, 1);

    //Only for mode==1, otherwise when compiling files separately, ICI output from different sources will be deleted
    if (mode==1)
    {
      sprintf(str1, _del " %s.*%s", var_ccc_ici_ft_st_fn, var_ccc_ici_ft_st_ext);
      system(str1);
    }

    if (strlen(str_an_opt_lev)!=0)
      process_original_cmd(str8, str_main1, str_file_remove_opt);
    else
      process_original_cmd(str8, str_main1, NULL);

    sprintf(str1, "%s %s %s", comp_a_name, str_an_opt_lev, str8);

    if ((strlen(str_extract_structure)>0) && ((ff1=fopen(str_extract_structure, "a"))!=NULL))
    {
      fprintf(ff1, " COMPILER_NAME=%s\n", comp_a_name);
      fprintf(ff1, "  PLUGIN=%s\n", var_ccc_ici_passes_record_plugin);
      fprintf(ff1, "  COMMAND_LINE=%s %s\n", str_an_opt_lev, str8);
      fclose(ff1);
    }

    if (var_ici_verbose==NULL)
      strcat(str1, " > /dev/null 2> /dev/null");

    fflush(stdout); fflush(stderr);

    system(str1);
  }

  if ((mode==1) || (mode==3))
  {
    //Compile again and extract features after specific pass
    printf("Extracting program features ...\n");

    //Only for mode==1, otherwise when compiling files separately, ICI output from different sources will be deleted
    if (mode==1)
    {
      sprintf(str1, _del " *.ft");
      system(str1);
    }

    setenv(var_ccc_ici_use, "1", 1); 
    setenv(var_ccc_ici_plugins, var_ccc_ici_ft_st_extract_plugin, 1);

    if (strlen(str_an_opt_lev)!=0)
      process_original_cmd(str8, str_main1, str_file_remove_opt);
    else
      process_original_cmd(str8, str_main1, NULL);

    sprintf(str1, "%s %s %s", comp_a_name, str_an_opt_lev, str8);

    if ((strlen(str_extract_structure)>0) && ((ff1=fopen(str_extract_structure, "a"))!=NULL))
    {
      fprintf(ff1, " COMPILER_NAME=%s\n", comp_a_name);
      fprintf(ff1, "  PLUGIN=%s\n", var_ccc_ici_ft_st_extract_plugin);
      fprintf(ff1, "  COMMAND_LINE=%s %s\n", str_an_opt_lev, str8);
      fclose(ff1);
    }

    if (var_ici_verbose==NULL)
      strcat(str1, " > /dev/null 2> /dev/null");

    fflush(stdout); fflush(stderr);

    system(str1);
  }

  if ((mode==1) || (mode==3))
  {
    //prepare list of files
    if (get_filename_uuid(str3, _tmp1 ".%s.tmp"))
    {
      printf("\nError: Can't generate TMP file using UUID!\n");
      exit(1);
    }

    fflush(stdout); fflush(stderr);

    sprintf(str2, _dir " %s.*%s > %s 2>/dev/null", var_ccc_ici_ft_st_fn, var_ccc_ici_ft_st_ext, str3);
    system(str2);

    sprintf(str2, ".%s%s", var_ici_prog_feat_pass, var_ccc_ici_ft_st_ext);

    //read file with features
    ff=fopen(str3, "r");
    if (ff!=NULL)
    {
      while (fgets(str1, 8191, ff)!=NULL)
      {
        int il=0;
        fparse1(str1);
        il=strlen(str1)-strlen(var_ccc_ici_ft_st_fn)-strlen(str2)-1;
        strncpy(func_name, &str1[strlen(var_ccc_ici_ft_st_fn)]+1, il);
        func_name[il]=0;

        first=1;
        ff1 = fopen (str1, "r");
        if (ff1!=NULL)
        {
          fgets(str1, 1023, ff1);
          fparse1(str1);
          fclose(ff1);

          if ((strlen(str_extract_structure)>0) && ((ff2=fopen(str_extract_structure, "a"))!=NULL))
          {
            fprintf(ff2, "   FUNCTION_NAME=%s\n", func_name);
            fprintf(ff2, "    STATIC_PROGRAM_FEATURES=%s\n", str1);
            fclose(ff2);
          }
        }
        else
          printf("\nWarning: Can't find file %s with features!\n", str1);
        strcpy(ici_features, str1);
      }

      fclose(ff);
    }
  }

  if (mode==1)
  {
    //accumulate features
    printf("\nChecking program features (and aggregating them if generated) ...\n");

    fflush(stdout); fflush(stderr);

    sprintf(str2, _ccc_ml_accum_ft " %s %s", str3, var_ccc_ici_ft_st_fn);
    system(str2);

    sprintf(str2, "%s%s", var_ccc_ici_ft_st_fn, var_ccc_ici_ft_st_ext);

    first=1;
    ff1 = fopen (str2, "r");
    if (ff1!=NULL)
    {
      fgets(str1, 1023, ff1);
      fclose(ff1);
    }
    else
      printf("\nWarning: Can't find file %s with features!\n", str1);

    if (strcmp(str1, "")!=0 && strcmp(str1, "\n")!=0)
    {
      fparse1(str1);

      printf("\nStatic program features:\n%s\n", str1);
      strcpy(ici_features, str1);

      fflush(stdout); fflush(stderr);

      sprintf(str2, _del " %s", str3);
      system(str2);

      //Submit features to the server
      strcpy(str1,"");
      printf("\nSubmitting features to the cTuning web-service to predict good optimizations ...\n");

      //prepare file to send to web-service
      if (get_filename_uuid(str3, _tmp1 ".%s.tmp"))
      {
        printf("\nError: Can't generate TMP file using UUID!\n");
        exit(1);
      }

      //prepare file for response
      if (get_filename_uuid(str4, _tmp1 ".%s.tmp"))
      {
        printf("\nError: Can't generate TMP file using UUID!\n");
        exit(1);
      }

      ff1 = fopen (str3, "w");
      fprintf(ff1, "PLATFORM_ID=%s\n", var_plat_id);
      fprintf(ff1, "ENVIRONMENT_ID=%s\n", var_env_id);
      fprintf(ff1, "COMPILER_ID=%s\n", var_cmplr_id);
      fprintf(ff1, "COMPILER_FEATURES_ID=%s\n", var_cmplr_f_id);
      fprintf(ff1, "ST_PROG_FEAT=%s\n", ici_features);
      fprintf(ff1, CCC_TIME_THRESHOLD "=%s\n", str_time_thr);
      fprintf(ff1, CCC_NOTES "=%s\n", str_notes);
      fprintf(ff1, CCC_PG_NOTE "=%s\n", str_pg_use);
      fprintf(ff1, CCC_OUTPUT_CORRECT "=%s\n", str_output_cor);
      fprintf(ff1, CCC_RUN_TIME "=%s\n", str_run_time);
      fprintf(ff1, CCC_SORT "=%s\n", str_sort);
      fprintf(ff1, CCC_DIR "=%s\n", str_dim);
      fprintf(ff1, CCC_CUT "=%s\n", str_cut);
      fprintf(ff1, CT_OPT_REPORT "=%s\n", str_opt_report);

      fclose(ff1);

      setenv("CCC_FILE", str3, 1); 
      setenv("CCC_FILE_OUT", str4, 1); 
      setenv("CCC_WEB_SERVICE", "predict_opt", 1); 
      setenv("CCC_CTS_DB", var_ccc_cts_db, 1);

      fflush(stdout); fflush(stderr);

      system("web-service-cod > /dev/null");

      strcpy(str1, "");
      if ((ff1=fopen (str4, "r"))!=NULL)
      {
        strcpy(str2,"");
        strcpy(str5,"");
        strcpy(str6,"");

        if (!feof(ff1)) fgets(str2, 8190, ff1);
         if (strncmp(str2, "OPT_FLAGS=",10)==0)
         {
           strcpy(str1, &str2[10]);
           fparse1(str1);
         }
         else
           printf("\nResponse from cTuning web-service:\n%s", str2);

        if (!feof(ff1)) fgets(str5, 8190, ff1);
         if (strncmp(str5, "OPT_FLAGS_ARCH=",15)==0)
         {
           strcpy(str6, &str5[15]);
           fparse1(str6);
         }

        if (!feof(ff1)) fgets(str2, 8190, ff1);
         if (strncmp(str2, "CTUNING_OPTIMIZATION_REPORT=",28)==0)
         {
           printf("\ncTuning Optimization Report (optimal optimization cases):\n\n");
           while (!feof(ff1))
           {
             strcpy(str2, "");
             fgets(str2, 8190, ff1);
             fparse1(str2);
             printf("%s\n", str2);
           }
         }

        fclose(ff1);

        sprintf(str2, _del " %s", str3);
        system(str2);

        sprintf(str2, _del " %s", str4);
        system(str2);
      }
      else
      {
        printf("Error: Can't find response file!\n");
      }

      printf("\n");
      if (strcmp(str1, "")==0)
      {
        printf("Couldn't find better optimizations based on program features - using default optimizations ...\n");
        process_original_cmd(str8, str_main1, NULL);
        sprintf(str2, "%s %s", comp_u_name, str8);

        if ((strlen(str_extract_structure)>0) && ((ff1=fopen(str_extract_structure, "a"))!=NULL))
        {
          fprintf(ff1, " COMPILER_NAME=%s\n", comp_u_name);
          fprintf(ff1, "  COMMAND_LINE=%s %s\n", str8);
          fclose(ff1);
        }
      }
      else
      {
        //Select optimization
        printf("Predicted flags:\n");
        printf(" %s\n", str1);
        if (ivar_ccc_opt_arch_use==0) strcpy(str6, "");

        process_original_cmd(str8, str_main1, str_file_remove_opt);
        sprintf(str2, "%s %s %s %s", comp_u_name, str1, str6, str8);

        if ((strlen(str_extract_structure)>0) && ((ff1=fopen(str_extract_structure, "a"))!=NULL))
        {
          fprintf(ff1, " COMPILER_NAME=%s\n", comp_u_name);
          fprintf(ff1, "  COMMAND_LINE=%s %s %s\n", str1, str6, str8);
          fclose(ff1);
        }

      }
    }
    else
    {
      printf("\nUsing default optimizations (no features) ...\n");

      process_original_cmd(str8, str_main1, NULL);
      sprintf(str2, "%s %s", comp_u_name, str8);

      if ((strlen(str_extract_structure)>0) && ((ff1=fopen(str_extract_structure, "a"))!=NULL))
      {
        fprintf(ff1, " COMPILER_NAME=%s\n", comp_u_name);
        fprintf(ff1, "  COMMAND_LINE=%s %s\n", str8);
        fclose(ff1);
      }
    }
  }
  else //mode!=1 (2 & 3 - recompile native)
  {
    process_original_cmd(str8, str_main1, NULL);
    sprintf(str2, "%s %s", comp_a_name, str8);

    if ((strlen(str_extract_structure)>0) && ((ff1=fopen(str_extract_structure, "a"))!=NULL))
    {
      fprintf(ff1, " COMPILER_NAME=%s\n", comp_a_name);
      fprintf(ff1, "  COMMAND_LINE=%s\n", str8);
      fclose(ff1);
    }
  }

  printf("\nInvoking command:\n%s\n\n", str2);

  fflush(stdout); fflush(stderr);

  setenv(var_ccc_ici_use, "0", 1); 
  exit(system(str2));
}

void process_original_cmd(char* str, char** arr, char* rmv)
{
  char strx[1024]="";
  char dummy[2]="";

  int i=0;
  int y=0;
  int removed;

  strcpy(str, "");

  while (arr[i]!=NULL)
  {
    removed=0;

    if ((rmv!=NULL) && (strlen(rmv)>0))
      removed=find_flag_to_remove(arr[i]);

    if (removed==0)
    {
      if (strncmp(arr[i], "-D", 2)==0)
      {
        strcpy(strx, "-D\"");
        for(y=2; y<strlen(arr[i]); y++)
          strx[y+1]=arr[i][y];
        strx[strlen(arr[i])+1]='\0';
        strcat(strx, "\"");
        strcat(str, strx);
      }
      else
        strcat(str, arr[i]);

      strcat(str, " ");
    }
    i++;
  }
}

void glob_flags_read(char* name)
{
  char str1[8192];

  FILE* ff;
  int ix=0;

  ioptsm=0;
  ff = fopen (name, "r");
  if (ff!=NULL)
  {
    while ((fgets(str1, 1023, ff)!=NULL) && (feof(ff)==0))
    {
      fparse1(str1);

      if (strlen(str1)!=0)
      {
        strcpy(&opts[ix], str1);
        iopts[ioptsm]=ix;
        ioptsm++;
        ix+=strlen(str1)+1;
      }
    }
    fclose (ff);
  }
  else
  {
    printf("Error: Can't find file with compiler optimization flags!\n");
    exit(1);
  }
}

int find_flag_to_remove(char* flag)
{
  char stry1[8192]="";
  char stry2[8192]="";
  char stry3[8192]="";
  char* stry4;

  int i,j,k,l;
  int iv, iv1, iv_min, iv_max;
  int param;

  int iii=0;
  int ii=0;

  for (i=ii; i<ioptsm; i++)
  {
    strcpy(stry1, &opts[iopts[i]]);

    strcpy(stry2, "");
    if (stry1[0]=='3')
    {
      int j=2;
      int k=0;
      strcpy(stry3, "");
      for (; j<strlen(stry1); j++)
      {
        if (stry1[j]==',') break;
        else stry3[k++]=stry1[j];
      }
      if (j==strlen(stry1))
      {
        printf("\nError: Inconsistent file with switches!\n");
        exit(1);
      }
      stry3[k]=0;
      iv_min=0;
      iv_max=atoi(stry3);
      j++;

      for (iv=iv_min; iv<iv_max; iv++)
      {
        k=0;
        strcpy(stry3, "");
        for (; j<strlen(stry1); j++)
        {
          if (stry1[j]==',') break;
          else stry3[k++]=stry1[j];
        }
        stry3[k]=0;
        j++;

        if (strlen(stry3)==0)
        {
          printf("\nError: Inconsistent file with switches!\n");
          exit(1);
        }

        stry4=trim(stry3);
        if ((strlen(stry4)<=strlen(flag)) && (strncmp(flag, stry4, strlen(stry4))==0))
          return 1;
      }
    }
    else if (stry1[0]=='2')
    {
      copy_str(stry2, stry1, 2);
      stry4=trim(stry2);
      if ((strlen(stry4)<=strlen(flag)) && (strncmp(flag, stry4, strlen(stry4))==0))
        return 1;
    }
    else if (stry1[0]=='1')
    {
      int j=2;
      int k=0;
      strcpy(stry3, "");
      for (; j<strlen(stry1); j++)
      {
        if (stry1[j]==',') break;
        else stry3[k++]=stry1[j];
      }
      if (j==strlen(stry1))
      {
        printf("\nError: Inconsistent file with switches!\n");
        exit(1);
      }
      stry3[k]=0;
      iv_min=atoi(stry3);
      j++;

      k=0;
      strcpy(stry3, "");
      for (; j<strlen(stry1); j++)
      {
        if (stry1[j]==',') break;
        else stry3[k++]=stry1[j];
      }
      if (j==strlen(stry1))
      {
        printf("\nError: Inconsistent file with switches!\n");
        exit(1);
      }
      stry3[k]=0;
      iv_max=atoi(stry3);
      j++;

      copy_str(stry2, stry1, j);

      stry4=trim(stry2);
      if ((strlen(stry4)<=strlen(flag)) && (strncmp(flag, stry4, strlen(stry4))==0))
        return 1;
    }
    else if (stry1[0]=='0')
    {
      copy_str(stry2, stry1, 2);
      strcat(stry2, "on");
      stry4=trim(stry2);
      if ((strlen(stry4)<=strlen(flag)) && (strncmp(flag, stry4, strlen(stry4))==0))
        return 1;

      copy_str(stry2, stry1, 2);
      strcat(stry2, "off");
      stry4=trim(stry2);
      if ((strlen(stry4)<=strlen(flag)) && (strncmp(flag, stry4, strlen(stry4))==0))
        return 1;
    }
    else
    {
      printf("\nError: Inconsistent file with switches!\n");
      exit(1);
    }
  }

  return 0;
}