/*
 * Copyright (C) 2004-2009 by Grigori Fursin
 *
 * http://fursin.net/research
 *
 * UNIDAPT Group
 * http://unidapt.org
 */

#ifndef ccc_INCLUDED
#define ccc_INCLUDED

#define CCC_CFG                  "CCC_CFG"
#define CCC_LIB                  "CCC_LIB"
#define CCC_PLAT_ID              "CCC_PLATFORM_ID"
#define CCC_ENV_ID               "CCC_ENVIRONMENT_ID"
#define CCC_URL                  "CCC_URL"
#define CCC_DB                   "CCC_DB"
#define CCC_USER                 "CCC_USER"
#define CCC_PASS                 "CCC_PASS"
#define CCC_SSL                  "CCC_SSL"
#define CCC_C_URL                "CCC_C_URL"
#define CCC_C_DB                 "CCC_C_DB"
#define CCC_C_USER               "CCC_C_USER"
#define CCC_C_PASS               "CCC_C_PASS"
#define CCC_C_SSL                "CCC_C_SSL"
#define CCC_RUN_SSH              "CCC_RUN_SSH"
#define CCC_RUN_SSH_PATH         "CCC_RUN_SSH_PATH"
#define CCC_RUN_SSH_NFS          "CCC_RUN_SSH_NFS"
#define CCC_RUN_SSH_NFS_REMOVE   "CCC_RUN_SSH_NFS_REMOVE"
#define CCC_DB_VER               "CCC_DB_VER"
#define CCC_CTS_DB               "CCC_CTS_DB"

#define CCC_COMPILER_ID          "CCC_COMPILER_ID"

#define CCC_NOTES                "CCC_NOTES"
#define CCC_CTS_USER             "CCC_CTS_USER"
#define CCC_PAR_DYNAMIC          "CCC_PAR_DYNAMIC"
#define CCC_RUN_POWER            "CCC_RUN_POWER"
#define CCC_RUN_ENERGY           "CCC_RUN_ENERGY"

#define CCC_ARCH_CFG             "CCC_ARCH_CFG"
#define CCC_ARCH_SIZE            "CCC_ARCH_SIZE"
#define CCC_OPT_FINE             "CCC_OPT_FINE"
#define CCC_OPT_PAR_STATIC       "CCC_OPT_PAR_STATIC"

#define CCC_HC_PAPI_USE          "CCC_HC_PAPI_USE"
#define CCC_HC_PAPI_OUT          "CCC_HC_PAPI_OUT"

#define CCC_GPROF                "CCC_GPROF" //gprofile
#define CCC_OPROF                "CCC_OPROF" //oprofile
#define CCC_OPROF_PARAM          "CCC_OPROF_PARAM" //oprofile params

#define CCC_PROCESSOR_NUM        "CCC_PROCESSOR_NUM"
#define CCC_TASKSET              "CCC_TASKSET"

/* Ignore architectural DSE parameters during compilation 
to make baseline compilation/run to compare speedups */
#define CCC_BASERUN              "CCC_BASERUN"

/* Don't compile and run baseline optimization in
the search tool but use external info instead */
#define CCC_NO_BASERUN           "CCC_NO_BASERUN"

#define ICI_VERBOSE              "ICI_VERBOSE"

#define CCC_ENV                  "ccc-env"
#define CCC_ENV_C                CCC_ENV ".c"
#define CCC_ENV_RE               CCC_ENV ".re"

#define CCC_RE                   "CCC_RE"

#define CCC_COMPILER_FEATURES_ID "CCC_COMPILER_FEATURES_ID"

#define CCC_OPT_ARCH_USE         "CCC_OPT_ARCH_USE"

#define CCC_TIME_THRESHOLD       "TIME_THRESHOLD"
#define CCC_PG_NOTE              "PG_USE"
#define CCC_OUTPUT_CORRECT       "OUTPUT_CORRECT"
#define CCC_RUN_TIME             "RUN_TIME"
#define CCC_SORT                 "SORT"
#define CCC_DIR                  "DIR"
#define CCC_CUT                  "CUT"
#define CT_OPT_REPORT            "CT_OPT_REPORT"

#endif // ccc_INCLUDED


