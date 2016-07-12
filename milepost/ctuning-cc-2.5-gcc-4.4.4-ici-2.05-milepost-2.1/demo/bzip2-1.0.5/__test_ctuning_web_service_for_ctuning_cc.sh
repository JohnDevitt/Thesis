#!/bin/bash

# Author: Grigori Fursin (http://fursin.net/research)
#
# (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France
# (C) 2007-2010, UNIDAPT Group, INRIA, France (http://unidapt.org)

source ./___common_environment.sh

if [ -z "$CCC_CTS_PASS" ] ; then
 read -s -p "  Enter cTuning password for $CCC_CTS_USER : " pass
 export CCC_CTS_PASS=$pass
 echo ""
fi

ctuning-cc --ct-test *.c
