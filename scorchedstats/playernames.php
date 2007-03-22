<?
include('statsheader.php');
include('conversionfunctions.php');

$prefixid = getIntParameter('Prefix');
$seriesid = getIntParameter('Series');
$playerid = getIntParameter('PlayerID');


// General Player Stats
$playerquery = "SELECT * FROM scorched3d_players WHERE playerid=$playerid";
$playerresult = mysqlQuery($playerquery) or die("Query failed : " . mysql_error());
if (!($playerrow = mysql_fetch_object($playerresult))) die("Invalid player");
$playername=$playerrow->name;
?>

<table width="790" border="0" align="center">
<tr align=center>
<td>
<font size=+2><b><u><?=$playername?>'s Player Names</u></b></font><br>
<b>For all servers</b>
</td>
</tr>
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
