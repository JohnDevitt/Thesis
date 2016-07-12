/*
 * Copyright (C) 2004-2009 by Grigori Fursin
 *
 * http://fursin.net/research
 *
 * UNIDAPT Group
 * http://unidapt.org
 */

#ifndef stat_INCLUDED
#define stat_INCLUDED

#define _copy                 "cp -f"
#define _del                  "rm -f"
#define _dir                  "ls"
#define _make                 "make"

#define _results              "_ccc_results/"           
#define _run                  "./_ccc_run"
#define _run1                 "make clean"
#define _run2                 "./_ccc_prep"
#define _run3                 "./_ccc_post"
#define _comp                 "make"                 
/* #define _run_ssh              "./_ccc_run_ssh" */

#define _run_dataset_counter  "_finfo_dataset"

#define _fcomp_id             "_ccc_compilation_id_cur"
#define _fcmplr_id            "_ccc_compiler_id_cur"
#define _fprog_id             "_ccc_program_id"
#define _fprog_bin            "_ccc_program_bin"
#define _frun_a_id            "_ccc_run_assoc_id_cur"
#define _fobjmd5              "_ccc_obj_md5"
#define _fcmplr_flags         "_ccc_compiler_flags"
#define _fcomp_env_cur        "_ccc_compilation_env_cur"

#define _fout                 "a.out"

#define _tmp1                 "tmp-ccc1"
#define _tmp2                 "tmp-ccc2"
#define _tmp3                 "tmp-ccc3"

#define _objdump              "objdump -d " _fout " > " _tmp1
#define _objmd5               "md5sum " _tmp1 " > " _fobjmd5
#define _gprof                "gprof > " _tmp1
#define _gprof_o              "gmon.out"
#define _opt_pg               "-pg"
#define _oprof1               "opcontrol --init"
#define _oprof1r              "opcontrol --reset"
#define _oprof2               "opcontrol --no-vmlinux"
#define _oprof3               "opcontrol -i=%s"
#define _oprof4               "opcontrol --start"
#define _oprof5               "opcontrol --stop"
#define _oprof6               "opcontrol --dump"
#define _oprof7               "opreport -l image:%s > " _tmp1
#define _oprof8               "opcontrol --shutdown"

#define _taskset              "taskset -c %s "

#define _stat_comp            "_comp"
#define _stat_run             "_run"
#define _stat_passes          "_comp_passes"
#define _stat_features        "_prog_feat"

#define _diff1                "./_ccc_check_output.clean"
#define _diff2                "./_ccc_check_output.copy"
#define _diff3                "./_ccc_check_output.diff"

#define _usrbintime           "/usr/bin/time -o tmp-ccc-comp-time -f %U make"
#define _ftime                "tmp-ccc-time"
#define _ftmp_diff            "tmp-ccc-diff"
#define _ftmp_run_id          "tmp-ccc-run-id"
#define _stat_top_before      "_top_before"
#define _stat_top_after       "_top_after"
#define _stat_cpuinfo_before  "_cpuinfo_before"
#define _stat_cpuinfo_after   "_cpuinfo_after"

#define _dataset              "tmp-ccc-run-cmd"

#define _send_stats             "ccc-send-stats"
#define _send_stats_comp        "ccc-send-stats-comp"
#define _send_stats_comp_passes "ccc-send-stats-comp-passes"
#define _send_stats_run         "ccc-send-stats-run"
#define _send_stats_ft_st       "ccc-send-stats-prog-feat"

#define ccc_stat_comp              "STATS_COMP_GLOBAL_FLAGS"
#define ccc_stat_comp_passes       "PROGRAM_PASSES"
#define ccc_stat_run               "STATS_EXEC_GLOBAL"
#define ccc_opt_flags_global       "OPT_FLAGS_GLOBAL"
#define ccc_opt_flags_global_plat  "OPT_FLAGS_GLOBAL_PLATFORM"
#define ccc_opt_passes             "OPT_PASSES"
#define ccc_stat_prog_feat         "PROGRAM_FEATURES"
#define ccc_platform_features      "PLATFORM_FEATURES"
#define ccc_datasets               "DATASETS"
#define ccc_queue_in               "QUEUE_IN"
#define ccc_queue_out              "QUEUE_OUT"
#define ccc_queue_out2             "QUEUE_OUT2"
#define ccc_info                   "INFORMATION"
#define ccc_info_VERSION           "VERSION"

#define ccc_queue_opt1        "OPT_ID1"
#define ccc_queue_opt2        "OPT_ID2"
#define ccc_queue_run0        "RUN_ID0"
#define ccc_queue_run1        "RUN_ID1"
#define ccc_queue_run2        "RUN_ID2"
#define ccc_queue_key         "EXP_KEY"

#define _fall_compilers       "ccc-compilers.cfg"
#define _compiler_cfg_name    "COMPILER_CONFIGURATION_NAME="
#define _compiler_ext         "COMPILER_EXT="
#define _compiler_id          "COMPILER_ID="
#define _compiler_add_opts    "COMPILER_ADD_OPTS="
#define _fall_re              "ccc-re.cfg"
#define _re_cfg_name          "RE_CONFIGURATION_NAME="
#define _re_name              "RE_NAME="
#define _re_id                "RE_ID="

#define _ccc_info_datasets    "_ccc_info_datasets"

#define _ccc_comp             "ccc-comp"
#define _ccc_run              "ccc-run"
#define _ccc_run_pg           "ccc-run-pg"
#define _ccc_ml_accum_ft      "ccc-ml-accumulate-features"

#define _glob_flags           "ccc-glob-flags."
#define _glob_flags_e         ".cfg"

#endif
