#!/bin/bash

# Author: Grigori Fursin (http://fursin.net/research)
#
# (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France
# (C) 2004-2008, UNIDAPT Group, INRIA, France

FILE_CCC_TMP="tmp-ccc.tmp"

################################################################################
#set current directory since some tools can change that
#this variable is also used to avoid asking for removal of the platform
#directory unlike individual cases

CURINSTALL=$PWD

. ./ccc-configure-platform-name.sh
echo $CCC_PLATFORM
. ./ccc-configure-uuid.sh
. ./ccc-configure-database.sh
. ./ccc-configure-database-test.sh
. ./ccc-configure-platform.sh
. ./ccc-configure-environment.sh
. ./ccc-configure-compiler.sh
. ./ccc-configure-runtime-environment.sh
. ./ccc-configure-compile-all-tools.sh
cd $CURINSTALL
. ./ccc-configure-compile-all-plugins.sh
cd $CURINSTALL
. ./ccc-configure-compile-all-tools-aux.sh
cd $CURINSTALL
. ./ccc-configure-update.sh
. ./ccc-configure-set-environment.sh

echo ""
echo "CCC installation is done!"
