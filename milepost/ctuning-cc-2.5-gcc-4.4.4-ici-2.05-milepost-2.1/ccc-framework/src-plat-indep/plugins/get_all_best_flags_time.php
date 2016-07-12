<?php

# Copyright (C) 2007-2009 by Grigori Fursin
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

$start=(int) getenv("CCC_START");

// Output file with stats
$fname=getenv("CCC_STATS");

$fo=fopen($fname . ".txt", 'w') or die("Error: Can't open output file with stats for writing\n");

$fname1=$fname . ".all.txt";
$fo1=fopen($fname1, 'w') or die("Error: Can't open output file with stats for writing\n");
fclose($fo1);

$fname1rt=$fname . ".rt.txt";
$fo1rt=fopen($fname1rt, 'w') or die("Error: Can't open output file with stats for writing\n");
fclose($fo1rt);

$fname1rtc=$fname . ".rtc.txt";
$fo1rtc=fopen($fname1rtc, 'w') or die("Error: Can't open output file with stats for writing\n");
fclose($fo1rtc);

$fname1ut=$fname . ".ut.txt";
$fo1ut=fopen($fname1ut, 'w') or die("Error: Can't open output file with stats for writing\n");
fclose($fo1ut);

$fname1utc=$fname . ".utc.txt";
$fo1utc=fopen($fname1utc, 'w') or die("Error: Can't open output file with stats for writing\n");
fclose($fo1utc);

// Connect to the common database
echo "Connecting to the common database ...\n";
$c_link=mysql_connect($c_hostname, $c_user, $c_password, true, $ic_ssl) or die( "Error: Unable to connect to the database\n");
ccc_mysql_select_db($c_database, $c_link, $db_ver);

// Connect to the database with experiments
echo "Connecting to the database with experiments ...\n";
$link=mysql_connect($hostname, $user, $password, true, $i_ssl) or die( "Error: Unable to connect to the database\n");
ccc_mysql_select_db($database, $link, $db_ver);

echo "\n";

$prog_id=getenv("PROG_ID");
$plat_id=getenv("PLAT_ID");
$env_id=getenv("ENV_ID");
$cmplr_id=getenv("CMPLR_ID");
$ds_id=getenv("DS_ID");

$query="SELECT ";
$query1="";
$queryB=" WHERE ";
$query2="";

if (($prog_id=="") || ($plat_id=="") || ($env_id=="") || ($cmplr_id=="") || ($ds_id=="")) $query.="DISTINCT ";

if ($prog_id=="")
{
  $query1.= "PROGRAM_ID";
}
else
{
  $query2.= "(PROGRAM_ID=".$prog_id.") ";
}

if ($ds_id=="")
{
  if ($query1!="") $query1.=",";
  $query1.= "DATASET_ID";
}
else
{
  if ($query2!="") $query2.="AND ";
  $query2.= "(DATASET_ID=".$ds_id.") ";
}

if ($plat_id=="")
{
  if ($query1!="") $query1.=",";
  $query1.= "PLATFORM_ID";
}
else
{
  if ($query2!="") $query2.="AND ";
  $query2.= "(PLATFORM_ID=".$plat_id.") ";
}

if ($env_id=="")
{
  if ($query1!="") $query1.=",";
  $query1.= "ENVIRONMENT_ID";
}
else
{
  if ($query2!="") $query2.="AND ";
  $query2.= "(ENVIRONMENT_ID=".$env_id.") ";
}

if ($cmplr_id=="")
{
  if ($query1!="") $query1.=",";
  $query1.= "COMPILER_ID";
}
else
{
  if ($query2!="") $query2.="AND ";
  $query2.= "(COMPILER_ID=".$cmplr_id.") ";
}

if ($query1!="") $query.=$query1;
else $query.="*";
$query.=" FROM STATS_EXEC_GLOBAL";
if ($query2!="") $query.=$queryB.$query2;

print "started: " . strftime('%c') . "\n";
echo "\n".$query . "\n";
$result=mysql_query($query, $link);
print "\nfinished: " . strftime('%c') . "\n";
$imax=mysql_num_rows($result); 

