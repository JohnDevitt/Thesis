/*
 * Copyright (C) 2004-2009 by Grigori Fursin
 *
 * http://fursin.net/research
 *
 * UNIDAPT Group
 * http://unidapt.org
 */

#ifndef stat_comp_INCLUDED
#define stat_comp_INCLUDED

#define stat_comp_COMPILE_ID                   "COMPILE_ID"
#define stat_comp_PLATFORM_ID                  "PLATFORM_ID"
#define stat_comp_ENVIRONMENT_ID               "ENVIRONMENT_ID"
#define stat_comp_COMPILER_ID                  "COMPILER_ID"
#define stat_comp_PROGRAM_ID                   "PROGRAM_ID"
#define stat_comp_DATE                         "DATE"
#define stat_comp_TIME                         "TIME"
#define stat_comp_OPT_ID                       "OPT_ID"
#define stat_comp_OPT_PASSES_ID                "OPT_PASSES_ID"
#define stat_comp_OPT_FLAGS                    "OPT_FLAGS"
#define stat_comp_COMPILE_TIME                 "COMPILE_TIME"
#define stat_comp_BIN_SIZE                     "BIN_SIZE"
#define stat_comp_OBJ_MD5CRC                   "OBJ_MD5CRC"
#define stat_comp_ICI_PASSES_USE               "ICI_PASSES_USE"
#define stat_comp_ICI_FEATURES_STATIC_EXTRACT  "ICI_FEATURES_STATIC_EXTRACT"
#define stat_comp_IP                           "IP"
#define stat_comp_NOTES                        "NOTES"
#define stat_comp_USER                         "USER"
#define stat_comp_DATETIME                     "DATETIME"

#define stat_comp_ARCH_CFG                     "ARCH_CFG"
#define stat_comp_ARCH_SIZE                    "ARCH_SIZE"
#define stat_comp_OPT_FINE                     "OPT_FINE"
#define stat_comp_OPT_PAR_STATIC               "OPT_PAR_STATIC"

#define stat_comp_OPT_FLAGS_PLATFORM        "OPT_FLAGS_PLATFORM"
#define stat_comp_OPT_PLATFORM_ID           "OPT_PLATFORM_ID"

#define stat_comp_PLATFORM_FEATURE_ID       "PLATFORM_FEATURE_ID"

#define stat_comp_FUNC_NAME                 "FUNCTION_NAME"
#define stat_comp_PASSES                    "PASSES"

#define stat_ft_st_PASS                     "PASS"
#define stat_ft_st_STATIC_FEATURE_VECTOR    "STATIC_FEATURE_VECTOR"

#define CCC_OPTS                            "CCC_OPTS"
#define CCC_OPTS_ADD                        "CCC_OPTS_ADD"

#define CCC_ICI_USE                         "CCC_ICI_USE"
#define CCC_ICI_PASSES_RECORD               "CCC_ICI_PASSES_RECORD"
#define CCC_ICI_PASSES_OPT_BASE             "CCC_ICI_PASSES_OPT_BASE"
#define CCC_ICI_PASSES_USE                  "CCC_ICI_PASSES_USE"
#define CCC_ICI_PLUGINS                     "CCC_ICI_PLUGINS"
#define CCC_ICI_PASSES_FN                   "CCC_ICI_PASSES_FN"
#define CCC_ICI_PASSES_EXT                  "CCC_ICI_PASSES_EXT"
#define CCC_ICI_PASSES_RECORD_PLUGIN        "CCC_ICI_PASSES_RECORD_PLUGIN"
#define CCC_ICI_PASSES_USE_PLUGIN           "CCC_ICI_PASSES_USE_PLUGIN"

#define CCC_ICI_PASSES_USE_TEST_DIR         "CCC_ICI_PASSES_USE_TEST_DIR"
#define CCC_ICI_PASSES_USE_FUNCTION         "CCC_ICI_PASSES_USE_FUNCTION"

#define ICI_PASSES_ALL                      "ICI_PASSES_ALL"
#define ICI_PASSES_ALL_FUNC                 ""                        //what to record to a function when
                                                                      //when ICI_PASSES_ALL is used -
								      //for example, just empty string

#define CCC_ICI_FEATURES_ST_EXTRACT         "CCC_ICI_FEATURES_STATIC_EXTRACT"
#define CCC_ICI_FEATURES_ST_FN              "CCC_ICI_FEATURES_ST_FN"
#define CCC_ICI_FEATURES_ST_EXT             "CCC_ICI_FEATURES_ST_EXT"
#define CCC_ICI_FEATURES_ST_EXTRACT_PLUGIN  "CCC_ICI_FEATURES_ST_EXTRACT_PLUGIN"
#define ICI_PROG_FEAT_PASS                  "ICI_PROG_FEAT_PASS"

/* Additional platform compiler options to enable architectural DSE */
#define CCC_OPT_PLATFORM                    "CCC_OPT_PLATFORM"

/* Ignore architectural DSE parameters during compilation 
to make baseline compilation/run to compare speedups */
#define CCC_BASERUN                         "CCC_BASERUN"

#endif
