//
// Class CCLParse for command-line parsing
//
// Copyright (C) 2000-2004 by Grigori Fursin
//
// http://homepages.inf.ed.ac.uk/gfursin
//
// ICSA, School of Informatics,
// University of Edinburgh, UK
//

#if !defined(CCLParse_INCLUDED_)
#define CCLParse_INCLUDED_ 

class CCLParse
{
private:
  int XNumOptMax;
  int XLenStrMax;
  int XLenParamMax;
  int XIgnoreCase;
  char* XFileNameOpt;
  
  char* CL1;
  char* CL2;
  char* CL3;
  int* XHasParam;

  char* CL;

  int CurParam;

public:
  int Init(int, int, int, int, char*);
  int Add(const char*, const char*, const char*, const char*, int);
  int Parse(int, char* Args[]);
  char* GetParamStr(int);
  long GetParamLong(int);
  int GetParamInt(int);
};

#endif
