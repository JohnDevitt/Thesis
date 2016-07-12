/*
 * Copyright (C) 2000-2009 by Grigori Fursin
 *
 * http://fursin.net/research
 *
 * UNIDAPT Group
 * http://unidapt.org
 */

#include "param.h"

#include "global.h"
#include "clparse.h"

#include <string.h>

#include <fstream.h>
#include <iostream.h>

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
  char FileOpt[]="ccc-time.cl";

  char str1[32000]="";
  char str2[32000]="";
  char str3[32000]="";
  char str4[32000]="";
  char str5[32000]="";
  char str6[1024]="";
  char str7[1024]="";
  char str8[1024]="";

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

  //***********************************************************************
  cout.setf(ios::unitbuf);

  //Declaration of class with global variables
  CGlobal* glob = new CGlobal();
    
  //Declaration of class for command-line parsing
  glob->clp = new CCLParse();
     
  if (glob->clp->Init(10, 32, 256, 1, FileOpt)!=0)
  //                   |   |    |  |  | 
  //                   |   |    |  |  |---- Name of File with Options
  //                   |   |    |  |
  //                   |   |    |  |------- Case Sensitive (0) or not (1)
  //                   |   |    |
  //                   |   |    |---------- Max Length of Parameter
  //                   |   |
  //                   |   |--------------- Max Length of Option
  //                   |
  //                   |------------------- Max Number of Options
  {
    cerr << "\nError: Can't initialize object CCLParse!\n";
    return 1;
  }

  misc=0;
  
  misc =glob->clp->Add("--help",       "-h",  "-?", "", 0);             //Param 0
  misc+=glob->clp->Add("--version",    "-v",  "",   "", 0);             //Param 1
#ifdef W32
  misc+=glob->clp->Add("--file_exe",   "-fe", "",   "a.exe", 1);        //Param 2
#else
  misc+=glob->clp->Add("--file_exe",   "-fe", "",   "./a.out", 1);      //Param 2
#endif
  misc+=glob->clp->Add("--file_param", "-fp", "",   "", 1);             //Param 3
  misc+=glob->clp->Add("--file_time",  "-ft", "",   "ccc-time.tmp", 1); //Param 4

  if (misc!=0)
  {
    cerr << "\nError: Internal, while initializing CCLParse!\n";
    return 1;
  }

  misc=glob->clp->Parse(argc, argv);
  if (misc==1)
  {
    cerr << "\nError: Unknown option in command line!\n";
    return 1;
  }
  else if (misc==2)
  {
    cerr << "\nError: Option can't be used without parameter in command line!\n";
    return 1;
  }
  else if (misc==3)
  {
    cerr << "\nError: Option can't be used without parameter in file with options!\n";
    return 1;
  }
  else if (misc==4)
  {
    cerr << "\nError: Unknown option in file with options!\n";
    return 1;
  }
  else if (misc>0)
  {
    cerr << "\nError: Internal, while parsing command line!\n";
    return 1;
  }

  if (glob->clp->GetParamLong(paramVersion)==1)
  {
    cout << "\n";
    cout << "Version information:\n";
    cout << "  Version 1.02, released on 2000.Nov.10\n";
    cout << "                modified for CCC on 2007.Aug.01\n";

    return 0;
  }

  if (glob->clp->GetParamLong(paramHelp)==1)
  {
    //Help
    cout << "\n";
    cout << "Brief information:\n";    
    cout << "  This is CCC Time Function\n";
    cout << "\n";
    cout << "Copyright (C) 2000-2009 by Grigori Fursin\n";
    cout << "\n";
    cout << "http://fursin.net/research\n";
    cout << "\n";
    cout << "Alchemy group, INRIA Futurs, France\n";
    cout << "\n";
    cout << "Options (you can also add options into file " << FileOpt << "):\n";
    cout << "  --help, -h, -?               Help;\n";
    cout << "  --version, -v                Print version;\n";
#ifdef W32
    cout << "  --file_exe, -fe <filename>   File to execute [Default=./a.exe];\n";
#else
    cout << "  --file_exe, -fe <filename>   File to execute [Default=./a.out];\n";
#endif
    cout << "  --file_param, -fp <filename> File with parameters;\n";
    cout << "  --file_time, -ft <filename>  File with time [Default=ccc-time.tmp].\n";

    return 0;
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

  //Reading parameter
  if (strcmp(glob->clp->GetParamStr(paramFileParam), "")!=0)
  {
    //Don't use ios::nocreate in Unix!
#ifdef W32
    ifstream fin(glob->clp->GetParamStr(paramFileParam), ios::nocreate);
#else
    ifstream fin(glob->clp->GetParamStr(paramFileParam));
#endif

    if (!fin)
    {
      cerr << "\nError: Can't find file with parameters!\n";
      exit(1);  
    }

    fin.getline(str1, 8191);
  
    fin.close();
  }

  strcpy(str2, glob->clp->GetParamStr(paramFileExe));
  strcat(str2, " ");
  strcat(str2, str1);
  
  cout << "Running application:\n";
  cout << str2 << "\n";
  
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
    cerr << "\nError: Can't use function 'getrusage'!\n";
    exit(1);  
  }

  user = (double) childusage.ru_utime.tv_sec +
          childusage.ru_utime.tv_usec/clk;
  sys =  (double) childusage.ru_stime.tv_sec +
          childusage.ru_stime.tv_usec/clk;
#endif

  cout << "\n";
  cout << "User   time: " << user << " s.\n";
  cout << "System time: " << sys  << " s.\n";

  //Saving time to file
  ofstream fout(glob->clp->GetParamStr(paramFileTime));

  if (!fout)
  {
    cerr << "\nError: Can't open file to write time!\n";
    exit(1);  
  }    

  fout << user << "\n";
  fout << sys << "\n";
  
  fout.close();

  cout << "\nProgram finished successfully!\n";
  return 0;
}

int papi_process(char* fin, char* fout, int add_comma)
{
  char strx[1024];
  char stry[1024];
  FILE* ffx;
  FILE* ffy;
  int ret=0;

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
              for (int i=0; i<strlen(strx); i++)
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

void fparse1(char* str)
{
  int i=strlen(str);
  if (i>0)
  {
    int found=0;
    int j=0;
    for (j=0; (j<i) && (found==0); j++)
    {
      if (str[j]=='\r' || str[j]=='\n')
      {
        str[j]=0;
	found=1;
      }
    }
  }
}
