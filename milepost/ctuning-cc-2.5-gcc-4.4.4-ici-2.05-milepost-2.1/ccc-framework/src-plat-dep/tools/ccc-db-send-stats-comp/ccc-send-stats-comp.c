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
static char str4[8192];
static char str5[8192];
static char str6[8192];
static char str7[8192];
static char str_keys[32000];
static char str_values[32000];
static char comp_id[1024];
static char comp_arch_cfg[31000];
static char comp_arch_size[31000];
static char comp_date[255];
static char comp_time[255];
static char comp_user[255];
static char comp_ip[255];
static char comp_id[1024];
static char plat_id[1024];

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

  static unsigned long ida=0;
  static unsigned long idb=0;

  int ver_db1=0;
  int ver_db2=0;
  int ver_db=0;

  /********************************************************************/
  printf("CCC SEND STATS COMP (send compilation statistics to SQL server)\n\n");

  if (argc<2)
  {
    printf("Usage:\n");
    printf(" ccc-send-stats-comp <Local file with stats>\n");

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
      int ignore=0;
      
      printf("%s\n", str1);

      /* Pre-process some keys */
      if (strcmp(str2, stat_comp_DATE)==0)
	strcpy(comp_date, str3);
      else if (strcmp(str2, stat_comp_TIME)==0)
	strcpy(comp_time, str3);
      else if (strcmp(str2, stat_comp_USER)==0)
	strcpy(comp_user, str3);
      else if (strcmp(str2, stat_comp_IP)==0)
	strcpy(comp_ip, str3);
      else if (strcmp(str2, stat_comp_ARCH_CFG)==0)
      {
        printf("  %s detected\n", stat_comp_ARCH_CFG);
	strcpy(comp_arch_cfg, str3);
	ignore=1;
      }
      else if (strcmp(str2, stat_comp_ARCH_SIZE)==0)
      {
        printf("  %s detected\n", stat_comp_ARCH_SIZE );
	strcpy(comp_arch_size, str3);
	ignore=1;
      }
      else if (strcmp(str2, stat_comp_COMPILER_ID)==0)
      {
        printf("  %s detected\n", stat_comp_COMPILER_ID);
	strcpy(comp_id, str3);
      }
      else if (strcmp(str2, stat_comp_PLATFORM_ID)==0)
      {
        printf("  %s detected\n", stat_comp_PLATFORM_ID);
	strcpy(plat_id, str3);
      }
      else if (strcmp(str2, stat_comp_OPT_FLAGS)==0)
      {
        /* Process OPT_ID */
        int found=0;
        ignore=1;

	printf("  Processing %s\n", stat_comp_OPT_FLAGS);

        /* Search exisiting flags */

        sprintf(str4, "SELECT %s FROM %s WHERE (%s='%s' AND %s='%s')", 
	              stat_comp_OPT_ID, ccc_opt_flags_global, 
                      stat_comp_COMPILER_ID, comp_id,
		      stat_comp_OPT_FLAGS, str3);

        if (mysql_query(conn_c, str4)) 
        {
          printf("CCC Error: Can't query database (%s)!\n", mysql_error(conn_c));
          exit(1);
        }

        res_c = mysql_use_result(conn_c);

        if ((row_c = mysql_fetch_row(res_c)) != NULL) 
        {
          found=1;
	  
	  /* ADD found OPT_ID */
          if (strlen(str_keys)!=0) strcat(str_keys, ",");
          if (strlen(str_values)!=0) strcat(str_values, ",");
	  
          strcat(str_keys, stat_comp_OPT_ID);
          strcat(str_values, "'");
          strcat(str_values, row_c[0]);
          strcat(str_values, "'");
	  
          strcpy(str3, ""); /* Clean the OPT key */	  
        }

        mysql_free_result(res_c);
	
	if (found==0)
	{
          /* Generate OPT ID */
          if (get_uuid(&ida, &idb)!=0)
          {
            printf("\nError: Can't get UUID from the system!\n");
            exit(1);
          }
          sprintf(str5, "%u%u", ida, idb);

          /* Add Flag */
          printf("  Add flag to the database!\n");

          sprintf(str4, "INSERT INTO %s (%s,%s,%s,%s,%s,%s) VALUES ('%s','%s','%s','%s','%s','%s %s')", 
	                 ccc_opt_flags_global, 
			 stat_comp_OPT_ID, stat_comp_COMPILER_ID, stat_comp_OPT_FLAGS, stat_comp_USER, stat_comp_IP, stat_comp_DATETIME,
			 str5, comp_id, str3, comp_user, comp_ip, comp_date, comp_time);
	  
          if (mysql_query(conn_c, str4))
          {
            printf("CCC Error: Can't store flags in the database (%s)\n", mysql_error(conn_c));
            exit(1);
          }

 	  /* ADD found OPT_ID */
          if (strlen(str_keys)!=0) strcat(str_keys, ",");
          if (strlen(str_values)!=0) strcat(str_values, ",");
	  
          strcat(str_keys, stat_comp_OPT_ID);
          strcat(str_values, "'");
          strcat(str_values, str5);
          strcat(str_values, "'");
	  
          strcpy(str3, ""); /* Clean the OPT key */	  

          printf("  New %s = %s\n", stat_comp_OPT_ID, str5);
	}
	else
	{
          printf("  Found flag in the database!\n");
	}
      }
      else if (strcmp(str2, stat_comp_OPT_FLAGS_PLATFORM)==0)
      {
        /* Process OPT_ID */
        int found=0;
        ignore=1;
	
        strcpy(str6,""); //platform opt feature id
        if (strcmp(str3, "")!=0)
        {
  	  printf("  Processing %s\n", stat_comp_OPT_FLAGS_PLATFORM);

          /* Search exisiting flags */
          sprintf(str4, "SELECT %s FROM %s WHERE (%s='%s' AND %s='%s')", 
  	                stat_comp_OPT_PLATFORM_ID, ccc_opt_flags_global_plat, 
                        stat_comp_COMPILER_ID, comp_id,
	    	        stat_comp_OPT_FLAGS, str3);

          if (mysql_query(conn_c, str4)) 
          {
            printf("CCC Error: Can't query database (%s)!\n", mysql_error(conn_c));
            exit(1);
          }

          res_c = mysql_use_result(conn_c);

          if ((row_c = mysql_fetch_row(res_c)) != NULL) 
          {
            found=1;
            strcpy(str6, row_c[0]); /* Get OPT_ID */
          }

          mysql_free_result(res_c);
	  
  	  if (found==0)
  	  {
            /* Generate OPT ID */
            if (get_uuid(&ida, &idb)!=0)
            {
              printf("\nError: Can't get UUID from the system!\n");
              exit(1);
            }
            sprintf(str5, "%u%u", ida, idb);
            strcpy(str6, str5);

            /* Add Flag */
            printf("  Add flag to the database!\n");

            sprintf(str4, "INSERT INTO %s (%s,%s,%s,%s,%s,%s) VALUES ('%s','%s','%s','%s','%s','%s %s')", 
  	                   ccc_opt_flags_global_plat, 
		  	   stat_comp_OPT_PLATFORM_ID, stat_comp_COMPILER_ID, stat_comp_OPT_FLAGS, stat_comp_USER, stat_comp_IP, stat_comp_DATETIME,
			   str5, comp_id, str3, comp_user, comp_ip, comp_date, comp_time);
	  
            if (mysql_query(conn_c, str4))
            {
              printf("CCC Error: Can't store flags in the database (%s)\n", mysql_error(conn_c));
              exit(1);
            }
	  }
          else
    	  {
           printf("  Found flag in the database!\n");
	  }
        }

          //if only ARCH_CFG and ARCH_SIZE are set, then OPT_PLATFORM_ID (str6) is empty ...

          /* Search exisiting platform features */
          found=0;
          sprintf(str4, "SELECT %s FROM %s WHERE (%s='%s' AND %s='%s' AND %s='%s' AND %s='%s')", 
  	                stat_comp_PLATFORM_FEATURE_ID, ccc_platform_features, 
                        stat_comp_PLATFORM_ID, plat_id,
	    	        stat_comp_OPT_PLATFORM_ID, str6,
			stat_comp_ARCH_CFG, comp_arch_cfg,
			stat_comp_ARCH_SIZE, comp_arch_size);

          if (mysql_query(conn_c, str4)) 
          {
            printf("CCC Error: Can't query database (%s)!\n", mysql_error(conn_c));
            exit(1);
          }

          res_c = mysql_use_result(conn_c);

          if ((row_c = mysql_fetch_row(res_c)) != NULL) 
          {
            found=1;
            strcpy(str7, row_c[0]); /* Get PLATFORM_FEATURE_ID */

   	    /* ADD found PLATFORM_FEATURE_ID */
            if (strlen(str_keys)!=0) strcat(str_keys, ",");
            if (strlen(str_values)!=0) strcat(str_values, ",");
	  
            strcat(str_keys, stat_comp_PLATFORM_FEATURE_ID);
            strcat(str_values, "'");
            strcat(str_values, str7);
            strcat(str_values, "'");
	  
            strcpy(str3, ""); /* Clean the OPT key */	  

            printf("  Found %s = %s\n", stat_comp_PLATFORM_FEATURE_ID, str7);
          }

          mysql_free_result(res_c);
	  
          /* Search for platform features */
  	  if (found==0)
  	  {
            /* Generate PLATFORM FEATURE ID */
            if (get_uuid(&ida, &idb)!=0)
            {
              printf("\nError: Can't get UUID from the system!\n");
              exit(1);
            }
            sprintf(str5, "%u%u", ida, idb);
	    strcpy(str7, str5);

            /* Add feature */
            printf("  Add platform feature to the database!\n");

            sprintf(str4, "INSERT INTO %s (%s,%s,%s,%s,%s,%s,%s,%s) VALUES ('%s','%s','%s','%s','%s','%s','%s','%s %s')", 
  	                   ccc_platform_features, 
		  	   stat_comp_PLATFORM_FEATURE_ID, stat_comp_PLATFORM_ID, stat_comp_OPT_PLATFORM_ID, stat_comp_ARCH_CFG, stat_comp_ARCH_SIZE,
			   stat_comp_USER, stat_comp_IP, stat_comp_DATETIME,
			   str5, plat_id, str6, comp_arch_cfg, comp_arch_size,comp_user, comp_ip, comp_date, comp_time);

            if (mysql_query(conn_c, str4))
            {
              printf("CCC Error: Can't store flags in the database (%s)\n", mysql_error(conn_c));
              exit(1);
            }

   	    /* ADD found PLATFORM_FEATURE_ID */
            if (strlen(str_keys)!=0) strcat(str_keys, ",");
            if (strlen(str_values)!=0) strcat(str_values, ",");
	  
            strcat(str_keys, stat_comp_PLATFORM_FEATURE_ID);
            strcat(str_values, "'");
            strcat(str_values, str7);
            strcat(str_values, "'");
	  
            strcpy(str3, ""); /* Clean the OPT key */	  

            printf("  New %s = %s\n", stat_comp_PLATFORM_FEATURE_ID, str7);
	  }
          else
    	  {
            printf("  Found platform feature in the database!\n");
	  }
//	}
      }
      
      if (ignore==0)
      {
        if (strlen(str_keys)!=0) strcat(str_keys, ",");
        if (strlen(str_values)!=0) strcat(str_values, ",");
        strcat(str_keys, str2);
        strcat(str_values, "'");
        strcat(str_values, str3);
        strcat(str_values, "'");
      }
    }
  }
  fclose(ff);
  
  /* Prepare query */
  sprintf(str1, "INSERT INTO %s (%s) VALUES (%s)", ccc_stat_comp, str_keys, str_values);
  
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
