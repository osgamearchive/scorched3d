<?php

include('config.php');

$id=$_GET['id'];
$data="";
if($id) 
{

	$link = mysql_connect($dbhost, $dbuser, $dbpasswd) or die("Could not connect : " . mysql_error());
	mysql_select_db($dbname) or die("Could not select database");

	$query = "SELECT data FROM scorched3d_binary WHERE binaryid=$id";
	$result = mysql_query($query) or die("Query error : " . mysql_error());
	$row = mysql_fetch_object($result);
	$data = $row->data;
};

if ($data == "")
{
	$data = fread(fopen("player.gif", "r"), filesize("player.gif"));
}

Header( "Content-type: image/gif");
echo $data;
?>