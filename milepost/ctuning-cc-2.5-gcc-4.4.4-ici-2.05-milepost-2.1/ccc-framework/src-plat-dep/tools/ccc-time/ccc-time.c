/*
 * Copyright (C) 2000-2009 by Grigori Fursin
 *
 * http://fursin.net/research
 *
 * UNIDAPT Group
 * http://unidapt.org
 */

#include "ccc/futils.h"

#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#ifndef W32
#  include <sys/resource.h> //for cpu usage
#endif
#include <time.h>

#include "ccc/ccc.h"

int papi_process(char* fin, char* fout, int add_comma);
void fparse1(char* str);

int main(int argc, char* argv[])
{
  long misc;
  int x=0;

  char str1[32000]="";
  char str2[32000]="";
  char str3[32000]="";
  char str4[32000]="";
  char str5[32000]="";
  char str6[1024]="";
  char str7[1024]="";
  char str8[1024]="";

#ifdef W32
  char fe[1024]="a.exe";
#else
  char fe[1024]="./a.out";
#endif
  char fp[1024]="";
  char ft[1024]="ccc-time.tmp";

  FILE* ff=NULL;
  FILE* ff1=NULL;
  FILE* ff2=NULL;
  
  double clk=CLOCKS_PER_SEC;
  double user, sys;
#ifdef W32
  clock_t start, finish;
#else
  struct rusage	childusage;
#endif

  char* var_hc_papi_use=NULL;
  char* var_hc_papi_out=NULL;

  //Parse command line
  for (x=1; x<argc; x++)
  {
    if ((strcmp(argv[x], "-h")==0) || (strcmp(argv[x], "--help")==0))
    {
      printf("\n");
      printf("CCC Time (similar to native time but used with CCC Framework)\n");
      printf("\n");
      printf("Copyright (C) 2000-2009 by Grigori Fursin\n");
      printf("\n");
      printf("http://fursin.net/research\n");
      printf("\n");
      printf("Alchemy group, INRIA Futurs, France\n");
      printf("\n");
      printf("Options:\n");
      printf("  --help, -h, -?               Help\n");
      printf("  --version, -v                Print version\n");
#ifdef W32
      printf("  --file_exe, -fe <filename>   File to execute [Default=a.exe]\n");
#else
      printf("  --file_exe, -fe <filename>   File to execute [Default=./a.out]\n");
#endif
      printf("  --file_param, -fp <filename> File with command line\n");
      printf("  --file_time, -ft <filename>  File with time [Default=ccc-time.tmp]\n");

      return 0;
    }
    else if ((strcmp(argv[x], "-v")==0) || (strcmp(argv[x], "--version")==0))
    {
      printf("\n");
      printf("Version information:\n");
      printf("  Version 1.03, rewritten from C++ to C on 2009.April.29\n");
      printf("  Version 1.02, released on 2000.Nov.10\n");
      printf("                modified for CCC on 2007.Aug.01\n");

      return 0;
    }
    else if ((strcmp(argv[x], "--file_exe")==0) || (strcmp(argv[x], "-fe")==0))
    {
      if ((x+1)<argc) strcpy(fe, argv[++x]);
      else
      {
        printf("Error: wrong flag settings!\n");
        return 1;
      }
    }
    else if ((strcmp(argv[x], "--file_param")==0) || (strcmp(argv[x], "-fp")==0))
    {
      if ((x+1)<argc) strcpy(fp, argv[++x]);
      else
      {
        printf("Error: wrong flag settings!\n");
        return 1;
      }
    }
    else if ((strcmp(argv[x], "--file_time")==0) || (strcmp(argv[x], "-ft")==0))
    {
      if ((x+1)<argc) strcpy(ft, argv[++x]);
      else
      {
        printf("Error: wrong flag settings!\n");
        return 1;
      }
    }
    else
    {
      printf("Error: unknown command line flag!\n");
      return 1;
    }
  }

  //Checking PAPI
  if ((var_hc_papi_use = getenv(CCC_HC_PAPI_USE)) != NULL)
  {
    if (strlen(var_hc_papi_use)==0)
    {
      printf("Error: Environment variable " CCC_HC_PAPI_USE " is defined but empty!\n");
      exit(1);
    }

    if ((var_hc_papi_out = getenv(CCC_HC_PAPI_OUT)) == NULL)
    {
      printf("Error: Environment variable " CCC_HC_PAPI_OUT " is not defined!\n");
      exit(1);
    }
  }

  //Reading command line (parameter)
  strcpy(str1, "");
  if (strlen(fp)!=0)
  {
    ff=fopen(fp, "r");
    if (ff==NULL)
    {
      printf("Error: Can't open file %s with command line\n", fp);
      return 1;
    }

    if (fgets(str1, 8191, ff)==NULL)
    {
      printf("Error: Can't read from file %s with command line\n", fp);
      return 1;
    }

    fclose(ff);
  }

  strcpy(str2, fe);
  strcat(str2, " ");
  strcat(str2, str1);
  
  printf("Running application:\n");
  printf("%s\n", str2);
  
#ifdef W32
  strcpy(str1, "start /b /high /wait ");
  strcat(str1, str2);
  strcpy(str2, str1);

  start=clock();
  system(str2);
  finish=clock();

  user = (double)(finish - start) / clk;
  sys  =  0;
#else

  // Check if PAPI is used 
  if (var_hc_papi_use!=NULL)
  {
    int mul=0;
    int comma=0;
    int mul_max=16;
    int il=strlen(var_hc_papi_use);

    sprintf(str6, "%s.1.tmp", var_hc_papi_out);
    sprintf(str7, "%s.2.tmp", var_hc_papi_out);

    sprintf(str4, "rm -f %s", var_hc_papi_out);
    system(str4);

    //First process events without 'x', i.e. that can be multiplexed
    strcpy(str4, "papiex -m ");
    printf("\n");

    strcpy(str3, "");
    int i=0;
    int j=0;
    int finish=0;
    while (finish==0)
    {
      if ((i<il) && (var_hc_papi_use[i]!=','))
      {
        str3[j++]=var_hc_papi_use[i++];
      }
      else
      {
        if (i>=il) finish=1;
        i++;
        str3[j]=0;
        if ((j>0) && (str3[j-1]!='x'))
        {
          printf("Adding PAPI event %s to monitor ...\n", str3);
      
          strcat(str4, " -e ");
          strcat(str4, str3);

          mul++;
        }
	
        if ((finish==1) || (mul>=mul_max))
        {
          mul=0;

          strcat(str4, " -q -o ");
	  strcat(str4, str6);
	  strcat(str4, " -- ");
          strcat(str4, str2);

          if ((ff1=fopen(str7, "w"))==NULL)
          {
            printf("ERROR: Can't open file to write tmp CMD ...\n");
            exit(1);
          }

          fprintf(ff1, "%s\n", str4);
          fclose(ff1);

          remove(str6);

          sprintf(str8, "chmod 755 %s", str7);
          system(str8);
          system(str7);

          if (papi_process(str6, var_hc_papi_out, comma)<0)
          {
            printf("ERROR: Can't process output for PAPI events ...");
            exit(1);
          }
          if (comma==0) comma=1;

          strcpy(str4, "papiex -m ");
          printf("\n");
        }
        strcpy(str3,"");
	j=0;
      }
    }

    //Then process events with 'x', i.e. that can not be multiplexed
    strcpy(str4, "papiex ");
    printf("\n");

    strcpy(str3, "");
    i=0;
    j=0;
    finish=0;
    while (finish==0)
    {
      if ((i<il) && (var_hc_papi_use[i]!=','))
      {
        str3[j++]=var_hc_papi_use[i++];
      }
      else
      {
        if (i>=il) finish=1;
        i++;
        str3[j]=0;
        if ((j>0) && (str3[j-1]=='x'))
        {
          str3[j-1]=0;
	  
          printf("Adding unique PAPI event %s to monitor ...\n", str3);
      
          strcat(str4, " -e ");
          strcat(str4, str3);

          strcat(str4, " -q -o ");
	  strcat(str4, str6);
	  strcat(str4, " -- ");
          strcat(str4, str2);

          if ((ff1=fopen(str7, "w"))==NULL)
          {
            printf("ERROR: Can't open file to write tmp CMD ...\n");
            exit(1);
          }

          fprintf(ff1, "%s\n", str4);
          fclose(ff1);

          remove(str6);

          sprintf(str8, "chmod 755 %s", str7);
          system(str8);
          system(str7);

          if (papi_process(str6, var_hc_papi_out, comma)<0)
	  {
            printf("ERROR: Can't process output for PAPI events ...");
	    exit(1);
	  }
          if (comma==0) comma=1;

          strcpy(str4, "papiex ");
          printf("\n");
        }
        strcpy(str3,"");
	j=0;
      }
    }

  }
  else
  {
    system(str2);
  }
  
  //Getting user & system time
  if (getrusage(RUSAGE_CHILDREN, &childusage) < 0)
  {
    printf("\nError: Can't use function 'getrusage'!\n");
    return 1;  
  }

  user = (double) childusage.ru_utime.tv_sec +
          childusage.ru_utime.tv_usec/clk;
  sys =  (double) childusage.ru_stime.tv_sec +
          childusage.ru_stime.tv_usec/clk;
#endif

  printf("\n");
  printf("User   time: %f s.\n", user);
  printf("System time: %f s.\n", sys);

  //Saving time to file
  if ((ff=fopen(ft, "w"))==NULL)
  {
    printf("\nError: Can't open file to write time!\n");
    return 1;  
  }

  fprintf(ff, "%f\n", user);
  fprintf(ff, "%f\n", sys);

  fclose(ff);

  printf("\nProgram finished successfully!\n");
  return 0;
}

