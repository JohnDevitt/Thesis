<?php

$dir=getenv("CCC_PLUGINS");
if ($dir != "")
  $dir = $dir . "/";
$dir.="/include/";
include($dir . "ccc_script_functions.php");

$run_id=getenv("RUN_ID");

if ($run_id=="")
{
 echo "ERROR: RUN_ID is not specified!\n";
 exit;
}

// Database params
// Database parameters
$c_hostname=getenv("CCC_C_URL");
$c_database=getenv("CCC_C_DB");
$c_user    =getenv("CCC_C_USER");
$c_password=getenv("CCC_C_PASS");
$c_ssl     =getenv("CCC_C_SSL");
 if ($c_ssl != "") $ic_ssl=MYSQL_CLIENT_SSL;

$ct_hostname=getenv("CCC_CT_URL");
$ct_database=getenv("CCC_CT_DB");
$ct_user    =getenv("CCC_CT_USER");
$ct_password=getenv("CCC_CT_PASS");
$ct_ssl     =getenv("CCC_CT_SSL");
 if ($ct_ssl != "") $ict_ssl=MYSQL_CLIENT_SSL;

$hostname=getenv("CCC_URL");
$database=getenv("CCC_DB");
$user    =getenv("CCC_USER");
$password=getenv("CCC_PASS");
$ssl     =getenv("CCC_SSL");
 if ($ssl != "") $i_ssl=MYSQL_CLIENT_SSL;

$db_ver=getenv("CCC_DB_VER");

// Connect to the collective tuning database
echo "\n";
echo "Connecting to the collective tuning database ...\n";
$ct_link=mysql_connect($ct_hostname, $ct_user, $ct_password, true, $ict_ssl) or die( "Error: Unable to connect to the database\n");
ccc_mysql_select_db($ct_database, $ct_link, $db_ver);

// Connect to the database with experiments
echo "Connecting to the database with experiments ...\n";
$link=mysql_connect($hostname, $user, $password, true, $i_ssl) or die( "Error: Unable to connect to the database\n");
ccc_mysql_select_db($database, $link, $db_ver);

echo "Trying to copy RUN info (RUN_ID=" . $run_id . ") from DB1 to DB2 ...\n";

if (copy2ctdb_run($run_id, $link, $ct_link, true))
 echo "Status: success\n";
else
 echo "Status: failed\n";

mysql_close();

?>
