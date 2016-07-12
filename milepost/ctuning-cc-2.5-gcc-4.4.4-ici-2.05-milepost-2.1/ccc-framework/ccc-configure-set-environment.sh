#!/bin/bash

# Copyright (C) 2004-2009 by Grigori Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

FILE_CCC_ENV="ccc-env.sh"
FILE_CCC_ENV1="ccc-env.csh"
FILE_CCC_TMP="tmp-ccc.tmp"
FILE_CCC_TMP1="tmp-ccc1.tmp"
FILE_CCC_TMP2="tmp-ccc2.tmp"
FILE_CCC_TMP3="tmp-ccc3.tmp"

################################################################################
echo "*************** CCC: Configure environment ***************"

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
     
if [ ! -d "cfg/$CCC_PLATFORM" ] ; then
 echo ""
 echo "Directory cfg/$CCC_PLATFORM doesn't exist!"
 rm -f *.tmp
 return
 exit
fi

if [ ! -d "install/$CCC_PLATFORM" ] ; then
 echo ""
 echo "Directory install/$CCC_PLATFORM doesn't exist!"
 rm -f *.tmp
 return
 exit
fi

if [ -z "$CCC_PLATFORM_ID" ] ; then
 echo ""
 echo "\$CCC_PLATFORM_ID is not set!"
 rm -f *.tmp
 return
 exit
fi

if [ -z "$CCC_ENVIRONMENT_ID" ] ; then
 echo ""
 echo "\$CCC_ENVIRONMENT_ID is not set!"
 rm -f *.tmp
 return
 exit
fi

if [ -z "$CCC_URL" ] ; then
 echo ""
 echo "Since \$CCC_URL = \"\", it means that you decided not to use" 
 echo "  Collective Optimization Database. Later, if you change you mind,"
 echo "  you can install and configure it using appropriate individual installation script..."
else
 . ccc--db-cur-info.sh
fi

fenv=cfg/$CCC_PLATFORM/$FILE_CCC_ENV
fenv1=cfg/$CCC_PLATFORM/$FILE_CCC_ENV1
upd=0
if [ -f "$fenv" ] || [ -f "$fenv1" ] ; then
 echo "Files with CCC ENVIRONMENT VARIABLES $fenv or $fenv1 already exist."
 
 echo ""
 echo "Would you like to regenerate them (Y/N)?"
 read var
 
 if [ "$var" = "Y" ] || [ "$var" = "y" ] ; then
  echo ""
  echo "Regenerating ENVIRONMENT VARIABLES ..."
  
  upd=1
 fi
else
 upd=1
fi

if [ $upd -eq "1" ] ; then
################################################################
 echo "#!/bin/bash" > $fenv
 echo "" > $fenv

 echo "export CCC_ROOT=$PWD" > $fenv
 echo "export CCC_PLATFORM=$CCC_PLATFORM" >> $fenv
 echo "" >> $fenv
 echo "export CCC_PLUGINS=\$CCC_ROOT/src-plat-indep" >> $fenv
 echo "export PATH=\$CCC_ROOT:\$CCC_ROOT/install/\$CCC_PLATFORM/bin:\$CCC_PLUGINS/plugins:\$PATH" >> $fenv
 echo "export CLASSPATH=.:\$CCC_ROOT/install/\$CCC_PLATFORM/bin:\$CLASSPATH" >> $fenv
 echo "export CCC_CFG=\$CCC_ROOT/cfg/\$CCC_PLATFORM" >> $fenv
 echo "export CCC_DB_VER=\$CCC_ROOT/ccc-build-db.cfg" >> $fenv
 echo "export CCC_PLATFORM_ID=$CCC_PLATFORM_ID" >> $fenv
 echo "export CCC_ENVIRONMENT_ID=$CCC_ENVIRONMENT_ID" >> $fenv
 echo "export CCC_RUNS=2" >> $fenv
 echo "export CCC_C_URL=$CCC_C_URL" >> $fenv
 echo "export CCC_C_DB=$CCC_C_DB" >> $fenv
 echo "export CCC_C_USER=$CCC_C_USER" >> $fenv
 echo "export CCC_C_PASS=$CCC_C_PASS" >> $fenv
 echo "export CCC_C_SSL=\"$CCC_C_SSL\"" >> $fenv
 echo "export CCC_URL=$CCC_URL" >> $fenv
 echo "export CCC_DB=$CCC_DB" >> $fenv
 echo "export CCC_USER=$CCC_USER" >> $fenv
 echo "export CCC_PASS=$CCC_PASS" >> $fenv
 echo "export CCC_SSL=\"$CCC_SSL\"" >> $fenv
 echo "export CCC_CT_URL=$CCC_CT_URL" >> $fenv
 echo "export CCC_CT_DB=$CCC_CT_DB" >> $fenv
 echo "export CCC_CT_USER=$CCC_CT_USER" >> $fenv
 echo "export CCC_CT_PASS=$CCC_CT_PASS" >> $fenv
 echo "export CCC_CT_SSL=\"$CCC_CT_SSL\"" >> $fenv
 echo "export CCC_CTS_URL=$CCC_CTS_URL" >> $fenv
 echo "export CCC_CTS_DB=$CCC_CTS_DB" >> $fenv
 echo "export CCC_CTS_USER=$CCC_CTS_USER" >> $fenv
 echo "export CCC_CTS_PASS=$CCC_CTS_PASS" >> $fenv
 echo "export CCC_UUID=$CCC_UUID" >> $fenv

 echo "" >> $fenv
 echo "##CCC supports SSH execution with NFS (for ARC board for example)" >> $fenv
 echo "#export CCC_RUN_SSH=\"ssh root@arcangel -v -i ~/.ssh/id_rsa_arc\"" >> $fenv
 echo "#export CCC_RUN_SSH_PATH=\"export PATH=.:/nfs/arcangel/bin:\\\$PATH\"" >> $fenv
 echo "#export CCC_RUN_SSH_NFS=/nfs" >> $fenv
 echo "##remove number of directories from the original path before concatenating with CCC_RUN_SSH_NFS" >> $fenv
 echo "#export CCC_RUN_SSH_NFS_REMOVE=2" >> $fenv

 chmod 755 $fenv