int papi_process(char* fin, char* fout, int add_comma)
{
  char strx[1024];
  char stry[1024];
  FILE* ffx;
  FILE* ffy;
  int ret=0;
  int i=0;

  strcpy(strx, "");
  if ((ffx=fopen(fin, "r"))==NULL)
  {
    return -1;      
  }
  if ((ffy=fopen(fout, "a"))==NULL)
  {
    fclose(ffx);
    return -2;      
  }
    
  if (fgets(strx, 1024, ffx)!=NULL)
  {
    if (fgets(strx, 1024, ffx)!=NULL)
    {
      if (fgets(strx, 1024, ffx)!=NULL)
      {
        if (fgets(strx, 1024, ffx)!=NULL)
        {
          if (fgets(strx, 1024, ffx)!=NULL)
          {
            while (fgets(strx, 1024, ffx)!=NULL)
            {
              fparse1(strx);
              strcpy(stry, strx);
              for (i=0; i<strlen(strx); i++)
              {
                if (strx[i]==' ')
                {
                  stry[i]=0;
                  for (; i<strlen(strx); i++)
                  {
                    if (strx[i]!=' ')
  		    {
                      if (add_comma==1) fprintf(ffy, ","); else add_comma=1;
                      fprintf(ffy, "%s=%s", &strx[i], stry);
		      break;
		    }
                  }
		
  	          break;
		}
	      }
	    }
	  }
        }
      }
    }
  }

  fclose(ffy);
  fclose(ffx);

  return ret;
}
