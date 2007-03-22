<?
include('statsheader.php');
include('conversionfunctions.php');

$prefixid = getIntParameter('Prefix');
$seriesid = getIntParameter('Series');
$playerid = getIntParameter('PlayerID');

// General Player Stats
$playerquery = "SELECT playerid, osdesc, name, avatarid FROM scorched3d_players WHERE playerid=$playerid";
$playerresult = mysqlQuery($playerquery) or die("Query failed : " . mysql_error());
if (!($playerrow = mysql_fetch_object($playerresult)))
{
	die("Failed to find a player for id");
}
$playername=$playerrow->name;
?>

<table width="790" border="0" align="center">
<tr align=center>
<td>
<font size=+2><b><u><?=$playername?>'s Recent Server Events</u></b></font><br>
<i>Note: These stats are dependant on the number of events kept by the server</i><br>
<a href="playerevents.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>&PlayerID=<?=$playerid?>">
View all events involving this player.</a>
</td>
</tr>
</table>
<br>

<?
// Query player's stats for today
$query = "select (scorched3d_events.playerid) as playerid, (scorched3d_players.name) as name, SUM(IF(scorched3d_events.eventtype='1',1,0)) AS kills, SUM(IF(scorched3d_events.eventtype='2',1,0)) AS teamkills, SUM(IF(scorched3d_events.eventtype='3',1,0)) AS selfkills, SUM(IF(scorched3d_events.eventtype='4',1,0)) AS resigns, SUM(IF(scorched3d_events.eventtype='5',1,0)) AS wins, SUM(IF(scorched3d_events.eventtype='6',1,0)) AS gamewins FROM scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.playerid=scorched3d_players.playerid) WHERE (scorched3d_events.playerid=$playerid) and (TO_DAYS(NOW()) - TO_DAYS(scorched3d_events.eventtime) < 1) and scorched3d_events.prefixid=$prefixid and scorched3d_events.seriesid=$seriesid group by playerid limit 1";
$result = mysqlQuery($query) or die("Query failed : " . mysql_error());
?>
<table width=600 border="0" align="center">
<tr><td align=center><b>Stats For Today</b></td></tr>
</table>
<table width=600 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111 align=center><b>Player Name</b></td>
<td bgcolor=#111111 align=center><b>Kills</b></td>
<td bgcolor=#111111 align=center><b>Rnd Wins</b></td>
<td bgcolor=#111111 align=center><b>Game Wins</b></td>
<td bgcolor=#111111 align=center><b>Resigns</b></td>
<td bgcolor=#111111 align=center><b>Suicides</b></td>
<td bgcolor=#111111 align=center><b>Teamkills</b></td>
</tr>
<?
$rownum=0;
while ($row = mysql_fetch_object($result))
{
        ++$rownum;
        echo "<tr>";
		echo "<td><center>$rownum</center></td></center>";
        echo "<td><a href=\"playerstats.php?Prefix=$prefixid&Series=$seriesid&PlayerID=$row->playerid\">$row->name</a></td>";
        echo "<td><center>$row->kills</td></center>";
        echo "<td><center>$row->wins</td></center>";
        echo "<td><center>$row->gamewins</td></center>";
        echo "<td><center>$row->resigns</td></center>";
        echo "<td><center>$row->selfkills</td></center>";
        echo "<td><center>$row->teamkills</td></center>";
        echo "</tr>";
}
?>
</table>
<br>

<?
// Query player's stats for the last week
$query = "select (scorched3d_events.playerid) as playerid, (scorched3d_players.name) as name, SUM(IF(scorched3d_events.eventtype='1',1,0)) AS kills, SUM(IF(scorched3d_events.eventtype='2',1,0)) AS teamkills, SUM(IF(scorched3d_events.eventtype='3',1,0)) AS selfkills, SUM(IF(scorched3d_events.eventtype='4',1,0)) AS resigns, SUM(IF(scorched3d_events.eventtype='5',1,0)) AS wins, SUM(IF(scorched3d_events.eventtype='6',1,0)) AS gamewins FROM scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.playerid=scorched3d_players.playerid) WHERE (scorched3d_events.prefixid=$prefixid) and (scorched3d_events.seriesid=$seriesid) and (scorched3d_events.playerid=$playerid) and (TO_DAYS(NOW()) - TO_DAYS(scorched3d_events.eventtime) < 7) group by playerid limit 1";
$result = mysqlQuery($query) or die("Query failed : " . mysql_error());
?>
<table width=600 border="0" align="center">
<tr><td align=center><b>Stats For The Last 7 Days</b></td></tr>
</table>
<table width=600 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111 align=center><b>Player Name</b></td>
<td bgcolor=#111111 align=center><b>Kills</b></td>
<td bgcolor=#111111 align=center><b>Rnd Wins</b></td>
<td bgcolor=#111111 align=center><b>Game Wins</b></td>
<td bgcolor=#111111 align=center><b>Resigns</b></td>
<td bgcolor=#111111 align=center><b>Suicides</b></td>
<td bgcolor=#111111 align=center><b>Teamkills</b></td>
</tr>
<?
$rownum=0;
while ($row = mysql_fetch_object($result))
{
        ++$rownum;
        echo "<tr>";
		echo "<td><center>$rownum</center></td></center>";
        echo "<td><a href=\"playerstats.php?Prefix=$prefixid&Series=$seriesid&PlayerID=$row->playerid\">$row->name</a></td>";
        echo "<td><center>$row->kills</td></center>";
        echo "<td><center>$row->wins</td></center>";
        echo "<td><center>$row->gamewins</td></center>";
        echo "<td><center>$row->resigns</td></center>";
        echo "<td><center>$row->selfkills</td></center>";
        echo "<td><center>$row->teamkills</td></center>";
        echo "</tr>";
}
?>
</table>
<br>
	
