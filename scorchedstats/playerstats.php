<?
$playerid=$_GET['PlayerID'] or die ("No player specified");
include('statsheader.php');
?>

<?
// General Player Stats
$query = "SELECT * FROM scorched3d_players WHERE playerid=$playerid";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_object($result);
?>
<table width=640 border="0" align="center">
<tr><td align=center><b>Player Stats</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr><td bgcolor=#111111><b>Last Used Player Name</b></td><td><?=$row->name?></td></tr>
<tr><td bgcolor=#111111><b>Last Connected</b></td><td><?=$row->lastconnected?></td></tr>
<tr><td bgcolor=#111111><b>OS</b></td><td><?=$row->osdesc?></td></tr>
<tr><td bgcolor=#111111><b>Player ID</b></td><td><?=$row->playerid?></td></tr>
<tr><td bgcolor=#111111><b>Times Connected</b></td><td><?=$row->connects?></td></tr>
<tr><td bgcolor=#111111><b>Games Won</b></td><td><?=$row->overallwinner?></td></tr>
<tr><td bgcolor=#111111><b>Rounds Won</b></td><td><?=$row->wins?></td></tr>
<tr><td bgcolor=#111111><b>Rounds Resigned</b></td><td><?=$row->resigns?></td></tr>
<tr><td bgcolor=#111111><b>Total Shots</b></td><td><?=$row->shots?></td></tr>
<tr><td bgcolor=#111111><b>Opponent Kills</b></td><td><?=$row->kills?></td></tr>
<?
if ($row->shots==0)
	$calc=0.0;
else
	$calc=round((($row->kills-($row->selfkills+$row->teamkills))/$row->shots)*100, 1);
if ($row->kills==0)
	$calc2=0;
else
	$calc2=round($row->shots/$row->kills, 2);
echo "<tr><td bgcolor=#111111><b>Kill Ratio</b></td><td>$calc% ($calc2 shots/kill)</td></tr>";
if ($row->deaths==0)
	$calc="No Deaths Recorded";
else
	$calc = round((($row->kills-$row->teamkills)/$row->deaths), 2);
echo "<tr><td bgcolor=#111111><b>Kills/Death</b></td><td>$calc</td></tr>";
?>
<tr><td bgcolor=#111111><b>Self Kills</b></td><td><?=$row->selfkills?></td></tr>
<tr><td bgcolor=#111111><b>Team Kills</b></td><td><?=$row->teamkills?></td></tr>
<tr><td bgcolor=#111111><b>Deaths</b></td><td><?=$row->deaths?></td></tr>
</table>
<table width="600" border="0" align="center">
<tr><td><a href="playerevents.php?PlayerID=<?=$playerid?>">
View all this players events.</a></td></tr>
</table>
<br>

<?
// Player Aliases
$query = "SELECT * FROM scorched3d_names where playerid=$playerid";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width="600" border="0" align="center">
<tr><td align=center><b>Player Aliases</b></td></tr>
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
// Query player's stats for today
$query = "select (scorched3d_events.playerid) as playerid, (scorched3d_players.name) as name, (scorched3d_players.lastconnected) as lastcon, SUM(IF(scorched3d_events.eventtype='1',1,0)) AS killcount, SUM(IF(scorched3d_events.eventtype='5',1,0)) AS wincount, SUM(IF(scorched3d_events.eventtype='6',1,0)) AS gamewins FROM scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.playerid=scorched3d_players.playerid) WHERE (scorched3d_events.playerid=$playerid) and TO_DAYS(NOW()) - TO_DAYS(scorched3d_events.eventtime) < 1 group by playerid desc limit 1";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width=600 border="0" align="center">
<tr><td align=center><b>Player's Stats For Today</b></td></tr>
</table>
<table width=600 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><center><b>Player Name</b></center></td>
<td bgcolor=#111111><center><b>Kills</b></center></td>
<td bgcolor=#111111><center><b>Rnd Wins</b></center></td>
<td bgcolor=#111111><center><b>Game Wins</b></center></td>
<td bgcolor=#111111><center><b>Last Connect</b></center></td>
</tr>
<?
$rownum=0;
while ($row = mysql_fetch_object($result))
{
	$tmp = preg_replace("/[\(-].*/", "", $row->osdesc);
        ++$rownum;
        echo "<tr>";
		echo "<td><center>$rownum</center></td></center>";
        echo "<td><a href=\"playerstats.php?PlayerID=$row->playerid\">$row->name</a></td>";
        echo "<td><center>$row->killcount</td></center>";
        echo "<td><center>$row->wincount</td></center>";
        echo "<td><center>$row->gamewins</td></center>";
        echo "<td><center>$row->lastcon</td></center>";
		echo "<td>$tmp</td>";
        echo "</tr>";
}
?>
</table>
<?
$query = "select count(playerid) from scorched3d_players;";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_array($result);
?>
<table width=600 border =0 align=center>
<tr><td><a href="allplayers.php?PlayerID=0">
View all players (<? print $row[0]; ?>). 
</a>
</tr></td>
</table>
<br>

