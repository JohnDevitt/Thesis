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
#include "ccc/stat-run.h"
#include "ccc/futils.h"
#include "ccc/fuuid.h"

/* for gprof/oprof */
void parse_gprofile_info(char*, char*);
void parse_oprofile_info(char*, char*);

static char str1[8192];
static char str2[8192];
static char str3[8192];
static char str4[8192];
static char str5[8192];
static char str6[8192];
static char str7[8192];
static char str8[8192];
static char str9[8192];
static char str10[8192];
static char str11[8192];
static char ip[128];
static char comp_id[128];
static char var_prog_id[128];
static char run_id[128];
static char run_id_assoc[128];

int main(int argc, char* argv[])
{
  int first=0;
  int ds=0; //dataset

  time_t tstart;
  double tdiff;

  int i;

  int out_diff_cor=1;  

  char var_re_name[256];
  char var_re_id[256];

  FILE* ff;
  FILE* ff1;
  FILE* ff2;

  long fobjsize=0;

  static unsigned long ida=0;
  static unsigned long idb=0;

  char* var_ccc_cfg=NULL;
  char* var_plat_id=NULL;
  char* var_env_id=NULL;
  char* var_ccc_url=NULL;
  char* var_ccc_c_url=NULL;

  char* var_env=NULL;

  char* var_run_ssh=NULL;
  char* var_run_ssh_path=NULL;
  char* var_run_ssh_nfs=NULL;
  char* var_run_ssh_nfs_remove=NULL;

  char* var_run_time_background=NULL;
 
  char* var_hc_papi_use=NULL;
  char* var_hc_papi_out=NULL;

  char* var_ccc_cts_user=NULL;
  char* var_ccc_notes=NULL;
  char* var_ccc_par_dynamic=NULL;
  char* var_ccc_run_power=NULL;
  char* var_ccc_run_energy=NULL;

  char* var_ccc_run_re=NULL;
  char* var_oprof_param=NULL;

  char* var_ccc_proc_num=NULL;

  int var_gprof=0;
  int var_oprof=0;

  char var_compiler_id[128];
  char var_program_id[128];
  
  struct stat buf;

  double t1, t2, tt, tt1;
  
  time_t curtime;
  struct tm *loctime;

  int re_name_found=0;

  int runs=1;
  int qruns=0;
  int iruns=0;

  /* for gprof/oprof - not clean */
  #define prof_max 8192
  static char prof[prof_max];

  /********************************************************************/
  printf("CCC RUN (run program once and send stats to CCC server)\n\n");

  if ((var_env = getenv(CCC_NO_RUN)) != NULL)
    if (atoi(var_env)!=0)
    {
       printf("Exiting as instructed by environment variable %s ...\n", CCC_NO_RUN);
       exit(0);
    }

  if ((var_env = getenv(CCC_RUNS)) != NULL)
  {
    qruns=1;
    runs=atoi(var_env);
    if (runs<1)
    {
       printf("Error: environment variable %s should be >0 ...\n", CCC_RUNS);
       exit(0);
    }
  }

  if (argc<2)
  {
    printf("Usage:\n");
    printf(" ccc-run <Dataset> <First run (optional)>\n");

    return 1;
  }

  first=0;
  if (argc>2)
    first=atoi(argv[2]); //first run (0 or 1) to copy std files

  //Init
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

  var_run_ssh=getenv(CCC_RUN_SSH);
  var_run_ssh_path=getenv(CCC_RUN_SSH_PATH);
  var_run_ssh_nfs=getenv(CCC_RUN_SSH_NFS);
  var_run_ssh_nfs_remove=getenv(CCC_RUN_SSH_NFS_REMOVE);

  var_run_time_background=getenv(CCC_RUN_TIME_BACKGROUND);

  var_ccc_notes       = getenv(CCC_NOTES);
  var_ccc_cts_user    = getenv(CCC_CTS_USER);
  var_ccc_par_dynamic = getenv(CCC_PAR_DYNAMIC);
  var_ccc_run_power   = getenv(CCC_RUN_POWER);
  var_ccc_run_energy  = getenv(CCC_RUN_ENERGY);

  var_ccc_proc_num        = getenv(CCC_PROCESSOR_NUM);

  //Checking run-time environment
  strcpy(var_re_id,"");
  if ((var_ccc_run_re = getenv(CCC_RUN_RE)) != NULL)
  {
    printf("Searching run-time environment %s ...\n", var_ccc_run_re);

    if ((var_ccc_cfg = getenv(CCC_CFG)) == NULL)
    {
      printf("Error: Environment variable " CCC_CFG " is not defined!\n");
      exit(1);
    }

    sprintf(str2, "%s/" _fall_re, var_ccc_cfg);
    re_name_found=0;
    if ((ff=fopen(str2, "rt"))!=NULL)
    {
      strcpy(str1, "");
      while (fgets(str1, 1024, ff)!=NULL)
      {
        fparse1(str1);
        if (strncmp(str1, _re_cfg_name, strlen(_re_cfg_name))==0)
        {
          if (re_name_found==1) re_name_found=0;
  	  else if (strcmp(&str1[strlen(_re_cfg_name)], var_ccc_run_re)==0) re_name_found=1;
        }
        else if ((re_name_found==1) && (strncmp(str1, _re_id, strlen(_re_id))==0))
          strcpy(var_re_id, &str1[strlen(_re_id)]);
        else if ((re_name_found==1) && (strncmp(str1, _re_name, strlen(_re_name))==0))
          strcpy(var_re_name, &str1[strlen(_re_name)]);
      }
      fclose(ff);
    }
    else
    {
      printf("Error: Can't find run-time environment configuration file %s ...!\n", str2);
      exit(1);
    }

    if (strlen(var_re_id)==0)
    {
      printf("Error: Can't find " _re_id " in configuration file %s ...!\n", str2);
      exit(1);
    }

    printf("Found runtime environment information ...\n");
  }

  //Checking PAPI
  if ((var_hc_papi_use = getenv(CCC_HC_PAPI_USE)) != NULL)
  {
    if (strlen(var_hc_papi_use)==0)
    {
      printf("Error: Environment variable " CCC_HC_PAPI_USE " is defined but empty!\n");
      exit(1);
    }
  }

  if ((var_env = getenv(CCC_GPROF)) != NULL)
  {
    var_gprof=atoi(var_env);
  }

  if ((var_env = getenv(CCC_OPROF)) != NULL)
  {
    var_oprof=atoi(var_env);
  }

  var_oprof_param = getenv(CCC_OPROF_PARAM);

  //Get IP address
  get_ip(ip);

  //Read last compilation id
  ff = fopen (_fcomp_id, "r");
  if (ff!=NULL)
  {
    if (fgets(str1, 1023, ff)!=NULL)
    {
      fparse1(str1);
      strcpy(comp_id, str1);
    }
    else
    {
      printf("\nError: Can't read file with compilation id (%s)!\n", _fcomp_id);
      exit(1);
    }
    fclose(ff);
  }
  else
  {
    printf("\nError: Can't find file with compilation id (%s)!\n", _fcomp_id);
    exit(1);
  }

  //Read last compiler id
  ff = fopen (_fcmplr_id, "r");
  if (ff!=NULL)
  {
    if (fgets(str1, 1023, ff)!=NULL)
    {
      fparse1(str1);
      strcpy(var_compiler_id, str1);
    }
    else
    {
      printf("\nError: Can't read file with compiler id (%s)!\n", _fcmplr_id);
      exit(1);
    }
    fclose(ff);
  }
  else
  {
    printf("\nError: Can't find file with compiler id (%s)!\n", _fcmplr_id);
    exit(1);
  }

  //Read associate run_id if exist
  strcpy(run_id_assoc, "");
  if ((ff=fopen(_frun_a_id, "r"))!=NULL)
  {
    if (fgets(str1, 1023, ff)!=NULL)
    {
      fparse1(str1);
      strcpy(run_id_assoc, str1);
    }
    fclose(ff);
  }

  //Read program id
  ff = fopen (_fprog_id, "r");
  if (ff!=NULL)
  {
    if (fgets(str1, 1023, ff)!=NULL)
    {
      fparse1(str1);
      strcpy(var_program_id, str1);
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

  ds=atoi(argv[1]); //dataset

  mkdir(_results, 0777);

  printf("Prepare project ...\n");
  if (fileExist(_run2)==1)
  {
    sprintf(str1, _run2 " %u", ds);
    system(str1);
  }

  printf("Dataset: %u ...\n", ds);

  //executing
  fobjsize=0;
  if (fileExist(_fout)==1)
  {
    //filesize
    stat(_fout, &buf);
    fobjsize=buf.st_size;
    printf("File Size: %u\n", fobjsize);

    for (iruns=0; iruns<runs; iruns++)
    {
      printf("---------- Repeat the same run: %u out of %u ----------\n", iruns+1, runs);

      printf("Clean output ...\n");
      system(_diff1);
  
//      printf("\nCheck TOP processes ... \n", str1);
//      sprintf(str1, "top -n 1 >> "_results _stat_top_before);
//      system(str1);

//      printf("\nCheck CPU Info ... \n", str1);
//      sprintf(str1, "more -f /proc/cpuinfo > "_results _stat_cpuinfo_before);
//      system(str1);

      //execute with specific dataset
      //generate run id
      if (get_uuid(&ida, &idb)!=0)
      {
        printf("\nError: Can't get UUID from the system!\n");
        exit(1);
      }
      sprintf(run_id, "%u%u", ida, idb);

      /* Run program 1st time */
      if (var_gprof==1)
      {
        printf("Remove " _gprof_o " ...\n");
        system(_del " " _gprof_o);
      }

      if (var_run_ssh != NULL)
      {
        int v=0;
        int v1=0;
        int v2=0;

        strcpy(str5, getcwd(NULL, 0));
        if (var_run_ssh_nfs!=NULL && var_run_ssh_nfs_remove!=NULL)
        {
          strcpy(str4, str5);
 
          for (v=0; ((v<strlen(str4)) && (v2==0)); v++)
          {
            if (str4[v]=='/')
            {
              v1++;
              if (v1==atoi(var_run_ssh_nfs_remove))
                v2=v;
            }
          }
  
          if (v2!=0)
            sprintf(str5, "%s%s", var_run_ssh_nfs, &str4[v2]);
        }
  
        if (var_run_ssh_path == NULL)
        {
          sprintf(str1, " %s \"cd %s && %s %u\"", var_run_ssh, str5, _run, ds);
        }
        else
        {
          sprintf(str1, " %s \"cd %s && %s && %s %u\"", var_run_ssh, str5, var_run_ssh_path, _run, ds);
        }
      }
      else
      {
        sprintf(str1, _run " %u", ds);
      }
      strcpy(str5,str1);

      if (var_hc_papi_use!=NULL)
      {
        //Generate output file
        if (get_uuid(&ida, &idb)!=0)
        {
          printf("\nError: Can't get UUID from the system!\n");
          exit(1);
        }
        sprintf(str11, _del " " _tmp1 ".%u%u", ida, idb);

        system(str11);
        
        sprintf(str11, _tmp1 ".%u%u.tmp", ida, idb);

        setenv(CCC_HC_PAPI_OUT, str11, 1); 
      }

      system(_del " " _ftime);

      //Check if current compile time environment exists
      strcpy(str7, "");	
      if (fileExist(_fcomp_env_cur)==1)
      {
        if ((ff=fopen(_fcomp_env_cur, "r"))!=NULL)
        {
          fgets(str1, 1023, ff);
          fparse1(str1);
          sprintf(str7, ". %s;", str1);
          fclose(ff);
          printf("Compile time environment found - %s ...\n", str7);
        }
      }

      if (var_oprof==1)
      {
        /* check if bin file exist otherwise a.out */
        strcpy(str8, "$PWD/a.out");

        if (fileExist(_fprog_bin)==1)
        {
          if ((ff=fopen(_fprog_bin, "r"))!=NULL)
          {
            fgets(str8, 1023, ff);
            fparse1(str8);
            printf("Detected binary file: %s ...\n", str8);
          }
        }

        system(_oprof1);
        system(_oprof1r);

        strcpy(str1, _oprof2);
        if (var_oprof_param!=NULL) sprintf(str1, _oprof2 " %s", var_oprof_param);
        system(str1);

        sprintf(str1, _oprof3, str8); /* otherwise doesn't work with non a.out files */
        system(str1);
        system(_oprof4);
      }

      //Check processor number
      if (var_ccc_proc_num!=NULL)
      {
        sprintf(str10, _taskset, var_ccc_proc_num);
        setenv(CCC_TASKSET, str10, 1); 
        printf("Setting processor number to %s ...\n", var_ccc_proc_num);
      }

      strcpy(str6,"");
      if (strlen(var_re_id)!=0)
      {
        printf("Setting runtime environment name " CCC_RE " = %s\n", var_re_name);
	setenv(CCC_RE, var_re_name, 1);
        printf("\nSource run-time environment " CCC_ENV_RE ".%s if exists and run program %s !..\n", var_ccc_run_re, str5);
        sprintf(str6, ". %s/" CCC_ENV_RE ".%s; %s %s" , var_ccc_cfg, var_ccc_run_re, str7, str5);
      }
      else
      {
        printf("\nRun program %s ... \n", str5);
        sprintf(str6, "%s %s" , str7, str5);
      }

      //Run program
      system(str6);

      //Run postprocessing script if needed
      if (fileExist(_run3)==1)
      {
        sprintf(str1, _run3 " %u", ds);
        system(str1);
      }

      if (var_oprof==1)
      {
        printf("\nObtaining OProfile distribution ...\n");
        system(_del " " _tmp1);

        /* check if bin file exist otherwise a.out */
        strcpy(str8, "$PWD/a.out");

        if (fileExist(_fprog_bin)==1)
        {
          if ((ff=fopen(_fprog_bin, "r"))!=NULL)
          {
            fgets(str8, 1023, ff);
            fparse1(str8);
            printf("Detected binary file: %s ...\n", str8);
          }
        }

//        system(_oprof5);
        system(_oprof6);
        sprintf(str1, _oprof7, str8);
	system(str1);
        system(_oprof8);

        //Parsing profile info
        strcpy(prof, "");
        parse_oprofile_info(prof, _tmp1);
      }


      t1=getTime1(_ftime);
      t2=getTime2(_ftime);
      tt=t1+t2;
      printf("Run Time: user=(%6.1f), sys=(%6.1f), total=(%6.1f)\n", t1, t2, tt);

      if (var_gprof==1)
      {
        printf("\nObtaining GProfile distribution ...\n");
        system(_del " " _tmp1);
        system(_gprof);

        //Parsing profile info
        strcpy(prof, "");
        parse_gprofile_info(prof, _tmp1);
      }

      tt1=0;

      if (first==1)
      {
        if (iruns==0)
        {
          //copy std output files
          out_diff_cor=1;
          printf("Copy original output ...\n");
          system(_diff2);

          //Saving associate run_id
          strcpy(run_id_assoc, run_id);
          if ((ff=fopen(_frun_a_id, "w"))!=NULL)
          {
            fprintf(ff, "%s\n", run_id_assoc);
            fclose(ff);
          }
        }
      }
      else
      {
        //compare outputs
        printf("Compare outputs ...\n");
        if (fileDiffCorrect(_diff3, _ftmp_diff)!=1)
        {
          out_diff_cor=0;
          printf("Warning: Output is different from the original!\n");
        }
      }

      //Saving execution data
      if (((ff=fopen(_results _stat_run, "at"))!=NULL) && ((ff1=fopen(_tmp3, "w"))!=NULL))
      {
        fprintf(ff, sep1 "\n");

        fprintf(ff, stat_run_RUN_ID "=%s\n", run_id);
        fprintf(ff1, stat_run_RUN_ID "=%s\n", run_id);
 
        fprintf(ff, stat_run_RUN_ID_ASSOCIATE "=%s\n", run_id_assoc);
        fprintf(ff1, stat_run_RUN_ID_ASSOCIATE "=%s\n", run_id_assoc);
 
        fprintf(ff, stat_run_COMPILE_ID "=%s\n", comp_id);
        fprintf(ff1, stat_run_COMPILE_ID "=%s\n", comp_id);

        fprintf(ff, stat_run_COMPILER_ID "=%s\n", var_compiler_id);
        fprintf(ff1, stat_run_COMPILER_ID "=%s\n", var_compiler_id);

        fprintf(ff, stat_run_PLATFORM_ID "=%s\n", var_plat_id);
        fprintf(ff1, stat_run_PLATFORM_ID "=%s\n", var_plat_id);

        fprintf(ff, stat_run_ENVIRONMENT_ID "=%s\n", var_env_id);
        fprintf(ff1, stat_run_ENVIRONMENT_ID "=%s\n", var_env_id);

        fprintf(ff, stat_run_PROGRAM_ID "=%s\n", var_program_id);
        fprintf(ff1, stat_run_PROGRAM_ID "=%s\n", var_program_id);

        if (strlen(var_re_id)!=0)
        {
          fprintf(ff, stat_run_RE_ID "=%s\n", var_re_id);
          fprintf(ff1, stat_run_RE_ID "=%s\n", var_re_id);
        }
	
        fprintf(ff, stat_run_HOST_IP "=%s\n", ip);
        fprintf(ff1, stat_run_HOST_IP "=%s\n", ip);

        curtime=time(NULL);
        loctime=localtime(&curtime);
        strftime(str1, 127, "%Y-%m-%d", loctime);
        fprintf(ff, stat_run_DATE "=%s\n", str1);
        fprintf(ff1, stat_run_DATE "=%s\n", str1);

        strftime(str1, 127, "%H:%M:%S", loctime);
        fprintf(ff, stat_run_TIME "=%s\n", str1);
        fprintf(ff1, stat_run_TIME "=%s\n", str1);

        //Read dataset
        strcpy(str1, "");
        strcpy(str2, "");
        ff2 = fopen (_dataset, "r");
        if (ff2!=NULL)
        {
          if (fgets(str1, 1023, ff2)!=NULL)
          {
            fparse1(str1);
          }
          else
            printf("\nWarning: Can't read file with dataset!\n");
          fclose(ff2);
        }
        else
          printf("\nWarning: Can't find file with dataset!\n");
        fprintf(ff, stat_run_RUN_COMMAND_LINE "=%u) %s\n", ds, str1);
        fprintf(ff1, stat_run_RUN_COMMAND_LINE "=%u) %s\n", ds, str1);

        fprintf(ff, stat_run_BIN_SIZE "=%u\n", fobjsize);
        fprintf(ff1, stat_run_BIN_SIZE "=%u\n", fobjsize);

        fprintf(ff, stat_run_OUTPUT_CORRECT "=%u\n", out_diff_cor);
        fprintf(ff1, stat_run_OUTPUT_CORRECT "=%u\n", out_diff_cor);

        fprintf(ff, stat_run_RUN_TIME "=%f\n", tt);
        fprintf(ff1, stat_run_RUN_TIME "=%f\n", tt);

        fprintf(ff, stat_run_RUN_TIME1 "=%f\n", tt1);
        fprintf(ff1, stat_run_RUN_TIME1 "=%f\n", tt1);

        fprintf(ff, stat_run_RUN_TIME_USER "=%f\n", t1);
        fprintf(ff1, stat_run_RUN_TIME_USER "=%f\n", t1);

        fprintf(ff, stat_run_RUN_TIME_SYS "=%f\n", t2);
        fprintf(ff1, stat_run_RUN_TIME_SYS "=%f\n", t2);

        if (var_gprof==1 || var_oprof==1)
	{
          fprintf(ff, stat_run_RUN_PG "=%s\n", prof);
          fprintf(ff1, stat_run_RUN_PG "=%s\n", prof);
        }
 
        if (var_hc_papi_use!=NULL)
        {
          strcpy(str1, "");
          ff2 = fopen (str11, "r");
          if (ff2!=NULL)
          {
            if (fgets(str1, 1023, ff2)!=NULL)
              fparse1(str1);
            fclose(ff2);
          }

          if (strlen(str1)!=0)
	  {
            fprintf(ff, stat_run_RUN_HC "=%s\n", str1);
            fprintf(ff1, stat_run_RUN_HC "=%s\n", str1);
	  }
        }

        if (var_run_time_background != NULL)
	{
          fprintf(ff, stat_run_RUN_TIME_BACKGROUND "=%s\n", var_run_time_background);
          fprintf(ff1, stat_run_RUN_TIME_BACKGROUND "=%s\n", var_run_time_background);
	}

        if (var_ccc_notes!=NULL)
        {
          fprintf(ff, stat_run_NOTES "=%s\n", var_ccc_notes);
          fprintf(ff1, stat_run_NOTES "=%s\n", var_ccc_notes);
        }
        
        if (var_ccc_cts_user!=NULL)
        {
          fprintf(ff, stat_run_USER "=%s\n", var_ccc_cts_user);
          fprintf(ff1, stat_run_USER "=%s\n", var_ccc_cts_user);
        }

        if (var_ccc_par_dynamic!=NULL)
        {
          fprintf(ff, stat_run_PAR_DYNAMIC "=%s\n", var_ccc_par_dynamic);
          fprintf(ff1, stat_run_PAR_DYNAMIC "=%s\n", var_ccc_par_dynamic);
        }

        if (var_ccc_run_power!=NULL)
        {
          fprintf(ff, stat_run_POWER "=%s\n", var_ccc_run_power);
          fprintf(ff1, stat_run_POWER "=%s\n", var_ccc_run_power);
        }

        if (var_ccc_run_energy!=NULL)
        {
          fprintf(ff, stat_run_ENERGY "=%s\n", var_ccc_run_energy);
          fprintf(ff1, stat_run_ENERGY "=%s\n", var_ccc_run_energy);
        }

        if (var_ccc_proc_num!=NULL)
        {
          fprintf(ff, stat_run_PROCESSOR_NUM "=%s\n", var_ccc_proc_num);
          fprintf(ff1, stat_run_PROCESSOR_NUM "=%s\n", var_ccc_proc_num);
        }

        fclose(ff1);
        fclose(ff);
      }

      //try to send stats
      if ((var_ccc_url!=NULL) && (var_ccc_c_url!=NULL) && (strcmp(var_ccc_url,"")!=0))
      { 
        printf(sep2 "\n");
        printf("Trying to send RUN statistics to the Global Database ...\n");
        sprintf(str1, _send_stats_run " %s", _tmp3);
        printf("%s\n", str1);
        system(str1);
        printf(sep2 "\n");
      }
    }
  }

  printf("\nProgram finished successfully!\n");
  
  exit(1);
}

void parse_gprofile_info(char* prof, char* filename)
{
  int finish=0;
  int found=0;
  int i=0;
  FILE* ff;
  double tp,xt1,xt2,xt3,xt4,xt5;
  long cl;

  //Parsing profile info
  strcpy(prof, "");
  if ((ff=fopen(filename, "r"))!=NULL)
  {
    finish=0;
    found=0;
    i=0;
    while ((fgets(str1, 1023, ff)!=NULL) && (finish==0))
    {
      fparse1(str1);
      tp=atof(str1);
      if (tp>1)
      {
        found=1;

        sscanf(str1, "%lf %lf %lf %lu %lf %lf %s", &xt1, &xt2, &xt3, &cl, &xt4, &xt5, str2);
	  
	sprintf(str3, "{%s=%4.2f,%lu,%4.10f}", str2, xt3, cl, (double) xt3/cl);
	  
        if (strlen(prof)+strlen(str3)>prof_max) finish=1;
	else
	{
          strcat(prof, str3);
    	  i++;
	  if (i>32) finish=1;
	}
      }
      else if (found==1) finish=1;
    }

    fclose(ff);
  }
}

void parse_oprofile_info(char* prof, char* filename)
{
  int finish=0;
  int found=0;
  int i=0;
  int j=0;
  FILE* ff;
  double tp, xt2;
  unsigned long xt1;

  //Parsing profile info
  strcpy(prof, "");
  if ((ff=fopen(filename, "r"))!=NULL)
  {
    finish=0;
    found=0;
    i=0;
    while ((fgets(str1, 1023, ff)!=NULL) && (finish==0))
    {
      fparse1(str1);
      tp=atof(str1);
      if (tp>1)
      {
        found=1;

        sscanf(str1, "%lu %lf %s %s", &xt1, &xt2, str2, str9);

        for (j=0; j<strlen(str2); j++)
	  if (str2[j]=='(') {str2[j]=0; break;}
        for (j=0; j<strlen(str9); j++)
	  if (str9[j]=='(') {str9[j]=0; break;}
	  
        if (strcmp(str9,"")==0) 
  	  sprintf(str3, "{%s=%lu}", str2, xt1);
	else
  	  sprintf(str3, "{%s=%lu}", str9, xt1);
	  
        if (strlen(prof)+strlen(str3)>=prof_max) finish=1;
	else
	{
          strcat(prof, str3);
    	  i++;
	  if (i>32) finish=1;
	}
      }
      else if (found==1) finish=1;
    }

    fclose(ff);
  }
}