$i=0;
while ($row=mysql_fetch_assoc($result))  //get one row at a time
{
  $i++;

  if (($start==0) || ($i>=$start))
  {
    $plat_id1=$plat_id;  if ($plat_id=="") $plat_id1=$row['PLATFORM_ID'];
    $env_id1=$env_id;    if ($env_id=="") $env_id1=$row['ENVIRONMENT_ID'];
    $prog_id1=$prog_id;  if ($prog_id=="") $prog_id1=$row['PROGRAM_ID'];
    $cmplr_id1=$cmplr_id;if ($cmplr_id=="") $cmplr_id1=$row['COMPILER_ID'];
    $ds_id1=$ds_id;      if ($ds_id=="") $ds_id1=$row['DATASET_ID'];

    $fo1=fopen($fname1, 'a') or die("Error: Can't open output file with stats for writing\n");
    $fo1rt=fopen($fname1rt, 'a') or die("Error: Can't open output file with stats for writing\n");
    $fo1rtc=fopen($fname1rtc, 'a') or die("Error: Can't open output file with stats for writing\n");
    $fo1ut=fopen($fname1ut, 'a') or die("Error: Can't open output file with stats for writing\n");
    $fo1utc=fopen($fname1utc, 'a') or die("Error: Can't open output file with stats for writing\n");

    echo "\n";
    echo "***********************************************************************************************\n";
    echo "DISTINCT CASE " . $i . " / " . $imax . "\n\n";

    fwrite($fo1, "\n");
    fwrite($fo1, "***********************************************************************************************\n");
    fwrite($fo1, "DISTINCT CASE " . $i . " / " . $imax . "\n\n");

    fwrite($fo1rt, "\n");
    fwrite($fo1rt, "***********************************************************************************************\n");
    fwrite($fo1rt, "DISTINCT CASE " . $i . " / " . $imax . "\n\n");

    fwrite($fo1rtc, "\n");
    fwrite($fo1rtc, "***********************************************************************************************\n");
    fwrite($fo1rtc, "DISTINCT CASE " . $i . " / " . $imax . "\n\n");

    fwrite($fo1ut, "\n");
    fwrite($fo1ut, "***********************************************************************************************\n");
    fwrite($fo1ut, "DISTINCT CASE " . $i . " / " . $imax . "\n\n");

    fwrite($fo1utc, "\n");
    fwrite($fo1utc, "***********************************************************************************************\n");
    fwrite($fo1utc, "DISTINCT CASE " . $i . " / " . $imax . "\n\n");

    $str=$plat_id1 . "  " . get_platform_name($plat_id1, $c_link);
    echo $str . "\n";
    fwrite($fo1, $str . "\n");
    fwrite($fo1rt, $str . "\n");
    fwrite($fo1rtc, $str . "\n");
    fwrite($fo1ut, $str . "\n");
    fwrite($fo1utc, $str . "\n");

    $str=$env_id1 . "  " . get_environment_name($env_id1, $c_link);
    echo $str . "\n";
    fwrite($fo1, $str . "\n");
    fwrite($fo1rt, $str . "\n");
    fwrite($fo1rtc, $str . "\n");
    fwrite($fo1ut, $str . "\n");
    fwrite($fo1utc, $str . "\n");

    $str=$prog_id1 . "  " . get_prog_name($prog_id1, $c_link) . " " . get_prog_version($prog_id1, $c_link);
    echo $str . "\n";
    fwrite($fo1, $str . "\n");
    fwrite($fo1rt, $str . "\n");
    fwrite($fo1rtc, $str . "\n");
    fwrite($fo1ut, $str . "\n");
    fwrite($fo1utc, $str . "\n");

    $str=$cmplr_id1 . "  " . get_compiler_name($cmplr_id1, $c_link);
    echo $str . "\n";
    fwrite($fo1, $str . "\n");
    fwrite($fo1rt, $str . "\n");
    fwrite($fo1rtc, $str . "\n");
    fwrite($fo1ut, $str . "\n");
    fwrite($fo1utc, $str . "\n");

    $str=$ds_id1 . "  " . get_command_line($ds_id1, $c_link);
    echo $str . "\n";
    fwrite($fo1, $str . "\n");
    fwrite($fo1rt, $str . "\n");
    fwrite($fo1rtc, $str . "\n");
    fwrite($fo1ut, $str . "\n");
    fwrite($fo1utc, $str . "\n");

    echo "\n";
    fwrite($fo1, "\n");
    fwrite($fo1rt, "\n");
    fwrite($fo1rtc, "\n");
    fwrite($fo1ut, "\n");
    fwrite($fo1utc, "\n");

    fclose($fo1);
    fclose($fo1rt);
    fclose($fo1rtc);
    fclose($fo1ut);
    fclose($fo1utc);

    putenv("DB_PARAM_IGNORE=1");
    putenv("COPY2CTDB=1");
    system("get-best-flag-time " . $prog_id1 . " " . $plat_id1 . " " . $env_id1 . " " . $cmplr_id1 . " 0 " . $ds_id1); 

    fwrite($fo, $prog_id1 . "  " . $plat_id1 . "  " . $env_id1 . "  " . $cmplr_id1 . "  " . $ds_id1 . "\n" );
  }
}

mysql_free_result($result);

mysql_close();

?>
