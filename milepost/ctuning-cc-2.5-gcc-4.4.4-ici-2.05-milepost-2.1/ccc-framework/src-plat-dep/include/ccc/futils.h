/*
 * Copyright (C) 2004-2009 by Grigori Fursin
 *
 * http://fursin.net/research
 *
 * UNIDAPT Group
 * http://unidapt.org
 */

#ifndef futils_INCLUDED
#define futils_INCLUDED

void setFComp(char* name, int mode, char* fr, char* fw);
double getTime1(char* name);
double getTime2(char* name);
int fileExist(char* name);
int fileDiffCorrect(char* bat, char* name);
long readLastIteration(char* name);
long readLastIterationS(char* name);
void writeIteration(char* name, long mode);
void writeIterationS(char* name, long mode, long mode1);
void writeTime(char* name, double t1, double t2);
void writeTime1(char* name, double t1, double t2, double t1x, double t2x);
void writeIterStatus(char* name, int inf0, int inf1, int inf2);
void cleanFile(char* name);
void writeInfo(char* name, char* str);
void appendOut(char* name, char* str);
void appendOutPrint(char* name, char* str);
void fparse(char* str);
void fparse1(char* str);
int copyFile(char* fin, char* fout);
void copy_str(char* stro, char* stri, int j);
int get_ip(char* str);
void create_str_from_array(char* str, char** arr);
char* trim (char* string);

#define sep1 "==============================================================================="
#define sep2 "*******************************************************************************"

#endif // futils_INCLUDED
