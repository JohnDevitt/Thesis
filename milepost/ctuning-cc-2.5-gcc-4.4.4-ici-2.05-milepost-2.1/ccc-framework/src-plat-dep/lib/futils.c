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

/*** to get IP ***/
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#define inaddrr(x) (*(struct in_addr *) &ifr->x[sizeof sa.sin_port])
#define IFRSIZE   ((int)(size * sizeof (struct ifreq)))

#define WHITESPACE_STR  " \f\n\r\t\v"

char stra1[8192];

double getTime1(char* name)
{
  FILE* fil=NULL;
  double ret=0;
  char str1[1024];

  if ((fil=fopen(name, "r"))!=NULL)
  {
    fgets(str1, 1023, fil);
    ret=atof(str1);
    fclose(fil);
  }

  return ret;
}

double getTime2(char* name)
{
  FILE* fil=NULL;
  double ret=0;
  char str1[1024];

  if ((fil=fopen(name, "r"))!=NULL)
  {
    fgets(str1, 1023, fil);
    fgets(str1, 1023, fil);
    ret=atof(str1);
    fclose(fil);
  }

  return ret;
}

int fileExist(char* name)
{
  FILE* fil=NULL;
  int ret=0;

  if ((fil=fopen(name, "r"))!=NULL)
  {
    ret=1;
    fclose(fil);
  }

  return ret;
}

int fileDiffCorrect(char* bat, char* name)
{
  FILE* fil=NULL;
  int ret=0;
  char str1[1024];

  system(bat);

  if ((fil=fopen(name, "r"))!=NULL)
  {
    if (fgets(str1, 1023, fil)==NULL) ret=1;
    if (strcmp(str1,"")==0) ret=1;
    if (feof(fil)!=0) ret=1;
    fclose(fil);
  }

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

void copy_str(char* stro, char* stri, int j)
{
  int i;
  for (i=j; i<=strlen(stri); i++) stro[i-j]=stri[i];
}

int get_ip(char* str)
{
  unsigned char      *u;
  int                sockfd, size  = 1;
  struct ifreq       *ifr;
  struct ifconf      ifc;
  struct sockaddr_in sa;

  strcpy(str,"");

  if (0 > (sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)))
  {
    /* fprintf(stderr, "Cannot open socket.\n"); */
    return -1;
  }

  ifc.ifc_len = IFRSIZE;
  ifc.ifc_req = NULL;

  do 
  {
    ++size;
    /* realloc buffer size until no overflow occurs  */
    ifc.ifc_req = realloc(ifc.ifc_req, IFRSIZE);
    ifc.ifc_len = IFRSIZE;
    if (ioctl(sockfd, SIOCGIFCONF, &ifc)) 
    {
      /* perror("ioctl SIOCFIFCONF"); */
      return -3;
    }
  } while  (IFRSIZE <= ifc.ifc_len);

  ifr = ifc.ifc_req;
  for (;(char *) ifr < (char *) ifc.ifc_req + ifc.ifc_len; ++ifr) 
  {

    if (ifr->ifr_addr.sa_data == (ifr+1)->ifr_addr.sa_data) 
    {
      continue;  /* duplicate, skip it */
    }

    if (ioctl(sockfd, SIOCGIFFLAGS, ifr)) 
    {
      continue;  /* failed to get flags, skip it */
    }

    if (strcmp(inet_ntoa(inaddrr(ifr_addr.sa_data)), "127.0.0.1")!=0)
    {
      if (strlen(str)!=0) strcat(str, ", ");
      strcat(str, inet_ntoa(inaddrr(ifr_addr.sa_data)));
    }
  }

  close(sockfd);
  return 0;
}

/**
 * Remove whitespace characters from both ends of a copy of
 *  '\0' terminated STRING and return the result.
 **/
char *
trim (char *string)
{
  char *result = string;

  /* Ignore NULL pointers.  */
  if ((string!=NULL) && strlen(string)!=0)
    {
      char *ptr = string;

      /* Skip leading whitespace.  */
      while (strchr (WHITESPACE_STR, *ptr))
        ++ptr;

      /* Make a copy of the remainder.  */
      result = strdup (ptr);

      /* Move to the last character of the copy.  */
      for (ptr = result; *ptr; ++ptr)
        /* NOWORK */;
      --ptr;

      /* Remove trailing whitespace.  */
      for (--ptr; strchr (WHITESPACE_STR, *ptr); --ptr)
          *ptr = '\0';
   }

  return result;
}

/*
  Create a string from the array
*/

void create_str_from_array(char* str, char** arr)
{
  int i=0;
  strcpy(str, "");
  while (arr[i]!=NULL)
  {
    strcat(str, arr[i]);
    strcat(str, " ");
  }
}
