#!/bin/bash

# Author: Grigori Fursin (http://fursin.net/research)
#
# (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France
# (C) 2004-2009, UNIDAPT Group, INRIA, France

################################################################################
echo "*************** CCC: Select UUID variable ***************"

export PATH=$PWD/src-plat-indep/plugins:$PATH

### Check/select platform
if [ -z "$CCC_PLATFORM" ] ; then
 echo ""
 echo "CCC Platform has not been yet configured since \$CCC_PLATFORM variable is empty."
 echo ""
 echo "Run INSTALL.sh first!"
 rm -f *.tmp
 exit
fi

echo "Checking uuid tool:"
echo ""
echo "Execute uuid"
uuid
echo ""
echo "Execute uuidgen"
uuidgen

echo ""
echo "Enter the name of the UUID tool (<Enter> for uuidgen):"
read CCC_UUID
if [ -z $CCC_UUID ] ; then
 CCC_UUID="uuidgen"
fi
export CCC_UUID

rm -f *.tmp
