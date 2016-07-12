<?php

# Copyright (C) 2004-2009 by Grigori Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

# Send data to cDatabase through cTuning Web Service

# Warning: Currently unsecure connection - need to add SSL!!!

$cod_file=getenv("CCC_FILE");
$cod_url_std=getenv("CCC_CTS_URL");
$cod_service=getenv("CCC_WEB_SERVICE");
$username=getenv("CCC_CTS_USER");
$password=getenv("CCC_CTS_PASS");
$database=getenv("CCC_CTS_DB");

$cod_url=$cod_url_std . $cod_service;

$data=array();

$data["USERNAME"]=$username;
$data["PASSWORD"]=$password;
$data["DATABASE_NAME"]=$database;

$f=fopen($cod_file, "r") or die("Can't open file for reading ...");

$str="";
while (!feof($f)) 
{
  $str=trim(fgets($f));
  $i=strpos($str, "=");
  if ($i)
  {
    $key=trim(substr($str,0,$i));
    $value=trim(substr($str,$i+1));

    $data[$key]=$value;
  }
}

fclose($f);

$server=$cod_url;
$path="";
$i=strpos($cod_url, "/");
if ($i)
{
  $path=trim(substr($cod_url,$i));
  $server=trim(substr($cod_url,0, $i));
}

echo "Server: " . $server . "\n";
echo "Path:   " . $path . "\n";

echo "\n";
echo "Sending request ...\n";

$parse_output=true;
$ret=sendDataToCOD($server, $path, $data, $parse_output);

echo "\n";
echo "Response:\n";
echo $ret . "\n";

exit;

function sendDataToCOD($server, $path, $data, $parse)
{
  $boundary = "AaB03x"; 

  $post = "";

  $result = "";
  $ret = "";

  // Build the header
  $header  = "POST $path HTTP/1.0\r\n";
  $header .= "Host: $server\r\n";
  $header .= "Content-Type: multipart/form-data; boundary=$boundary\r\n";

  // Attach post vars
  foreach($data AS $index => $value)
  {
      $post .="--$boundary\r\n";
      $post .= "Content-Disposition: form-data; name=\"".$index."\"\r\n";
      $post .= "\r\n".$value."\r\n";
  } 

  $post .="--$boundary--\r\n"; 

  $header .= "Content-length: " . strlen($post) . "\r\n\r\n";

  $fp = fsockopen($server, 80, $errno, $errstr, $timeout=30); 

  if(!$fp)
    die("Error: Can't connect to remote server: $errstr ($errno)\n");

  fputs($fp, $header.$post);

  // get the response
  while (!feof($fp)) $ret .= fread($fp,32000);

  // fputs($fp, "Connection: close\r\n\r\n");

  fclose($fp); 

  $j=0;
  while ($i=strpos($ret, "CTS_RESPONSE={", $j))
  {
    $j=strpos($ret, "}", $i);
    if ($j)
      $result .= trim(substr($ret, $i+14, $j-$i-14)) . "\n";
  }   

  if (!$parse) $result=$ret;

  return $result;
}

?>