<?
// Query player's stats for the last week
$query = "select (scorched3d_events.playerid) as playerid, (scorched3d_players.name) as name, (scorched3d_players.lastconnected) as lastcon, SUM(IF(scorched3d_events.eventtype='1',1,0)) AS killcount, SUM(IF(scorched3d_events.eventtype='5',1,0)) AS wincount, SUM(IF(scorched3d_events.eventtype='6',1,0)) AS gamewins FROM scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.playerid=scorched3d_players.playerid) WHERE (scorched3d_events.playerid=$playerid) and TO_DAYS(NOW()) - TO_DAYS(scorched3d_events.eventtime) <= 6 group by playerid desc limit 1";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width=600 border="0" align="center">
<tr><td align=center><b>Player's Stats For The Last 7 Days</b></td></tr>
</table>
<table width=600 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><center><b>Player Name</b></center></td>
<td bgcolor=#111111><center><b>Kills</b></center></td>
<td bgcolor=#111111><center><b>Rnd Wins</b></center></td>
<td bgcolor=#111111><center><b>Game Wins</b></center></td>
<td bgcolor=#111111><center><b>Last Connect</b></center></td>
</tr>
<?
$rownum=0;
while ($row = mysql_fetch_object($result))
{
	$tmp = preg_replace("/[\(-].*/", "", $row->osdesc);
        ++$rownum;
        echo "<tr>";
		echo "<td><center>$rownum</center></td></center>";
        echo "<td><a href=\"playerstats.php?PlayerID=$row->playerid\">$row->name</a></td>";
        echo "<td><center>$row->killcount</td></center>";
        echo "<td><center>$row->wincount</td></center>";
        echo "<td><center>$row->gamewins</td></center>";
        echo "<td><center>$row->lastcon</td></center>";
		echo "<td>$tmp</td>";
        echo "</tr>";
}
?>
</table>
<?
$query = "select count(playerid) from scorched3d_players;";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_array($result);
?>
<table width=600 border =0 align=center>
<tr><td><a href="allplayers.php?PlayerID=0">
View all players (<? print $row[0]; ?>). 
</a>
</tr></td>
</table>
<br>
	
<?
// List top targets
$query="select (scorched3d_events.otherplayerid) as killedid, (scorched3d_players.name) as name, (count(*)) AS tally from scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.otherplayerid=scorched3d_players.playerid) where scorched3d_events.playerid=$playerid and eventtype=1 group by scorched3d_events.otherplayerid order by tally desc limit 25";
$result = mysql_query($query) or die("Query failed : " . mysql_error(). "<BR>query=$query");
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
	echo "<td><a href=\"playerstats.php?PlayerID=$row->killedid\">$row->name</a></td>";
	echo "<td>$row->tally</td>";
	echo "</tr>";
}
?>
</table>
<br>

<?
// Top Killers
$query=" select (scorched3d_events.playerid) as killedid, (scorched3d_players.name) as name, (count(*)) AS tally from scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.playerid=scorched3d_players.playerid) where otherplayerid=$playerid and eventtype=1 group by scorched3d_events.playerid order by tally desc limit 25";
$result = mysql_query($query) or die("Query failed : " . mysql_error(). "<BR>query=$query");
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
        echo "<td><a href=\"playerstats.php?PlayerID=$row->killedid\">$row->name</a></td>";
        echo "<td>$row->tally</td>";
	echo "</tr>";
}
?>
</table>
<br>

<?	
// Top Weapons
$query="select (scorched3d_events.weaponid) as weaponid, (count(*)) as tally, (scorched3d_weapons.name) as name from scorched3d_events LEFT JOIN scorched3d_weapons ON (scorched3d_events.weaponid=scorched3d_weapons.weaponid) where playerid=$playerid and eventtype=1 group by weaponid order by tally desc";
$result = mysql_query($query) or die("Query failed : " . mysql_error(). "<BR>query=$query");
?>
<table width="600" border="0" align="center">
<tr><td align=center><b>Top Weapons</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b>Weapon</b></td>
<td bgcolor=#111111><b>Kills</b></td>
</tr>
<?
$rownumber=0;
while ($row = mysql_fetch_object($result))
{
	++$rownumber;
	echo "<tr>";
	echo "<td>$rownumber<br></td>";
	echo "<td><a href=\"weaponstats.php?WeaponID=$row->weaponid\">$row->name</a></td>";
	echo "<td>$row->tally<br></td>";
	echo "</tr>";
}
?>
</table>

<?
include('statsfooter.php');
?>
