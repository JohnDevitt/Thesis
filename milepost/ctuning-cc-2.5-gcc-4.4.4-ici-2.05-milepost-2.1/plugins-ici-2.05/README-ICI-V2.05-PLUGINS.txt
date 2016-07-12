****************************************************************
ICI 2.0x plugin repository

ICI website (downloads, documents, collaborative development, examples):
 http://cTuning.org/ici

cTuning/ICI mailing lists (feedback, comments and bug reports):
 http://cTuning.org/community

cTuning Compiler Collection
 http://cTuning.org/ctuning-cc
 
MILEPOST GCC
 http://cTuning.org/milepost-gcc

Maintained by cTuning community and UNIDAPT Group:
 http://cTuning.org
 http://unidapt.org

****************************************************************
Authors: 

 Grigori Fursin (http://fursin.net/research)
 Cupertino Miranda <cupertinomiranda@gmail.com>
 Zbigniew Chamski <zbigniew.chamski@gmail.com>
 Mircea Namolaru <namolaru@il.ibm.com>

****************************************************************
History:

 2.05  - 20100517 - Grigori added extract_program_structure plugin
                    to support cTuning Compiler Collection (http://cTuning.org/ctuning-cc)
 
         20100315 - Small bug fixes.

 2.0   - 20090529 - Major update for all plugins to support new ICI 2.0.

 1.0   - 20080613 - Updated plugins for MILEPOST GCC.
                    More info is available at http://unidapt.org/software.html#milepostgcc

****************************************************************
Available plugins:

* save_executed_passes.legacy

  This plugin saves the executed passes per function in 
  external files "ici_passes_function.<function_name>.txt"
  (legacy code from ICI 1.0)

* save_executed_passes_with_time.legacy

  This plugin times the execution of the passes 
  (legacy code from ICI 1.0) for split-compilation.

* substitute-passes.legacy

  This plugin substitutes original GCC pass order with 
  the one read from either external files "ici_passes_function.<function_name>.txt",
  1 global file "ici_passes_all.txt" or environment variable ICI_PASSES_ALL
  (passes are separated by comma) thus allowing external 
  manipulation with passes (adding, removing or reordering)
  (legacy code from ICI 1.0).

* extract_program_static_features

  This plugin extracts program static features per function as vectors
  and saves them into "ici_features_function.<function_name>.<pass_to_extract>.txt"

  Enrivonment variables:
   ICI_PROG_FEAT_PASS = <pass_to_extract> - after this pass extract features

  This pass calls ml-feat-proc routine that communicates with Prolog (XSB)
  to produce feature vector

  Remark:
   This plugin works only with MILEPOST GCC v1.0 or higher.

   More information is avaiable at http://unidapt.org/software.html#milepostgcc

                  and in the paper:

   MILEPOST GCC: machine learning based research compiler. 
   Grigori Fursin, Cupertino Miranda, Olivier Temam, Mircea Namolaru, Elad Yom-Tov, 
   Ayal Zaks, Bilha Mendelson, Phil Barnard, Elton Ashton, Eric Courtois, Francois Bodin, 
   Edwin Bonilla, John Thomson, Hugh Leather, Chris Williams, Michael O'Boyle.
   Proceedings of the GCC Developers' Summit, Ottawa, Canada, June 2008

   http://unidapt.org/publications.html

* extract_program_structure

  This beta plugin extracts some program structure and saves it to
  the file specified by the environment variables CTUNING_EXTRACT_STRUCTURE
  or CTUNING_EXTRACT_FEATURES. 

  This plugin works with cTuning CC V2.5 or higher (http://cTuning.org/ctuning-cc)

* test1
* test2.legacy

  These plugins demonstrate new ICI 2.0 features.

****************************************************************
Testing:
 Directory "tests" has several C and Fortran programs to demonstrate
 ICI plugin system in use.

 Before using test, check and source _set_environment_for_plugins_tests.sh.

 The available tests are:

 Clean the directory:
  __ici_clean.sh

 Record passes at function level (using -fici and environment variable):
  __ici_compile_generate_passes_at_function_level1.sh
  __ici_compile_generate_passes_at_function_level.sh

 Substitute default GCC passes with the ones from files:
  __ici_compile_read_passes_from_file1.sh
  __ici_compile_read_passes_from_file.sh

 Extract program static features:
  __ici_compile_read_passes_from_file_ml_feat1.sh
  __ici_compile_read_passes_from_file_ml_feat.sh

 ICI tests (printing different info and demonstrating ICI 2.0 features):
  __ici_compile_test1.sh
  __ici_compile_test2.sh

 Run compiled program:
  __ici_run.sh

****************************************************************
