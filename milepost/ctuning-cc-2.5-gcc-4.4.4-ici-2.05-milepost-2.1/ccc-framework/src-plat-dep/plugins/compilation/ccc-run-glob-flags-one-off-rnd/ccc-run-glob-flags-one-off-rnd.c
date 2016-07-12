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

#define rand_max  8
#define rand_sel1 2

static char str1[8192];
static char str2[8192];
static char str3[8192];
static char ip[128];
static char comp_id[128];
static char run_id[128];
static char run_id_assoc[128];
static char str_opt[8192];
static char fobjmd5[255]="";
static char fobjmd5_base[255]="";

static char opts[16384];
static int iopts[8192];
static int iused[8192];
static int itried[8192];
static int ioptsm=0;

void glob_flags_parse(char*);
int select_one_off_rnd_opt(char*, int);

int main(int argc, char* argv[])
{
  int first=0;
  int ds=0; //dataset

  time_t tstart;
  double tdiff;

  int i;

  int out_diff_cor=1;  

  FILE* ff;
  FILE* ff1;
  FILE* ff2;

  static unsigned int ida, idb;

  char* var_ccc_cfg=NULL;
  char* var_ccc_no_baserun=NULL;
  int ivar_ccc_no_baserun=0;

  unsigned long var_compiler_id;
  unsigned long var_program_id;
  
  double t1, t2, tt, tt1;
  double dt;
  
  time_t curtime;
  struct tm *loctime;

  char main_opts[1024];
  char comp_name[1024];
  int mds=0;
  int ipos=0;
  int ipos1=0;
  int ipos2=0;
  int cc=0;

  long istep=0;
  long imax=0;

  /********************************************************************/
  printf("CCC RUN ONE OFF RND (run program with random options)\n\n");

  if (argc<7)
  {
    printf("Usage:\n");
    printf(" ccc-run-glob-flags-one-off-rnd \"Compiler flags\" <Compiler name> <Baseline opt> <Rnd seed> <Time diff tolerance> <Dataset>\n");

    return 1;
  }

  //Init
  if ((var_ccc_cfg = getenv(CCC_CFG)) == NULL)
  {
    printf("Error: Environment variable " CCC_CFG " is not defined!\n");
    exit(1);
  }

  if ((var_ccc_no_baserun = getenv(CCC_NO_BASERUN)) != NULL)
  {
    ivar_ccc_no_baserun=atoi(getenv(CCC_NO_BASERUN));
  }

  dt=atof(argv[5]);

  strcpy(comp_name, argv[2]);

  glob_flags_parse(argv[1]);
  imax=ioptsm+3;

  mds=atoi(argv[6]);

  //datasets
  istep=1;
  srand(atol(argv[4]));

  printf(sep1 "\n");
  while (istep<=imax)
  {
    printf(sep2 "\n");
    printf("Step: %u \n", istep);

    int change=-1;
    setenv(CCC_BASERUN, "0", 1); 
    if ((istep==1) && (ivar_ccc_no_baserun==0))
    {
      strcpy(str_opt, argv[3]);
      setenv(CCC_BASERUN, "1", 1); 

      first=1;
    }
    else if (istep==imax)
    {
      printf("\n\nFinal compiler flags selection:\n");
      strcpy(str_opt, "");
      int i=0;
      for (i=0; i<ioptsm; i++)
      {
        if (iused[i]==1)
        {
          strcat(str_opt, &opts[iopts[i]]);
          strcat(str_opt, " ");
        }
      }
      printf("%s\n\n", str_opt);
    }
    else 
    {
      change=select_one_off_rnd_opt(str_opt, istep-1);
      first=0;
      if (change>=0) printf("Removing option %s ...\n\n", &opts[iopts[change]]);
    }

    fflush(NULL);
    system(_del " " _fout);
    printf("Compiling with compiler %s and option %s\n\n", comp_name, trim(str_opt));
    sprintf(str2, _ccc_comp " %s \"%s\"", comp_name, trim(str_opt));
    fflush(NULL);
    system(str2);

    if (fileExist(_fobjmd5)==1)
    {
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

      if ((strlen(fobjmd5)>0) && (istep==1))
      {
        strcpy(fobjmd5_base, fobjmd5);
      }
    }

    printf("\nBASELINE OBJ MD5=%s\n", fobjmd5_base);
    printf(" CURRENT OBJ MD5=%s\n", fobjmd5);

    if ((strlen(fobjmd5)>0) && (strcmp(fobjmd5,fobjmd5_base)==0) && (istep>1) && (istep!=imax))
    {
      printf("\nOBJ MD5 = BASELINE OBJ MD5 after removing option %s ...\n", &opts[iopts[change]]);
    }
    else
    {
      printf("\n\n");
      printf("Running program with dataset %u, first time=%u\n", mds, first);
	    
      sprintf(str2, _ccc_run " %u %u", mds, first);
      fflush(NULL);
      system(str2);
	
      //Read time
      t1=getTime1(_ftime);
      t2=getTime2(_ftime);
      tt=t1+t2;
 
      if (istep==2) tt1=tt;
      else if ((istep!=imax) && (istep!=1))
      {
        if ((tt>(tt1+dt)) || (tt<1.0))
        {
          iused[change]=1;
          printf("\n Option %s is important, keep it ...\n", &opts[iopts[change]]);
        }
        else if (tt<tt1-dt) 
        {
          tt1=tt;
          printf("\n Base line exec.time improved after removing option %s ...\n", &opts[iopts[change]]);
	  strcpy(fobjmd5_base, fobjmd5);
        }
        else 
        {
          printf("\n Exec.time didn't change after removing option %s ...\n", &opts[iopts[change]]);
          strcpy(fobjmd5_base, fobjmd5);
	}
      }
    }
  
    istep++;
  }

  printf("\n\nFinal compiler flags selection:\n");
  printf("%s\n\n", str_opt);

  printf("\nProgram finished successfully!\n");
  
  return 0;
}

