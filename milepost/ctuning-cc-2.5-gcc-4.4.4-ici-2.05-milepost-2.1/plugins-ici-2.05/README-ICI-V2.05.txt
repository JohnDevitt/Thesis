****************************************************************
GCC-ICI v2.0x (Interactive Compilation Interface)

The Interactive Compilation Interface (or 'ICI' for short) is a plugin
system with a high-level compiler-independent and low-level
compiler-dependent API to transform current compilers into collaborative
open modular interactive toolsets. The ICI framework acts as a "middleware"
interface between the compiler and the user-definable plugins. It opens up
and reuses the production-quality compiler infrastructure to enable program
analysis and instrumentation, fine-grain program optimizations, simple
prototyping of new development and research ideas while avoiding building
new compilation tools from scratch. For example, it is used in MILEPOST GCC
to automate compiler and architecture design and program optimizations based
on statistical analysis and machine learning. It should enable universal
self-tuning compilers adaptable to heterogeneous, reconfigurable, multi-core
architectures ranging from supercomputers to embedded systems.

Currently ICI is a community-driven project to define common API to
transform production-quality compilers with state-of-the-art transformations
into collaborative modular plugin-enabled interactive R&D toolsets that
allows quick prototyping of research ideas. In case the research idea
becomes mature, it can now move to the mainline production compiler much
faster. ICI can help end-users optimize their programs and architectures at
fine-grain level to improve execution time, code size, power consumption,
compilation time, etc. We also hope that using ICI will improve the quality
and reproducibility of academic research. Eventually, we hope that it will
help to overcome the complexity of current, emerging and future computing
systems (including heterogeneous multi-core architectures) by simplifying
and automating compiler design and testing as well as program optimization.

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
 Grigori Fursin <gfursin@gmail.com>
 Cupertino Miranda <cupertinomiranda@gmail.com>
 Zbigniew Chamski <zbigniew.chamski@gmail.com>

****************************************************************
Brief Information:

 The main aim of the Interactive Compilation Interface (ICI)
 is to transform GCC (production quality non-research compiler
 that supports many languages and architectures) into research
 compiler with minimal changes. This helps to avoid writing
 research tools from scratch and utilize stable, production
 quality compilers.

 ICI can be used to enable research on iterative fine-grain program
 optimizations for different constraints (performance, code size,
 power consumption, DSE, different ISAs, etc),
 to unify optimization knowledge reuse among different programs
 and architectures using statistical and machine learning techniques,
 and to improve default compiler optimization heuristic.

 We hope that it will eventually make compilers truly modular
 with automatically tuned evolving optimization heuristics.

 More information:
  http://cTuning.org/wiki/index.php/CTools:ICI:Motivation

****************************************************************
History:
 2.05  - 20100315 - Joern Rennecke suggested to use lang_hooks.decl_printable_name
                    function when returning names of functions to support g++.
                    Grigori changed a few more files to support g++ and changed
                    a bug in ml-feat.c to support g++ function names.

 2.01  - 20090530 - Small bug fix.

 2.0   - 20090529 - Major update, support for GCC 4.4.0, synchronization with GCC plugin branch,
                    full documentation, community-based extension projects.

 1.0   - 20080613 - New version for MILEPOST GCC. ICI Plugins are separated from the
                    distribution since they can be independent of GCC.
                    Machine learning routines will be included in the MILEPOST GCC release.
                    More info is available at http://unidapt.org/software.html#milepostgcc .
 
 0.9.6 - 20080312 - Updated version for the MILEPOST GCC
                    (was not officially released).

 0.9.5 - 20080211 - Updated design for correct pass selection and reordering
                    avoiding pass-gate ... + some bug fixes.

 0.9.1 - 20070916 - Bug fix release.

 0.9   - 20070915 - New design using dynamically linked plugins
                    and pass manipulations routines.

 0.1   - 20070126 - Prototyping
 
 0.0.1 - 20061103 - First prototype based on PathScale/Open64 ICI
                    developed by Grigori Fursin in 2004.

****************************************************************
Directories/files:

 gcc-4.4.0                       - GCC 4.4.0 source directory (core + gfortran)


 src-third-party                 - Third party support tools
  |
  +-- gmp-4.3.0                  - GMP library
  +-- mpfr-2.4.1                 - MPFR library
  +-- ppl-0.10.2                 - PPL library (for GRAPHITE)
  +-- cloog                      - CLOOG library (for GRAPHITE)
  +-- XSB                        - Prolog for machine learning tools (MILEPOST, UNIDAPT, cTuning)

 plugins-ici-2.0                 - Plugins for GCC 4.4.0 ICI 2.0 (see README inside this directory)

 install                         - Directory with installed binaries

****************************************************************
Installation:

 ./build_gcc.sh will build GCC with all the third-party tools.

 ./build_plugins.sh will build all plugins.

 Now you can test the plugin system using self-explanatory tests 
 in plugins-ici-2.0/tests directory

 Full documentation is available here:
  http://cTuning.org/wiki/index.php/CTools:ICI:Documentation

****************************************************************
Acknowledgments:

 * Fabio Arnone (STMicro, France)
 * Phil Barnard (ARC, UK)
 * Francois Bodin (CAPS Entreprise, France)
 * Zbigniew Chamski (InfraSoft IT Solutions, Poland)
 * Bjorn Franke (University of Edinburgh, UK)
 * Grigori Fursin (INRIA, France)
 * Taras Glek (Mozilla, USA)
 * Cupertino Miranda (INRIA, France)
 * Mircea Namolaru (IBM, Israel)
 * Diego Novillo (Google, USA)
 * Sebastian Pop (AMD, USA)
 * Joern Rennecke (UK)
 * Basile Starynkevitch (CEA, France)
 * Ayal Zaks (IBM, Israel) 

 HiPEAC network of excellence (http://www.hipeac.net)
 Milepost project (http://www.milepost.eu)
 SARC project (http://www.sarc-ip.org)
 GGCC (http://www.ggcc.info)

 Other colleagues from IBM, NXP, STMicro, ARC, CAPS Enterprise ...

****************************************************************
Future work:
 It's a community-driven project. You can find information
 about extensions at http://cTuning.org/ici

 Since 2009 we are working with GCC community to move parts 
 of the ICI to the new GCC plugin branch.

****************************************************************
