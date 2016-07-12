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
#include <math.h>

#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "ccc/ccc.h"
#include "ccc/stat.h"
#include "ccc/stat-comp.h"
#include "ccc/futils.h"
#include "ccc/fuuid.h"

static char str1[8192];
static char str2[8192];
static char str3[8192];
static char str4[8192];
static char str5[8192];

#define FEATMAX 1024
float feat[FEATMAX];
int sfeat[FEATMAX];

void init_sockaddr (struct sockaddr_in *name, const char *hostname, unsigned short int port);
void write_to_server (int filedes, char* feat);
void read_from_server(int filesrc, char* fname);

int main(int argc, char* argv[])
{
  FILE* ff;
  FILE* ff1;
  int feat_max=0;
  int i=0;
  int j=0;
  char* strx;
  char* stry;
  int finish=0;

  /********************************************************************/
  if (argc<3)
  {
    printf("CCC ACCUMULATE FEATURES (from function level to global level)\n");
    printf("\n");
    
    printf("Usage:\n");
    printf(" ccc-ml-accumulate-features <FILE LIST> <FILE TO SAVE ACCUMULATED FEATURES>\n");

    return 1;
  }

  for (i=0; i<FEATMAX; i++)
  {
    feat[i]=0;
    sfeat[i]=0;
  }

  i=0;

  if ((ff=fopen(argv[1], "r"))==NULL)
  {
    printf("CCC Error: Can't find file with the list of feature files (%s) ...\n", argv[1]);
    exit(1);
  }

  while (fgets(str1, 8191, ff)!=NULL)
  {
    fparse1(str1);

    if ((ff1=fopen(str1, "r"))==NULL)
    {
      printf("CCC Error: Can't find feature file (%s) ...\n", str1);
      exit(1);
    }

    if (fgets(str2, 8191, ff1)==NULL)
    {
      printf("CCC Error: Can't read from feature file (%s) ...\n", str1);
    }

//    printf("FEAT_CUR: %s\n", str2);
    fparse1(str2);

    /* Parsing */
    finish=0;
    i=0;
    strx=str2;
    while ((finish==0))
    {
      strx=strstr(strx, "ft");
      stry=strstr(strx, "=");
      *stry=0;
      stry++;
      strx+=2;

      strcpy(str3, strx);
      strx=stry;

      sfeat[i]=atoi(str3);

      /* Get number */
      strcpy(str4, "");
      j=0;
      while ((*stry!=0) && (*stry!=','))
      {
        str4[j]=*stry;
	j++;
	stry++;
      }
      str4[j]=0;
      if (*stry==0) finish=1;
      
      feat[i]+=atof(str4);

/*      printf("FT(%s)=(%s, %3.3f)\n", str3, str4, feat[i]); */

      i++;
    }

    fclose(ff1);
  }
  
  fclose(ff);

  sprintf(str5, "%s.ft", argv[2]);
  if ((ff=fopen(argv[2], "w"))==NULL)
  {
    printf("CCC Error: Can't open file for writing (%s) ...\n", argv[2]);
    exit(1);
  }
  if ((ff1=fopen(str5, "w"))==NULL)
  {
    printf("CCC Error: Can't open file for writing (%s) ...\n", str5);
    exit(1);
  }

  for (j=0; j<i; j++)
  {
    if (j!=0)
    {
      fprintf(ff, ",");
      fprintf(ff1, ",");
    }

    if (ceil(feat[j])==feat[j])
    {
      fprintf(ff, " %.0f", feat[j]);
      fprintf(ff1, " ft%u=%.0f", sfeat[j], feat[j]);
    }
    else
    {
      fprintf(ff, " %.2f", feat[j]);
      fprintf(ff1, " ft%u=%.2f", sfeat[j], feat[j]);
    }
  }

  fprintf(ff,"\n");
  fprintf(ff1,"\n");
  
  fclose(ff1);
  fclose(ff);
  
  exit(0);
}
