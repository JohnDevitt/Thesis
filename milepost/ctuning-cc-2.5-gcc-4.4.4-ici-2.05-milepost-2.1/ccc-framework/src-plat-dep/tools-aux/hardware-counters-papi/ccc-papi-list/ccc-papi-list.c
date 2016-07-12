/*
 * Copyright (C) 2002-2009 by Grigori Fursin
 *
 * http://fursin.net/research
 *
 * UNIDAPT Group
 * http://unidapt.org
 *
 * Originally developed for EOS and FCO frameworks
 */

#include <papi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char str1[8192];
static char str2[8192];

int main(int argc, char* argv[])
{
  FILE* ff;

  int retval, i;
  int EventSet=PAPI_NULL, count = 0;
  long_long values;
  PAPI_event_info_t info;

  /********************************************************************/
  printf("CCC PAPI LIST (list PAPI events)\n");

  if (argc<=1)
  {
    printf("\n");
    
    printf("Usage:\n");
    printf(" ccc-papi-list <output file name>\n");

    return 1;
  }

  if ((ff=fopen(argv[1], "w"))==NULL)
  {
    printf("ERROR: Can't open file to save PAPI events ...\n");
    exit(1);
  }

  retval = PAPI_library_init(PAPI_VER_CURRENT);
  if (retval != PAPI_VER_CURRENT)
  {
    printf("Error: PAPI_library_init failed ...\n");
    exit(1);
  }

  retval = PAPI_create_eventset(&EventSet);
  if (retval != PAPI_OK)
  {
    printf("Error: PAPI_create_eventset failed ...\n");
    exit(1);
  } 

  for (i = 0; i < PAPI_MAX_PRESET_EVENTS; i++) 
  {
    if (PAPI_get_event_info(PAPI_PRESET_MASK|i, &info) != PAPI_OK) continue;
    if (!(info.count)) continue;

    retval = PAPI_add_event(EventSet, info.event_code);
    if (retval == PAPI_OK) 
    {
      printf("Added %s successful\n", info.symbol);
      if (i!=0) fprintf(ff, ",");
      fprintf(ff, "%s", info.symbol);
      if (strcmp(info.symbol, "PAPI_L1_TCM")==0) fprintf(ff, "x");
      if (strcmp(info.symbol, "PAPI_L1_TCH")==0) fprintf(ff, "x");
      if (strcmp(info.symbol, "PAPI_L2_TCA")==0) fprintf(ff, "x");

      count++;
    }
    retval = PAPI_remove_event(EventSet, info.event_code);
  }
  fprintf(ff, "\n");

  printf("\n");
  printf("Number of events recorded = %u\n", count);

  fclose(ff);

  return 0;
}
