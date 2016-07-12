<?php

# Copyright (C) 2004-2009 by Grigori Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

$cmplr=getenv("CCC_CFG") . "/ccc-compilers.cfg";
$cmplr_name=getenv("COMPILER_CONFIGURATION_NAME");

$find1="COMPILER_CONFIGURATION_NAME";
$lfind1=strlen($find1);
$find2="COMPILER_ID";
$lfind2=strlen($find2);

$cmplr_id="";
$found1=0;

$fl=fopen($cmplr, "r");

if ($fl)
{
 while ((!feof($fl)) && ($found1!=2))
 {
   $line=fgets($fl);
   if ($found1==0)
   {
     if ((strlen($line)>$lfind1) && (strncmp($line, $find1, $lfind1)==0))
     {
       $cmplr_x=substr($line, $lfind1+1);
       $x=substr($cmplr_x,-1);
       if ($x=="\n") $cmplr_x=substr($cmplr_x,0,-1);
       if ($cmplr_x==$cmplr_name) $found1=1;
     }
   }   
   else if ($found1==1)
   {
     if ((strlen($line)>$lfind2) && (strncmp($line, $find2, $lfind2)==0))
     {
       $cmplr_id=substr($line, $lfind2+1);
       $x=substr($cmplr_id,-1);
       if ($x=="\n") $cmplr_id=substr($cmplr_id,0,-1);
       $found1=2;
     }
   }
 }
}

fclose($fl);

if ($found1==2) echo $cmplr_id . "\n";

?>
