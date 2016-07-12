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

#define _rm    "rm "
#define _uuid1 "uuidgen "
#define _uuid2 "tmp_uuid"

static unsigned long ida=0;
static unsigned long idb=0;

int get_uuid(unsigned long* rnd1, unsigned long* rnd2)
{
  char uuidstr[128];
  char str1[128];
  char str2[128];
  int i;
  int j;
  FILE* ff;

  srand((unsigned)time(NULL));
  sprintf(str2, _uuid2 ".%u.tmp", rand());

  if ((ff=fopen(str2, "wt"))==NULL)
    return 1;
  else
  fclose(ff);
  sprintf(str1, _uuid1 " > %s", str2);
  system(str1);
  
  if ((ff=fopen(str2, "rt"))==NULL)
    return 2;
  else
  {
    if ((fgets(uuidstr, 127, ff)==NULL) || (strlen(uuidstr)<24))
    {
      fclose(ff);
      return 3;
    }
    
    fclose(ff);
  }

  sprintf(str1, _rm "%s", str2);
  system(str1);

  j=0;
  for (i=0; i<7; i++)
    if (uuidstr[i]!='-') str1[j++]=uuidstr[i];
  str1[j]=0;

  *rnd1=strtoul(str1, 0, 16);
  
  j=0;
  for (i=13; i<22; i++)
    if (uuidstr[i]!='-') str1[j++]=uuidstr[i];
  str1[j]=0;
  
  *rnd2=strtoul(str1, 0, 16);

  return 0;
}

int get_filename_uuid(char* str, char* name)
{
  char str1[128];
  
  if (get_uuid(&ida, &idb)!=0)
  {
    return 1;
  }
  sprintf(str1, "%u%u", ida, idb);
  sprintf(str, name, str1);
  
  return 0;
}
