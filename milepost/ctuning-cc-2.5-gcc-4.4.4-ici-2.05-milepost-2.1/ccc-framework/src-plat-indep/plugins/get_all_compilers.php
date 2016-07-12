<?php

# Copyright (C) 2004-2009 by Grigori Fursin
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

// Output file with stats
$fname=getenv("CCC_STATS");

$fo=fopen($fname . ".txt", 'w') or die("Error: Can't open output file with stats for writing\n");

// Connect to the common database
echo "Connecting to the common database ...\n";
$c_link=mysql_connect($c_hostname, $c_user, $c_password, true, $ic_ssl) or die( "Error: Unable to connect to the database\n");
ccc_mysql_select_db($c_database, $c_link, $db_ver);

// Connect to the database with experiments
echo "Connecting to the database with experiments ...\n";
$link=mysql_connect($hostname, $user, $password, true, $i_ssl) or die( "Error: Unable to connect to the database\n");
ccc_mysql_select_db($database, $link, $db_ver);

echo "\n";

$query="SELECT DISTINCT COMPILER_ID FROM STATS_EXEC_GLOBAL";
$result=mysql_query($query);

while ($row=mysql_fetch_assoc($result))  //get one row at a time
{
  $comp_id=$row['COMPILER_ID'];

  $str=$comp_id . "  " . get_compiler_name($comp_id, $c_link);
  echo $str . "\n";
  fwrite($fo, $str . "\n");
}

mysql_free_result($result);

mysql_close();

?>
