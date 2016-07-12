<?php

# Copyright (C) 2004-2010 by Grigori Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

$dir=getenv("CCC_PLUGINS");
if ($dir != "")
  $dir = $dir . "/";
$dir.="/include/";
include($dir . "ccc_script_functions.php");

$prog_id=getenv("PROG_ID");
$plat_id=getenv("PLAT_ID");
$env_id=getenv("ENV_ID");
$cmplr_id=getenv("CMPLR_ID");
$ds_num=getenv("DS_NUM");
$ds_id=getenv("DS_ID");

$notes=getenv("NOTES");
$pg_use=(int)getenv("PG_USE");
$output_cor=1;
$output_cor1=getenv("OUTPUT_CORRECT");
 if ($output_cor1=="0") $output_cor=0;

$run_time=getenv("RUN_TIME");
 if ($run_time=="") $run_time="RUN_TIME";
$time_thr=(double)getenv("TIME_THRESHOLD");
 if ($time_thr==0) $time_thr="0.3";
$file_tmp=getenv("CCC_FILE_TMP");
 if ($file_tmp=="") $file_tmp="tmp";

$sort=getenv("SORT");
$dim=getenv("DIM");
$str_cut=getenv("CUT");

$copy2ctdb=getenv("COPY2CTDB");

// Output file with stats
$fname=getenv("CCC_STATS");
$fo=fopen($fname . ".txt", 'a') or die("Error: Can't open output file with stats for writing!\n");

// Print various parameters
ccc_print($fo, "PROG_ID=               " . $prog_id . "\n");
ccc_print($fo, "DS_NUM=                " . $ds_num . "\n");
ccc_print($fo, "PLAT_ID=               " . $plat_id . "\n");
ccc_print($fo, "ENV_ID=                " . $env_id . "\n");
ccc_print($fo, "CMPLR_ID=              " . $cmplr_id . "\n");
ccc_print($fo, "\n");
ccc_print($fo, "NOTES=                 " . $notes . "\n");
ccc_print($fo, "PG_USE=                " . $pg_use . "\n");
ccc_print($fo, "OUTPUT_CORRECT=        " . $output_cor . "\n");
ccc_print($fo, "RUN_TIME=              " . $run_time . "\n");
ccc_print($fo, "TIME_THRESHOLD=        " . $time_thr . "\n");
ccc_print($fo, "\n");
ccc_print($fo, "SORT=                  " . $sort . "\n");
ccc_print($fo, "DIM=                   " . $dim . "\n");
ccc_print($fo, "CUT=                   " . $str_cut . "\n");
ccc_print($fo, "\n");
ccc_print($fo, "CCC_FILE_TMP=          " . $file_tmp . "\n");

// Database parameters
$c_hostname=getenv("CCC_C_URL");
$c_database=getenv("CCC_C_DB");
$c_user    =getenv("CCC_C_USER");
$c_password=getenv("CCC_C_PASS");
$c_ssl     =getenv("CCC_C_SSL");
$ic_ssl=0;
 if ($c_ssl != "") $ic_ssl=MYSQL_CLIENT_SSL;

$ct_hostname=getenv("CCC_CT_URL");
$ct_database=getenv("CCC_CT_DB");
$ct_user    =getenv("CCC_CT_USER");
$ct_password=getenv("CCC_CT_PASS");
$ct_ssl     =getenv("CCC_CT_SSL");
$ict_ssl=0;
 if ($ct_ssl != "") $ict_ssl=MYSQL_CLIENT_SSL;

$hostname=getenv("CCC_URL");
$database=getenv("CCC_DB");
$user    =getenv("CCC_USER");
$password=getenv("CCC_PASS");
$ssl     =getenv("CCC_SSL");
$i_ssl=0;
 if ($ssl != "") $i_ssl=MYSQL_CLIENT_SSL;

$db_ver=getenv("CCC_DB_VER");

