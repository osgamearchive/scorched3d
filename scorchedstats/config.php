<?php

$dbhost = '';
$dbname = '';
$dbuser = '';
$dbpasswd = '';

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
	'none'
);

function mysqlQuery($query)
{
	if (stristr($query, 'uniqueid') != FALSE ||
		stristr($query, 'ipaddress') != FALSE)
	{
		die("Invalid query");
	}
	
	return mysql_query($query);
}

function getIntParameter($paramname)
{
	global $HTTP_GET_VARS;
	return ( isset($HTTP_GET_VARS[$paramname]) ) ? intval($HTTP_GET_VARS[$paramname]) : 0;
}

function getDirectionParameter($paramname)
{
	global $HTTP_GET_VARS;
	$dir = ( isset($HTTP_GET_VARS[$paramname]) ) ? htmlspecialchars($HTTP_GET_VARS[$paramname]) : 'desc';
	if ($dir != 'desc' && $dir != 'asc') $dir = 'desc';	
	return $dir;
}

function getCompareParameter($paramname)
{
	global $HTTP_GET_VARS;
	$compare = ( isset($HTTP_GET_VARS[$paramname]) ) ? urldecode($HTTP_GET_VARS[$paramname]) : Null;
	if ($compare != '<=' && $compare != '<' && $compare != '=' && 
		$compare != '>' && $compare != '>=') $compare = '<=';
	return $compare;
}

function getColumnParameter($paramname, $defaultvalue)
{
	global $HTTP_GET_VARS;
	global $columnindex;
	$filterby = ( isset($HTTP_GET_VARS[$paramname]) ) ? htmlspecialchars($HTTP_GET_VARS[$paramname]) : $defaultvalue;
	for ($i=0; $i<count($columnindex); $i++) 
	{
		if ($filterby == $columnindex[$i]) return $filterby;
  	}
	return $defaultvalue;
}

?>