<?
// List top targets
$query="select (scorched3d_events.otherplayerid) as killedid, (scorched3d_players.name) as name, (count(*)) AS tally from scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.otherplayerid=scorched3d_players.playerid) where scorched3d_events.playerid=$playerid and scorched3d_events.prefixid=$prefixid and scorched3d_events.seriesid=$seriesid and eventtype=1 group by scorched3d_events.otherplayerid order by tally desc limit 25";
$result = mysqlQuery($query) or die("Query failed : " . mysql_error());
?>
<table width="600" border="0" align="center">
<tr><td align=center><b>Top 25 Targets</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b>Target</b></td>
<td bgcolor=#111111><b>Number of times target killed</b></td>
</tr>
<?
$rownumber = 0;
while ($row = mysql_fetch_object($result))
{
	++$rownumber;
	echo "<tr>";
	echo "<td>$rownumber</td>";
	echo "<td><a href=\"playerstats.php?Prefix=$prefixid&Series=$seriesid&PlayerID=$row->killedid\">$row->name</a></td>";
	echo "<td>$row->tally</td>";
	echo "</tr>";
}
?>
</table>
<br>

<?
// Top Killers
$query=" select (scorched3d_events.playerid) as killedid, (scorched3d_players.name) as name, (count(*)) AS tally from scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.playerid=scorched3d_players.playerid) where otherplayerid=$playerid and prefixid=$prefixid and seriesid=$seriesid and eventtype=1 group by scorched3d_events.playerid order by tally desc limit 25";
$result = mysqlQuery($query) or die("Query failed : " . mysql_error());
?>
<table width="600" border="0" align="center">
<tr><td align=center><b>Top 25 Killers</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b>Killer</b></td>
<td bgcolor=#111111><b>Number of times killed</b></td>
</tr>
<?
$rownumber=0;
while ($row = mysql_fetch_object($result))
{
	++$rownumber;
        echo "<tr>";
        echo "<td>$rownumber</td>";
        echo "<td><a href=\"playerstats.php?Prefix=$prefixid&Series=$seriesid&PlayerID=$row->killedid\">$row->name</a></td>";
        echo "<td>$row->tally</td>";
	echo "</tr>";
}
?>
</table>
<br>

<?	
// Top Weapons
$query="select (scorched3d_events.weaponid) as weaponid, (count(*)) as tally, (scorched3d_weapons.armslevel) as armslevel, (scorched3d_weapons.name) as name from scorched3d_events LEFT JOIN scorched3d_weapons ON (scorched3d_events.weaponid=scorched3d_weapons.weaponid) where playerid=$playerid and scorched3d_events.prefixid=$prefixid and scorched3d_events.seriesid=$seriesid and eventtype=1 group by weaponid order by tally desc";
$result = mysqlQuery($query) or die("Query failed : " . mysql_error());
?>
<table width="600" border="0" align="center">
<tr><td align=center><b>Top Weapons</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b>Weapon</b></td>
<td bgcolor=#111111><b>Armslevel</b></td>
<td bgcolor=#111111><b>Kills</b></td>
</tr>
<?
$rownumber=0;
while ($row = mysql_fetch_object($result))
{
	++$rownumber;
	echo "<tr>";
	echo "<td>$rownumber<br></td>";
	echo "<td><a href=\"weaponstats.php?Prefix=$prefixid&Series=$seriesid&WeaponID=$row->weaponid\">$row->name</a></td>";
	echo "<td>$row->armslevel<br></td>";
	echo "<td>$row->tally<br></td>";
	echo "</tr>";
}
?>
</table>

<?
include('statsfooter.php');
?>
