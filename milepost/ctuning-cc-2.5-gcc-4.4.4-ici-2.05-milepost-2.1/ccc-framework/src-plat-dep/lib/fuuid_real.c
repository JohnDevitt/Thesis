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
#include <uuid/uuid.h>

void get_uuid(unsigned long* rnd1, unsigned long* rnd2)
{
  uuid_t uuid;
  char uuidstr[128];
  char str1[128];
  unsigned long x;
  int i;
  int j;

  uuid_generate(uuid);
  uuid_unparse(uuid, uuidstr);

  j=0;
  for (i=0; i<8; i++)
    if (uuidstr[i]!='-') str1[j++]=uuidstr[i];
  str1[j]=0;

  *rnd1=strtoul(str1, 0, 16);
  
  j=0;
  for (i=13; i<21; i++)
    if (uuidstr[i]!='-') str1[j++]=uuidstr[i];
  str1[j]=0;
  
  *rnd2=strtoul(str1, 0, 16);
}