// Print database parameters
ccc_print($fo, "\n");
ccc_print($fo, "CCC_URL=               " . $hostname . "\n");
ccc_print($fo, "CCC_DB=                " . $database . "\n");
ccc_print($fo, "CCC_USER=              " . $user . "\n");
ccc_print($fo, "\n");
ccc_print($fo, "CCC_C_URL=             " . $c_hostname . "\n");
ccc_print($fo, "CCC_C_DB=              " . $c_database . "\n");
ccc_print($fo, "CCC_C_USER=            " . $c_user . "\n");
ccc_print($fo, "\n");
ccc_print($fo, "CCC_CT_URL=            " . $ct_hostname . "\n");
ccc_print($fo, "CCC_CT_DB=             " . $ct_database . "\n");
ccc_print($fo, "CCC_CT_USER=           " . $ct_user . "\n");

// Connect to the common database
echo "\n";
echo "Connecting to the common database ...\n";
$c_link=mysql_connect($c_hostname, $c_user, $c_password, true, $ic_ssl) or die( "Error: Unable to connect to the database\n");
ccc_mysql_select_db($c_database, $c_link, $db_ver);

// Connect to the database with experiments
echo "Connecting to the database with experiments $database ...\n";
$link=mysql_connect($hostname, $user, $password, true, $i_ssl) or die( "Error: Unable to connect to the database\n");
ccc_mysql_select_db($database, $link, $db_ver);

//Checking dataset id
ccc_print($fo, "\n");
if ($ds_num!="0")
{
 ccc_print($fo, "Searching DS_ID associated with DS_NUM (" . $ds_num .")...\n");
 $ds_id=get_dataset_id($prog_id, $ds_num, $c_link);
}

ccc_print($fo, "DS_ID=    " . $ds_id . "\n");

//Analysis
ccc_print($fo, "\n");
ccc_print($fo, "Quering execution time range ...\n");
 $tx=get_time_range($prog_id, $plat_id, $env_id, $cmplr_id, $ds_id, $time_thr, "RUN_TIME", $link);
 $tmin=$tx[0]; $tmax=$tx[1];
ccc_print($fo, sprintf("Time range min = %4.3f\n", $tmin));
ccc_print($fo, sprintf("Time range max = %4.3f\n", $tmax));

ccc_print($fo, "\n");
ccc_print($fo, "Quering compile time range ...\n");
 $ctx=get_compile_time_range($prog_id, $plat_id, $env_id, $cmplr_id, $time_thr, $link);
 $ctmin=$ctx[0]; $ctmax=$ctx[1];
ccc_print($fo, sprintf("Compile time range min = %4.3f\n", $ctmin));
ccc_print($fo, sprintf("Compile time range max = %4.3f\n", $ctmax));

ccc_print($fo, "\n");
ccc_print($fo, "Quering bin size range ...\n");
 $bs=get_bin_size_range($prog_id, $plat_id, $env_id, $cmplr_id, $ds_id, $link);
 $bs_min=$bs[0]; $bs_max=$bs[1];
ccc_print($fo, sprintf("Bin size range min = %8u\n", $bs_min));
ccc_print($fo, sprintf("Bin size range max = %8u\n", $bs_max));

// Get opt cases for only correct outputs [overall time]
$opt_cases=get_opt_cases1($prog_id, $plat_id, $env_id, $cmplr_id, $ds_id, $time_thr, $output_cor, $run_time, $link, $c_link, $file_tmp, $pg_use, $notes);

$speedup=$opt_cases[0];
$info=$opt_cases[1];

$ns=sizeof($speedup[0]);

ccc_print($fo, "\n");
ccc_print($fo, "Number of cases: " . $ns . "\n");

