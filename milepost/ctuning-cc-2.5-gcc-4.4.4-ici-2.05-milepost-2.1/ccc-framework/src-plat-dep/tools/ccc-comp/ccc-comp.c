/*
 * Copyright (C) 2004-2009 by Grigori Fursin
 *
 * http://fursin.net/research
 *
 * UNIDAPT Group
 * http://unidapt.org
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

static char str1[8192];
static char str1x[8192];
static char str2[8192];
static char str3[8192];
static char stropt[8192];
static char stroptmain[8192];
static char stroptadd[8192];
static char stroptadd1[8192];
static char fobjmd5[128];
static char comp_id[128];
static char ip[128];
static char ici_passes[8192];
static char ici_features[8192];
static char func_name[1024];
static char var_ccc_comp_opts[8192];
static char var_ccc_plat_opts[8192];

int main(int argc, char* argv[])
{
  double tdiff;

  int first=0;

  struct stat buf;

  long fsize=0;
  long fobjsize=0;

  int i;

  FILE* ff;
  FILE* ff1;
  FILE* ff2;

  static unsigned long ida=0;
  static unsigned long idb=0;

  char* var_ccc_cfg=NULL;
  char* var_plat_id=NULL;
  char* var_env_id=NULL;
  char* var_ccc_url=NULL;
  char* var_ccc_c_url=NULL;

  int mode_ccc_ici_passes_record=0;
  int mode_ccc_ici_passes_use=0;
  char* var_ccc_ici_use=NULL;
  char* var_ccc_ici_plugins=NULL;
  char* var_ccc_ici_passes_fn=NULL;
  char* var_ccc_ici_passes_ext=NULL;
  char* var_ccc_ici_passes_record_plugin=NULL;
  char* var_ccc_ici_passes_use_plugin=NULL;
  char* var_ccc_ici_passes_opt_base=NULL;
  char* var_ccc_ici_passes_use_test_dir=NULL;
  char* var_ccc_ici_passes_use_function=NULL;
  char* var_ici_passes_all=NULL;
  char* var_ici_prog_feat_pass=NULL;
  char* var_ccc_cts_user=NULL;
  char* var_ccc_notes=NULL;
  char* var_ccc_arch_size=NULL;
  char* var_ccc_arch_cfg=NULL;
  char* var_ccc_opt_fine=NULL;
  char* var_ccc_opt_par_static=NULL;

  int mode_ccc_ici_features_st_extract=0;
  char* var_ccc_ici_ft_st_fn=NULL;
  char* var_ccc_ici_ft_st_ext=NULL;
  char* var_ccc_ici_ft_st_extract_plugin=NULL;

  int var_gprof=0;
  int var_baserun=0;

  char* var_env=NULL;

  char var_compiler_id[256];
  char var_compiler_ext[256];
  char var_program_id[256];
  
  time_t curtime;
  struct tm *loctime;

  int compiler_name_found=0;
  /********************************************************************/
  printf("CCC COMP (compile program and send stats to CCC server)\n");

  if (argc<3)
  {
    printf("\n");
    
    printf("Usage:\n");
    printf(" ccc-comp <Makefile compiler extension> \"<Compiler flags>\" \"<Additional flags>\"\n");

    return 1;
  }

  //Init
  if ((var_ccc_cfg = getenv(CCC_CFG)) == NULL)
  {
    printf("Error: Environment variable " CCC_CFG " is not defined!\n");
    exit(1);
  }

  if ((var_env = getenv(CCC_ICI_PASSES_RECORD)) != NULL)
     mode_ccc_ici_passes_record=atoi(var_env);
  else if ((var_env = getenv(CCC_ICI_PASSES_USE)) != NULL)
     mode_ccc_ici_passes_use=atoi(var_env);
  if ((var_env = getenv(CCC_ICI_FEATURES_ST_EXTRACT)) != NULL)
     mode_ccc_ici_features_st_extract=atoi(var_env);

  if (mode_ccc_ici_passes_record==1 || mode_ccc_ici_features_st_extract==1)
  {
    if ((var_ccc_ici_use = getenv(CCC_ICI_USE)) == NULL)
    {
      printf("Error: Environment variable " CCC_ICI_USE " is not defined!\n");
      exit(1);
    }

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
  }  

  if (mode_ccc_ici_passes_use==1)
  {
    if ((var_ccc_ici_use = getenv(CCC_ICI_USE)) == NULL)
    {
      printf("Error: Environment variable " CCC_ICI_USE " is not defined!\n");
      exit(1);
    }

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

    if ((var_ccc_ici_passes_use_plugin = getenv(CCC_ICI_PASSES_USE_PLUGIN)) == NULL)
    {
      printf("Error: Environment variable " CCC_ICI_PASSES_USE_PLUGIN " is not defined!\n");
      exit(1);
    }
  }  

  if (mode_ccc_ici_features_st_extract==1)
  {
    if ((var_ccc_ici_use = getenv(CCC_ICI_USE)) == NULL)
    {
      printf("Error: Environment variable " CCC_ICI_USE " is not defined!\n");
      exit(1);
    }

    if ((var_ccc_ici_plugins = getenv(CCC_ICI_PLUGINS)) == NULL)
    {
      printf("Error: Environment variable " CCC_ICI_PLUGINS " is not defined!\n");
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

  if ((var_ccc_url = getenv(CCC_URL)) == NULL)
  {
    printf("Warning: Environment variable " CCC_URL " is not defined!\n");
  }

  if ((var_ccc_c_url = getenv(CCC_C_URL)) == NULL)
  {
    printf("Warning: Environment variable " CCC_C_URL " is not defined!\n");
  }

  if ((var_env = getenv(CCC_GPROF)) != NULL)
  {
    var_gprof=atoi(var_env);
  }

  if ((var_env = getenv(CCC_BASERUN)) != NULL)
  {
    var_baserun=atoi(var_env);
  }

  var_ccc_notes          = getenv(CCC_NOTES);
  var_ccc_cts_user       = getenv(CCC_CTS_USER);
  var_ccc_arch_size      = getenv(CCC_ARCH_SIZE);
  var_ccc_arch_cfg       = getenv(CCC_ARCH_CFG);
  var_ccc_opt_fine       = getenv(CCC_OPT_FINE);
  var_ccc_opt_par_static = getenv(CCC_OPT_PAR_STATIC);

  strcpy(var_ccc_plat_opts, "");
  if ((getenv(CCC_OPT_PLATFORM)) != NULL)
    strcpy(var_ccc_plat_opts, getenv(CCC_OPT_PLATFORM));

  //Searching compiler ID in the CFG
  sprintf(str2, "%s/" _fall_compilers, var_ccc_cfg);
  strcpy(var_compiler_id,"");
  strcpy(var_ccc_comp_opts,"");
  compiler_name_found=0;
  if ((ff=fopen(str2, "rt"))!=NULL)
  {
    strcpy(str1, "");
    while (fgets(str1, 1024, ff)!=NULL)
    {
      fparse1(str1);
      if (strncmp(str1, _compiler_cfg_name, strlen(_compiler_cfg_name))==0)
      {
        if (compiler_name_found==1) compiler_name_found=0;
	else if (strcmp(&str1[strlen(_compiler_cfg_name)], argv[1])==0) compiler_name_found=1;
      }
      else if ((compiler_name_found==1) && (strncmp(str1, _compiler_id, strlen(_compiler_id))==0))
        strcpy(var_compiler_id, &str1[strlen(_compiler_id)]);
      else if ((compiler_name_found==1) && (strncmp(str1, _compiler_ext, strlen(_compiler_ext))==0))
        strcpy(var_compiler_ext, &str1[strlen(_compiler_ext)]);
      else if ((compiler_name_found==1) && (strncmp(str1, _compiler_add_opts, strlen(_compiler_add_opts))==0))
        strcpy(var_ccc_comp_opts, &str1[strlen(_compiler_add_opts)]);
    }
    fclose(ff);
  }
  else
  {
    printf("Error: Can't find file configuration file %s ...!\n", str2);
    exit(1);
  }

  if (strlen(var_compiler_id)==0)
  {
    printf("Error: Can't find " _compiler_id " in configuration file %s ...!\n", str2);
    exit(1);
  }

  //Copying Makefile
  printf("\nCopying Makefile.%s to Makefile ...\n", var_compiler_ext);
  sprintf(str1, _copy " Makefile.%s Makefile", var_compiler_ext);
  system(str1);

  //Get IP address
  get_ip(ip);

  //Read program id
  ff = fopen (_fprog_id, "r");
  if (ff!=NULL)
  {
    if (fgets(str1, 1023, ff)!=NULL)
    {
      fparse1(str1);
      strcpy(var_program_id,str1);
    }
    else
    {
      printf("\nError: Can't read file with program id!\n");
      exit(1);
    }
    fclose(ff);
  }
  else
  {
    printf("\nError: Can't find file with program id!\n");
    exit(1);
  }

  srand((unsigned)time(NULL)*atol(var_program_id));

  mkdir(_results, 0777);

  strcpy(stroptmain, argv[2]);
  strcpy(stroptadd, "");
  strcpy(stroptadd1, "");
  if (argc>3)
    strcpy(stroptadd, argv[3]);

  //Write optimization to file if needed for external processing
  //such as pruning flags
  if ((ff=fopen(_fcmplr_flags, "wt"))!=NULL)
  {
    fprintf(ff, "%s\n", stroptmain);
    fclose(ff);
  }  

  strcpy(stropt, stroptmain);
  if (strlen(stroptadd)!=0)
  {
    strcat(stropt, " ");
    strcat(stropt, stroptadd);
  }

  if (var_gprof==1)
    strcat(stroptadd1, _opt_pg);

  if ((strlen(var_ccc_plat_opts) != 0) && (var_baserun!=1))
  {
    strcat(stropt, " ");
    strcat(stropt, var_ccc_plat_opts);
  }

  if (strlen(var_ccc_comp_opts) != 0)
  {
    strcat(stropt, " ");
    strcat(stropt, var_ccc_comp_opts);
  }

  if (mode_ccc_ici_passes_record==1)
  {
    setenv(var_ccc_ici_use, "1", 1); 
    setenv(var_ccc_ici_plugins, var_ccc_ici_passes_record_plugin, 1);

    sprintf(str1, _del " %s.*%s", var_ccc_ici_passes_fn, var_ccc_ici_passes_ext);
    system(str1);
  }
  else if (mode_ccc_ici_features_st_extract==1)
  {
    setenv(var_ccc_ici_use, "1", 1); 
    setenv(var_ccc_ici_plugins, var_ccc_ici_passes_record_plugin, 1);

    sprintf(str1, _del " %s.*%s", var_ccc_ici_ft_st_fn, var_ccc_ici_ft_st_ext);
    system(str1);
  }
  else if (mode_ccc_ici_passes_use==1)
  {
    setenv(var_ccc_ici_use, "1", 1); 
    setenv(var_ccc_ici_plugins, var_ccc_ici_passes_use_plugin, 1);
  }

  //generate comp id
  if (get_uuid(&ida, &idb)!=0)
  {
    printf("\nError: Can't get UUID from the system!\n");
    exit(1);
  }
  sprintf(comp_id, "%u%u", ida, idb);

  //Saving compilation ID
  printf("Saving compilation ID %s into %s ...\n", comp_id, _fcomp_id);
  if ((ff=fopen(_fcomp_id, "w"))!=NULL)
  {
    fprintf(ff, "%s", comp_id);
    fclose(ff);
  }

  //Saving compiler ID
  printf("Saving compiler ID %s into %s ...\n", var_compiler_id, _fcmplr_id);
  if ((ff=fopen(_fcmplr_id, "w"))!=NULL)
  {
    fprintf(ff, "%s", var_compiler_id);
    fclose(ff);
  }

  printf("Clean project ...\n");
  system(_run1);
  
  //Explicitly remove a.out
  printf("Remove " _fout " ...\n");
  system(_del " " _fout);

  setenv(CCC_OPTS, stropt, 1);
  if (strlen(stroptadd1)!=0)
    setenv(CCC_OPTS_ADD, stroptadd1, 1);

  printf("\nSource compiler environment " CCC_ENV_C ".%s if exists and compile program with \"%s\" flags !..\n", argv[1], stropt);
  /* Saving it to file _fcomp_env_cur for ccc-run (to init gfortran) */
  if ((ff=fopen(_fcomp_env_cur, "w"))!=NULL)
  {
    fprintf(ff, "%s/" CCC_ENV_C ".%s", var_ccc_cfg, argv[1]);
    fclose(ff);
  }

  sprintf(str1, ". %s/" CCC_ENV_C ".%s; " _usrbintime, var_ccc_cfg, argv[1]);

  system(str1);

//FGG: I don't know why do we check _fout here ...
//  if (fileExist(_fout)==1 && mode_ccc_ici_features_st_extract==1)
  if (mode_ccc_ici_features_st_extract==1)
  {
    //Compile again and extract features after specific pass
    setenv(var_ccc_ici_use, "1", 1); 
    setenv(var_ccc_ici_plugins, var_ccc_ici_ft_st_extract_plugin, 1);
  
    printf("Clean project ...\n");
    system(_run1);

  	system(_usrbintime);
  
    //prepare list of files
    if (get_filename_uuid(str3, _tmp1 ".%s.tmp"))
    {
      printf("\nError: Can't generate TMP file using UUID!\n");
      exit(1);
    }

    sprintf(str2, _dir " %s.*%s > %s", var_ccc_ici_ft_st_fn, var_ccc_ici_ft_st_ext, str3);
    system(str2);
 
    sprintf(str2, ".%s%s",var_ici_prog_feat_pass, var_ccc_ici_ft_st_ext);

    //read file with features
    printf("\n");
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

        printf("Recording function features %s ...\n", func_name);
	  
        first=1;
        ff1 = fopen (str1, "r");
        if (ff1!=NULL)
        {
          fgets(str1, 1023, ff1);
          fclose(ff1);
        }
        else
          printf("\nWarning: Can't find file %s with features!\n", str1);
        strcpy(ici_features, str1);

        //Saving features data
        if (((ff2=fopen(_results _stat_features, "at"))!=NULL) && ((ff1=fopen(_tmp3, "w"))!=NULL))
        {
          fprintf(ff2, sep1 "\n");

          fprintf(ff2, stat_comp_COMPILE_ID "=%s\n", comp_id);
          fprintf(ff1, stat_comp_COMPILE_ID "=%s\n", comp_id);

          fprintf(ff2, stat_comp_FUNC_NAME "=%s\n", func_name);
          fprintf(ff1, stat_comp_FUNC_NAME "=%s\n", func_name);
      
          fprintf(ff2, stat_ft_st_PASS "=%s\n", var_ici_prog_feat_pass);
          fprintf(ff1, stat_ft_st_PASS "=%s\n", var_ici_prog_feat_pass);

          fprintf(ff2, stat_ft_st_STATIC_FEATURE_VECTOR "=%s\n", ici_features);
          fprintf(ff1, stat_ft_st_STATIC_FEATURE_VECTOR "=%s\n", ici_features);

          fclose(ff1);
          fclose(ff2);
        }

        //try to send stats
        if ((var_ccc_url!=NULL) && (var_ccc_c_url!=NULL) && (strcmp(var_ccc_url,"")!=0))
        {
          printf(sep2 "\n");
          printf("Trying to send FEATURES statistics to the Global Database ...\n\n");
          sprintf(str1, _send_stats_ft_st " %s", _tmp3);
          printf("%s\n", str1);
          system(str1);
          printf(sep2 "\n");
        }
      }

      fclose(ff);
    }

    //accumulate features
    printf("Aggregating features ...\n");

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
    strcpy(ici_features, str1);

    //Saving features data
    if (((ff2=fopen(_results _stat_features, "at"))!=NULL) && ((ff1=fopen(_tmp3, "w"))!=NULL))
    {
      fprintf(ff2, sep1 "\n");

      fprintf(ff2, stat_comp_COMPILE_ID "=%s\n", comp_id);
      fprintf(ff1, stat_comp_COMPILE_ID "=%s\n", comp_id);

      fprintf(ff2, stat_comp_FUNC_NAME "=\n");
      fprintf(ff1, stat_comp_FUNC_NAME "=\n");
      
      fprintf(ff2, stat_ft_st_PASS "=%s\n", var_ici_prog_feat_pass);
      fprintf(ff1, stat_ft_st_PASS "=%s\n", var_ici_prog_feat_pass);

      fprintf(ff2, stat_ft_st_STATIC_FEATURE_VECTOR "=%s\n", ici_features);
      fprintf(ff1, stat_ft_st_STATIC_FEATURE_VECTOR "=%s\n", ici_features);

      fclose(ff1);
      fclose(ff2);
    }

    //try to send stats
    if ((var_ccc_url!=NULL) && (strcmp(var_ccc_url,"")!=0))
    {
      printf(sep2 "\n");
      printf("Trying to send FEATURES statistics to the Global Database ...\n\n");
      sprintf(str1, _send_stats_ft_st " %s", _tmp3);
      printf("%s\n", str1);
      system(str1);
      printf(sep2 "\n");
    }

    remove(str3);
  }

	FILE *ftcomptime = fopen("tmp-ccc-comp-time", "r");
	if (ftcomptime == NULL)
		{
			printf("ERROR: Cannot find file with compilation time.");
		}

	char str_tdiff[1024];

	if (fgets(str_tdiff, 1023, ftcomptime) == NULL) 
		{
			printf("ERROR: Cannot read compilation time from file."); 
		}
	fclose(ftcomptime);

	fparse1(str_tdiff);

	tdiff = atof(str_tdiff);

  printf("Compile time : %6.1f\n", tdiff);

  fsize=0;
  fobjsize=0;
  strcpy(fobjmd5, "");

  sprintf(str1, _del " %s", _fobjmd5);
  system(str1);

  //check if created
  if (fileExist(_fout)!=1)
    printf("Error: Executable is not created!\n");
  else
  {
    //filesize
    stat(_fout, &buf);
    fobjsize=buf.st_size;
    printf("\nFile Size: %u\n", fobjsize);

    //objdump & md5sum
    printf("\nSource compiler environment " CCC_ENV_C ".%s if exists and run objdump!\n", argv[1]);
    sprintf(str1, ". %s/" CCC_ENV_C ".%s; " _objdump, var_ccc_cfg, argv[1]);
    system(str1);
	
    system(_objmd5);
	
    //read file with md5
    ff = fopen (_fobjmd5, "r");
    if (ff!=NULL)
    {
      if (fgets(fobjmd5, 127, ff)!=NULL)
      {
        for (i=0; i<strlen(fobjmd5); i++)
          if (fobjmd5[i]==' ')
          {
            fobjmd5[i]=0;
            break;
          }
          fparse1(fobjmd5);
      }
      else
        printf("\nWarning: Can't read file md5 sum!\n");
      fclose(ff);
    }
    else
      printf("\nWarning: Can't find file with md5 sum!\n");

    printf("Md5sum of objfile: %s\n", fobjmd5);

    if (mode_ccc_ici_passes_record==1 || mode_ccc_ici_passes_use==1)
    {
      if ((var_ici_passes_all=getenv(ICI_PASSES_ALL)) != NULL)
      {
        //Saving passes data
        if (((ff2=fopen(_results _stat_passes, "at"))!=NULL) && ((ff1=fopen(_tmp3, "w"))!=NULL))
        {
          fprintf(ff2, sep1 "\n");

          curtime=time(NULL);
          loctime=localtime(&curtime);
          strftime(str1, 127, "%Y-%m-%d", loctime);
          strftime(str1x, 127, "%H:%M:%S", loctime);
          fprintf(ff, stat_comp_DATETIME "=%s %s\n", str1, str1x);
          fprintf(ff1, stat_comp_DATETIME "=%s %s\n", str1, str1x);

          if (var_ccc_cts_user!=NULL)
          {
            fprintf(ff, stat_comp_USER "=%s\n", var_ccc_cts_user);
            fprintf(ff1, stat_comp_USER "=%s\n", var_ccc_cts_user);
          }
      
          fprintf(ff, stat_comp_IP "=%s\n", ip);
          fprintf(ff1, stat_comp_IP "=%s\n", ip);

          fprintf(ff2, stat_comp_COMPILER_ID "=%s\n", var_compiler_id);
          fprintf(ff1, stat_comp_COMPILER_ID "=%s\n", var_compiler_id);

          fprintf(ff2, stat_comp_COMPILE_ID "=%s\n", comp_id);
          fprintf(ff1, stat_comp_COMPILE_ID "=%s\n", comp_id);

          fprintf(ff2, stat_comp_FUNC_NAME "=%s\n", ICI_PASSES_ALL_FUNC);
          fprintf(ff1, stat_comp_FUNC_NAME "=%s\n", ICI_PASSES_ALL_FUNC);
      
          fprintf(ff2, stat_comp_PASSES "=%s\n", var_ici_passes_all);
          fprintf(ff1, stat_comp_PASSES "=%s\n", var_ici_passes_all);

          fclose(ff1);
          fclose(ff2);
        }

        //try to send stats
        if ((var_ccc_url!=NULL) && (strcmp(var_ccc_url,"")!=0))
        {
          printf(sep2 "\n");
          printf("Trying to send COMPILATION (PASSES) statistics to the Global Database ...\n\n");
          sprintf(str1, _send_stats_comp_passes " %s", _tmp3);
          printf("%s\n", str1);
          system(str1);
          printf(sep2 "\n");
        }
      }
      else
      {
        //prepare list of files
        if (get_filename_uuid(str3, _tmp1 ".%s.tmp"))
        {
          printf("\nError: Can't generate TMP file using UUID!\n");
          exit(1);
        }

        sprintf(str2, _dir " %s.*%s > %s", var_ccc_ici_passes_fn, var_ccc_ici_passes_ext, str3);
        system(str2);
 
        //read file with passes
        printf("\n");
        ff=fopen(str3, "r");
        if (ff!=NULL)
        {
          while (fgets(str1, 1023, ff)!=NULL)
          {
            int il=0;
            fparse1(str1);
            il=strlen(str1)-strlen(var_ccc_ici_passes_fn)-strlen(var_ccc_ici_passes_ext)-1;
            strncpy(func_name, &str1[strlen(var_ccc_ici_passes_fn)]+1, il);
            func_name[il]=0;

            printf("Recording function passes %s ...\n", func_name);
	  
            first=1;
            strcpy(ici_passes, "");
            ff1 = fopen (str1, "r");
            if (ff1!=NULL)
            {
              while (fgets(str1, 1023, ff1)!=NULL)
              {
                fparse1(str1);
                if (first!=1) strcat(ici_passes, ",");
                else first=0;
                strcat(ici_passes, str1);
              }
              fclose(ff1);
            }
            else
              printf("\nWarning: Can't find file %s with passes!\n", str1);

            //Saving passes data
            if (((ff2=fopen(_results _stat_passes, "at"))!=NULL) && ((ff1=fopen(_tmp3, "w"))!=NULL))
            {
              fprintf(ff2, sep1 "\n");
 
              fprintf(ff2, stat_comp_COMPILE_ID "=%s\n", comp_id);
              fprintf(ff1, stat_comp_COMPILE_ID "=%s\n", comp_id);

              fprintf(ff2, stat_comp_COMPILER_ID "=%s\n", var_compiler_id);
              fprintf(ff1, stat_comp_COMPILER_ID "=%s\n", var_compiler_id);

              fprintf(ff2, stat_comp_FUNC_NAME "=%s\n", func_name);
              fprintf(ff1, stat_comp_FUNC_NAME "=%s\n", func_name);
      
              fprintf(ff2, stat_comp_PASSES "=%s\n", ici_passes);
              fprintf(ff1, stat_comp_PASSES "=%s\n", ici_passes);

              fclose(ff1);
              fclose(ff2);
            }

            //try to send stats
            if ((var_ccc_url!=NULL) && (strcmp(var_ccc_url,"")!=0))
            {
              printf(sep2 "\n");
              printf("Trying to send COMPILATION (PASSES) statistics to the Global Database ...\n\n");
              sprintf(str1, _send_stats_comp_passes " %s", _tmp3);
              printf("%s\n", str1);
              system(str1);
              printf(sep2 "\n");
            }
  	  }

          fclose(ff);
 	
          remove(str3);
        }
      }
    }
  }

  //Saving compilation data
  if (((ff=fopen(_results _stat_comp, "at"))!=NULL) && ((ff1=fopen(_tmp3, "w"))!=NULL))
  {
    fprintf(ff, sep1 "\n");

    fprintf(ff, stat_comp_COMPILE_ID "=%s\n", comp_id);
    fprintf(ff1, stat_comp_COMPILE_ID "=%s\n", comp_id);

    fprintf(ff, stat_comp_PLATFORM_ID "=%s\n", var_plat_id);
    fprintf(ff1, stat_comp_PLATFORM_ID "=%s\n", var_plat_id);

    fprintf(ff, stat_comp_ENVIRONMENT_ID "=%s\n", var_env_id);
    fprintf(ff1, stat_comp_ENVIRONMENT_ID "=%s\n", var_env_id);

    fprintf(ff, stat_comp_COMPILER_ID "=%s\n", var_compiler_id);
    fprintf(ff1, stat_comp_COMPILER_ID "=%s\n", var_compiler_id);

    fprintf(ff, stat_comp_PROGRAM_ID "=%s\n", var_program_id);
    fprintf(ff1, stat_comp_PROGRAM_ID "=%s\n", var_program_id);

    if (var_ccc_arch_size!=NULL)
    {
      fprintf(ff, stat_comp_ARCH_SIZE "=%s\n", var_ccc_arch_size);
      fprintf(ff1, stat_comp_ARCH_SIZE "=%s\n", var_ccc_arch_size);
    }

    if (var_ccc_arch_cfg!=NULL)
    {
      fprintf(ff, stat_comp_ARCH_CFG "=%s\n", var_ccc_arch_cfg);
      fprintf(ff1, stat_comp_ARCH_CFG "=%s\n", var_ccc_arch_cfg);
    }

    if (var_ccc_opt_fine!=NULL)
    {
      fprintf(ff, stat_comp_OPT_FINE "=%s\n", var_ccc_opt_fine);
      fprintf(ff1, stat_comp_OPT_FINE "=%s\n", var_ccc_opt_fine);
    }

    if (var_ccc_opt_par_static!=NULL)
    {
      fprintf(ff, stat_comp_OPT_PAR_STATIC "=%s\n", var_ccc_opt_par_static);
      fprintf(ff1, stat_comp_OPT_PAR_STATIC "=%s\n", var_ccc_opt_par_static);
    }

    curtime=time(NULL);
    loctime=localtime(&curtime);
    strftime(str1, 127, "%Y-%m-%d", loctime);
    fprintf(ff, stat_comp_DATE "=%s\n", str1);
    fprintf(ff1, stat_comp_DATE "=%s\n", str1);

    strftime(str1, 127, "%H:%M:%S", loctime);
    fprintf(ff, stat_comp_TIME "=%s\n", str1);
    fprintf(ff1, stat_comp_TIME "=%s\n", str1);

    fprintf(ff, stat_comp_OPT_FLAGS "=%s\n", trim(stroptmain));
    fprintf(ff1, stat_comp_OPT_FLAGS "=%s\n", trim(stroptmain));

    if (var_ccc_cts_user!=NULL)
    {
      fprintf(ff, stat_comp_USER "=%s\n", var_ccc_cts_user);
      fprintf(ff1, stat_comp_USER "=%s\n", var_ccc_cts_user);
    }

    fprintf(ff, stat_comp_IP "=%s\n", ip);
    fprintf(ff1, stat_comp_IP "=%s\n", ip);

    if ((var_baserun!=1))
    {
      fprintf(ff, stat_comp_OPT_FLAGS_PLATFORM "=%s\n", trim(var_ccc_plat_opts));
      fprintf(ff1, stat_comp_OPT_FLAGS_PLATFORM "=%s\n", trim(var_ccc_plat_opts));
    }
    
    fprintf(ff, stat_comp_COMPILE_TIME "=%f\n", tdiff);
    fprintf(ff1, stat_comp_COMPILE_TIME "=%f\n", tdiff);
    
    fprintf(ff, stat_comp_BIN_SIZE "=%u\n", fobjsize);
    fprintf(ff1, stat_comp_BIN_SIZE "=%u\n", fobjsize);
    
    fprintf(ff, stat_comp_OBJ_MD5CRC "=%s\n", fobjmd5);
    fprintf(ff1, stat_comp_OBJ_MD5CRC "=%s\n", fobjmd5);

    fprintf(ff, stat_comp_ICI_PASSES_USE "=%u\n", mode_ccc_ici_passes_use);
    fprintf(ff1, stat_comp_ICI_PASSES_USE "=%u\n", mode_ccc_ici_passes_use);

    fprintf(ff, stat_comp_ICI_FEATURES_STATIC_EXTRACT "=%u\n", mode_ccc_ici_features_st_extract);
    fprintf(ff1, stat_comp_ICI_FEATURES_STATIC_EXTRACT "=%u\n", mode_ccc_ici_features_st_extract);
    if (var_ccc_notes!=NULL)
    {
      fprintf(ff, stat_comp_NOTES "=%s\n", var_ccc_notes);
      fprintf(ff1, stat_comp_NOTES "=%s\n", var_ccc_notes);
    }
    
    fclose(ff1);
    fclose(ff);
  }

  //try to send stats
  if ((var_ccc_url!=NULL) && (strcmp(var_ccc_url,"")!=0))
  {
    printf(sep2 "\n");
    printf("Trying to send COMPILATION statistics to the Global Database ...\n\n");
    sprintf(str1, _send_stats_comp " %s", _tmp3);
    printf("%s\n", str1);
    system(str1);
    printf(sep2 "\n");
  }

  printf("\nProgram finished successfully!\n");
  
  exit(1);
}