################################################################
 echo "setenv CCC_ROOT $PWD" > $fenv1
 echo "setenv CCC_PLATFORM $CCC_PLATFORM" >> $fenv1
 echo "" >> $fenv1
 echo "setenv CCC_PLUGINS \$CCC_ROOT/src-plat-indep" >> $fenv1
 echo "setenv PATH \$CCC_ROOT:\$CCC_ROOT/install/\$CCC_PLATFORM/bin:\$CCC_PLUGINS/plugins:\$PATH" >> $fenv1
 echo "if ( \$?CLASSPATH == 1) then" >> $fenv1
 echo "  setenv CLASSPATH .:\$CCC_ROOT/install/\$CCC_PLATFORM/bin:\$CLASSPATH" >> $fenv1
 echo "else" >> $fenv1
 echo "  setenv CLASSPATH .:\$CCC_ROOT/install/\$CCC_PLATFORM/bin" >> $fenv1
 echo "endif" >> $fenv1
 echo "setenv CCC_CFG \$CCC_ROOT/cfg/\$CCC_PLATFORM" >> $fenv1
 echo "setenv CCC_DB_VER \$CCC_ROOT/ccc-build-db.cfg" >> $fenv1
 echo "setenv CCC_PLATFORM_ID $CCC_PLATFORM_ID" >> $fenv1
 echo "setenv CCC_ENVIRONMENT_ID $CCC_ENVIRONMENT_ID" >> $fenv1
 echo "setenv CCC_RUNS 2" >> $fenv1
 echo "setenv CCC_C_URL $CCC_C_URL" >> $fenv1
 echo "setenv CCC_C_DB $CCC_C_DB" >> $fenv1
 echo "setenv CCC_C_USER $CCC_C_USER" >> $fenv1
 echo "setenv CCC_C_PASS $CCC_C_PASS" >> $fenv1
 echo "setenv CCC_C_SSL \"$CCC_C_SSL\"" >> $fenv1
 echo "setenv CCC_URL $CCC_URL" >> $fenv1
 echo "setenv CCC_DB $CCC_DB" >> $fenv1
 echo "setenv CCC_USER $CCC_USER" >> $fenv1
 echo "setenv CCC_PASS $CCC_PASS" >> $fenv1
 echo "setenv CCC_SSL \"$CCC_SSL\"" >> $fenv1
 echo "setenv CCC_CT_URL $CCC_CT_URL" >> $fenv1
 echo "setenv CCC_CT_DB $CCC_CT_DB" >> $fenv1
 echo "setenv CCC_CT_USER $CCC_CT_USER" >> $fenv1
 echo "setenv CCC_CT_PASS $CCC_CT_PASS" >> $fenv1
 echo "setenv CCC_CT_SSL \"$CCC_CT_SSL\"" >> $fenv1
 echo "setenv CCC_CTS_URL $CCC_CTS_URL" >> $fenv1
 echo "setenv CCC_CTS_DB $CCC_CTS_DB" >> $fenv1
 echo "setenv CCC_CTS_USER $CCC_CTS_USER" >> $fenv1
 echo "setenv CCC_CTS_PASS $CCC_CTS_PASS" >> $fenv1

 echo "" >> $fenv1
 echo "##CCC supports SSH execution with NFS (for ARC board for example)" >> $fenv1
 echo "#setenv CCC_RUN_SSH \"ssh root@arcangel -v -i ~/.ssh/id_rsa_arc\"" >> $fenv1
 echo "#setenv CCC_RUN_SSH_PATH \"setenv PATH .:/nfs/arcangel/bin:\\\$PATH\"" >> $fenv1
 echo "#setenv CCC_RUN_SSH_NFS /nfs" >> $fenv1
 echo "##remove number of directories from the original path before concatenating with CCC_RUN_SSH_NFS" >> $fenv1
 echo "#setenv CCC_RUN_SSH_NFS_REMOVE 2" >> $fenv1

 chmod 755 $fenv1

################################################################
 echo ""
 echo "NOTE: You can edit $fenv or $fenv1"
 echo "        if you need to update some environment variables (for ICI, for example)!"
 echo "      You should run $fenv or $fenv1"
 echo "        before using CCC Framework !"

 cp -rf cfg/_default/ici-passes cfg/$CCC_PLATFORM
 cp -rf cfg/_default/application cfg/$CCC_PLATFORM

 echo ""
 echo "Visit http://cTuning.org for more info about collaborative R&D tools!"

fi