if ($ns>0)
{
  // Save some speedups in csv
  $fo1=fopen($fname . ".csv", 'w') or die("Error: Can't open output file with stats for writing!\n");
   for ($i=0; $i<$ns; $i++)
   {
     fwrite($fo1, $speedup[0][$i] . ",  " . $speedup[1][$i] . ", ". $speedup[2][$i] . ", " .
        $info[0][$i] . ", " . $info[1][$i] . "\n");
   }
  fclose($fo1);

  // Sort array
  // Find which order
  // 0 - execution time speedup
  // 1 - code size improvement
  // 2 - compilation time speedup
  $srt=array(0,1,2);
  for ($i=0; $i<strlen($sort); $i++)
  {
   $x=substr($sort, $i, 1);
   if ($x=="0") $srt[$i]=0;
   else if ($x=="1") $srt[$i]=1;
   else if ($x=="2") $srt[$i]=2;
  }

  array_multisort($speedup[$srt[0]], SORT_DESC, SORT_NUMERIC,
                  $speedup[$srt[1]], SORT_DESC, SORT_NUMERIC,
                  $speedup[$srt[2]], SORT_DESC, SORT_NUMERIC,
                  $info[0], $info[1], $info[2], $info[3]);

  $fo1=fopen($fname . ".sort_by_".$srt[0]."".$srt[1]."".$srt[2].".csv", 'w') or die("Error: Can't open output file with stats for writing!\n");
   for ($i=0; $i<$ns; $i++)
   {
     fwrite($fo1, $speedup[0][$i] . ", " . $speedup[1][$i] . ", ". $speedup[2][$i] . ", " .
        $info[0][$i] . ", " . $info[1][$i] . "\n");
   }
  fclose($fo1);

  // Print a few best cases
  $speedup_max0=$speedup[0][0];
  $speedup_max1=$speedup[1][0];
  $speedup_max2=$speedup[2][0];
  $comp_id=$info[1][0];

  $opt_arch=get_opt_flags_platform(get_opt_platform_id(
                              get_opt_platform_feature_id($comp_id, $link), $c_link), $cmplr_id, $c_link);
  $opt_id=get_opt_id($comp_id, $link);
  $opt=get_opt_flags(get_opt_id($comp_id, $link), $cmplr_id, $c_link);

  ccc_print($fo, "\n");
  ccc_print($fo, "Speedup 0 (execution time)   = " . $speedup_max0 . "\n");
  ccc_print($fo, "Speedup 1 (code size)        = " . $speedup_max1 . "\n");
  ccc_print($fo, "Speedup 2 (compilation time) = " . $speedup_max2 . "\n");

  ccc_print($fo, "\n\n");
  ccc_print($fo, "Optimization                 = " . $opt . "\n");
  ccc_print($fo, "Optimization (arch)          = " . $opt_arch . "\n");

  // Find frontier cases
  // Find dimension
  // 0 - execution time speedup
  // 1 - code size improvement
  // 2 - compilation time speedup
  $dimension=array();
  for ($i=0; $i<strlen($dim); $i++)
  {
   $x=substr($dim, $i, 1);
   if ($x=="0") $dimension[$i]=0;
   else if ($x=="1") $dimension[$i]=1;
   else if ($x=="2") $dimension[$i]=2;
  }

  // Cut cases
  // CUT=1.1,0,1.2
  $cut=explode(",", $str_cut);

  $xopt_cases=find_frontier($speedup, $info, $dimension, $cut);

  $xspeedup=$xopt_cases[0];
  $xinfo=$xopt_cases[1];

  $xns=sizeof($xspeedup[0]);

  $fo1=fopen($fname . ".frontier.csv", 'w') or die("Error: Can't open output file with stats for writing!\n");
   for ($i=0; $i<$xns; $i++)
   {
     fwrite($fo1, $xspeedup[0][$i] . ", " . $xspeedup[1][$i] . ", ". $xspeedup[2][$i] . ", " .
        $xinfo[0][$i] . ", " . $xinfo[1][$i] . "\n");
   }
  fclose($fo1);
}

fclose($fo);

mysql_close();

?>
