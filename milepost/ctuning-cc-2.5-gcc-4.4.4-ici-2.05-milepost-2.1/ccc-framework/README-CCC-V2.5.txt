****************************************************************
Continuous Collective Compilation Framework (CCC)

Continuous Collective Compilation Framework (CCC) is a collaborative modular
plugin-enabled R&D infrastructure to automate code, architecture and dataset
analysis, characterization and multi-objective optimization (currently execution
time, code size and compilation time) based on empirical iterative feedback
directed compilation, statistical analysis, collective optimization and
machine learning (predictive modeling). CCC enables automatic sharing
of analysis and optimization information with the community through
the Collective Optimization Database (COD, http://cTuning.org/cdatabase).

CCC framework supports both global and fine-grain program optimizations
that can be extended through user plugins. It supports compilers with
Interactive Compilation Interface (http://cTuning.org/ici) such as MILEPOST
GCC (http://cTuning.org/milepost-gcc). It is also used in the cTuning
Compiler Collection (http://cTuning.org/ctuning-cc) to enable transparent
code optimization. CCC is used with Collective Benchmark
(http://cTuning.org/cbench) and many other standard benchmark suits
such as SPEC, Polyhedron, EEMBC, etc. When using standard benchmarks and kernels,
CCC can characterize code and architecture behavior and save it in the
COD to help end-users predict optimizations for a previously unseen program,
dataset, library, operating system and architecture. Finally,
it enables collaborative R&D and optimization knowledge reuse that is particularly
important for academia to be able to easily verify and reproduce published results.

CCC Framework is a community-driven project - you are welcome to join the project,
extend it, provide new plugins (particularly to support fine-grain optimizations,
JIT, architecture simulation and new machine learning/collective optimization
techniques) leave feedback and add your optimization data to help the community.

CCC framework website:
 http://cTuning.org/ccc

cTuning/CCC mailing lists (feedback, comments and bug reports):
 http://cTuning.org/community

CCC framework is currently used together with cTuning CC (http://cTuning.org/ctuning-cc)
and MILEPOST GCC (http://cTuning.org/milepost-gcc) to train machine learning models
and predict profitable optimizations (balancing execution time, code size and compilation
time among other characteristics) across multiple programs, datasets, compilers and architectures,
by correlating program features and run-time behavior.

****************************************************************
Low-level details:
 CCC written in C (platform dependent part and plugins) and in PHP (platform independent part and plugins)
 though any other language can be used.

 Current CCC supports empirical multi-objective optimization using flags (if compiler
 supports ICI) for major compilers such as GCC, Open64, LLVM, GCC4CLI, ICC, XL, etc.
 If compiler supports Interactive Compilation Interface (http://cTuning.org/ici),
 the plugins can perform pass selection and reordering on a function level.
 We plan to extend CCC to support fine-grain optimizations for loop unrolling,
 prefetching, polyhedral optimizations (tiling, splitting, fusion/fission), etc
 as well as extensions to predictive modeling and architecture simulation - volunteers
 are very welcome!

 CCC supports plugins to perform exhaustive, random, one-off empirical feedback-directed optimization.
 We have plugins to predict profitable optimizations based on static/dynamic program features,
 statistical analysis and machine learning using cTuning Compiler Collection (http://cTuning.org/ctuning-cc)
 and cTuning web-services (http://cTuning.org/cdatabase).

 CCC supports the following benchmarks: CBench/MiBench with MiDataSets (http://cTuning.org/cbench),
 EEMBC, SPEC CPU2006, SPEC CPU2000, SPEC CPU95, Polyhedron, etc.

 CCC can automatically improve code on most Unix-like major platforms (from server and supercomputer 
 to embedded architectures). It also supports bare-metal embedded systems.

 All optimization data is either collected off-line or in the Collective Optimization Database
 (http://cTuning.org/cdatabase) to avoid duplication of experiments, enable collective optimization,
 improve quality and boost innovation.

****************************************************************
Author: Grigori Fursin (http://fursin.net/research)

 (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France
 (C) 2004-2009, UNIDAPT Group, INRIA, France (FCO/CCC developments)
 (C) 1999-2003, University of Edinburgh, UK (EOS developments)

****************************************************************
Acknowledgments (suggestions, evaluation, bug fixes, etc):

 Yuriy Kashnikov (UVSQ, France)
 Abdul Wahid Memon (UVSQ, France)
 Franck Talbart (UVSQ, France)
 Menjato Rakoto (Paris South University and INRIA, France)
 Yuanjie Huang (ICT, China)
 Mircea Namolaru (IBM Haifa, Israel)
 John Thomson (University of Edinburgh, UK)
 Phil Barnard (ARC, UK)
 Erven Rohou (IRISA, France)
 Mingjie Xing (ICT, China)

****************************************************************
CCC Framework License:

 This program is free software; you can redistribute it and/or modify it 
 under the terms of the GNU General Public License version 2 as published 
 by the Free Software Foundation.

 This program is distributed in the hope that it will be useful, 
 but WITHOUT ANY WARRANTY; without even the implied warranty 
 of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 See the GNU General Public License for more details:
 http://www.gnu.org/copyleft/gpl.html

****************************************************************
Collective Optimization Database Copyright and License:

 Collective Optimization Database (cDatabase or COD ) has been developed
 by Grigori Fursin mostly in his spare time (Copyright 2004-current).
 It is described in FT2009 and FMTP2008 publications and in the MILEPOST
 project proposal and tech. reports (2005-2009).

 In 2009, Grigori opened public access to this database (http://cTuning.org/cdatabase)
 in a hope that it is useful to the community. cTuning website provides an open public
 access to cDatabase since it is intended to share optimization knowledge
 with the community and to enable community-driven program and system optimization.
 It is provided AS IS, with no warranty of any kind, express or implied.

 Replication, mirroring, extension and public re-hosting of cDatabase requires
 explicit agreement with Grigori Fursin to keep cDatabase, cTools APIs
 and web-services compatible and open to everyone. If you have any questions
 or comments, please don't hesitate to contact Grigori.

****************************************************************
CCC history:

 V2.5 Build 1020:  * Deprecated:
                     * src-plat-dep/plugins/ml-prediction/ccc-ml-predict-best-flag
                       (integrated with the cTuning Compiler Collection: http://cTuning.org/ctuning-cc)
                     * src-plat-dep/tools/milepost-gcc
                       (integrated with the cTuning Compiler Collection: http://cTuning.org/ctuning-cc)

                   * Added src-plat-dep/tools/ctuning-cc
                     new cTuning Compiler Collection (http://cTuning.org/ctuning-cc)
                     It is a wrapper around any user compiler to enable transparent architecture,
                     code and dataset analysis, characterization and multi-objective
                     optimization (currently execution time, code size and compilation time)
                     based on iterative feedback directed compilation, statistical analysis,
                     collective optimization and machine learning (predictive modeling)
                     (by Grigori Fursin).

                   * Changed src-plat-dep/plugins/ml-prediction to src-plat-dep/plugins/ml
                     (since not only optimization prediction plugins but more general machine learning plugins).

                   * Fixed bug in ctuning-cc/milepost-gcc to be able to recompile the whole Linux or kernel
                     (by Abdul Memon).

 V2.5 Build 1019:  * (2010.05.12) Multiple bug fixes and extensions in the configuration scripts 
                     (by Abdul Memon and Grigori Fursin).

 V2.5 Build 1018:  * (2010.03.15) Multiple aggregated updates (major updates in speedup calculation engine
                     and multi-objective optimization engine: execution time, code size, compilation time)

 Build 1017:       * Grigori fixed many issues with src-plat-indep/include/ccc_script_functions.php
                     to avoid crashes when MySQL queries returns NULL ...

 Build 1016:       * Grigori updated milepost-gcc and cTuning functions, 
                     fixed mistake with CCC_GPROF (requires small benchmark update as well).

 Build 1015:       * Grigori updated functionality in src-plat-indep/include/ccc_scripts_functions.php
                     to unify calculation of speedups for execution time, code size, compilation time,
                     average them correctly, detect cases with profiling and notes, etc:
                     - Function get_opt_cases is not deprecated and associated scripts are removed
                     - New function get_opt_cases1 to calculate speedups and return array of opt cases
                     - New function sort_speedups to sort based on multi-objective criteria such
                       as sort by exec.time, then compilation time, then code size, etc
                     - New function find_frontier to find frontier optimization cases in
                       multi-dimensional optimization spaces (2D, 3D, thresholds per dimension, etc).

                     Added associated script get-global-speedups, get-global-speedups-by-list.

                     Added plot-bar-graph to plot graphs based on cvs data the previous scripts.

                     Added directory src-plat-indep/plugins/Stats to save statistics/analysis info.

                     Updated most of the scripts in src-plat-indep/plugins to correctly support new functions.

 Build 1014:       * Added plugin src-plat-indep/plot-density-graph (uses R) 
                     that can be used with Speedup Test
                   * Updated src-plat-indep/include/ccc_script_functions.php
                     (synced with some cTuning.org web services)

 Build 1013:       * Fixed bug with EXTRAINCLUDE flags in 
                     src-plat-dep/tools-aux/hardware-counters-papi/ccc-papi-list/Makefile

 Build 1012:       * src-plat-dep/plugins/compilation/ccc-run-glob-flags-rnd-fixed/ccc-run-glob-flags-rnd-fixed.c:
                     src-plat-dep/plugins/compilation/ccc-run-glob-flags-rnd-uniform/ccc-run-glob-flags-rnd-uniform.c:
                     src-plat-dep/plugins/compilation/ccc-run-glob-flags-one-by-one/ccc-run-glob-flags-one-by-one.c:
                     src-plat-dep/plugins/compilation/ccc-run-glob-flags-one-off-rnd/ccc-run-glob-flags-one-off-rnd.c:
                       added fflush(NULL) before system(); 

 Build 1011:       * Time measurement mechanism changed. Now we are using /usr/bin/time program.

 Build 1010:       * Fixed bug in ccc-comp wrapper script that would not parse 
                     optimization flags properly before.

 Build 1009:       * Fixed bug with ccc-run.c and hardware counters,

 V2.0x             * (2009.05.29) Multiple updates based on user feedback:

                       * convert ccc-time CXX to C (now only C is needed)
                       * bug with SSL init in send data ...
                       * bug with Makefiles 
                       * bug when file size can be random if file is not generated
                       * remove . from $PATH
                       * set names of the compiler and linker in environment (ZCC, ZCXX, ZFC) 
                         to support different compilers
                       * add CCC_RE (runtime environment such as VM or simulator) to _run scripts
                       * add RE_ID to COD
                       * move compiler and runtime environment from ccc-env to separate files
                         to be able to install different version of the same compiler on the system:
                         # ccc-env.c.<compiler name> - compiler environment
                         # ccc-env.re.<runtime environment name> - runtime environment
                         # ccc-re.cfg - configuration file for runtime environments
                       * ccc-run now support CCC_RUN_RE environment variable to setup runtime environment
                       * add support for uuid / uuidgen in CCC_UUID environment variable
                       * add RUNTIME_ENVIRONMENTS table to COD
                       * sort out mistake at cTuning website to show function-level profiling
                       * update COD version
                       * changed tmp-ccc-run-datasets tmp-ccc-run-cmd
                       * fixed ccc-ml-accumulate-features (sometimes all features =0)
                       * add PathScale, GCC4CLI and LLVM compiler support
                       * save program binary name to _ccc_program_bin (for SPEC2006)
                       * save compiler environment to _ccc_compilation_env_cur (for SPEC2006)
                       * add support for OProfile ...
                       * CCC_OPROF
                       * CCC_OPROF_PARAM
                       * added CCC_TIMED_RUN instead of CCC_TIMEOUT
                       * added CCC_TASKSET
                       * added CCC_PROCESSOR_NUM to execute programs on a specific core
                       * MILEPOST GCC wrapper is added to support automatic feature extractor and querying
                         of COD web services to predict good optimizations with flags -ml, ml-e, -ml-c

 V1.x              * (2006-2009) Used in the MILEPOST project on machine learning to automate
                                 architecture and code design and optimization (http://cTuning.org/project-milepost)
                                 and UNIDAPT group research (http://unidapt.org)

 prototyping       * (2004-2006) Development of the Framework for Continuous Optimization (FCO)
                                 to support fine-grain optimizations (unrolling, tiling, loop interchange,
                                 fusion/fission, vectorization, prefetching) using Interactive Compilation
                                 Interface for PathScale/Open64 compilers:
                                 http://fursin.net/wiki/index.php5?title=Research:Developments:FCO

 prototyping       * (1999-2003) Development of the Edinburgh Optimizing Software used in the MHAOTEU project
                                 to perform iterative compilation, performance prediction and collaborative
                                 optimization to improve memory performance using traditional and polyhedral
                                 optimizations (supporting and extending one of the first practical polyhedral
                                 source-to-source MARS compiler which unfortunately has not been further
                                 developed due to lack of funding - since 2010, we plan to add support
                                 for polyhedral optimizations in CCC using GRAPHITE framework for GCC and LLVM).

****************************************************************
Documentation:
 Currently available online at:
 http://cTuning.org/wiki/index.php/CTools:CCC:Documentation

 You can find some additional info about Collective Optimization
 concept in the following publications:

 http://unidapt.org/index.php/Dissemination#FT2009
 http://unidapt.org/index.php/Dissemination#Fur2009
 http://unidapt.org/index.php/Dissemination#FMTP2008

****************************************************************
Installation:
 Run INSTALL.sh to configure CCC Framework and compile all the tools

****************************************************************
