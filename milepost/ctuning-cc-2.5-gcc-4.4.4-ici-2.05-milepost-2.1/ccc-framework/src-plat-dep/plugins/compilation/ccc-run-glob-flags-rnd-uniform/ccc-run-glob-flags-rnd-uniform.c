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

static char opts[16384];
static int iopts[8192];
static int iused[8192];
static int ioptsm=0;

void glob_flags_read(char*);
void select_rnd_opt(char*);

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
  printf("CCC RUN RND UNIFORM (run program with random options)\n\n");

  if (argc<5)
  {
    printf("Usage:\n");
    printf(" ccc-run-globl-flags-rnd-uniform <Number of runs> <Compiler name> <Baseline opt> <Rnd seed> <Dataset>\n");

    return 1;
  }

  imax=atol(argv[1]);

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

  strcpy(comp_name, argv[2]);

  mds=atoi(argv[5]);

  sprintf(str1, "%s/" _glob_flags "%s" _glob_flags_e, var_ccc_cfg, comp_name);
  glob_flags_read(str1);

  //datasets
  istep=1;
  srand(atol(argv[4]));

  printf(sep1 "\n");
  //Compile with base-line
  while (istep<=imax)
  {
    setenv(CCC_BASERUN, "0", 1); 
    if ((istep==1) && (ivar_ccc_no_baserun==0))
    {
      strcpy(str_opt, argv[3]);
      setenv(CCC_BASERUN, "1", 1); 

      first=1;
    }
    else 
    {
      select_rnd_opt(str_opt);
      first=0;
    }

    printf(sep2 "\n");
    printf("Step: %u\n\n", istep);

    printf("Compiling with compiler %s and option %s\n", comp_name, trim(str_opt));
    sprintf(str2, _ccc_comp " %s \"%s\"", comp_name, trim(str_opt));
    fflush(NULL);
    system(str2);

    printf(sep2 "\n");
    printf("Running program with dataset %u, first time=%u\n", mds, first);
	    
    sprintf(str2, _ccc_run " %u %u", mds, first);
    fflush(NULL);
    system(str2);
  
    istep++;
  }

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

void select_rnd_opt(char* stropt)
{
  int i,j,k;
  int iv, iv1, iv_min, iv_max;
  int param;

  strcpy(stropt, "");
  for (i=0; i<ioptsm; i++)
  iused[i]=rand() % 2;

  //HACK TODO (currently always select first option (-Oxxx)
  iused[0]=1;
      
  for (i=0; i<ioptsm; i++)
  {
    if (iused[i]==1)
    {
      strcpy(str1, &opts[iopts[i]]);
	  	  
      strcpy(str2, "");
      if (str1[0]=='3')
      {
        int j=2;
        int k=0;
        strcpy(str3, "");
        for (; j<strlen(str1); j++)
        {
          if (str1[j]==',') break;
          else str3[k++]=str1[j];
        }
        if (j==strlen(str1))
        {
          printf("\nError: Inconsistent file with switches!\n");
          exit(1);
        }
        str3[k]=0;
        iv_min=0;
        iv_max=atoi(str3);
        j++;

        int iv1=iv_min + (rand() % iv_max);

        for (iv=iv_min; iv<iv_max; iv++)
    	{
          k=0;
          strcpy(str3, "");
          for (; j<strlen(str1); j++)
      	  {
      	    if (str1[j]==',') break;
            else str3[k++]=str1[j];
      	  }
          str3[k]=0;
    	  j++;
    	 
     	  if (strlen(str3)==0)
          {
            printf("\nError: Inconsistent file with switches!\n");
      	    exit(1);
    	  }    
        
	  if (iv>=iv1)
    	  {
    	    strcat(str2, str3);
    	    break;
    	  }
       	}
      }
      else if (str1[0]=='2')
      {
        copy_str(str2, str1, 2);
      }
      else if (str1[0]=='1')
      {
        int j=2;
        int k=0;
        strcpy(str3, "");
        for (; j<strlen(str1); j++)
        {
          if (str1[j]==',') break;
          else str3[k++]=str1[j];
	}
	if (j==strlen(str1))
	{
          printf("\nError: Inconsistent file with switches!\n");
  	  exit(1);
	}
        str3[k]=0;
        iv_min=atoi(str3);
        j++;

        k=0;
        strcpy(str3, "");
        for (; j<strlen(str1); j++)
	{
	  if (str1[j]==',') break;
          else str3[k++]=str1[j];
	}
	if (j==strlen(str1))
	{
          printf("\nError: Inconsistent file with switches!\n");
  	  exit(1);
	}
        str3[k]=0;
        iv_max=atoi(str3);
	j++;
 
        copy_str(str2, str1, j);

        iv=iv_min + (rand() % (rand_max)) * (iv_max-iv_min) / (rand_max-1);
	
	sprintf(str3, "%d", iv);
        strcat(str2, str3);
      }
      else if (str1[0]=='0')
      {
        copy_str(str2, str1, 2);
        param=rand() % rand_sel1;
        if (param==1) strcat(str2, "on");
        else          strcat(str2, "off");
      }
      else
      {
        printf("\nError: Inconsistent file with switches!\n");
        exit(1);
      }
        
      strcat(stropt, str2);
    }
  }
}