void glob_flags_read(char* name)
{
  FILE* ff;
  int ix=0;
  
  ioptsm=0;
  ff = fopen (name, "r");
  if (ff!=NULL)
  {
    while ((fgets(str1, 1023, ff)!=NULL) && (feof(ff)==0))
    {
      fparse1(str1);

      if (strlen(str1)==0)
      {
        printf("\nError: Inconsistent file with compiler optimization flags!\n");
        exit(1);
      }

      strcpy(&opts[ix], str1);
      iopts[ioptsm]=ix;
      ioptsm++;
      ix+=strlen(str1)+1;
    }
    fclose (ff);
  }
  else
  {
    printf("Error: Can't find file with compiler optimization flags!\n");
    exit(1);
  }
}

void glob_flags_parse(char* optstr)
{
  int ix=0;
  ioptsm=0;

  int il=strlen(optstr);
  int j=0;
  int finish=0;
  
  while (j<il)
  {
    iopts[ioptsm++]=ix;

    finish=0;
    while ((j<il) && (finish==0))
    {
      if (optstr[j]==' ')
      {
        //Check exceptions for combinations of optimizations
	if (strcasecmp(&opts[iopts[ioptsm-1]], "--param")==0)
	  finish=0;
	else
          finish=1;
      }

      if (finish==0)
        opts[ix++]=optstr[j++];
    }

    j++;
    opts[ix++]=0;
  }

  for (ix=0; ix<ioptsm; ix++)
    printf("%u '%s'\n", ix, &opts[iopts[ix]]);
}


int select_one_off_rnd_opt(char* stropt, int step)
{
  int i,j,k;
  int iv, iv1, iv_min, iv_max;
  int param;

  j=-1;
  strcpy(stropt, "");

  if (step==1)
  {
    for (i=0; i<ioptsm; i++)
    {
      iused[i]=1;
      itried[i]=0;
    }
  }
  else
  {
    int found=0;
    while (found==0)
    {
      j=rand() % ioptsm;
      if (itried[j]==0)
      {
        itried[j]=1;
	iused[j]=0;
	found=1;
      }
    }
  }

  for (i=0; i<ioptsm; i++)
  {
    if (iused[i]==1)
    {
      strcat(stropt, &opts[iopts[i]]);
      strcat(stropt, " ");
    }
  }

  return j;
}
