#!/bin/bash

# Author: Grigori Fursin (http://fursin.net/research)
#
# (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France
# (C) 2004-2009, UNIDAPT Group, INRIA, France

FILE_CCC_CMPL_EXT="ccc-compilers.cfg"
FILE_CCC_CMPL_OPTS_EXT="ccc-glob-flags"
FILE_CCC_CENV="ccc-env.c"
FILE_CCC_TMP="tmp-ccc.tmp"
FILE_CCC_TMP1="tmp-ccc1.tmp"
FILE_CCC_TMP2="tmp-ccc2.tmp"
FILE_CCC_TMP3="tmp-ccc3.tmp"

################################################################################
echo "*************** CCC: Configure compiler ***************"

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

fcext=cfg/$CCC_PLATFORM/$FILE_CCC_CMPL_EXT

upd=0
if [ -f "$fcext" ] ; then
 echo ""
 echo "File with Compiler info $fcext already exists:"
 
 echo ""
 cat $fcext
fi

echo ""
echo "Would you like to configure a new compiler (Y/N)?"
read var
 
if [ "$var" = "Y" ] || [ "$var" = "y" ] ; then
 echo ""
else
 return
 exit
fi

 echo ""
 echo "Adding new compiler info ..."

 echo ""
 echo "Enter compiler name (for example, gcc, opencc, pathcc, icc, etc):"
 read var

 echo ""
 echo "Trying to run compiler with the --version flag"
 echo ""
 $var --version

 echo ""
 echo "Enter compiler extension (the same extension should be added to your Makefiles!)"
 echo "  (for example gcc, open64, pathscale, intel, gcc4cli, llvm, etc):"
 read ctitle

 echo ""
 echo "Enter some local name to idenfidy this compiler configuration (gcc44x, opencc_best_opts, pathscale, icc, etc):"
 read var_local

 ###### read database info ######
 if [ -z "$CCC_URL" ] ; then
  echo ""
  echo "Since \$CCC_URL = \"\", it means that you decided not to use" 
  echo "  Collective Optimization Database. Later, if you change you mind,"
  echo "  you can install and configure it using appropriate individual installation script..."
  rm -f *.tmp
 else
  . ccc--db-cur-info.sh
 fi

 if [ ! -z "$CCC_URL" ] ; then
  echo ""
  echo "Would you like to check if the same compiler exists in the database (Y/N)?"
  echo "  (it is needed to provide unique COMPILER_ID for Collective Tuning)"
  read var1

  if [ "$var1" = "Y" ] || [ "$var1" = "y" ] ; then
   ###### check if compiler exists ######
   echo ""
   echo "Checking if similar compilers exist in the COMMON database ..."

   echo "select COMPILER_ID, NAME, LANGUAGE, VERSION, NOTES from COMPILERS where (NAME=\"$ctitle\")" > $FILE_CCC_TMP
   mysql -h $CCC_C_URL -u $CCC_C_USER --password=$CCC_C_PASS $CCC_C_DB < $FILE_CCC_TMP > $FILE_CCC_TMP1

   i=0
   exec 9< $FILE_CCC_TMP1
   while <&9 read line ;
   do
    let i=i+1
   done

   if [ $i -ne 0 ] ; then
    echo ""
    echo "List of compilers:"
    echo ""
    cat $FILE_CCC_TMP1
   fi
  fi
 fi

 echo ""
 echo "Would you like to generate new UUID for your compiler (Y/N)?"
 echo "  (otherwise you will be able to enter an existing one"
 echo "   from the Collective Optimization Database http://cTuning.org/cdatabase)"

 read var1

 already=0
 if [ "$var1" = "Y" ] || [ "$var1" = "y" ] ; then
  echo ""
  echo "Generating UUID for this compiler ..."
 
  ccc-uuidgen > $FILE_CCC_TMP
  exec 9< $FILE_CCC_TMP
  read <&9 uuid1
  read <&9 uuid2
  rm -rf $FILE_CCC_TMP
  cid=$uuid1$uuid2

  echo $cid
 else
  already=1
  echo ""
  echo "Enter COMPILER_ID:"

  read cid
 fi

 echo ""
 echo "If needed, enter extra compiler flags (will always be added during compilation)"
 echo " (it may be needed for cross-compilation such as -static for ARC processors)"
 
 read cadd
 echo "COMPILER_CONFIGURATION_NAME="$var_local  >> $fcext
 echo "COMPILER_EXT="$ctitle >> $fcext
 echo "COMPILER_ID="$cid    >> $fcext
 echo "COMPILER_ADD_OPTS="$cadd   >> $fcext
 echo ""                           >> $fcext

 fcoext=cfg/$CCC_PLATFORM/$FILE_CCC_CMPL_OPTS_EXT.$var_local.cfg
 fcoenv=cfg/$CCC_PLATFORM/$FILE_CCC_CENV.$var_local

 upd=0
 if [ -f "$fcoext" ] ; then
  echo ""
  echo "File with the list of global optimization flags for iterative compilation $fcoext already exists:"
  echo "Would you like to remove it and copy a default one (Y/N)?"
  read var
 
  if [ "$var" = "Y" ] || [ "$var" = "y" ] ; then
   upd=1
  fi
 else
  upd=1
 fi
 
 if [ $upd -eq "1" ] ; then
  echo ""
  echo "Updating file with global flags ..."
  cp -f cfg/_default/$FILE_CCC_CMPL_OPTS_EXT.$ctitle.cfg-default $fcoext
 fi

 upd=0
 if [ -f "$fcoenv" ] ; then
  echo ""
  echo "File with the environment for this compiler already exists:"
  echo "Would you like to remove it and create a new one (Y/N)?"
  read var
 
  if [ "$var" = "Y" ] || [ "$var" = "y" ] ; then
   upd=1
  fi
 else
  upd=1
 fi

 if [ $upd -eq "1" ] ; then
  echo ""
  echo "Enter environment for the compiler if needed (Press <Ctrl-D> to finish and save file) ..."
  echo ""

   echo "Example:"
   echo "  #!/bin/bash"
   echo ""
   echo "  ##CCC supports SSH execution with NFS (for ARC board for example)"
   echo "  #export CCC_RUN_SSH=\"ssh root@arcangel -v -i ~/.ssh/id_rsa_arc\""
   echo "  #export CCC_RUN_SSH_PATH=\"export PATH=.:/nfs/arcangel/bin:\\\$PATH\""
   echo "  #export CCC_RUN_SSH_NFS=/nfs"
   echo "  ##remove number of directories from the original path before concatenating with CCC_RUN_SSH_NFS"
   echo "  #export CCC_RUN_SSH_NFS_REMOVE=2"

   echo ""
   echo "  ##GCC with ICI"
   echo "  #export PATH=<<<UPDATE>>>"
   echo "  #export LD_LIBRARY_PATH=<<<UPDATE>>>"
   echo "  #export XSB_DIR=<<<UPDATE>>>"
   echo "  #export ML_ST_FEAT_TOOL=<<<UPDATE>>>"

   echo ""
   echo "  #ICI PLUGINS"
   echo "  # the user should have ICI system installed with"
   echo "  # all the variables properly set"
   echo "  export CCC_ICI_USE=ICI_USE"
   echo "  export CCC_ICI_PLUGINS=ICI_PLUGIN"
   echo ""
   echo "  export CCC_ICI_PASSES_FN=ici_passes_function"
   echo "  export CCC_ICI_PASSES_EXT=.txt"
   echo "  #export CCC_ICI_PASSES_RECORD_PLUGIN=<<<UPDATE>>>/gcc-ici-plugins/save-executed-passes.so"
   echo "  #export CCC_ICI_PASSES_USE_PLUGIN=<<<UPDATE>>>/gcc-ici-plugins/use-pass-order.so"
   echo "  export CCC_ICI_PASSES_USE_TEST_DIR=\$CCC_CFG/ici-passes/"
   echo "  export CCC_ICI_PASSES_USE_FUNCTION=ici_passes_test"
   echo ""
   echo "  export CCC_ICI_FEATURES_ST_FN=ici_features_function"
   echo "  export CCC_ICI_FEATURES_ST_EXT=.ft"
   echo "  #export CCC_ICI_FEATURES_ST_EXTRACT_PLUGIN=<<<UPDATE>>>/gcc-ici-plugins/extract-program-static-features.so"
   echo ""
   echo "  #HARDWARE COUNTERS - PAPI"
   echo "  export CCC_HC_PAPI_LIST=PAPI_L1_DCM,PAPI_L1_ICM,PAPI_L2_DCM,PAPI_L2_ICM,PAPI_L1_TCMx,PAPI_L2_TCM,PAPI_FPU_IDL,PAPI_TLB_DM,PAPI_TLB_IM,PAPI_TLB_TL,PAPI_L1_LDM,PAPI_L1_STM,PAPI_L2_LDM,PAPI_L2_STM,PAPI_STL_ICY,PAPI_HW_INT,PAPI_BR_TKN,PAPI_BR_MSP,PAPI_TOT_INS,PAPI_FP_INS,PAPI_BR_INS,PAPI_VEC_INS,PAPI_RES_STL,PAPI_TOT_CYC,PAPI_L1_DCH,PAPI_L2_DCH,PAPI_L1_DCA,PAPI_L2_DCA,PAPI_L2_DCR,PAPI_L2_DCW,PAPI_L1_ICH,PAPI_L2_ICH,PAPI_L1_ICA,PAPI_L2_ICA,PAPI_L1_ICR,PAPI_L1_TCHx,PAPI_L2_TCH,PAPI_L1_TCA,PAPI_L2_TCAx,PAPI_FML_INS,PAPI_FAD_INS,PAPI_FP_OPS"

  echo ""
  cat > $fcoenv
 fi

 chmod 755 $fcoenv
 
 echo ""
 echo "Note: You may want to edit file $fcoext"
 echo "        with the list of global optimization flags for your compiler ..."
 echo "Note: You may want to edit file $fcoenv"
 echo "      with the environment for your compiler ..."

 echo ""
 echo "Compiler configuration files are created!"

 if [ ! -z "$CCC_URL" ] && [ $already -ne "1" ]; then
  ###### check fileds ######
  echo ""
  echo "Would you like to add information about your compiler to the COMMON database (Y/N)?"
  read var
 
  if [ "$var" = "Y" ] || [ "$var" = "y" ] ; then
   echo "show FIELDS from COMPILERS" > $FILE_CCC_TMP
   mysql -h $CCC_C_URL -u $CCC_C_USER --password=$CCC_C_PASS $CCC_C_SSL $CCC_C_DB < $FILE_CCC_TMP > $FILE_CCC_TMP1
   rm -rf $FILE_CCC_TMP

   echo ""
   echo "*************** Enter the following information for this compiler: ***************"

   echo ""
   echo "Available fields:"
   echo ""

   i=0
   fin=0
   exec 9< $FILE_CCC_TMP1
   while <&9 read line ;
   do
    if [ $i -gt "0" ] && [ $fin -ne "1" ]; then
      fld=${line%%	*}
      if [ $fld = "DATETIME" ] ; then
       fin=1
      else
       echo "$fld:"
      fi
    fi
    let i=i+1
   done

   echo ""

   echo -n "insert into COMPILERS (COMPILER_ID" > $FILE_CCC_TMP
   echo -n "values (\"$cid\"" > $FILE_CCC_TMP2

   i=0
   fin=0
   exec 9< $FILE_CCC_TMP1
   while <&9 read line ;
   do
    if [ $i -gt "1" ] && [ $fin -ne "1" ] ; then
      fld=${line%%	*}
 
      if [ $fld = "DATETIME" ] ; then
       fin=1
      else
       echo "$fld:"
       read xfld
    
       echo -n ,$fld >> $FILE_CCC_TMP
       echo -n ,\"$xfld\" >> $FILE_CCC_TMP2
      fi
    fi
    let i=i+1
   done

    #Add datetime
     fld="DATETIME"
      date '+%F %T' > $FILE_CCC_TMP3
      exec 8< $FILE_CCC_TMP3
      read <&8 xfld
      rm -rf $FILE_CCC_TMP3
     echo -n ,$fld >> $FILE_CCC_TMP
     echo -n ,\"$xfld\" >> $FILE_CCC_TMP2
   
    #Add IP   
     fld="IP"
      ccc-read-ip > $FILE_CCC_TMP3
      exec 8< $FILE_CCC_TMP3
      read <&8 xfld
      rm -rf $FILE_CCC_TMP3
     echo -n ,$fld >> $FILE_CCC_TMP
     echo -n ,\"$xfld\" >> $FILE_CCC_TMP2

    #Add USER
     fld="USER"
     xfld=$CCC_CTS_USER
     echo -n ,$fld >> $FILE_CCC_TMP
     echo -n ,\"$xfld\" >> $FILE_CCC_TMP2
 
   echo -n ") " >> $FILE_CCC_TMP
   echo -n ")" >> $FILE_CCC_TMP2

   cat $FILE_CCC_TMP2 >> $FILE_CCC_TMP
  
   echo ""
   echo "Recording data to the database ..."

   mysql -h $CCC_C_URL -u $CCC_C_USER --password=$CCC_C_PASS $CCC_C_SSL $CCC_C_DB < $FILE_CCC_TMP
 
   rm -f *.tmp
  fi
 fi

 rm -f *.tmp
