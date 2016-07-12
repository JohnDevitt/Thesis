//
// Additional Functions for Linux while converting from Windows
//
// Copyright (C) 2000-2004 by Grigori Fursin
//
// http://homepages.inf.ed.ac.uk/gfursin
//
// ICSA, School of Informatics,
// University of Edinburgh, UK
//

#include "fggunix.h"

#include <ctype.h>
#include <string.h>

char* strlwr(char* addr)
{
  char c;

  char* newline=new char[strlen(addr)];
  char* newline1=newline;
  
  while ((c=*addr)!=0)
  {
    *newline=tolower(c);
    addr++;
    newline++;
  }

  *newline=0;

  return newline1;
}
