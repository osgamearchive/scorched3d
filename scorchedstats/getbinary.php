<?php

include('config.php');

$id = getIntParameter('id');

$data="";
if($id != 0) 
{
	$filename = "imagecache/image-".$id.".gif";
	if (file_exists($filename))
	{
		$data = fread(fopen($filename, "r"), filesize($filename));
	}
	else
	{
		if (($link = mysql_connect($dbhost, $dbuser, $dbpasswd)) &&
			mysql_select_db($dbname))
		{
			$query = "SELECT data FROM scorched3d_binary WHERE binaryid=$id";
			if ($result = mysqlQuery($query))
			{
				$row = mysql_fetch_object($result);
				$data = $row->data;

				fwrite(fopen($filename, "w"), $data);
			}
		}
	}
};

if ($data == "")
{
	$data = fread(fopen("player.gif", "r"), filesize("player.gif"));
}

Header( "Content-type: image/gif");
echo $data;
?>
