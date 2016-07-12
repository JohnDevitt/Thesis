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

#include <time.h>
#include <fcntl.h>
#include <sys/types.h>

#include "ccc/ccc.h"
#include "ccc/stat-comp.h"
#include "ccc/stat.h"
#include "ccc/futils.h"
#include "ccc/fuuid.h"

#include <mysql/mysql.h>

static char str1[8192];
static char str2[32000];
static char str3[32000];
static char str4[1024];
static char str_keys[32000];
static char str_values[32000];
static char comp_id[1024];

void send_file(char* table, char* keys, char* values);
int separate_str(char*, char*, char*);

int main(int argc, char* argv[])
{
  int i;
  FILE* ff;

  char* var_ccc_url;
  char* var_ccc_db;
  char* var_ccc_user;
  char* var_ccc_pass;
  char* var_ccc_ssl;
  int  ivar_ccc_ssl=0;
  char* var_ccc_c_url;
  char* var_ccc_c_db;
  char* var_ccc_c_user;
  char* var_ccc_c_pass;
  char* var_ccc_c_ssl;
  int  ivar_ccc_c_ssl=0;

  char* var_ccc_db_ver;

  MYSQL*     conn;
  MYSQL_RES* res;
  MYSQL_ROW  row;
  MYSQL*     conn_c;
  MYSQL_RES* res_c;
  MYSQL_ROW  row_c;

  int ver_db1=0;
  int ver_db2=0;
  int ver_db=0;

  /********************************************************************/
  printf("CCC SEND STATS PROGRAM FEATURES (send statistics to SQL server)\n\n");

  if (argc<2)
  {
    printf("Usage:\n");
    printf(" ccc-send-stats-prog-feat <Local file with stats>\n");

    return 1;
  }

  //Init
  if ((var_ccc_url = getenv(CCC_URL)) == NULL)
  {
    printf("Error: Environment variable " CCC_URL " is not defined!\n");
    exit(1);
  }
  if ((var_ccc_db = getenv(CCC_DB)) == NULL)
  {
    printf("Error: Environment variable " CCC_DB " is not defined!\n");
    exit(1);
  }
  if ((var_ccc_user = getenv(CCC_USER)) == NULL)
  {
    printf("Error: Environment variable " CCC_USER " is not defined!\n");
    exit(1);
  }
  if ((var_ccc_pass = getenv(CCC_PASS)) == NULL)
  {
    printf("Error: Environment variable " CCC_PASS " is not defined!\n");
    exit(1);
  }
  if ((var_ccc_ssl = getenv(CCC_SSL)) == NULL)
  {
    printf("Error: Environment variable " CCC_SSL " is not defined!\n");
    exit(1);
  }
  if (strcmp(var_ccc_ssl,"")!=0) ivar_ccc_ssl=CLIENT_SSL;

  if ((var_ccc_c_url = getenv(CCC_C_URL)) == NULL)
  {
    printf("Error: Environment variable " CCC_C_URL " is not defined!\n");
    exit(1);
  }
  if ((var_ccc_c_db = getenv(CCC_C_DB)) == NULL)
  {
    printf("Error: Environment variable " CCC_C_DB " is not defined!\n");
    exit(1);
  }
  if ((var_ccc_c_user = getenv(CCC_C_USER)) == NULL)
  {
    printf("Error: Environment variable " CCC_C_USER " is not defined!\n");
    exit(1);
  }
  if ((var_ccc_c_pass = getenv(CCC_C_PASS)) == NULL)
  {
    printf("Error: Environment variable " CCC_C_PASS " is not defined!\n");
    exit(1);
  }
  if ((var_ccc_c_ssl = getenv(CCC_C_SSL)) == NULL)
  {
    printf("Error: Environment variable " CCC_C_SSL " is not defined!\n");
    exit(1);
  }
  if (strcmp(var_ccc_c_ssl,"")!=0) ivar_ccc_c_ssl=CLIENT_SSL;

  if ((var_ccc_db_ver = getenv(CCC_DB_VER)) == NULL)
  {
    printf("Error: Environment variable " CCC_DB_VER " is not defined!\n");
    exit(1);
  }

  /* Connect to the database */
  conn = mysql_init(NULL);
  if (!mysql_real_connect(conn, var_ccc_url, var_ccc_user, var_ccc_pass, var_ccc_db, 0, NULL, ivar_ccc_ssl)) 
  {
    printf("CCC Error: Can't connect to the database (%s)\n", mysql_error(conn));
    exit(1);
  }
  conn_c = mysql_init(NULL);
  if (!mysql_real_connect(conn_c, var_ccc_c_url, var_ccc_c_user, var_ccc_c_pass, var_ccc_c_db, 0, NULL, ivar_ccc_c_ssl)) 
  {
    printf("CCC Error: Can't connect to the database (%s)\n", mysql_error(conn_c));
    exit(1);
  }

  /* Check database version */
  if ((ff=fopen(var_ccc_db_ver, "r"))==NULL)
  {
    printf("CCC Error: Can't find file %s with db version ...\n", var_ccc_db_ver);
    exit(1);
  }  
  
  fgets(str1, 8191, ff);
  fparse1(str1);
  ver_db1=atoi(str1);

  fgets(str1, 8191, ff);
  fparse1(str1);
  ver_db2=atoi(str1);
  
  fclose(ff);
  
  sprintf(str4, "SELECT %s FROM %s", ccc_info_VERSION, ccc_info);
  if (mysql_query(conn_c, str4)) 
  {
    printf("CCC Error: Can't query database (%s)!\n", mysql_error(conn_c));
    exit(1);
  }

  res_c = mysql_use_result(conn_c);
  if ((row_c = mysql_fetch_row(res_c)) != NULL) 
    ver_db= atoi(row_c[0]);
  mysql_free_result(res_c);
  
  if ((ver_db<ver_db1) || (ver_db>ver_db2))
  {
    printf("CCC Error: Current framework can't work with the version of the remote database\n");
    printf("           Remote database version   : %u\n", ver_db);
    printf("           Current framework supports: %u .. %u\n", ver_db1, ver_db2);
    exit(1);
  }

  sprintf(str4, "SELECT %s FROM %s", ccc_info_VERSION, ccc_info);
  if (mysql_query(conn, str4)) 
  {
    printf("CCC Error: Can't query database (%s)!\n", mysql_error(conn));
    exit(1);
  }

  res = mysql_use_result(conn);
  if ((row = mysql_fetch_row(res)) != NULL) 
    ver_db= atoi(row[0]);
  mysql_free_result(res);
  
  if ((ver_db<ver_db1) || (ver_db>ver_db2))
  {
    printf("CCC Error: Current framework can't work with the version of the remote database\n");
    printf("           Remote database version   : %u\n", ver_db);
    printf("           Current framework supports: %u .. %u\n", ver_db1, ver_db2);
    exit(1);
  }

  /* Reading statistics file */
  if ((ff=fopen(argv[1], "r"))==NULL)
  {
    printf("CCC Error: Can't find file %s with statistics ...\n", argv[1]);
    exit(1);
  }  

  strcpy(str1, "");
  strcpy(str_keys, "");
  strcpy(str_values, "");
  while (fgets(str1, 8191, ff)!=NULL)
  {
    int i;
    fparse1(str1);

    if (separate_str(str1, str2, str3)>0)
    {
      printf("%s\n", str1);

      if (strlen(str_keys)!=0) strcat(str_keys, ",");
      if (strlen(str_values)!=0) strcat(str_values, ",");
      strcat(str_keys, str2);
      strcat(str_values, "'");
      strcat(str_values, str3);
      strcat(str_values, "'");
    }
  }
  fclose(ff);
  
  /* Prepare query */
  sprintf(str1, "INSERT INTO %s (%s) VALUES (%s)", ccc_stat_prog_feat, str_keys, str_values);
  
  printf("\n%s\n", str1);
  
  if (mysql_query(conn, str1))
  {
    printf("CCC Error: Can't add record to the database (%s)!\n", mysql_error(conn));
    exit(1);
  }

  mysql_close(conn_c);
  mysql_close(conn);

  exit(0);
}

int separate_str(char* xstr1, char* xstr2, char* xstr3)
{
  int ret=0;
  int i;

  strcpy(xstr2,"");
  strcpy(xstr3,"");
  
  for (i=0; i<strlen(xstr1); i++)
  {
    if (xstr1[i]=='=')
    {
      strncpy(xstr2, xstr1, i);
      xstr2[i]=0;
      strcpy(xstr3, &xstr1[i+1]);
      ret=1;
      break;
    }
  }

  return ret;
}
