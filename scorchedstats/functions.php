<?php

include('config.php');

$columnindex = array
(
	'games',
	'rounds',
	'type',
	'playerid',
	'lastconnected',
	'connects',
	'kills',
	'deaths',
	'selfkills',
	'teamkills',
	'shots',
	'wins',
	'overallwinner',
	'resigns',
	'gamesplayed',
	'timeplayed',
	'roundsplayed',
	'moneyearned',
	'skill',
	'cost',
	'bundlesize',
	'armslevel',
	'shots',
	'deathshots',
	'deathkills',
	'count',
	'name',
	'osdesc',
	'icon',
	'description',
	'killratio',
	'killsperround',
	'killsperhour',
	'killsperdeath',
	'moneyperround',
	'moneyperhour',
	'moneyperkill',
	'winratio',
	'roundsperresign',
	'timeperconnect',
	'scoreearned',
	'scoreperround',
	'scoreperhour',
	'scorepershot',
	'none'
);

function mysqlQuery($query)
{
	if (stristr($query, 'uniqueid') != FALSE ||
		stristr($query, 'ipaddress') != FALSE ||
		stristr($query, 'phpbb2') != FALSE)
	{
		die("Invalid query");
	}
	
	return mysql_query($query);
}

function getIntParameter($paramname)
{
	global $_GET;
	return ( isset($_GET[$paramname]) ) ? intval($_GET[$paramname]) : 0;
}

function getDirectionParameter($paramname)
{
	global $_GET;
	$dir = ( isset($_GET[$paramname]) ) ? htmlspecialchars($_GET[$paramname]) : 'desc';
	if ($dir != 'desc' && $dir != 'asc') $dir = 'desc';	
	return $dir;
}

function getCompareParameter($paramname)
{
	global $_GET;
	$compare = ( isset($_GET[$paramname]) ) ? urldecode($_GET[$paramname]) : Null;
	if ($compare != '<=' && $compare != '<' && $compare != '=' && 
		$compare != '>' && $compare != '>=') $compare = '<=';
	return $compare;
}

function getColumnParameter($paramname, $defaultvalue)
{
	global $_GET;
	global $columnindex;
	$filterby = ( isset($_GET[$paramname]) ) ? htmlspecialchars($_GET[$paramname]) : $defaultvalue;
	for ($i=0; $i<count($columnindex); $i++) 
	{
		if ($filterby == $columnindex[$i]) return $filterby;
  	}
	return $defaultvalue;
}

function getBinaryImage($id)
{
$use_file = "images/player.gif";
if($id != 0)
{
$filename = "imagecache/image-".$id.".gif";
if (file_exists($filename))
{
        $use_file = $filename;
}
else
{
        if (($link = mysql_connect($dbhost, $dbuser, $dbpasswd)) &&
                mysql_select_db($dbname))
        {
                $query = "SELECT data FROM scorched3d_binary WHERE binaryid=$id";
                if ($result = mysqlQuery($query))
                {
                        if ($row = mysql_fetch_object($result))
                        {
                                $data = $row->data;
                                fwrite(fopen($filename, "w"), $data);

                                $use_file = $filename;
                        }
                }
        }
}
};

return $use_file;
}

