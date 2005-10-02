<?
include('statsheader.php');
include('conversionfunctions.php');

$prefixid = getIntParameter('Prefix');
$seriesid = getIntParameter('Series');
$playerid = getIntParameter('PlayerID');


// General Player Stats
$playerquery = "SELECT * FROM scorched3d_players WHERE playerid=$playerid";
$playerresult = mysqlQuery($playerquery) or die("Query failed : " . mysql_error());
$playerrow = mysql_fetch_object($playerresult);
$playername=$playerrow->name;
?>

<table width="790" border="0" align="center">
<tr align=center>
<td>
<font size=+2><b><u><?=$playername?>'s User Info</u></b></font><br>
<b>On all servers</b>
</td>
</tr>
</table>
<br>

<table width=640 border="0" align="center">
<tr><td align=center><b>Info</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr><td bgcolor=#111111><b>Player Icon</b></td><td><img src="getbinary.php?id=<?=$playerrow->avatarid?>"></td></tr>
<tr><td bgcolor=#111111><b>Last Used Player Name</b></td><td><?=$playerrow->name?></td></tr>
<tr><td bgcolor=#111111><b>OS</b></td><td><?=$playerrow->osdesc?></td></tr>
<tr><td bgcolor=#111111><b>Player ID</b></td><td><?=$playerrow->playerid?></td></tr>
</table>
<br>

<?
// Player Aliases
$query = "SELECT * FROM scorched3d_names where playerid=$playerid order by count DESC";
$result = mysqlQuery($query) or die("Query failed : " . mysql_error());
?>
<table width="600" border="0" align="center">
<tr><td align=center><b>Aliases</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111><b>Player Alias</b></td>
<td bgcolor=#111111><b>Times Used</b></td>
</tr>
<?
	while ($row = mysql_fetch_object($result))
	{
		echo "<tr><td>$row->name</td><td>$row->count</td></tr>\n";
	}
?>
</table>
<br>

<?
include('statsfooter.php');
?>
