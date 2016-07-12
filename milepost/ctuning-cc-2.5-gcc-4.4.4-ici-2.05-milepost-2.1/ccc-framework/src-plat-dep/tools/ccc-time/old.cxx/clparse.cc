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

#include "clparse.h"
#ifndef W32
#  include "fggunix.h"
#endif

#include <string.h> //+defines NULL
#include <stdlib.h>

#include <fstream.h>

//****************************************************************************
//"Initialization"
//
//Input:  NumOptMax   = Max Number of Options
//        LenStrMax   = Max Length for String (Option)
//        LenParamMax = Max Length for String (Parameter)
//        IgnoreCase  = 1 - Ignore case, 0 - Don't ignore case
//        FileNameOpt = Name of file with options
//                      (Options and parameters are on different lines
//                      End is an empty line) 
//
//Return: 0  - Ok
//        >0 - Error
//

int CCLParse::Init(int NumOptMax,
                   int LenStrMax,
                   int LenParamMax,
                   int IgnoreCase,
                   char* FileNameOpt)
{
  int misc1, misc2;
  int x;

  XNumOptMax=NumOptMax;
  XLenStrMax=LenStrMax;
  XLenParamMax=LenParamMax;
  XIgnoreCase=IgnoreCase;    
  XFileNameOpt=FileNameOpt;

  if (NumOptMax<1) return 1;

  if (LenStrMax<2) return 1;

  if ((IgnoreCase!=0) && (IgnoreCase!=1)) return 1;

  misc1=XNumOptMax*XLenStrMax;
  misc2=XNumOptMax*XLenParamMax;
 
  CL1=new char [misc1];
  CL2=new char [misc1];
  CL3=new char [misc1];
  CL=new char[misc2];
  XHasParam=new int[XNumOptMax];

  for (x=0; x<misc1; x++) 
  {
     CL1[x]=0;
     CL2[x]=0;  
     CL3[x]=0;
  }

  for (x=0; x<misc2; x++) 
  {
     CL[x]=0;
  }

  for (x=0; x<XNumOptMax; x++)
  {
     XHasParam[x]=0;
  }

  CurParam=0;

  return 0;
}

//****************************************************************************
//"Add Option"
//
//Input:  Opt1     = Option 1
//        Opt2     = Option 2 (or "") - alternative option
//        Opt3     = Option 3 (or "") - alternative option
//        Default  = Default Parameter
//        HasParam = 0 - Option doesn't have parameter, 1 - it has
//
//Return: 0  - Ok
//        >0 - Error
//
int CCLParse::Add(const char* Opt1, const char* Opt2, const char* Opt3, const char* Default, int HasParam)
{
  if (CurParam>XNumOptMax) return 1;

  strcpy(&CL1[XLenStrMax*CurParam], Opt1);
  strcpy(&CL2[XLenStrMax*CurParam], Opt2);
  strcpy(&CL3[XLenStrMax*CurParam], Opt3);

  if (XIgnoreCase==1)
  {
     strcpy(&CL1[XLenStrMax*CurParam],strlwr(&CL1[XLenStrMax*CurParam]));
     strcpy(&CL2[XLenStrMax*CurParam],strlwr(&CL2[XLenStrMax*CurParam]));     
     strcpy(&CL3[XLenStrMax*CurParam],strlwr(&CL3[XLenStrMax*CurParam]));
  }

  strcpy(&CL[XLenParamMax*CurParam], Default);

  XHasParam[CurParam]=HasParam;

  CurParam++;

  return 0;
}

//****************************************************************************
//"Parse Command Line"
//
//Return: 0  - Ok
//        >0 - Error
//
int CCLParse::Parse(int NumArgs, char* Args[])
{
  int x, y;

  //Checking command line
  if (NumArgs>1)
  {
    int CurArg=2;

    do
    {
      char* Args1=new char[strlen(Args[CurArg-1])];
      strcpy(Args1, Args[CurArg-1]);    
      if (XIgnoreCase==1) strcpy(Args1, strlwr(Args1));
    
      y=0;
      for (x=0; x<CurParam; x++)
      {
         if      (strcmp(Args1, &CL1[XLenStrMax*x])==0) {y=1; break;}
         else if (strcmp(Args1, &CL2[XLenStrMax*x])==0) {y=1; break;}
         else if (strcmp(Args1, &CL3[XLenStrMax*x])==0) {y=1; break;}
      } 

      if (y==1)
      {
        if (XHasParam[x]==0) strcpy(&CL[XLenParamMax*x], "1");
        else
        {
          CurArg++;
          if (CurArg>NumArgs) return 2; // Option without parameter
          strcpy(&CL[XLenParamMax*x], Args[CurArg-1]);
        }
      }
      else return 1; // Unknown option
  
      CurArg++;

    }
    while (CurArg<=NumArgs);
  }

  //Checking file with parameters
#ifdef W32
  ifstream fin(XFileNameOpt, ios::nocreate);
#else
  ifstream fin(XFileNameOpt);
#endif

  if (!fin) return 0; //There is no file
 
  char* str1=new char[XLenStrMax];
  char* str2=new char[XLenParamMax];

  strcpy (str1, "");
  strcpy (str2, "");

  while (!fin.eof())
  {
    fin.getline(str1, XLenStrMax);
    if (*str1==0) {fin.close();return 0;}
    
    if (XIgnoreCase==1) strcpy(str1, strlwr(str1));

    y=0;
    for (x=0; x<CurParam; x++)
    {
      if      (strcmp(str1, &CL1[XLenStrMax*x])==0) {y=1; break;}
      else if (strcmp(str1, &CL2[XLenStrMax*x])==0) {y=1; break;}
      else if (strcmp(str1, &CL3[XLenStrMax*x])==0) {y=1; break;}
    } 

    if (y==1)
    {
      if (XHasParam[x]==0) strcpy(&CL[XLenParamMax*x], "1");
      else
      {
        if (!fin.eof())
        {
          fin.getline(str2, XLenParamMax);
          if (*str2==0) return 3; //Option without parameter in file
          
          strcpy(&CL[XLenParamMax*x], str2);
        }
      }
    }
    else return 4; //Unknown option in file    
  }

  fin.close();
  return 0;
}

//****************************************************************************
//"Get String Parameter"
//
char* CCLParse::GetParamStr(int NumParam)
{   
  if ((NumParam<0) || (NumParam>=CurParam)) return NULL;

  return &CL[XLenParamMax*NumParam];
}

//****************************************************************************
//"Get Long Parameter"
//
long CCLParse::GetParamLong(int NumParam)
{
  if ((NumParam<0) || (NumParam>=CurParam)) return 0;

  return atol(&CL[XLenParamMax*NumParam]);
}

//****************************************************************************
//"Get Int Parameter"
//
int CCLParse::GetParamInt(int NumParam)
{
  if ((NumParam<0) || (NumParam>=CurParam)) return 0;

  return atoi(&CL[XLenParamMax*NumParam]);
}
