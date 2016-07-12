#!/bin/bash

# Author: Grigori Fursin (http://fursin.net/research)
#
# (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France
# (C) 2004-2009, UNIDAPT Group, INRIA, France

FILE_CCC_RE_EXT="ccc-re.cfg"
FILE_CCC_RENV="ccc-env.r"
FILE_CCC_TMP="tmp-ccc.tmp"
FILE_CCC_TMP1="tmp-ccc1.tmp"
FILE_CCC_TMP2="tmp-ccc2.tmp"
FILE_CCC_TMP3="tmp-ccc3.tmp"

################################################################################
echo "*************** CCC: Configure runtime environment ***************"

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

frext=cfg/$CCC_PLATFORM/$FILE_CCC_RE_EXT

upd=0
if [ -f "$frext" ] ; then
 echo ""
 echo "File with runtime environment info $frext already exists:"
 
 echo ""
 cat $frext
fi
 
echo ""
echo "Would you like to add a new runtime environment (such as cil32-mono, lli or simulator) (Y/N)?"
read var
 
if [ "$var" = "Y" ] || [ "$var" = "y" ] ; then
 echo ""
else
 return
 exit
fi

 echo ""
 echo "Adding new runtime environment info ..."

 echo ""
 echo "Enter runtime environment name (for example, cil32-mono, lli, simplescalar, etc):"
 read var

 echo ""
 echo "Trying to run runtime environment with the --version flag"
 echo ""
 $var --version

 echo ""
 echo "Enter some local name to idenfidy this environment configuration (gcc42cil32-mono2, llvm25, simplescalar, etc):"
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
  echo "Would you like to check if the same runtime environment exists in the database (Y/N)?"
  echo "  (it is needed to provide unique RE_ID for Collective Tuning)"
  read var1

  if [ "$var1" = "Y" ] || [ "$var1" = "y" ] ; then
   ###### check if runtime environment exists ######
   echo ""
   echo "Checking if similar runtime environment exist in the COMMON database ..."

   echo "select RE_ID, NAME, VERSION, NOTES from RUNTIME_ENVIRONMENTS where (NAME=\"$var\")" > $FILE_CCC_TMP
   mysql -h $CCC_C_URL -u $CCC_C_USER --password=$CCC_C_PASS $CCC_C_DB < $FILE_CCC_TMP > $FILE_CCC_TMP1

   i=0
   exec 9< $FILE_CCC_TMP1
   while <&9 read line ;
   do
    let i=i+1
   done
   
   if [ $i -ne 0 ] ; then
    echo ""
    echo "List of runtime environments:"
    echo ""
    cat $FILE_CCC_TMP1
   fi
  fi
 fi

 echo ""
 echo "Would you like to generate new UUID for your runtime environment (Y/N)?"
 echo "  (otherwise you will be able to enter an existing one"
 echo "   from the Collective Optimization Database http://cTuning.org/cdatabase)"

 read var1

 already=0
 if [ "$var1" = "Y" ] || [ "$var1" = "y" ] ; then
  echo ""
  echo "Generating UUID for this runtime environment ..."
 
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
  echo "Enter RE_ID:"

  read cid
 fi

 echo "RE_CONFIGURATION_NAME="$var_local >> $frext
 echo "RE_NAME="$var                     >> $frext
 echo "RE_ID="$cid                       >> $frext
 echo ""                                 >> $frext

 frenv=cfg/$CCC_PLATFORM/$FILE_CCC_RENV.$var_local

 upd=0
 if [ -f "$frenv" ] ; then
  echo ""
  echo "File with the system environment for this runtime environment already exists:"
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
  echo "Enter system environment for the runtime environment if needed (Press <Ctrl-D> to finish and save file) ..."
  echo ""

   echo "Example:"
   echo ""
   echo "  #!/bin/bash"
   echo ""
   echo "  echo \"Setting environment for LLVM 2.5 runtime environment ...\""
   echo ""
   echo "  export PATH=~/llvm/llvm-2.5/install/bin:\$PATH"
   echo "  export LD_LIBRARY_PATH=~/llvm/llvm-2.5/install/lib:\$LD_LIBRARY_PATH"
   echo ""
   echo "  export CCC_RE=lli"

  echo ""
  cat > $frenv
 fi

 chmod 755 $frenv

 echo ""
 echo "Note: You may want to edit file $frenv"
 echo "        with the system environment for your runtime environment ..."

 echo ""
 echo "Runtime environment configuration files are created!"

 if [ ! -z "$CCC_URL" ] && [ $already -ne "1" ]; then
  ###### check fileds ######
  echo ""
  echo "Would you like to add information about your runtime environment to the COMMON database (Y/N)?"
  read var
 
  if [ "$var" = "Y" ] || [ "$var" = "y" ] ; then
   echo "show FIELDS from RUNTIME_ENVIRONMENTS" > $FILE_CCC_TMP
   mysql -h $CCC_C_URL -u $CCC_C_USER --password=$CCC_C_PASS $CCC_C_SSL $CCC_C_DB < $FILE_CCC_TMP > $FILE_CCC_TMP1
   rm -rf $FILE_CCC_TMP

   echo ""
   echo "*************** Enter the following information for this runtime environment: ***************"

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

   echo -n "insert into RUNTIME_ENVIRONMENTS (RE_ID" > $FILE_CCC_TMP
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
